#include "stdafx.h"
#include "script/lua/Environment.h"

#include "LFunction.h"

vscript::lua::Environment::Environment() :
    m_state(nullptr),
    m_namespaceDepth(0),
    m_maxScriptLength(INT32_MAX / 10) {
    // Initialise the Lua environment.
    m_state = luaL_newstate();
    luaL_openlibs(m_state);

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
    addCClosure("SubscribeToEvent", 1, &makeLCallback);
}

vscript::lua::Environment::Environment(const std::string& filepath) :
    Environment() {
    load(filepath);
}

Script::Lua::Environment::~Environment() {
    for (auto& lFunction : m_lFunctions) {
        lFunction.second.reset();
    }
    lua_close(m_state);
}

bool Script::Lua::Environment::load(const vio::Path& filepath) {
    // Get a File reference.
    if (!file.isFile()) return false;
    vio::File file;
    if (!file.asFile(&file)) return false;

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

        script = new char[l + 1];
        size_t end = filestream.read(length, 1, script);
        script[end] = 0;
    }

    // Load and run script, clean up and return success state.
    bool success = luaL_dostring(m_state, script) == 0;
    delete[] script;

    return success;
}

