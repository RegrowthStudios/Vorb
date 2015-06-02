//
// IContext.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IContext.h
 * @brief The context hold graphics resources that multiple devices may share.
 */

#pragma once

#ifndef Vorb_IContext_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IContext_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        /* @brief
         */
        class IContext {
        public:
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IContext_h__
