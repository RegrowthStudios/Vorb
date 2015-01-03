#include "stdafx.h"
#include "LuaDevConsole.h"

#include <OOLua/oolua.h>
extern "C" {
    #include <OOLua\lua\lua.h>
}

vui::LuaDevConsole::LuaDevConsole(size_t storageSize /*= LUA_DEV_CONSOLE_NUM_COMMANDS_STORED*/) :
    m_storedCommands(storageSize) {
    m_vm = new OOLUA::Script;
    onStream[0] = Event<const cString>(this);
    onStream[1] = Event<const cString>(this);
}
vui::LuaDevConsole::~LuaDevConsole() {
    delete m_vm;
}

void vorb::ui::LuaDevConsole::invokeCommand(const nString& c) {
    i64 fpos[2] = {
        ftell(m_streams[0]),
        ftell(m_streams[1])
    };
    
    // Redirect standard output
    FILE old1 = *stdout;
    FILE old2 = *stderr;
    *stdout = *m_streams[DEV_CONSOLE_STREAM_OUT];
    *stderr = *m_streams[DEV_CONSOLE_STREAM_ERR];

    m_vm->run_chunk(c);
    m_storedCommands.push_back(c);

    // Reset standard output
    *stdout = old1;
    *stderr = old2;

    // Send data from output (if any)
    for (size_t i = 0; i < 2; i++) {
        size_t fs = (size_t)(ftell(m_streams[i]) - fpos[i]);
        if (fs != 0) {
            // Output was modified
            char* s = new char[fs + 1];
            fseek(m_streams[i], 0, SEEK_SET);
            fread(s, 1, fs, m_streams[i]);
            s[fs] = '\0';
            onStream[i](s);
            delete[] s;
            rewind(m_streams[i]);
        }
    }
}

const nString& vorb::ui::LuaDevConsole::getCommand(const size_t& i) const {
    return m_storedCommands[i];
}
