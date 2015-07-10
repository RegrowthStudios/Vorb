//
// D3DGraphicsContext.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DGraphicsContext.h
 * @brief 
 */

#pragma once

#ifndef Vorb_D3DGraphicsContext_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DGraphicsContext_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <d3d11.h>

#include "graphics/IAdapter.h"

struct IDXGIAdapter;
struct IDXGIDevice;
struct IDXGIFactory;

namespace vorb {
    namespace graphics {
        class D3DAdapter : public IAdapter {
        public:
            D3DAdapter(APIVersion v, D3D_FEATURE_LEVEL l) : IAdapter(v),
                requestedFeatureLevel(l) {
                // Empty
            }

            virtual IContext* createContext(OUT OPT IDevice** defaultDevice) override;
            virtual IDevice* createDevice(IContext* context) override;

            virtual void attachToWindow(IContext* context, void* hWnd) override;

            D3D_FEATURE_LEVEL requestedFeatureLevel;
        };
    }
}

#endif // !Vorb_D3DGraphicsContext_h__
