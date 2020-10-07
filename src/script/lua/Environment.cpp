#include "Vorb/stdafx.h"
#include "Vorb/script/lua/Environment.h"

#include <sstream>

#include "Vorb/io/File.h"
#include "Vorb/io/FileStream.h"
#include "Vorb/script/EnvironmentRegistry.hpp"

vscript::lua::Environment::Environment() :
    IEnvironment(),
    m_state(nullptr),
    m_namespaceDepth(0),
    m_parent(nullptr) {
    // Empty
}

void vscript::lua::Environment::init() {
    // Initialise the Lua environment.
    m_state = luaL_newstate();
    luaL_openlibs(m_state);

    // Add mapping from handle to this env.
    handleEnvMap.insert(std::make_pair(m_state, this));

    // TODO(Matthew): Error handler.

    // Create a table for storing registered functions.
    lua_newtable(m_state);
    lua_setfield(m_state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);

    // Set global namespace.
    setGlobalNamespace();

    // Create a Lua function registration closure that captures this environment instance.
    ValueMediator<void*>::push(m_state, (void*)this);
    addCClosure("RegisterFunction", 1, &registerLFunction);
}

void vscript::lua::Environment::init(IEnvironment* env) {
    // Cast to Lua environment.
    Environment* luaEnv = static_cast<Environment*>(env);

    // Initialise the Lua environment.
    m_state = lua_newthread(luaEnv->m_state);

    // Add mapping from handle to this env.
    handleEnvMap.insert(std::make_pair(m_state, this));

    // Set parent.
    m_parent = luaEnv;

    // Set global namespace.
    setGlobalNamespace();

    // TODO(Matthew): Anything more to make this work?
}

void vscript::lua::Environment::init(const vio::Path& filepath) {
    init();
    load(filepath);
}

void vscript::lua::Environment::dispose() {
    for (auto& lFunction : m_lFunctions) {
        // Note: we probably don't need to properly dispose of each registered function.
        lFunction.second->dispose();
        delete lFunction.second;
    }
    LFunctionList().swap(m_lFunctions);

    lua_close(m_state);

    IEnvironment::dispose();
}

bool vscript::lua::Environment::load(const vio::Path& filepath) {
    // Get a File reference.
    if (!filepath.isFile()) return false;
    vio::File file;
    if (!filepath.asFile(&file)) return false;

    cString script = nullptr;
    {
        // Get a File stream
        vio::FileStream filestream = file.openReadOnly(false);
        if (!filestream.isOpened()) return false;

        // Get length of the file
        vio::FileSeekOffset length = filestream.length();

        // If the script length is too great, we should fail to avoid gobbling RAM.
        if (length + 1 > m_maxScriptLength) {
            return false;
        }

        script = new char[length + 1];
        size_t end = filestream.read(length, 1, script);
        script[end] = 0;
    }

    // Load and run script, clean up and return success state.
    bool success = luaL_loadstring(m_state, script) == 0;
    delete[] script;

    return success;
}

bool vscript::lua::Environment::load(const nString& script) {
    // Load and run script, returning success state.
    return luaL_loadstring(m_state, script.c_str()) == 0;
}

bool vscript::lua::Environment::run() {
    // Run Lua function currently on top of Lua state stack, returning success state.
    return lua_pcall(m_state, 0, LUA_MULTRET, 0) == 0;
}

bool vscript::lua::Environment::run(const vio::Path& filepath) {
    // Get a File reference.
    if (!filepath.isFile()) return false;
    vio::File file;
    if (!filepath.asFile(&file)) return false;

    cString script = nullptr;
    {
        // Get a File stream
        vio::FileStream filestream = file.openReadOnly(false);
        if (!filestream.isOpened()) return false;

        // Get length of the file
        vio::FileSeekOffset length = filestream.length();

        // If the script length is too great, we should fail to avoid gobbling RAM.
        if (length + 1 > m_maxScriptLength) {
            return false;
        }

        script = new char[length + 1];
        size_t end = filestream.read(length, 1, script);
        script[end] = '\0';
    }

    // Load and run script, clean up and return success state.
    bool success = luaL_dostring(m_state, script) == 0;
    delete[] script;

    if (!success) {
        printf("Error running script: %s\n", ValueMediator<const cString>::pop(m_state));
    }

    return success;
}

bool vscript::lua::Environment::run(const nString& script) {
    // Load and run script, and return success state.
    return luaL_dostring(m_state, script.c_str()) == 0;
}

vscript::lua::Environment* vscript::lua::Environment::environmentFromHandle(Handle handle) {
    try {
        return handleEnvMap.at(handle);
    } catch (std::out_of_range& e) {
        return nullptr;
    }
}

void vscript::lua::Environment::setLockingMechanism() {
    lua_setLock([](void* L) {
        Environment* env = environmentFromHandle(static_cast<Handle>(L));
        vscript::EnvironmentRegistry<vscript::lua::Environment>::lock(env);
    });
    lua_setUnlock([](void* L) {
        Environment* env = environmentFromHandle(static_cast<Handle>(L));
        vscript::EnvironmentRegistry<vscript::lua::Environment>::unlock(env);
    });
}

