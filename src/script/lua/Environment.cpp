#include "Vorb/stdafx.h"
#include "Vorb/script/lua/Environment.h"

#include "Vorb/io/File.h"
#include "Vorb/io/FileStream.h"

vscript::lua::Environment::Environment() :
    IEnvironment(),
    m_state(nullptr),
    m_namespaceDepth(0) {
    // Empty
}

void vscript::lua::Environment::init() {
    // Initialise the Lua environment.
    m_state = luaL_newstate();
    luaL_openlibs(m_state);

    // TODO(Matthew): Error handler.

    // Create a table for storing registered functions.
    lua_newtable(m_state);
    lua_setfield(m_state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);

    // Set global namespace.
    setGlobalNamespace();

    // Create a Lua function registration closure that captures this environment instance.
    ValueMediator<void*>::push(m_state, (void*)this);
    addCClosure("RegisterFunction", 1, &makeLFunction);

    // Create a event subscription closure for Lua functions that captures this environment instance.
    ValueMediator<void*>::push(m_state, (void*)this);
    addCClosure("SubscribeToGlobalEvent", 1, &makeLCallback);
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

bool vscript::lua::Environment::addLFunction(LFunction* lFunction) {
    return m_lFunctions.insert({ lFunction->getName(), lFunction }).second;
}

bool vscript::lua::Environment::subscribeLFunction(const nString& eventName, LFunction* lFunction) {
    // Find the event we want to register to, if we don't find it then return false.
    const auto& it = m_events.find(eventName);
    if (it == m_events.end()) {
        return false;
    }

    // Call the adder function, which manages all the actual work as it knows the template data.
    EventAdder adder = it->second.adder;
    (this->*adder)(lFunction, it->second.event);

    return true;
}

vscript::lua::LFunction* vscript::lua::Environment::getLFunction(const nString& name) {
    const auto& it = m_lFunctions.find(name);
    if (it == m_lFunctions.end()) {
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

// Helper function for the next two functions; pops const cString values off the top of the stack in the correct order.
using Index  = i32;
using Values = std::vector<const cString>;
static Values popValues(vscript::lua::Handle state, i32 n) {
    // Container to store the values popped, resized to minimise calls to new.
    Values values;
    values.resize(n);

    // For loop over indices of values on the stack in the desired order.
    for (i32 i = -1; i >= -1 * n; --i) {
        const cString value;
        // Try to pop a string type value, if we fail to do so then we have an invalid parameter; fail.
        if (!vscript::lua::ValueMediator<const cString>::tryPop(state, value)) {
            return Values();
        }
        values[i + n] = value;
    }

    return values;
}

// vscript::GenericScriptFunction vscript::lua::Environment::createScriptFunction() {
//     // Get "depth" of the function to make a reference to.
//     // If this is 0 or 1, then there's no function to reference!
//     i32 depth = lua_gettop(m_state);
//     if (depth <= 1) {
//         return nullptr;
//     }

//     // Pop the values provided, ensuring they're all string types.
//     // If they aren't, then we don't assume anything and must fail.
//     Values values = popValues(m_state, depth);
//     if (values.empty()) {
//         return nullptr;
//     }

//     nString name(values.front());

//     // First see if the lFunction we just got already exists in the environment's register.
//     LFunction* lFunction = getLFunction(name);
//     if (lFunction != nullptr) return nullptr;

//     Index currIndex;

//     // Get our script function table in the Lua registry.
//     lua_getfield(m_state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);

//     // Get the first layer (table, or function itself) and create a reference to it in the table.
//     // Store the index of the reference as the current index value in the lua registry index.
//     lua_getglobal(m_state, values.at(1));
//     currIndex = luaL_ref(m_state, -2);

//     auto it = values.begin();
//     if (++(++it) != values.end()) {
//         for (; it != values.end(); ++it) {
//             // Get a raw access to the table referenced at the last index added to the list. 
//             lua_rawgeti(m_state, -1, currIndex);
//             // Get the value of the field with key equal to the value currently pointer to by the iterator.
//             lua_getfield(m_state, -1, *it);
//             // Create a reference to this value, be it another table or function, and set it as the new current index value.
//             currIndex = luaL_ref(m_state, -3);
//             // Pop the raw access to the previous table.
//             lua_pop(m_state, 1);
//         }
//     }

//     // LFunction doesn't yet exist, make one and add to register.
//     lFunction = new LFunction(m_state, name, currIndex);

//     // Add the unregistered LFunction to the register.
//     if (!addLFunction(lFunction)) {
//         // If we reached here, we failed to register a new LFunction object due to some failure in addLFunction.
//         return nullptr;
//     }

//     return lFunction;
// }
// TODO(Matthew): We shouldn't be making the Lua scripts choose their names in C, we should be able to determine a prefix. Using lua_newthread we can!
int vscript::lua::makeLFunction(Handle state) {
    // Get "depth" of the function to make a reference to.
    // If this is 0 or 1, then there's no function to reference!
    i32 depth = lua_gettop(state);
    if (depth <= 1) {
        return -1;
    }

    // Pop the values provided, ensuring they're all string types.
    // If they aren't, then we don't assume anything and must fail.
    Values values = popValues(state, depth);
    if (values.empty()) {
        return -1;
    }

    nString name(values.front());

    // Get the captured environment pointer.
    Environment* env = static_cast<Environment*>(lua_touserdata(state, lua_upvalueindex(1)));

    // First see if the lFunction we just got already exists in the environment's register.
    LFunction* lFunction = env->getLFunction(name);
    if (lFunction != nullptr) return -2;

    Index currIndex;

    // Get our script function table in the Lua registry.
    lua_getfield(state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);

    // Get the first layer (table, or function itself) and create a reference to it in the table.
    // Store the index of the reference as the current index value in the lua registry index.
    lua_getglobal(state, values.at(1));
    currIndex = luaL_ref(state, -2);

    auto it = values.begin();
    if (++(++it) != values.end()) {
        for (; it != values.end(); ++it) {
            // Get a raw access to the table referenced at the last index added to the list. 
            lua_rawgeti(state, -1, currIndex);
            // Get the value of the field with key equal to the value currently pointer to by the iterator.
            lua_getfield(state, -1, *it);
            // Create a reference to this value, be it another table or function, and set it as the new current index value.
            currIndex = luaL_ref(state, -3);
            // Pop the raw access to the previous table.
            lua_pop(state, 1);
        }
    }

    // LFunction doesn't yet exist, make one and add to register.
    lFunction = new LFunction(state, name, currIndex);

    // Add the unregistered LFunction to the register.
    // If we succeed, return 0, otherwise -3.
    return env->addLFunction(lFunction) ? 0 : -3;
}

int vscript::lua::makeLCallback(Handle state) {
    // Get "depth" of the function to make a reference to.
    // If this is 0, 1 or 2, then there's no function to reference!
    i32 depth = lua_gettop(state);
    if (depth <= 2) {
        return -1;
    }

    const cString eventName = nullptr;
    // Try to pop string type value for the event name, if we fail to do so then we have an invalid parameter; fail.
    if (!ValueMediator<const cString>::tryRetrieve(state, 1, eventName)) {
        return -1;
    }

    // Try to pop string type value for the function name, if we fail to do so then we have an invalid parameter; fail.
    const cString functionName = nullptr;
    if (!ValueMediator<const cString>::tryRetrieve(state, 1, functionName)) {
        return -1;
    }

    // Reevaluate the depth of the stack.
    depth = lua_gettop(state);

    // Pop the values left, ensuring they're all string types.
    // If they aren't, then we don't assume anything and must fail.
    Values values = popValues(state, depth);
    if (values.empty()) {
        return -1;
    }

    // Get the captured environment pointer.
    Environment* env = static_cast<Environment*>(lua_touserdata(state, lua_upvalueindex(1)));

    // First see if the lFunction already exists in the environment's register, if so, just subscribe it to the given event.
    LFunction* lFunction = env->getLFunction(functionName);
    if (lFunction != nullptr) {
        // Subscribe the generated lFunction to the desired event.
        env->subscribeLFunction(eventName, lFunction);

        return 0;
    }

    Index currIndex;

    // Get our script function table in the Lua registry.
    lua_getfield(state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);

    // Get the first layer (table, or function itself) and create a reference to it in the table.
    // Store the index of the reference in the indices vector.
    lua_getglobal(state, values.at(0));
    currIndex = luaL_ref(state, -2);

    auto it = values.begin();
    if (++it != values.end()) {
        for (; it != values.end(); ++it) {
            // Get a raw access to the table referenced at the last index added to the list. 
            lua_rawgeti(state, -1, currIndex);
            // Get the value of the field with key equal to the value currently pointer to by the iterator.
            lua_getfield(state, -1, *it);
            // Create a reference to this value, be it another table or function, and set it as the new current index value.
            currIndex = luaL_ref(state, -3);
            // Pop the raw access to the previous table.
            lua_pop(state, 1);
        }
    }

    // LFunction doesn't yet exist, make one and add to register.
    lFunction = new LFunction(state, functionName, currIndex);

    // Add the unregistered LFunction to the register.
    // If we fail to register it then we can't susbcribe it to an event.
    if (!env->addLFunction(lFunction)) {
        return -2;
    }

    // Subscribe the generated lFunction to the desired event.
    env->subscribeLFunction(eventName, lFunction);

    return 0;
}
