///
/// LuaDevConsole.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 12 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef LuaDevConsole_h__
#define LuaDevConsole_h__


#include "Events.hpp"
#include "MemFile.h"
#include "RingBuffer.hpp"

namespace OOLUA {
    class Script;
}

namespace vorb {
    namespace ui {
#define LUA_DEV_CONSOLE_NUM_COMMANDS_STORED 16

#define DEV_CONSOLE_STREAM_OUT 0
#define DEV_CONSOLE_STREAM_ERR 1

        class LuaDevConsole {
        public:
            LuaDevConsole(size_t storageSize = LUA_DEV_CONSOLE_NUM_COMMANDS_STORED);
            virtual ~LuaDevConsole();

            void invokeCommand(const nString& c);

            const nString& getCommand(const size_t& i) const;

            Event<const cString> onStream[2];
        private:
            vorb::ring_buffer<nString> m_storedCommands;

            OOLUA::Script* m_vm;

            MemFile m_streams[2];
        };
    }
}
namespace vui = vorb::ui;

#endif // LuaDevConsole_h__