void vscript::lua::Environment::addCFunction(const nString& name, CFunction::Type function) {
    // Push function pointer onto Lua stack and set the field of key name to that pointer.
    lua_pushcfunction(m_state, function);
    lua_setfield(m_state, -2, name.c_str());
}

void vscript::lua::Environment::addCClosure(const nString& name, ui8 upvalueCount, CFunction::Type function) {
    // Push function pointer onto Lua stack, capturing upvalueCount many values below it on the stack, then set the field of key name to that pointer.
    lua_pushcclosure(m_state, function, upvalueCount);
    lua_setfield(m_state, (-1 * (i32)upvalueCount) - 1, name.c_str());
}

i32 vscript::lua::Environment::makeLFunction(const nString& name) {
    using Index  = i32;

    // Helper function for the next LFunction registration function: explodes a string into its parts about the given delimeter.
    auto explode = [](const nString& name, char delimeter, OUT std::vector<nString>& buffer) {
        buffer.clear();

        std::stringstream sstream(name);

        for (nString token; std::getline(sstream, token, delimeter);) {
            buffer.emplace_back(std::move(token));
        }
    };

    // Split the given name into parts.
    std::vector<nString> parts;
    explode(name, '.', parts);

    // Ensure we have at least one part for a function name.
    if (parts.size() == 0) return -3;

    Index currIndex;

    // Get our script function table in the Lua registry.
    lua_getfield(m_state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);

    // Get the first layer (table, or function itself) and create a reference to it in the table.
    // Store the index of the reference as the current index value in the lua registry index.
    lua_getglobal(m_state, parts[0].c_str());
    currIndex = luaL_ref(m_state, -2);

    auto it = parts.begin();
    if (++it != parts.end()) {
        while (it++ != parts.end()) {
            // Get a raw access to the table referenced at the last index added to the list. 
            lua_rawgeti(m_state, -1, currIndex);
            // Get the value of the field with key equal to the value currently pointer to by the iterator.
            lua_getfield(m_state, -1, (*it).c_str());
            // Create a reference to this value, be it another table or function, and set it as the new current index value.
            currIndex = luaL_ref(m_state, -3);
            // Pop the raw access to the previous table.
            lua_pop(m_state, 1);
        }
    }

    // LFunction doesn't yet exist, make one and add to register.
    LFunction* lFunction = new LFunction(m_state, name, currIndex);

    // Add the unregistered LFunction to the register.
    // If we succeed, return 0, otherwise -4.
    return addLFunction(lFunction) ? 0 : -4;
}

bool vscript::lua::Environment::addLFunction(LFunction* lFunction) {
    return getRegisteredLFunctions().insert({ lFunction->getName(), lFunction }).second;
}

vscript::lua::LFunction* vscript::lua::Environment::getLFunction(const nString& name) {
    const auto& it = getRegisteredLFunctions().find(name);
    if (it == getRegisteredLFunctions().end()) {
        return nullptr;
    }
    return it->second;
}

void vscript::lua::Environment::setGlobalNamespace() {
    lua_pushglobaltable(m_state);
}

void vscript::lua::Environment::pushNamespace(const nString& namespace_) {
    // See if a field already exists with the key of this namespace.
    lua_getfield(m_state, -1, namespace_.c_str());
    // If it doesn't, we can add it.
    if (lua_isnil(m_state, -1)) {
        // Pop the result of looking for the namespace.
        lua_pop(m_state, 1);
        // Add a clean table to the stack.
        lua_newtable(m_state);
        // Add this clean table to the previous table as a field with a key of namespace_.
        lua_setfield(m_state, -2, namespace_.c_str());
        // Get the new namespace back on top of the stack (lua_setfield pops the top value on the stack).
        lua_getfield(m_state, -1, namespace_.c_str());
    }
}

int vscript::lua::registerLFunction(Handle state) {
    // Get the captured environment pointer.
    Environment* env = static_cast<Environment*>(lua_touserdata(state, lua_upvalueindex(1)));

    // We expect only one parameter - a string identifying the function to be registered.
    if (lua_gettop(state) != 1) {
        vscript::lua::ValueMediator<i32>::push(state, -1);
        return 1;
    }

    // Try to get the name of the function, if we can't then return.
    nString name;
    if (!vscript::lua::ValueMediator<nString>::tryPop(state, name)) {
        vscript::lua::ValueMediator<i32>::push(state, -2);
        return 1;
    }

    // First see if the lFunction we just got already exists in the environment's register.
    LFunction* lFunction = env->getLFunction(name);
    if (lFunction != nullptr) {
        vscript::lua::ValueMediator<i32>::push(state, 0);
        return 1;
    }

    vscript::lua::ValueMediator<i32>::push(state, env->makeLFunction(name));
    return 1;
}

vscript::lua::HandleEnvMap vscript::lua::Environment::handleEnvMap = vscript::lua::HandleEnvMap();
