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
#include "ScriptImpl.h"

namespace vorb {
    namespace script {
        class Function;

        namespace impl {
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
