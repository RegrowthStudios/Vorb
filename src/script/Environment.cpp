#include "stdafx.h"
#include "script/Environment.h"

extern "C" {
#include <OOLua/lua/lua.h>
#include <OOLua/lua/lualib.h>
#include <OOLua/lua/lauxlib.h>
}

#include "io/File.h"
#include "io/FileStream.h"

#define VORB_SCRIPT_ENVIRONMENT_TABLE "VORB_VALUE_TABLE"
#define VORB_SCRIPT_ENVIRONMENT_VALUE_FORMAT "VORB_VALUE_%d"

vscript::Environment::Environment() {
    m_state = luaL_newstate();
    luaL_openlibs(m_state);
    lua_register(m_state, "register", vscript::Environment::registration);

    // Create our special function table
    lua_pushglobaltable(m_state);
    lua_newtable(m_state);
    lua_setfield(m_state, -2, VORB_SCRIPT_ENVIRONMENT_TABLE);
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
        scriptData = new char[l];
        size_t end = fs.read(l, 1, scriptData);
        scriptData[end] = 0;
    }
    bool success = luaL_dostring(m_state, scriptData) == 0;
    delete[] scriptData;

    if (!success) return false;

    lua_getglobal(m_state, "registerFuncs");
    
    if (lua_isfunction(m_state, -1)) {
        // Register functions
        lua_pushlightuserdata(m_state, this);
        lua_call(m_state, 1, 0);
        lua_pop(m_state, 1);

        // Delete register function
        lua_pushnil(m_state);
        lua_setglobal(m_state, "registerFuncs");
    } else {
        lua_pop(m_state, 1);
    }

    return true;
}

vscript::Function& vscript::Environment::operator[](const nString& name) {
    return m_functions.at(name);
}
const vscript::Function& vscript::Environment::operator[](const nString& name) const {
    return m_functions.at(name);
}

int vscript::Environment::registration(lua_State* L) {
    if (lua_gettop(L) != 3) return 0;
    if (!lua_isuserdata(L, 1)) return 0;
    if (!lua_isstring(L, 2)) return 0;
    if (!lua_isstring(L, 3)) return 0;
    
    vscript::Environment* e = (vscript::Environment*)lua_touserdata(L, 1);
    nString name(lua_tostring(L, 2));

    {
        char buf[255];
        sprintf(buf, VORB_SCRIPT_ENVIRONMENT_VALUE_FORMAT, e->m_idGen.generate());
    }

    nString nameLua(lua_tostring(L, 3));
    e->m_functions[name] = nameLua;

    return 0;
}

void vorb::script::Environment::addCFunction(const nString& name, int(*f)(EnvironmentHandle)) {
    lua_register(m_state, name.c_str(), f);
}
void vorb::script::Environment::addCClosure(const nString& name, int(*f)(EnvironmentHandle)) {
    lua_pushcclosure(m_state, f, 1);
    lua_setglobal(m_state, name.c_str());
}
void vorb::script::Environment::setValueName(const nString& name) {
    lua_setglobal(m_state, name.c_str());
}
