//
// Function.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Function.h
 * @brief A function that may be invoked within an Environment.
 */

#pragma once

#ifndef Vorb_Function_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Function_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Script.h"

namespace vorb {
    namespace script {
        class Environment;

        class Function {
            friend class Environment;
            template<typename Ret> friend class RFunction;
        public:
            Function() : Function(nullptr, "") {
                // Empty
            }

            const nString& getName() const {
                return m_name;
            }
            bool isNil() const {
                return m_name.empty();
            }
            template<typename Ret>
            RFunction<Ret> as() const;

            template<typename... Args>
            void call(Args... args) const {
                EnvironmentHandle hnd = m_env->getHandle();

                impl::pushToTop(hnd, *this);
                impl::pushArgs(hnd, args...);
                impl::call(hnd, sizeof...(Args), 0);
                impl::popStack(hnd);
            }
            template<typename Ret, typename... Args>
            void rcall(OUT Ret* retValue, Args... args) const {
                EnvironmentHandle hnd = m_env->getHandle();

                impl::pushToTop(hnd, *this);
                impl::pushArgs(hnd, args...);
                impl::call(hnd, sizeof...(Args), ScriptValueSender<Ret>::getNumValues());
                *retValue = impl::popValue<Ret>(hnd);
                impl::popStack(hnd);
            }

            template<typename... Args>
            void operator()(Args... args) const {
                call(args...);
            }
        protected:
            static Function nil;

            Function(Environment* env, const nString& name) :
                m_env(env),
                m_name(name) {
                // Empty
            }

            Environment* m_env;
            nString m_name;
        };

        template<typename Ret>
        class RFunction : public Function {
        public:
            RFunction() : Function() {
                // Empty
            }
            RFunction(const Function& f) : Function(f.m_env, f.m_name) {
                // Empty
            }

            template<typename... Args>
            Ret operator()(Args... args) const {
                Ret val;
                Function::rcall(&val, args...);
                return std::move(val);
            }
        };

        template<typename Ret>
        RFunction<Ret> Function::as() const {
            return RFunction<Ret>(*this);
        }
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_Function_h__
