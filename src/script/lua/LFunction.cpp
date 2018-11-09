#include "stdafx.h"
#include "script/lua/LFunction.h"

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
    // just cout that error message.
    std::cout << ValueMediator<std::string>::pop(m_state) << std::endl;
}
