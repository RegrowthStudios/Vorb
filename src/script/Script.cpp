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
    template<> \
    void vscript::ScriptValueSender<TYPE>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushinteger(h, static_cast<lua_Integer>(v)); \
    } \
    template<> \
    TYPE vscript::ScriptValueSender<TYPE>::pop(EnvironmentHandle h) { \
        return static_cast<TYPE>(lua_tointeger(h, -1)); \
    }
CAST_INTEGER(i8)
CAST_INTEGER(i16)
CAST_INTEGER(i32)
CAST_INTEGER(i64)
#define CAST_UNSIGNED(TYPE) \
    template<> \
    void vscript::ScriptValueSender<TYPE>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushunsigned(h, static_cast<lua_Unsigned>(v)); \
    } \
    template<> \
    TYPE vscript::ScriptValueSender<TYPE>::pop(EnvironmentHandle h) { \
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
    template<> \
    void vscript::ScriptValueSender<TYPE>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushnumber(h, static_cast<lua_Number>(v)); \
    } \
    template<> \
    TYPE vscript::ScriptValueSender<TYPE>::pop(EnvironmentHandle h) { \
        return static_cast<TYPE>(lua_tonumber(h, -1)); \
    }
CAST_NUMBER(f32);
CAST_NUMBER(f64);

/************************************************************************/
/* Boolean cast                                                         */
/************************************************************************/
template<>
void vscript::ScriptValueSender<bool>::push(EnvironmentHandle h, bool v) {
    lua_pushboolean(h, v);
}
template<>
bool vscript::ScriptValueSender<bool>::pop(EnvironmentHandle h) {
    return lua_toboolean(h, -1);
}

/************************************************************************/
/* String casts                                                         */
/************************************************************************/
template<>
void vscript::ScriptValueSender<nString>::push(EnvironmentHandle h, nString v) {
    lua_pushstring(h, v.c_str());
}
template<>
nString vscript::ScriptValueSender<nString>::pop(EnvironmentHandle h) {
    return nString(lua_tostring(h, -1));
}
template<>
void vscript::ScriptValueSender<const cString>::push(EnvironmentHandle h, const cString v) {
    lua_pushstring(h, v);
}
template<>
const cString vscript::ScriptValueSender<const cString>::pop(EnvironmentHandle h) {
    return lua_tostring(h, -1);
}
/************************************************************************/
/* User data cast                                                      */
/************************************************************************/
template<>
void vscript::ScriptValueSender<void*>::push(EnvironmentHandle h, void* v) {
    lua_pushlightuserdata(h, v);
}
template<>
void* vscript::ScriptValueSender<void*>::pop(EnvironmentHandle h) {
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
