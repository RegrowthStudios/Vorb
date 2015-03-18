//
// ScriptValueSenders.h
// Vorb Engine
//
// Created by Cristian Zaloj on 18 Mar 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ScriptValueSenders.h
 * @brief 
 * 
 * 
 */

#pragma once

#ifndef Vorb_ScriptValueSenders_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ScriptValueSenders_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

struct lua_State;

namespace vorb {
    namespace script {
        typedef lua_State* EnvironmentHandle;

        template<typename T> struct ScriptValueSender;

#define SCRIPT_SENDER(TYPE)
        template<typename TYPE> \
        struct ScriptValueSender { \
        public: \
                static void push(EnvironmentHandle h, TYPE v); \
                static TYPE pop(EnvironmentHandle h); \
        }
        SCRIPT_SENDER(i8);
        SCRIPT_SENDER(i16);
        SCRIPT_SENDER(i32);
        SCRIPT_SENDER(i64);
        SCRIPT_SENDER(ui8);
        SCRIPT_SENDER(ui16);
        SCRIPT_SENDER(ui32);
        SCRIPT_SENDER(ui64);
        SCRIPT_SENDER(f32);
        SCRIPT_SENDER(f64);
        SCRIPT_SENDER(bool);
        SCRIPT_SENDER(nString);
        SCRIPT_SENDER(const cString);
        SCRIPT_SENDER(void*);
    }
}

#endif // !Vorb_ScriptValueSenders_h__
