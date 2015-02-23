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

namespace vorb {
    namespace script {
        class Environment;

        class Function {
            friend class Environment;
        public:
            Function() : Function("") {
                // Empty
            }

            const nString& getName() const {
                return m_name;
            }
        private:
            Function(const nString& name) :
                m_name(name) {
                // Empty
            }

            nString m_name;
        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_Function_h__
