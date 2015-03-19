#include <stdafx.h>
#include "script/Script.h"

extern "C" {
#include "OOLua/lua/lua.h"
#include "OOLua/lua/lualib.h"
#include "OOLua/lua/lauxlib.h"
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
