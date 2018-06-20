#include "Vorb/stdafx.h"
#include "Vorb/script/ScriptImpl.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "Vorb/script/Function.h"
#include "Vorb/script/Environment.h"

void vscript::impl::pushToTop(EnvironmentHandle h, const Function& f) {
    lua_getfield(h, LUA_REGISTRYINDEX, VORB_SCRIPT_FUNCTION_TABLE);
    lua_getfield(h, -1, f.getName().c_str());
}
void vscript::impl::call(EnvironmentHandle h, size_t n, size_t r) {
    lua_pcall(h, (int)n, (int)r, 1);
}
void vorb::script::impl::popStack(EnvironmentHandle h) {
    lua_pop(h, 1);
}

void* vorb::script::impl::popUpvalueObject(EnvironmentHandle h) {
    void* val = lua_touserdata(h, lua_upvalueindex(1));
    return val;
}

void vorb::script::impl::pushGlobalNamespace(EnvironmentHandle h) {
    lua_pushglobaltable(h);
}
void vorb::script::impl::pushNamespace(EnvironmentHandle h, const nString& name) {
    // Attempt to get the namespace
    lua_getfield(h, -1, name.c_str());
    if (lua_isnil(h, -1)) {
        // Create the new namespace
        lua_pop(h, 1);
        lua_newtable(h);
        lua_setfield(h, -2, name.c_str());

        // Push new namespace
        lua_getfield(h, -1, name.c_str());
    }
}
void vorb::script::impl::popNamespaces(EnvironmentHandle h, i32 n) {
    lua_pop(h, n);
}

bool vorb::script::impl::objectHasField(EnvironmentHandle h, const nString& name) {
    lua_pushstring(h, name.c_str());
    lua_gettable(h, -2);
    bool hasValue = !lua_isnil(h, -1);
    lua_pop(h, 1);
    return hasValue;
}

void vorb::script::impl::dumpStack(EnvironmentHandle h) {
    i32 end = lua_gettop(h);
    puts("Lua-Stack:");
    for (i32 i = 1; i <= end; i++) {
        auto t = lua_type(h, i);
        switch (t) {
        case LUA_TSTRING:
            printf("%5d: String=  \"%s\"\n", i, lua_tostring(h, i));
            break;
        case LUA_TBOOLEAN:
            printf("%5d: Bool=    %s\n", i, lua_toboolean(h, i) ? "True" : "False");
            break;
        case LUA_TNUMBER:
            printf("%5d: Number=  %g\n", i, lua_tonumber(h, i));
            break;
        case LUA_TLIGHTUSERDATA:
            printf("%5d: UserPtr= %d\n", i, (ptrdiff_t)lua_touserdata(h, i));
            break;
        default:
            printf("%5d: %s\n", i, lua_typename(h, t));
            break;
        }
    }
}


