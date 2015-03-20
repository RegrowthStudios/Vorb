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

#include "ScriptValueSenders.h"
#include "Environment.h"

namespace vorb {
    namespace script {
        class Environment;
        class Function;

        namespace impl {
            void pushToTop(EnvironmentHandle h, const Function& f);
            void call(EnvironmentHandle h, size_t n, size_t r);
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
            inline i32 pushArgs(EnvironmentHandle h) {
                return 0;
            }

            template<typename Arg>
            Arg popValue(EnvironmentHandle h) {
                Arg a = ScriptValueSender<Arg>::pop(h);
                return a;
            }

            template<typename F, typename... Args>
            i32 cCall(EnvironmentHandle h, F func) {
                func(popValue<Args>(h)...);
                return 0;
            }
            template<typename... Args>
            i32 fCall(EnvironmentHandle h, void* del) {
                typedef RDelegate<void, Args...>* FunctionPtr;
                FunctionPtr f = (FunctionPtr)del;
                f->invoke(popValue<Args>(h)...);
                return 0;
            }
            template<typename Ret, typename... Args>
            i32 fRCall(EnvironmentHandle h, void* del) {
                typedef RDelegate<Ret, Args...>* FunctionPtr;
                FunctionPtr f = (FunctionPtr)del;
                Ret retValue = f->invoke(popValue<Args>(h)...);
                return ScriptValueSender<Ret>::push(h, retValue);
            }

            template<typename F, F f, typename... Args>
            int luaCall(lua_State* s) {
                return cCall<F, Args...>(s, f);
            }
            template<typename... Args>
            int luaDCall(lua_State* s) {
                void* del = popUpvalueObject(s);
                return fCall<Args...>(s, del);
            }
            template<typename Ret, typename... Args>
            int luaDRCall(lua_State* s) {
                void* del = popUpvalueObject(s);
                return fRCall<Ret, Args...>(s, del);
            }
        }

        template<typename... Args>
        bool call(Environment& env, const nString& f, Args... args) {
            Function& scr = env[f];
            EnvironmentHandle hnd = env.getHandle();

            impl::pushToTop(hnd, scr);
            impl::pushArgs(hnd, args...);
            impl::call(hnd, sizeof...(Args), 0);
            return true;
        }
        template<typename Ret, typename... Args>
        bool callReturn(Environment& env, const nString& f, OUT Ret* retValue, Args... args) {
            Function& scr = env[f];
            EnvironmentHandle hnd = env.getHandle();

            impl::pushToTop(hnd, scr);
            impl::pushArgs(hnd, args...);
            impl::call(hnd, sizeof...(Args), ScriptValueSender<Ret>::getNumValues());
            *retValue = impl::popValue<Ret>(hnd);
            return true;
        }

        typedef int(*ScriptFunc)(EnvironmentHandle s);
        template<void* f, typename... Args>
        ScriptFunc fromFunction(void (*func)(Args...)) {
            typedef void(*FuncType)(Args...);
            return impl::luaCall<FuncType, (FuncType)f, Args...>;
        }
        typedef int(*ScriptFunc)(EnvironmentHandle s);
        template<typename... Args>
        ScriptFunc fromDelegate() {
            return impl::luaDCall<Args...>;
        }
        template<typename Ret, typename... Args>
        ScriptFunc fromRDelegate() {
            return impl::luaDRCall<Ret, Args...>;
        }
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_Script_h__
