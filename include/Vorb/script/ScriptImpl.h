//
// ScriptImpl.h
// Vorb Engine
//
// Created by Cristian Zaloj on 23 Mar 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ScriptImpl.h
 * @brief 
 */

#pragma once

#ifndef Vorb_ScriptImpl_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ScriptImpl_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "ScriptValueSenders.h"

namespace vorb {
    namespace script {
        class Function;

        namespace impl {
            /*! @brief Dumps the scripting environment's stack contents to the console
             * 
             * @pre: The handle must a valid handle
             * @param h: Environment handle
             */
            void dumpStack(EnvironmentHandle h);

            /*! @brief Put the global namespace on the stack
             * 
             * 
             * 
             * @param h: 
             * @return 
             */
            void pushGlobalNamespace(EnvironmentHandle h);
            void pushNamespace(EnvironmentHandle h, const nString& name);
            inline void pushNamespaces(EnvironmentHandle h VORB_UNUSED) {
                // Empty
            }
            template<typename Arg, typename... Args>
            void pushNamespaces(EnvironmentHandle h, Arg a, Args... args) {
                pushNamespace(h, a);
                pushNamespaces(h, args...);
            }

            void popNamespaces(EnvironmentHandle h, i32 n);

            bool objectHasField(EnvironmentHandle h, const nString& name);

            void pushToTop(EnvironmentHandle h, const Function& f);
            int call(EnvironmentHandle h, size_t n, size_t r);
            void popStack(EnvironmentHandle h);
            void* popUpvalueObject(EnvironmentHandle h);

            template<typename Arg>
            i32 pushArgs(EnvironmentHandle h, Arg a) {
                return ScriptValueSender<Arg>::push(h, a);
            }
            template<typename Arg, typename... Args>
            i32 pushArgs(EnvironmentHandle h, Arg a, Args... other) {
                i32 val = ScriptValueSender<Arg>::push(h, a);
                return val + pushArgs<Args...>(h, other...);
            }
            inline i32 pushArgs(EnvironmentHandle h VORB_UNUSED) {
                return 0;
            }

            template<typename Arg>
            Arg popValue(EnvironmentHandle h) {
                return ScriptValueSender<Arg>::pop(h);
            }
        }
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_ScriptImpl_h__
