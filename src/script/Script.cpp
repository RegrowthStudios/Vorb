#include <stdafx.h>
#include "script/Script.h"

extern "C" {
#include "OOLua/lua/lua.h"
#include "OOLua/lua/lualib.h"
#include "OOLua/lua/lauxlib.h"
}

#define CAST_INTEGER(TYPE) \
    template<> \
    void vscript::ScriptValueSender<TYPE>::push(EnvironmentHandle h, TYPE v) { \
        lua_pushinteger(h, static_cast<lua_Integer>(v)); \
    } \
    template<> \
    TYPE vscript::ScriptValueSender<TYPE>::pop(EnvironmentHandle h) { \
        return static_cast<TYPE>(lua_tointeger(h, -1)); \
    }
CAST_INTEGER(i32)
CAST_INTEGER(ui32)
CAST_INTEGER(i64)
CAST_INTEGER(ui64)

void vscript::impl::pushToTop(EnvironmentHandle h, const Function& f) {
    lua_getglobal(h, f.getName().c_str());
}

void vscript::impl::call(EnvironmentHandle h, size_t n, size_t r) {
    lua_pcall(h, n, r, 0);
}
