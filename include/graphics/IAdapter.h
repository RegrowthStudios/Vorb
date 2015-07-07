//
// IAdapter.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IAdapter.h
 * @brief 
 */

#pragma once

#ifndef Vorb_IAdapter_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IAdapter_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

// https://developer.nvidia.com/sites/default/files/akamai/gamedev/files/gdc12/Efficient_Buffer_Management_McDonald.pdf

namespace vorb {
    namespace graphics {
        class IContext;
        class IDevice;

        class IAdapter {
        public:
            typedef void* WindowHandle;

            virtual IContext* createContext(OUT OPT IDevice** defaultDevice) = 0;
            virtual IDevice* createDevice(IContext* context) = 0;

            virtual void attachToWindow(IContext* context, void* hWnd) = 0;
        };

        IAdapter* getD3DAdapter();


    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IAdapter_h__
