//
// REPL.h
// Vorb Engine
//
// Created by Cristian Zaloj on 20 Mar 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file REPL.h
 * @brief 
 */

#pragma once

#ifndef Vorb_REPL_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_REPL_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#include "../decorators.h"
#endif // !VORB_USING_PCH

#include "../Events.hpp"
#include "../io/MemFile.h"
#include "../RingBuffer.hpp"

#define VORB_REPL_NUM_COMMANDS_STORED 16

#define VORB_REPL_STREAM_OUT 0
#define VORB_REPL_STREAM_ERR 1

namespace vorb {
    namespace script {
        class Environment;

        class REPL {
        public:
            REPL(Environment* env = nullptr, size_t storageSize = VORB_REPL_NUM_COMMANDS_STORED);
            virtual ~REPL();

            void invokeCommand(const nString& c);
            const nString& getCommand(size_t i) const;

            void set(Environment* o = nullptr);

            Event<const cString> onStream[2];
        private:
            VORB_NON_COPYABLE(REPL);

            vorb::ring_buffer<nString> m_storedCommands;

            Environment* m_vm = nullptr;
            bool m_delete = false;

            MemFile m_streams[2];
        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_REPL_h__
