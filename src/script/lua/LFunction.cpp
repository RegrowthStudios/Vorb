#include "Vorb/stdafx.h"
#include "Vorb/script/lua/Environment.h"
#include "Vorb/script/lua/LFunction.h"

#include <cstdio>

const vscript::lua::LFunction vscript::lua::LFunction::Nil = vscript::lua::LFunction();

vscript::lua::LFunction::~LFunction() {
    if (m_state != nullptr) {
        // Get script function table and unreference the function wrapped by this instance.
        lua_getfield(m_state, LUA_REGISTRYINDEX, SCRIPT_FUNCTION_TABLE);
        luaL_unref(m_state, -1, m_index);
    }
    m_state = nullptr;
}

bool vscript::lua::LFunction::isNil() const {
    return *this == Nil;
}

bool vscript::lua::LFunction::operator==(const LFunction & rhs) const {
    return (m_index == rhs.m_index) && (m_state == rhs.m_state);
}

void vscript::lua::LFunction::dumpErrors() const {
    // lua_pcall strictly only adds one error at most to the stack on failure,
    // just ouput that error message.
    printf("Error calling Lua Function: %s.\n", ValueMediator<const cString>::pop(m_state));
}
