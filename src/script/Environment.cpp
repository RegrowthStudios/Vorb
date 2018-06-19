#include "Vorb/stdafx.h"
#include "Vorb/script/Environment.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "Vorb/io/File.h"
#include "Vorb/io/FileStream.h"

#define VORB_SCRIPT_ENVIRONMENT_TABLE "VORB_VALUE_TABLE"
#define VORB_SCRIPT_ENVIRONMENT_VALUE_FORMAT "VORB_VALUE_%d"

vscript::Function vscript::Function::nil;

namespace {
    int errFunc(lua_State* s) {
        // Obtain the offending environment
        lua_pushglobaltable(s);
        vscript::impl::pushNamespace(s, "Vorb");
        lua_getfield(s, -1, "env");
        vscript::Environment* env = (vscript::Environment*)lua_touserdata(s, -1);
        lua_pop(s, 3);

        // Execute the error handler
        if (env->errorHandler) {
            const cString errorMessage = lua_tostring(s, -1);
            env->errorHandler(env, errorMessage);
        }

        // Cleanup
        lua_pop(s, 1);
        return 0;
    }
}

vscript::Environment::Environment() {
    // Create Lua environment
    m_state = luaL_newstate();
    luaL_openlibs(m_state);
    lua_register(m_state, "register", vscript::Environment::registration);

    // Add error handler
    lua_pushlightuserdata(m_state, this);
    lua_pushcclosure(m_state, errFunc, 1);

    // Create our special function table
    lua_newtable(m_state);
    lua_setfield(m_state, LUA_REGISTRYINDEX, VORB_SCRIPT_FUNCTION_TABLE);

    // Add registration function
    lua_pushglobaltable(m_state);
    impl::pushNamespace(m_state, "Vorb");
    lua_pushlightuserdata(m_state, this);
    lua_pushcclosure(m_state, vscript::Environment::registration, 1);
    lua_setfield(m_state, -2, "register");
    lua_pushlightuserdata(m_state, this);
    lua_setfield(m_state, -2, "env");
    lua_pop(m_state, 1);
}
vscript::Environment::~Environment() {
    lua_close(m_state);
}

bool vscript::Environment::load(const vio::Path& file) {
    // Get a File reference
    if (!file.isFile()) return false;
    vio::File f;
    if (!file.asFile(&f)) return false;

    cString scriptData = nullptr;
    {
        // Get a File stream
        vio::FileStream fs = f.openReadOnly(false);
        if (!fs.isOpened()) return false;

        // Read the file
        vio::FileSeekOffset l = fs.length();
        scriptData = new char[l + 1];
        size_t end = fs.read(l, 1, scriptData);
        scriptData[end] = 0;
    }
    bool success = luaL_dostring(m_state, scriptData) == 0;
    delete[] scriptData;

    if (!success) return false;

    return true;
}

bool vscript::Environment::hasFunction(const nString& name) {
    auto& kvp = m_functions.find(name);
    return (kvp != m_functions.end());
}

vscript::Function& vscript::Environment::operator[](const nString& name) {
    auto& kvp = m_functions.find(name);
    return kvp == m_functions.end() ? Function::nil : kvp->second;
}
const vscript::Function& vscript::Environment::operator[](const nString& name) const {
    auto& kvp = m_functions.find(name);
    return kvp == m_functions.end() ? Function::nil : kvp->second;
}

int vscript::Environment::registration(lua_State* L) {
    // Get arguments
    if (lua_gettop(L) != 2) return 0;
    if (!lua_isstring(L, 1)) return 0;
    if (!lua_isfunction(L, 2)) return 0;
    vscript::Environment* e = (vscript::Environment*)lua_touserdata(L, lua_upvalueindex(1));
    nString name(lua_tostring(L, 1));

    // Add function to the registry
    e->m_functions[name] = Function(e, name);
    lua_getfield(L, LUA_REGISTRYINDEX, VORB_SCRIPT_FUNCTION_TABLE);
    lua_insert(L, -2);
    lua_setfield(L, -2, name.c_str());

    return 0;
}

void vorb::script::Environment::addCFunction(const nString& name, int(*f)(EnvironmentHandle)) {
    lua_pushcfunction(m_state, f);
    lua_setfield(m_state, -2, name.c_str());
}
void vorb::script::Environment::addCClosure(const nString& name, int(*f)(EnvironmentHandle)) {
    lua_pushcclosure(m_state, f, 1);
    lua_setfield(m_state, -2, name.c_str());
}
void vorb::script::Environment::setValueName(const nString& name) {
    lua_setfield(m_state, -2, name.c_str());
}
