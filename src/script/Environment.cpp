#include "stdafx.h"
#include "script/Environment.h"

extern "C" {
#include <OOLua/lua/lua.h>
#include <OOLua/lua/lualib.h>
#include <OOLua/lua/lauxlib.h>
}

#include "io/File.h"
#include "io/FileStream.h"

vscript::Environment::Environment() {
    m_state = luaL_newstate();
    luaL_openlibs(m_state);
    lua_register(m_state, "register", vscript::Environment::registration);
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
    nString nameLua(lua_tostring(L, 3));
    e->m_functions[name] = nameLua;

    return 0;
}
