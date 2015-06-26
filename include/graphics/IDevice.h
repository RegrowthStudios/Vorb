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
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Enums.h"
#include "IResource.h"
#include "IObject.h"

namespace vorb {
    namespace graphics {
        /* @brief
         */
        class IDevice {
        public:
            virtual void setClearColor(const f64v4& v) = 0;
            virtual void setClearDepth(f64 v) = 0;
            virtual void setClearStencil(ui32 v) = 0;
            virtual void clear(ClearBits bits) = 0;

            virtual IRenderTarget* create(ITexture2D* res) = 0;

            virtual void computeUse(IComputeShader* shader) = 0;

            virtual void dispatchThreads(ui32 x, ui32 y, ui32 z) = 0;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IDevice_h__
