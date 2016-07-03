//
// Script.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file Script.h
 * 
 * \brief Operations that can be performed on scripting objects.
 */

#pragma once

#ifndef Vorb_Script_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Script_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include <tuple>
#include <iostream>
#include <array>
#include <utility>

#include "ScriptValueSenders.h"
#include "ScriptImpl.h"
#include "../Events.hpp"

namespace vorb {
    namespace script {
        class Function;

        namespace impl {
            template<size_t... Is>
            struct index_sequence {};
            template<size_t N, size_t... Is>
            struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...> {};
            template<size_t... Is>
            struct make_index_sequence<0, Is...> : index_sequence<Is...> {};
            template<typename... T>
            struct index_sequence_for : make_index_sequence<sizeof...(T)> {};

            template<typename... T>
            std::tuple<T...> popArgs(EnvironmentHandle h) {
                std::tuple<T...> tValue { ScriptValueSender<T>::defaultValue()... };
                popArgsR<T...>(h, &tValue);
                return std::move(tValue);
            }
            template<typename T>
            void popArgsR(EnvironmentHandle h, std::tuple<T>* v) {
                T tValue = popValue<T>(h);
                T* ptr = &std::get<0>(*v);
                *ptr = tValue;
            }
            template<typename T>
            void popArgsR(EnvironmentHandle h, std::tuple<T&>* v) {
                void* tValue = popValue<void*>(h);
                memcpy(v, &tValue, sizeof(void*));
            }
            template<typename V, typename... T>
            void popArgsR(EnvironmentHandle h, std::tuple<V, T...>* v) {
                std::tuple<T...> tPopped { ScriptValueSender<T>::defaultValue()... };
                popArgsR<T...>(h, &tPopped);
                std::tuple<V> tValue { ScriptValueSender<V>::defaultValue() };
                popArgsR<V>(h, &tValue);
                std::swap(*v, std::tuple_cat(tValue, tPopped));
            }

            template <typename Ret, typename F, typename Tuple, size_t... Is>
            Ret invokeCall(F f, Tuple& t, index_sequence<Is...>) {
                return f(std::get<Is>(t)...);
            }
            template <typename Ret, typename F, typename Tuple, size_t... Is>
            Ret invoke(F f, Tuple& t, index_sequence<Is...>) {
                return f->invoke(std::get<Is>(t)...);
            }
            template <typename F, typename Tuple, size_t... Is>
            void invokeNone(F f, Tuple& t, index_sequence<Is...>) {
                f->invoke(std::get<Is>(t)...);
            }

            template<typename F, typename... Args>
            i32 cCall(EnvironmentHandle h, void(*func)(Args...)) {
                std::tuple<Args...> tValue = popArgs<Args...>(h);
                invokeCall<void>(func, tValue, index_sequence_for<Args...>());
                return 0;
            }
            template<typename F>
            i32 cCall(EnvironmentHandle h, void(*func)()) {
                func();
                return 0;
            }

            template<typename... Args>
            i32 fCall(EnvironmentHandle h, RDelegate<void, Args...>* del) {
                std::tuple<Args...> tValue = popArgs<Args...>(h);
                invokeNone(del, tValue, index_sequence_for<Args...>());
                return 0;
            }
            inline i32 fCall(EnvironmentHandle h, RDelegate<void>* del) {
                del->invoke();
                return 0;
            }

            template<typename Ret, typename... Args>
            i32 fRCall(EnvironmentHandle h, RDelegate<Ret, Args...>* del) {
                std::tuple<Args...> tValue = popArgs<Args...>(h);
                Ret retValue = invoke<Ret>(del, tValue, index_sequence_for<Args...>());
                return ScriptValueSender<Ret>::push(h, retValue);
            }
            template<typename Ret>
            i32 fRCall(EnvironmentHandle h, RDelegate<Ret>* del) {
                Ret retValue = del->invoke();
                return ScriptValueSender<Ret>::push(h, retValue);
            }

            template<typename F, F f, typename... Args>
            int luaCall(lua_State* s) {
                typedef void(*FPtr)(Args...);
                return cCall<F>(s, (FPtr)f);
            }
            template<typename... Args>
            int luaDCall(lua_State* s) {
                void* del = popUpvalueObject(s);
                return fCall(s, (RDelegate<void, Args...>*)del);
            }
            template<typename Ret, typename... Args>
            int luaDRCall(lua_State* s) {
                void* del = popUpvalueObject(s);
                return fRCall<Ret, Args...>(s, (RDelegate<Ret, Args...>*)del);
            }
        }

        typedef int(*ScriptFunc)(EnvironmentHandle s);
        template<void* f, typename... Args>
        ScriptFunc fromFunction(void(*func)(Args...)) {
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
