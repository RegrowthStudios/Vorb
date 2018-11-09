#include "stdafx.h"
#include "script/lua/ValueMediator.h"

/************************************************************************/
/* Integer Cast                                                         */
/************************************************************************/
#define MEDIATE_INTEGER(TYPE)                                                                                   \
        TYPE vscript::lua::ValueMediator<TYPE, void>::defaultValue() {                                          \
            return 0;                                                                                           \
        }                                                                                                       \
        i32 vscript::lua::ValueMediator<TYPE, void>::getValueCount() {                                          \
            return 1;                                                                                           \
        }                                                                                                       \
        i32 vscript::lua::ValueMediator<TYPE, void>::push(Handle state, TYPE value) {                           \
            lua_pushinteger(state, static_cast<lua_Integer>(value));                                            \
            return 1;                                                                                           \
        }                                                                                                       \
        TYPE vscript::lua::ValueMediator<TYPE, void>::pop(Handle state) {                                       \
            TYPE value = static_cast<TYPE>(lua_tointeger(state, -1));                                           \
            lua_pop(state, 1);                                                                                  \
            return value;                                                                                       \
        }                                                                                                       \
        bool vscript::lua::ValueMediator<TYPE, void>::tryPop(Handle state, OUT TYPE& value) {                   \
            if (lua_isinteger(state, -1)) {                                                                     \
                value = pop(state);                                                                             \
                return true;                                                                                    \
            }                                                                                                   \
            return false;                                                                                       \
        }                                                                                                       \
        TYPE vscript::lua::ValueMediator<TYPE, void>::retrieve(Handle state, ui32 index) {                      \
            TYPE value = static_cast<TYPE>(lua_tointeger(state, index));                                        \
            lua_remove(state, index);                                                                           \
            return value;                                                                                       \
        }                                                                                                       \
        bool vscript::lua::ValueMediator<TYPE, void>::tryRetrieve(Handle state, ui32 index, OUT TYPE& value) {  \
            if (lua_isinteger(state, index)) {                                                                  \
                value = retrieve(state, index);                                                                 \
                return true;                                                                                    \
            }                                                                                                   \
            return false;                                                                                       \
        }
MEDIATE_INTEGER(i8)
MEDIATE_INTEGER(i16)
MEDIATE_INTEGER(i32)
MEDIATE_INTEGER(i64)
MEDIATE_INTEGER(ui8)
MEDIATE_INTEGER(ui16)
MEDIATE_INTEGER(ui32)
MEDIATE_INTEGER(ui64)
#undef MEDIATE_INTEGER

/************************************************************************/
/* Floating-point Cast                                                  */
/************************************************************************/
#define MEDIATE_FLOATING_POINT(TYPE)                                                                       \
    TYPE vscript::lua::ValueMediator<TYPE, void>::defaultValue() {                                         \
        return 0;                                                                                          \
    }                                                                                                      \
    i32 vscript::lua::ValueMediator<TYPE, void>::getValueCount() {                                         \
        return 1;                                                                                          \
    }                                                                                                      \
    i32 vscript::lua::ValueMediator<TYPE, void>::push(Handle state, TYPE value) {                          \
        lua_pushnumber(state, static_cast<lua_Number>(value));                                             \
        return 1;                                                                                          \
    }                                                                                                      \
    TYPE vscript::lua::ValueMediator<TYPE, void>::pop(Handle state) {                                      \
        TYPE v = static_cast<TYPE>(lua_tonumber(state, -1));                                               \
        lua_pop(state, 1);                                                                                 \
        return v;                                                                                          \
    }                                                                                                      \
    bool vscript::lua::ValueMediator<TYPE, void>::tryPop(Handle state, OUT TYPE& value) {                  \
        if (lua_isnumber(state, -1)) {                                                                     \
            value = pop(state);                                                                            \
            return true;                                                                                   \
        }                                                                                                  \
        return false;                                                                                      \
    }                                                                                                      \
    TYPE vscript::lua::ValueMediator<TYPE, void>::retrieve(Handle state, ui32 index) {                     \
        TYPE value = static_cast<TYPE>(lua_tonumber(state, index));                                        \
        lua_remove(state, index);                                                                          \
        return value;                                                                                      \
    }                                                                                                      \
    bool vscript::lua::ValueMediator<TYPE, void>::tryRetrieve(Handle state, ui32 index, OUT TYPE& value) { \
        if (lua_isnumber(state, index)) {                                                                  \
            value = retrieve(state, index);                                                                \
            return true;                                                                                   \
        }                                                                                                  \
        return false;                                                                                      \
    }
