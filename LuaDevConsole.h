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

#include <boost/circular_buffer.hpp>

#include "Events.hpp"
#include "MemFile.h"

namespace OOLUA {
    class Script;
}

namespace vorb {
    namespace ui {
#define LUA_DEV_CONSOLE_NUM_COMMANDS_STORED 16

        class LuaDevConsole {
        public:
            LuaDevConsole(size_t storageSize = LUA_DEV_CONSOLE_NUM_COMMANDS_STORED);
            virtual ~LuaDevConsole();

            void invokeCommand(const nString& c);

            const nString& getCommand(const size_t& i) const;

            union {
                Event<const cString> onStream[2];
                struct {
                    Event<const cString> onStreamOut;
                    Event<const cString> onStreamErr;
                };
            };
        private:
            boost::circular_buffer<nString> m_storedCommands;

            OOLUA::Script* m_vm;

            MemFile m_streams[2];
        };
    }
}
namespace vui = vorb::ui;

#endif // LuaDevConsole_h__