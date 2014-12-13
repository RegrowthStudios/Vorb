#include "stdafx.h"
#include "LuaDevConsole.h"

#include <lua/oolua.h>
extern "C" {
    #include <lua\lua\lua.h>
}

vui::LuaDevConsole::LuaDevConsole(size_t storageSize /*= LUA_DEV_CONSOLE_NUM_COMMANDS_STORED*/) :
    m_storedCommands(storageSize) {
    m_vm = new OOLUA::Script;
}
vui::LuaDevConsole::~LuaDevConsole() {
    delete m_vm;
}

void vorb::ui::LuaDevConsole::invokeCommand(const nString& c) {
    i64 fpos[3] = {
        ftell(m_streams[0]),
        ftell(m_streams[1]),
        ftell(m_streams[2])
    };
    
    // Redirect standard output
    FILE old0 = *stdin;
    FILE old1 = *stdout;
    FILE old2 = *stderr;
    *stdin = *m_streams[0];
    *stdout = *m_streams[1];
    *stderr = *m_streams[2];

    m_vm->run_chunk(c);

    // Reset standard output
    *stdin = old0;
    *stdout = old1;
    *stderr = old2;

    for (size_t i = 0; i < 3; i++) {
        fpos[i] = ftell(m_streams[i]) - fpos[i];
        if (fpos[i] != 0) {

        }
        rewind(m_streams[0]);
    }

}

const nString& vorb::ui::LuaDevConsole::getCommand(const size_t& i) const {
    return m_storedCommands[i];
}
