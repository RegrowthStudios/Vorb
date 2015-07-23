//
// D3DObject.h
// Vorb Engine
//
// Created by Cristian Zaloj on 25 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DObject.h
 * @brief 
 *
 *
 */

#pragma once

#ifndef Vorb_D3DObject_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DObject_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include <d3d11.h>

#include "graphics/IObject.h"

namespace vorb {
    namespace graphics {
        class D3DRenderTarget : public IRenderTarget {
        public:
            D3DRenderTarget(IDevice* owner) : IRenderTarget(owner) {
                // Empty
            }

            virtual void disposeInternal() override {
                if (view) view->Release();
            }

            ID3D11RenderTargetView* view = nullptr;
        };

        class D3DSamplerState : public ISamplerState {
        public:
            D3DSamplerState(IDevice* owner) : ISamplerState(owner) {
                // Empty
            }

            virtual void disposeInternal() override {
                if (state) state->Release();
            }

            ID3D11SamplerState* state = nullptr;
        };
    }
}

#endif // !Vorb_D3DObject_h__
