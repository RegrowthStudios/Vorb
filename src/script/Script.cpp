#include <stdafx.h>
#include "script/Script.h"

extern "C" {
#include "OOLua/lua/lua.h"
#include "OOLua/lua/lualib.h"
#include "OOLua/lua/lauxlib.h"
}

/************************************************************************/
/* Integer casts                                                        */
/************************************************************************/
#define CAST_INTEGER(TYPE) \
    void vscript::ScriptValueSender<TYPE, void>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushinteger(h, static_cast<lua_Integer>(v)); \
    } \
    TYPE vscript::ScriptValueSender<TYPE, void>::pop(EnvironmentHandle h) { \
        return static_cast<TYPE>(lua_tointeger(h, -1)); \
    }
CAST_INTEGER(i8)
CAST_INTEGER(i16)
CAST_INTEGER(i32)
CAST_INTEGER(i64)
#define CAST_UNSIGNED(TYPE) \
    void vscript::ScriptValueSender<TYPE, void>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushunsigned(h, static_cast<lua_Unsigned>(v)); \
    } \
    TYPE vscript::ScriptValueSender<TYPE, void>::pop(EnvironmentHandle h) { \
        return static_cast<TYPE>(lua_tounsigned(h, -1)); \
    }
CAST_UNSIGNED(ui8)
CAST_UNSIGNED(ui16)
CAST_UNSIGNED(ui32)
CAST_UNSIGNED(ui64)

/************************************************************************/
/* Number casts                                                         */
/************************************************************************/
#define CAST_NUMBER(TYPE) \
    void vscript::ScriptValueSender<TYPE, void>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushnumber(h, static_cast<lua_Number>(v)); \
    } \
    TYPE vscript::ScriptValueSender<TYPE, void>::pop(EnvironmentHandle h) { \
        return static_cast<TYPE>(lua_tonumber(h, -1)); \
    }
CAST_NUMBER(f32);
CAST_NUMBER(f64);

/************************************************************************/
/* Boolean cast                                                         */
/************************************************************************/
void vscript::ScriptValueSender<bool, void>::push(EnvironmentHandle h, bool v) {
    lua_pushboolean(h, v);
}
bool vscript::ScriptValueSender<bool, void>::pop(EnvironmentHandle h) {
    return lua_toboolean(h, -1);
}

/************************************************************************/
/* String casts                                                         */
/************************************************************************/
void vscript::ScriptValueSender<nString, void>::push(EnvironmentHandle h, nString v) {
    lua_pushstring(h, v.c_str());
}
nString vscript::ScriptValueSender<nString, void>::pop(EnvironmentHandle h) {
    return nString(lua_tostring(h, -1));
}
void vscript::ScriptValueSender<const cString, void>::push(EnvironmentHandle h, const cString v) {
    lua_pushstring(h, v);
}
const cString vscript::ScriptValueSender<const cString, void>::pop(EnvironmentHandle h) {
    return lua_tostring(h, -1);
}
/************************************************************************/
/* User data cast                                                      */
/************************************************************************/
void vscript::ScriptValueSender<void*, void>::push(EnvironmentHandle h, void* v) {
    lua_pushlightuserdata(h, v);
}
void* vscript::ScriptValueSender<void*, void>::pop(EnvironmentHandle h) {
    return lua_touserdata(h, -1);
}

void vscript::impl::pushToTop(EnvironmentHandle h, const Function& f) {
    lua_getglobal(h, f.getName().c_str());
}
void vscript::impl::call(EnvironmentHandle h, size_t n, size_t r) {
    lua_pcall(h, n, r, 0);
}
void vorb::script::impl::popStack(EnvironmentHandle h) {
    lua_pop(h, 1);
}

void* vorb::script::impl::popUpvalueObject(EnvironmentHandle h) {
    void* val = lua_touserdata(h, lua_upvalueindex(1));
    return val;
}
