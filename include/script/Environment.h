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

#define VORB_SCRIPT_FUNCTION_TABLE "VorbScriptFuncs"

        class Environment {
        public:
            typedef void (*ErrorHandler)(Environment*, const cString);

            Environment();
            ~Environment();

            EnvironmentHandle getHandle() const {
                return m_state;
            }

            bool load(const vio::Path& file);
            /*! @brief Determines if the function exists.
             * It's more efficient to get the function with operator [] and
             * check vscript::Function::isNil().
             * @param name: Name of the function. 
             * @return true if the function exists.
             */
            bool hasFunction(const nString& name);
            void addCFunction(const nString& name, int(*f)(EnvironmentHandle));
            template<typename... Args>
            void addCDelegate(const nString& name, RDelegate<void, Args...>& del) {
                typedef RDelegate<void, Args...> DelegateType;
                m_cFuncs.emplace_back(new DelegateType(del));
                ScriptFunc f = fromDelegate<Args...>();
                ScriptValueSender<void*>::push(m_state, (DelegateType*)m_cFuncs.back().get());
                addCClosure(name, f);
            }
            template<typename Ret, typename... Args>
            void addCRDelegate(const nString& name, RDelegate<Ret, Args...>& del) {
                typedef RDelegate<Ret, Args...> DelegateType;
                m_cFuncs.emplace_back(new DelegateType(del));
                ScriptFunc f = fromRDelegate<Ret, Args...>();
                ScriptValueSender<void*>::push(m_state, (DelegateType*)m_cFuncs.back().get());
                addCClosure(name, f);
            }
            template<typename T>
            void addValue(const nString& name, T value) {
                ScriptValueSender<T>::push(m_state, value);
                setValueName(name);
            }
            
            template<typename... Args>
            void setNamespaces(Args... v) {
                if (namespaceCount > 0) {
                    impl::popNamespaces(m_state, namespaceCount);
                }
                impl::pushNamespaces(m_state, v...);
                namespaceCount = sizeof...(Args);
            }

            Function& operator[] (const nString& name);
            const Function& operator[] (const nString& name) const;

            ErrorHandler errorHandler = nullptr;
        private:
            VORB_NON_COPYABLE(Environment);
            static int registration(lua_State* L);

            void addCClosure(const nString& name, int(*f)(EnvironmentHandle));
            void setValueName(const nString& name);

            std::unordered_map<nString, Function> m_functions;
            EnvironmentHandle m_state;
            vcore::IDGenerator<i32> m_idGen;

            size_t namespaceCount = 0;
            std::vector<std::unique_ptr<DelegateBase>> m_cFuncs;
        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_Environment_h__
