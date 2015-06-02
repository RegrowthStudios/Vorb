//
// IDevice.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IDevice.h
 * @brief The device is responsible for submitting draw calls to the graphics device.
 */

#pragma once

#ifndef Vorb_IDevice_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IDevice_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        /* @brief
         */
        class IDevice {
        public:
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IDevice_h__
