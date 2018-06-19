#include "Vorb/stdafx.h"
#include "Vorb/script/ScriptValueSenders.h"

#define LUA_COMPAT_APIINTCASTS

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

/************************************************************************/
/* Integer casts                                                        */
/************************************************************************/
#define CAST_INTEGER(TYPE) \
    TYPE vscript::ScriptValueSender<TYPE, void>::defaultValue() { \
        return 0; \
    } \
    i32 vscript::ScriptValueSender<TYPE, void>::getNumValues() { \
        return 1; \
    } \
    i32 vscript::ScriptValueSender<TYPE, void>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushinteger(h, static_cast<lua_Integer>(v)); \
        return 1; \
    } \
    TYPE vscript::ScriptValueSender<TYPE, void>::pop(EnvironmentHandle h) { \
        auto v = static_cast<TYPE>(lua_tointeger(h, -1)); \
        lua_pop(h, 1); \
        return v; \
    }
CAST_INTEGER(i8)
CAST_INTEGER(i16)
CAST_INTEGER(i32)
CAST_INTEGER(i64)
#define CAST_UNSIGNED(TYPE) \
    TYPE vscript::ScriptValueSender<TYPE, void>::defaultValue() { \
        return 0; \
    } \
    i32 vscript::ScriptValueSender<TYPE, void>::getNumValues() { \
        return 1; \
    } \
    i32 vscript::ScriptValueSender<TYPE, void>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushunsigned(h, static_cast<lua_Unsigned>(v)); \
        return 1; \
    } \
    TYPE vscript::ScriptValueSender<TYPE, void>::pop(EnvironmentHandle h) { \
        auto v = static_cast<TYPE>(lua_tounsigned(h, -1)); \
        lua_pop(h, 1); \
        return v; \
    }
CAST_UNSIGNED(ui8)
CAST_UNSIGNED(ui16)
CAST_UNSIGNED(ui32)
CAST_UNSIGNED(ui64)

/************************************************************************/
/* Number casts                                                         */
/************************************************************************/
#define CAST_NUMBER(TYPE) \
    TYPE vscript::ScriptValueSender<TYPE, void>::defaultValue() { \
        return 0; \
    } \
    i32 vscript::ScriptValueSender<TYPE, void>::getNumValues() { \
        return 1; \
    } \
    i32 vscript::ScriptValueSender<TYPE, void>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushnumber(h, static_cast<lua_Number>(v)); \
        return 1; \
    } \
    TYPE vscript::ScriptValueSender<TYPE, void>::pop(EnvironmentHandle h) { \
        auto v = static_cast<TYPE>(lua_tonumber(h, -1)); \
        lua_pop(h, 1); \
        return v; \
    }
CAST_NUMBER(f32);
CAST_NUMBER(f64);

/************************************************************************/
/* Boolean cast                                                         */
/************************************************************************/
bool vscript::ScriptValueSender<bool, void>::defaultValue() {
    return false;
}
i32 vscript::ScriptValueSender<bool, void>::getNumValues() {
    return 1;
}
i32 vscript::ScriptValueSender<bool, void>::push(EnvironmentHandle h, bool v) {
    lua_pushboolean(h, v);
    return 1;
}
bool vscript::ScriptValueSender<bool, void>::pop(EnvironmentHandle h) {
    auto v = lua_toboolean(h, -1);
    lua_pop(h, 1);
    return v != 0;
}

/************************************************************************/
/* String casts                                                         */
/************************************************************************/
nString vscript::ScriptValueSender<nString, void>::defaultValue() {
    return nString();
}
i32 vscript::ScriptValueSender<nString, void>::getNumValues() {
    return 1;
}
i32 vscript::ScriptValueSender<nString, void>::push(EnvironmentHandle h, const nString& v) {
    lua_pushstring(h, v.c_str());
    return 1;
}
nString vscript::ScriptValueSender<nString, void>::pop(EnvironmentHandle h) {
    nString v(lua_tostring(h, -1));
    lua_pop(h, 1);
    return v;
}
const cString vscript::ScriptValueSender<const cString, void>::defaultValue() {
    return nullptr;
}
i32 vscript::ScriptValueSender<const cString, void>::getNumValues() {
    return 1;
}
i32 vscript::ScriptValueSender<const cString, void>::push(EnvironmentHandle h, const cString v) {
    lua_pushstring(h, v);
    return 1;
}
const cString vscript::ScriptValueSender<const cString, void>::pop(EnvironmentHandle h) {
    auto v = lua_tostring(h, -1);
    lua_pop(h, 1);
    return v;
}
/************************************************************************/
/* User data cast                                                      */
/************************************************************************/
void* vscript::ScriptValueSender<void*, void>::defaultValue() {
    return nullptr;
}
i32 vscript::ScriptValueSender<void*, void>::getNumValues() {
    return 1;
}
i32 vscript::ScriptValueSender<void*, void>::push(EnvironmentHandle h, void* v) {
    lua_pushlightuserdata(h, v);
    return 1;
}
void* vscript::ScriptValueSender<void*, void>::pop(EnvironmentHandle h) {
    auto v = lua_touserdata(h, -1);
    lua_pop(h, 1);
    return v;
}
/************************************************************************/
/* Color4 cast                                                          */
/************************************************************************/
color4 vscript::ScriptValueSender<color4, void>::defaultValue() {
    return color4();
}
i32 vscript::ScriptValueSender<color4, void>::getNumValues() {
    return 4;
}
i32 vscript::ScriptValueSender<color4, void>::push(EnvironmentHandle h, const color4& v) {
    ScriptValueSender<ui8>::push(h, v.r);
    ScriptValueSender<ui8>::push(h, v.g);
    ScriptValueSender<ui8>::push(h, v.b);
    ScriptValueSender<ui8>::push(h, v.a);
    return 4;
}
color4 vscript::ScriptValueSender<color4, void>::pop(EnvironmentHandle h) {
    color4 v;
    v.a = ScriptValueSender<ui8>::pop(h);
    v.b = ScriptValueSender<ui8>::pop(h);
    v.g = ScriptValueSender<ui8>::pop(h);
    v.r = ScriptValueSender<ui8>::pop(h);
    return v;
}

