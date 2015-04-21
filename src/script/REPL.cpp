#include "stdafx.h"
#include "script/REPL.h"

extern "C" {
#include <OOLua/lua/lua.h>
#include <OOLua/lua/lualib.h>
#include <OOLua/lua/lauxlib.h>
}

#include "script/Environment.h"

vscript::REPL::REPL(vscript::Environment* env /*= nullptr*/, size_t storageSize /*= VORB_REPL_NUM_COMMANDS_STORED*/) :
    m_storedCommands(storageSize) {
    set(env);
    onStream[0] = Event<const cString>(this);
    onStream[1] = Event<const cString>(this);
}
vscript::REPL::~REPL() {
    if (m_vm && m_delete) delete m_vm;
}

void vscript::REPL::invokeCommand(const nString& c) {
    i64 fpos[2] = {
        ftell(m_streams[0]),
        ftell(m_streams[1])
    };

    // Redirect standard output
    FILE old1 = *stdout;
    FILE old2 = *stderr;
    *stdout = *m_streams[VORB_REPL_STREAM_OUT];
    *stderr = *m_streams[VORB_REPL_STREAM_ERR];

    luaL_dostring(m_vm->getHandle(), c.c_str());
    m_storedCommands.push(c);

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
const nString& vscript::REPL::getCommand(size_t i) const {
    return m_storedCommands.at(i);
}

void vscript::REPL::set(Environment* o /* = nullptr */) {
    if (m_vm && m_delete) delete m_vm;
    m_delete = o == nullptr;
    m_vm = m_delete ? new Environment : o;
}
