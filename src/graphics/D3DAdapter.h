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

#include "graphics/IAdapter.h"

struct IDXGIAdapter;
struct IDXGIDevice;
struct IDXGIFactory;

namespace vorb {
    namespace graphics {
        class D3DAdapter : public IAdapter {
        public:
            virtual IContext* createContext(OUT OPT IDevice** defaultDevice) override;
            virtual IDevice* createDevice(IContext* context) override;
            virtual WindowHandle createWindow(IContext* context) override;

            virtual void attachToWindow(IContext* context, void* hWnd) override;

        private:
        };
    }
}

#endif // !Vorb_D3DGraphicsContext_h__
