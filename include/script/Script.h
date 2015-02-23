//
// Script.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Script.h
 * @brief Operations that can be performed on scripting objects.
 */

#pragma once

#ifndef Vorb_Script_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Script_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Environment.h"

namespace vorb {
    namespace script {
        class Environment;
        class Function;

        template<typename T> struct ScriptValueSender;

#define SCRIPT_SENDER(TYPE)
        template<typename TYPE> \
        struct ScriptValueSender { \
        public: \
                static void push(EnvironmentHandle h, TYPE v); \
                static TYPE pop(EnvironmentHandle h); \
        }
        SCRIPT_SENDER(i32);
        SCRIPT_SENDER(ui32);
        SCRIPT_SENDER(i64);
        SCRIPT_SENDER(ui64);

        namespace impl {
            void pushToTop(EnvironmentHandle h, const Function& f);
            void call(EnvironmentHandle h, size_t n, size_t r);

            template<typename Arg>
            void pushArgs(EnvironmentHandle h, Arg a) {
                ScriptValueSender<Arg>::push(h, a);
            }
            template<typename Arg, typename... Args>
            void pushArgs(EnvironmentHandle h, Arg a, Args... other) {
                ScriptValueSender<Arg>::push(h, a);
                pushArgs<Args...>(h, other...);
            }

            template<typename Arg>
            Arg popValue(EnvironmentHandle h) {
                return ScriptValueSender<Arg>::pop(h);
            }
        }

        template<typename... Args>
        bool call(Environment& env, const nString& f, Args... args) {
            Function& scr = env[f];
            EnvironmentHandle hnd = env.getHandle();

            impl::pushToTop(hnd, scr);
            impl::pushArgs<Args...>(hnd, args...);
            impl::call(hnd, sizeof...(Args), 0);
            return true;
        }
        template<typename Ret, typename... Args>
        bool callReturn(Environment& env, const nString& f, OUT Ret* retValue, Args... args) {
            Function& scr = env[f];
            EnvironmentHandle hnd = env.getHandle();

            impl::pushToTop(hnd, scr);
            impl::pushArgs<Args...>(hnd, args...);
            impl::call(hnd, sizeof...(Args), 1);
            *retValue = impl::popValue<Ret>(hnd);
            return true;
        }
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_Script_h__