MEDIATE_FLOATING_POINT(f32);
MEDIATE_FLOATING_POINT(f64);
#undef MEDIATE_FLOATING_POINT

/************************************************************************/
/* Boolean Cast                                                         */
/************************************************************************/
bool vscript::lua::ValueMediator<bool, void>::defaultValue() {
    return false;
}

i32 vscript::lua::ValueMediator<bool, void>::getValueCount() {
    return 1;
}

i32 vscript::lua::ValueMediator<bool, void>::push(Handle state, bool value) {
    lua_pushboolean(state, value);
    return 1;
}

bool vscript::lua::ValueMediator<bool, void>::pop(Handle state) {
    int value = lua_toboolean(state, -1);
    lua_pop(state, 1);
    return value != 0;
}

bool vscript::lua::ValueMediator<bool, void>::tryPop(Handle state, OUT bool& value) {
    if (lua_isboolean(state, -1)) {
        value = pop(state);
        return true;
    }
    return false;
}

bool vscript::lua::ValueMediator<bool, void>::retrieve(Handle state, ui32 index) {
    int value = lua_toboolean(state, index);
    lua_remove(state, index);
    return value != 0;
}

bool vscript::lua::ValueMediator<bool, void>::tryRetrieve(Handle state, ui32 index, OUT bool& value) {
    if (lua_isboolean(state, index)) {
        value = retrieve(state, index);
        return true;
    }
    return false;
}

/************************************************************************/
/* String Cast                                                          */
/************************************************************************/
nString vscript::lua::ValueMediator<nString, void>::defaultValue() {
    return nString();
}

i32 vscript::lua::ValueMediator<nString, void>::getValueCount() {
    return 1;
}

i32 vscript::lua::ValueMediator<nString, void>::push(Handle state, const nString& value) {
    lua_pushstring(state, value.c_str());
    return 1;
}

nString vscript::lua::ValueMediator<nString, void>::pop(Handle state) {
    nString value(lua_tostring(state, -1));
    lua_pop(state, 1);
    return value;
}

bool vscript::lua::ValueMediator<nString, void>::tryPop(Handle state, OUT nString& value) {
    if (lua_isstring(state, -1)) {
        value = pop(state);
        return true;
    }
    return false;
}

nString vscript::lua::ValueMediator<nString, void>::retrieve(Handle state, ui32 index) {
    nString value(lua_tostring(state, index));
    lua_remove(state, index);
    return value;
}

bool vscript::lua::ValueMediator<nString, void>::tryRetrieve(Handle state, ui32 index, OUT nString& value) {
    if (lua_isstring(state, index)) {
        value = retrieve(state, index);
        return true;
    }
    return false;
}

/************************************************************************/
/* C-String Cast                                                        */
/************************************************************************/
const cString vscript::lua::ValueMediator<const cString, void>::defaultValue() {
    return nullptr;
}

i32 vscript::lua::ValueMediator<const cString, void>::getValueCount() {
    return 1;
}

i32 vscript::lua::ValueMediator<const cString, void>::push(Handle state, const cString value) {
    lua_pushstring(state, value);
    return 1;
}

const cString vscript::lua::ValueMediator<const cString, void>::pop(Handle state) {
    const cString value = lua_tostring(state, -1);
    lua_pop(state, 1);
    return value;
}

bool vscript::lua::ValueMediator<const cString, void>::tryPop(Handle state, OUT const cString& value) {
    if (lua_isstring(state, -1)) {
        value = pop(state);
        return true;
    }
    return false;
}

const cString vscript::lua::ValueMediator<const cString, void>::retrieve(Handle state, ui32 index) {
    const cString value = lua_tostring(state, index);
    lua_remove(state, index);
    return value;
}

