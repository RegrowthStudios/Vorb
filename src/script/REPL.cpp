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
    int old1 = _dup(1);
    int old2 = _dup(2);
    _dup2(_fileno(m_streams[VORB_REPL_STREAM_OUT]), 1);
    _dup2(_fileno(m_streams[VORB_REPL_STREAM_ERR]), 2);

    // Create a reader thread
    HANDLE mtxIO = CreateEvent(nullptr, TRUE, FALSE, "VORB REPL IO Lock");
    //ResetEvent(mtxIO);
    HANDLE mtxCompletion = CreateEvent(nullptr, TRUE, FALSE, "VORB REPL Completion Lock");
    //ResetEvent(mtxCompletion);
    std::thread tReader([&] () {
        while (true) {
            switch (WaitForSingleObject(mtxIO, 100)) {
            case WAIT_OBJECT_0:
                // Time to exit
                SetEvent(mtxCompletion);
                return;
            case WAIT_TIMEOUT:
                // Try to read some IO
                _dup2(old1, 1);
                _dup2(old2, 2);
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
                _dup2(_fileno(m_streams[VORB_REPL_STREAM_OUT]), 1);
                _dup2(_fileno(m_streams[VORB_REPL_STREAM_ERR]), 2);
                break;
            }
        }
    });

    // Execute command
    luaL_dostring(m_vm->getHandle(), c.c_str());
    m_storedCommands.push(c);

    // Signal and wait for reader to exit
    SignalObjectAndWait(mtxIO, mtxCompletion, INFINITE, FALSE);

    // Reset standard output
    fflush(stdout);
    fflush(stderr);
    _dup2(old1, 1);
    _dup2(old2, 2);

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