#define TYPE_VEC2(TYPE) \
    TYPE##v2 vscript::ScriptValueSender<TYPE##v2, void>::defaultValue() { \
        return TYPE##v2(); \
    } \
    i32 vscript::ScriptValueSender<TYPE##v2, void>::getNumValues() { \
        return 2; \
    } \
    i32 vscript::ScriptValueSender<TYPE##v2, void>::push(EnvironmentHandle h, const TYPE##v2& v) { \
        ScriptValueSender<TYPE>::push(h, v[0]); \
        ScriptValueSender<TYPE>::push(h, v[1]); \
        return 2; \
    } \
    TYPE##v2 vscript::ScriptValueSender<TYPE##v2, void>::pop(EnvironmentHandle h) { \
        TYPE##v2 v; \
        v[1] = ScriptValueSender<TYPE>::pop(h); \
        v[0] = ScriptValueSender<TYPE>::pop(h); \
        return v; \
    }
#define TYPE_VEC3(TYPE) \
    TYPE##v3 vscript::ScriptValueSender<TYPE##v3, void>::defaultValue() { \
        return TYPE##v3(); \
    } \
    i32 vscript::ScriptValueSender<TYPE##v3, void>::getNumValues() { \
        return 3; \
    } \
    i32 vscript::ScriptValueSender<TYPE##v3, void>::push(EnvironmentHandle h, const TYPE##v3& v) { \
        ScriptValueSender<TYPE>::push(h, v[0]); \
        ScriptValueSender<TYPE>::push(h, v[1]); \
        ScriptValueSender<TYPE>::push(h, v[2]); \
        return 3; \
    } \
    TYPE##v3 vscript::ScriptValueSender<TYPE##v3, void>::pop(EnvironmentHandle h) { \
        TYPE##v3 v; \
        v[2] = ScriptValueSender<TYPE>::pop(h); \
        v[1] = ScriptValueSender<TYPE>::pop(h); \
        v[0] = ScriptValueSender<TYPE>::pop(h); \
        return v; \
    }
#define TYPE_VEC4(TYPE) \
    TYPE##v4 vscript::ScriptValueSender<TYPE##v4, void>::defaultValue() { \
        return TYPE##v4(); \
    } \
    i32 vscript::ScriptValueSender<TYPE##v4, void>::getNumValues() { \
        return 4; \
    } \
    i32 vscript::ScriptValueSender<TYPE##v4, void>::push(EnvironmentHandle h, const TYPE##v4& v) { \
        ScriptValueSender<TYPE>::push(h, v[0]); \
        ScriptValueSender<TYPE>::push(h, v[1]); \
        ScriptValueSender<TYPE>::push(h, v[2]); \
        ScriptValueSender<TYPE>::push(h, v[3]); \
        return 4; \
    } \
    TYPE##v4 vscript::ScriptValueSender<TYPE##v4, void>::pop(EnvironmentHandle h) { \
        TYPE##v4 v; \
        v[3] = ScriptValueSender<TYPE>::pop(h); \
        v[2] = ScriptValueSender<TYPE>::pop(h); \
        v[1] = ScriptValueSender<TYPE>::pop(h); \
        v[0] = ScriptValueSender<TYPE>::pop(h); \
        return v; \
    }

TYPE_VEC2(i8)
TYPE_VEC2(i16)
TYPE_VEC2(i32)
TYPE_VEC2(i64)
TYPE_VEC2(ui8)
TYPE_VEC2(ui16)
TYPE_VEC2(ui32)
TYPE_VEC2(ui64)
TYPE_VEC2(f32)
TYPE_VEC2(f64)

TYPE_VEC3(i8)
TYPE_VEC3(i16)
TYPE_VEC3(i32)
TYPE_VEC3(i64)
TYPE_VEC3(ui8)
TYPE_VEC3(ui16)
TYPE_VEC3(ui32)
TYPE_VEC3(ui64)
TYPE_VEC3(f32)
TYPE_VEC3(f64)

TYPE_VEC4(i8)
TYPE_VEC4(i16)
TYPE_VEC4(i32)
TYPE_VEC4(i64)
TYPE_VEC4(ui8)
TYPE_VEC4(ui16)
TYPE_VEC4(ui32)
TYPE_VEC4(ui64)
TYPE_VEC4(f32)
TYPE_VEC4(f64)
