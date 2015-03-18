//
// Environment.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Environment.h
 * @brief This is a scripting environment.
 * 
 * This environment acts like a database. It tracks and manages
 * all scripted function pointers. It also contains C++ callbacks
 * that scripts may use. 
 */

#pragma once

#ifndef Vorb_Environment_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Environment_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <unordered_map>
#include "../types.h"
#include "../decorators.h"
#endif // !VORB_USING_PCH

#include "../IDGenerator.h"
#include "../io/Path.h"
#include "Function.h"
#include "ScriptValueSenders.h"
#include "Script.h"

struct lua_State;

namespace vorb {
    namespace script {
        typedef lua_State* EnvironmentHandle;

        class Environment {
        public:
            Environment();
            ~Environment();

            EnvironmentHandle getHandle() const {
                return m_state;
            }

            bool load(const vio::Path& file);
            void addCFunction(const nString& name, int(*f)(EnvironmentHandle));
            template<typename... Args>
            void addCDelegate(const nString& name, RDelegate<void, Args...>* del) {
                ScriptFunc f = fromDelegate<Args...>();
                ScriptValueSender<void*>::push(m_state, del);
                addCClosure(name, f);
            }
            template<typename Ret, typename... Args>
            void addCRDelegate(const nString& name, RDelegate<Ret, Args...>* del) {
                ScriptFunc f = fromRDelegate<Ret, Args...>();
                ScriptValueSender<void*>::push(m_state, del);
                addCClosure(name, f);
            }
            template<typename T>
            void addValue(const nString& name, T value) {
                ScriptValueSender<T>::push(value);
                setValueName(name);
            }

            Function& operator[] (const nString& name);
            const Function& operator[] (const nString& name) const;
        private:
            VORB_NON_COPYABLE(Environment);
            static int registration(lua_State* L);

            void addCClosure(const nString& name, int(*f)(EnvironmentHandle));
            void setValueName(const nString& name);

            std::unordered_map<nString, Function> m_functions;
            EnvironmentHandle m_state;
            vcore::IDGenerator<i32> m_idGen;
        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_Environment_h__
