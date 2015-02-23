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

#include "../io/Path.h"
#include "Function.h"

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

            Function& operator[] (const nString& name);
            const Function& operator[] (const nString& name) const;
        private:
            VORB_NON_COPYABLE(Environment);
            static int registration(lua_State* L);

            std::unordered_map<nString, Function> m_functions;
            EnvironmentHandle m_state;
        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_Environment_h__
