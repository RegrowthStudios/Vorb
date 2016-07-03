//
// Wrappers.h
// Vorb Engine
//
// Created by Cristian Zaloj on 1 Jul 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file Wrappers.h
 * 
 * \brief 
 *
 *
 */

#pragma once

#ifndef Vorb_Wrappers_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Wrappers_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        class IBuffer;

        struct BufferBindings {
        public:
            IBuffer** buffers;
            ui32 startSlot;
            ui32 numBuffers;
            ui32* offsets;
            ui32* strides;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_Wrappers_h__