bool vscript::lua::ValueMediator<const cString, void>::tryRetrieve(Handle state, ui32 index, OUT const cString& value) {
    if (lua_isstring(state, index)) {
        value = retrieve(state, index);
        return true;
    }
    return false;
}

/************************************************************************/
/* User Data Cast                                                       */
/************************************************************************/
void* vscript::lua::ValueMediator<void*, void>::defaultValue() {
    return nullptr;
}

i32 vscript::lua::ValueMediator<void*, void>::getValueCount() {
    return 1;
}

i32 vscript::lua::ValueMediator<void*, void>::push(Handle state, void* value) {
    lua_pushlightuserdata(state, value);
    return 1;
}

void* vscript::lua::ValueMediator<void*, void>::pop(Handle state) {
    void* value = lua_touserdata(state, -1);
    lua_pop(state, 1);
    return value;
}

bool vscript::lua::ValueMediator<void*, void>::tryPop(Handle state, OUT void*& value) {
    if (lua_islightuserdata(state, -1)) {
        value = pop(state);
        return true;
    }
    return false;
}

void* vscript::lua::ValueMediator<void*, void>::retrieve(Handle state, ui32 index) {
    void* value = lua_touserdata(state, index);
    lua_remove(state, index);
    return value;
}

bool vscript::lua::ValueMediator<void*, void>::tryRetrieve(Handle state, ui32 index, OUT void*& value) {
    if (lua_islightuserdata(state, index)) {
        value = retrieve(state, index);
        return true;
    }
    return false;
}

/************************************************************************/
/* Color4 Cast                                                          */
/************************************************************************/

color4 vscript::lua::ValueMediator<color4, void>::defaultValue() {
    return color4();
}

i32 vscript::lua::ValueMediator<color4, void>::getNumValues() {
    return 4;
}

i32 vscript::lua::ValueMediator<color4, void>::push(Handle state, const color4& value) {
    ValueMediator<ui8>::push(state, value.r);
    ValueMediator<ui8>::push(state, value.g);
    ValueMediator<ui8>::push(state, value.b);
    ValueMediator<ui8>::push(state, value.a);
    return 4;
}

color4 vscript::lua::ValueMediator<color4, void>::pop(Handle state) {
    color4 value;
    value.a = ValueMediator<ui8>::pop(state);
    value.b = ValueMediator<ui8>::pop(state);
    value.g = ValueMediator<ui8>::pop(state);
    value.r = ValueMediator<ui8>::pop(state);
    return value;
}

bool vscript::lua::ValueMediator<color4, void>::tryPop(Handle state, OUT color4& value) {
    if (lua_isinteger(state, -1) &&
        lua_isinteger(state, -2) &&
        lua_isinteger(state, -3) &&
        lua_isinteger(state, -4)) {
        value = pop(state);
        return true;
    }
    return false;
}
color4 vscript::lua::ValueMediator<color4, void>::retrieve(Handle state, ui32 index) {
    if (index < 4) return color4();
    color4 value;
    value.a = lua_tointeger(state, index);
    value.b = lua_tointeger(state, index - 1);
    value.g = lua_tointeger(state, index - 2);
    value.r = lua_tointeger(state, index - 3);
    lua_remove(state, index);
    lua_remove(state, index - 1);
    lua_remove(state, index - 2);
    lua_remove(state, index - 3);
    return value;
}

bool vscript::lua::ValueMediator<color4, void>::tryRetrieve(Handle state, ui32 index, OUT color4& value) {
    if (lua_isinteger(state, index) &&
        lua_isinteger(state, index - 1) &&
        lua_isinteger(state, index - 2) &&
        lua_isinteger(state, index - 3)) {
        value = retrieve(state, index);
        return true;
    }
    return false;
}


#define INT_TYPE_VEC2(TYPE)                                                                                                             \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::defaultValue() {                                                              \
        return TYPE##v2();                                                                                                              \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v2, void>::getNumValues() {                                                                   \
        return 2;                                                                                                                       \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v2, void>::push(Handle state, const TYPE##v2& value) {                                        \
        ScriptValueSender<TYPE>::push(state, value[0]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[1]);                                                                                 \
        return 2;                                                                                                                       \
    }                                                                                                                                   \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::pop(Handle state) {                                                           \
        TYPE##v2 value;                                                                                                                 \
        value[1] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[0] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::tryPop(Handle state, OUT TYPE##v2& value) {                                   \
        if (lua_isinteger(state, -1) &&                                                                                                 \
            lua_isinteger(state, -2)) {                                                                                                 \
            value = pop(state);                                                                                                         \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::retrieve(Handle state, ui32 index) {                                          \
        if (index < 2) return TYPE##v2();                                                                                               \
        TYPE##v2 value;                                                                                                                 \
        value[0] = lua_tointeger(state, index);                                                                                         \
        value[1] = lua_tointeger(state, index - 1);                                                                                     \
        lua_remove(state, index);                                                                                                       \
        lua_remove(state, index - 1);                                                                                                   \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::tryRetrieve(Handle state, ui32 index, OUT TYPE##v2& value) {                  \
        if (lua_isinteger(state, index) &&                                                                                              \
            lua_isinteger(state, index - 1)) {                                                                                          \
            value = retrieve(state, index);                                                                                             \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }
INT_TYPE_VEC2(i8)
INT_TYPE_VEC2(i16)
INT_TYPE_VEC2(i32)
INT_TYPE_VEC2(i64)
INT_TYPE_VEC2(ui8)
INT_TYPE_VEC2(ui16)
INT_TYPE_VEC2(ui32)
INT_TYPE_VEC2(ui64)
#undef INT_TYPE_VEC2
#define FLT_TYPE_VEC2(TYPE)                                                                                                             \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::defaultValue() {                                                              \
        return TYPE##v2();                                                                                                              \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v2, void>::getNumValues() {                                                                   \
        return 2;                                                                                                                       \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v2, void>::push(Handle state, const TYPE##v2& value) {                                        \
        ScriptValueSender<TYPE>::push(state, value[0]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[1]);                                                                                 \
        return 2;                                                                                                                       \
    }                                                                                                                                   \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::pop(Handle state) {                                                           \
        TYPE##v2 value;                                                                                                                 \
        value[1] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[0] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::tryPop(Handle state, OUT TYPE##v2& value) {                                   \
        if (lua_isnumber(state, -1) &&                                                                                                  \
            lua_isnumber(state, -2)) {                                                                                                  \
            value = pop(state);                                                                                                         \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::retrieve(Handle state, ui32 index) {                                          \
        if (index < 2) return TYPE##v2();                                                                                               \
        TYPE##v2 value;                                                                                                                 \
        value[0] = lua_tonumber(state, index);                                                                                          \
        value[1] = lua_tonumber(state, index - 1);                                                                                      \
        lua_remove(state, index);                                                                                                       \
        lua_remove(state, index - 1);                                                                                                   \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v2 vscript::lua::ValueMediator<TYPE##v2, void>::tryRetrieve(Handle state, ui32 index, OUT TYPE##v2& value) {                  \
        if (lua_isnumber(state, index) &&                                                                                               \
            lua_isnumber(state, index - 1)) {                                                                                           \
            value = retrieve(state, index);                                                                                             \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }
FLT_TYPE_VEC2(f32)
FLT_TYPE_VEC2(f64)
#undef FLT_TYPE_VEC2

#define INT_TYPE_VEC3(TYPE)                                                                                                             \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::defaultValue() {                                                              \
        return TYPE##v3();                                                                                                              \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v3, void>::getNumValues() {                                                                   \
        return 3;                                                                                                                       \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v3, void>::push(Handle state, const TYPE##v3& value) {                                        \
        ScriptValueSender<TYPE>::push(state, value[0]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[1]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[2]);                                                                                 \
        return 3;                                                                                                                       \
    }                                                                                                                                   \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::pop(Handle state) {                                                           \
        TYPE##v3 value;                                                                                                                 \
        value[2] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[1] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[0] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::tryPop(Handle state, OUT TYPE##v3& value) {                                   \
        if (lua_isinteger(state, -1) &&                                                                                                 \
            lua_isinteger(state, -2) &&                                                                                                 \
            lua_isinteger(state, -3)) {                                                                                                 \
            value = pop(state);                                                                                                         \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::retrieve(Handle state, ui32 index) {                                          \
        if (index < 3) return TYPE##v3();                                                                                               \
        TYPE##v3 value;                                                                                                                 \
        value[0] = lua_tointeger(state, index);                                                                                         \
        value[1] = lua_tointeger(state, index - 1);                                                                                     \
        value[2] = lua_tointeger(state, index - 2);                                                                                     \
        lua_remove(state, index);                                                                                                       \
        lua_remove(state, index - 1);                                                                                                   \
        lua_remove(state, index - 2);                                                                                                   \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::tryRetrieve(Handle state, ui32 index, OUT TYPE##v3& value) {                  \
        if (lua_isinteger(state, index) &&                                                                                              \
            lua_isinteger(state, index - 1)                                                                                             \
            lua_isinteger(state, index - 2)) {                                                                                          \
            value = retrieve(state, index);                                                                                             \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }
INT_TYPE_VEC3(i8)
INT_TYPE_VEC3(i16)
INT_TYPE_VEC3(i32)
INT_TYPE_VEC3(i64)
INT_TYPE_VEC3(ui8)
INT_TYPE_VEC3(ui16)
INT_TYPE_VEC3(ui32)
INT_TYPE_VEC3(ui64)
#undef INT_TYPE_VEC3
#define FLT_TYPE_VEC3(TYPE)                                                                                                             \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::defaultValue() {                                                              \
        return TYPE##v3();                                                                                                              \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v3, void>::getNumValues() {                                                                   \
        return 3;                                                                                                                       \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v3, void>::push(Handle state, const TYPE##v3& value) {                                        \
        ScriptValueSender<TYPE>::push(state, value[0]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[1]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[2]);                                                                                 \
        return 3;                                                                                                                       \
    }                                                                                                                                   \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::pop(Handle state) {                                                           \
        TYPE##v3 value;                                                                                                                 \
        value[2] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[1] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[0] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::tryPop(Handle state, OUT TYPE##v3& value) {                                   \
        if (lua_isnumber(state, -1) &&                                                                                                  \
            lua_isnumber(state, -2) &&                                                                                                  \
            lua_isnumber(state, -3)) {                                                                                                  \
            value = pop(state);                                                                                                         \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::retrieve(Handle state, ui32 index) {                                          \
        if (index < 3) return TYPE##v3();                                                                                               \
        TYPE##v3 value;                                                                                                                 \
        value[0] = lua_tonumber(state, index);                                                                                          \
        value[1] = lua_tonumber(state, index - 1);                                                                                      \
        value[2] = lua_tonumber(state, index - 2);                                                                                      \
        lua_remove(state, index);                                                                                                       \
        lua_remove(state, index - 1);                                                                                                   \
        lua_remove(state, index - 2);                                                                                                   \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v3 vscript::lua::ValueMediator<TYPE##v3, void>::tryRetrieve(Handle state, ui32 index, OUT TYPE##v3& value) {                  \
        if (lua_isnumber(state, index) &&                                                                                               \
            lua_isnumber(state, index - 1)                                                                                              \
            lua_isnumber(state, index - 2)) {                                                                                           \
            value = retrieve(state, index);                                                                                             \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }
FLT_TYPE_VEC3(f32)
FLT_TYPE_VEC3(f64)
#undef FLT_TYPE_VEC3

#define INT_TYPE_VEC4(TYPE)                                                                                                             \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::defaultValue() {                                                              \
        return TYPE##v4();                                                                                                              \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v4, void>::getNumValues() {                                                                   \
        return 4;                                                                                                                       \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v4, void>::push(Handle state, const TYPE##v4& value) {                                        \
        ScriptValueSender<TYPE>::push(state, value[0]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[1]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[2]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[3]);                                                                                 \
        return 4;                                                                                                                       \
    }                                                                                                                                   \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::pop(Handle state) {                                                           \
        TYPE##v4 value;                                                                                                                 \
        value[3] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[2] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[1] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[0] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::tryPop(Handle state, OUT TYPE##v4& value) {                                   \
        if (lua_isinteger(state, -1) &&                                                                                                 \
            lua_isinteger(state, -2) &&                                                                                                 \
            lua_isinteger(state, -3) &&                                                                                                 \
            lua_isinteger(state, -4)) {                                                                                                 \
            value = pop(state);                                                                                                         \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::retrieve(Handle state, ui32 index) {                                          \
        if (index < 3) return TYPE##v4();                                                                                               \
        TYPE##v4 value;                                                                                                                 \
        value[0] = lua_tointeger(state, index);                                                                                         \
        value[1] = lua_tointeger(state, index - 1);                                                                                     \
        value[2] = lua_tointeger(state, index - 2);                                                                                     \
        value[3] = lua_tointeger(state, index - 3);                                                                                     \
        lua_remove(state, index);                                                                                                       \
        lua_remove(state, index - 1);                                                                                                   \
        lua_remove(state, index - 2);                                                                                                   \
        lua_remove(state, index - 3);                                                                                                   \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::tryRetrieve(Handle state, ui32 index, OUT TYPE##v4& value) {                  \
        if (lua_isinteger(state, index) &&                                                                                              \
            lua_isinteger(state, index - 1)                                                                                             \
            lua_isinteger(state, index - 2)                                                                                             \
            lua_isinteger(state, index - 3)) {                                                                                          \
            value = retrieve(state, index);                                                                                             \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }
INT_TYPE_VEC4(i8)
INT_TYPE_VEC4(i16)
INT_TYPE_VEC4(i32)
INT_TYPE_VEC4(i64)
INT_TYPE_VEC4(ui8)
INT_TYPE_VEC4(ui16)
INT_TYPE_VEC4(ui32)
INT_TYPE_VEC4(ui64)
#undef INT_TYPE_VEC4
#define FLT_TYPE_VEC4(TYPE)                                                                                                             \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::defaultValue() {                                                              \
        return TYPE##v4();                                                                                                              \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v4, void>::getNumValues() {                                                                   \
        return 4;                                                                                                                       \
    }                                                                                                                                   \
    i32 vscript::lua::ValueMediator<TYPE##v4, void>::push(Handle state, const TYPE##v4& value) {                                        \
        ScriptValueSender<TYPE>::push(state, value[0]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[1]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[2]);                                                                                 \
        ScriptValueSender<TYPE>::push(state, value[3]);                                                                                 \
        return 4;                                                                                                                       \
    }                                                                                                                                   \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::pop(Handle state) {                                                           \
        TYPE##v4 value;                                                                                                                 \
        value[3] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[2] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[1] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        value[0] = ScriptValueSender<TYPE>::pop(state);                                                                                 \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::tryPop(Handle state, OUT TYPE##v4& value) {                                   \
        if (lua_isnumber(state, -1) &&                                                                                                  \
            lua_isnumber(state, -2) &&                                                                                                  \
            lua_isnumber(state, -3) &&                                                                                                  \
            lua_isnumber(state, -4)) {                                                                                                  \
            value = pop(state);                                                                                                         \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::retrieve(Handle state, ui32 index) {                                          \
        if (index < 3) return TYPE##v4();                                                                                               \
        TYPE##v4 value;                                                                                                                 \
        value[0] = lua_tonumber(state, index);                                                                                          \
        value[1] = lua_tonumber(state, index - 1);                                                                                      \
        value[2] = lua_tonumber(state, index - 2);                                                                                      \
        value[3] = lua_tonumber(state, index - 3);                                                                                      \
        lua_remove(state, index);                                                                                                       \
        lua_remove(state, index - 1);                                                                                                   \
        lua_remove(state, index - 2);                                                                                                   \
        lua_remove(state, index - 3);                                                                                                   \
        return value;                                                                                                                   \
    }                                                                                                                                   \
    TYPE##v4 vscript::lua::ValueMediator<TYPE##v4, void>::tryRetrieve(Handle state, ui32 index, OUT TYPE##v4& value) {                  \
        if (lua_isnumber(state, index) &&                                                                                               \
            lua_isnumber(state, index - 1)                                                                                              \
            lua_isnumber(state, index - 2)                                                                                              \
            lua_isnumber(state, index - 3)) {                                                                                           \
            value = retrieve(state, index);                                                                                             \
            return true;                                                                                                                \
        }                                                                                                                               \
        return false;                                                                                                                   \
    }
FLT_TYPE_VEC4(f32)
FLT_TYPE_VEC4(f64)
#undef FLT_TYPE_VEC4
