//
// D3DResource.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DResource.h
 * @brief 
 */

#pragma once

#ifndef Vorb_D3DResource_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DResource_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <d3d11.h>

#include "graphics/IResource.h"

namespace vorb {
    namespace graphics {

        class D3DBuffer : public IBuffer {
        public:
            D3DBuffer(IContext* owner) : IBuffer(owner) {
                // Empty
            }

            virtual void disposeInternal() override {
                data->Release();
            }

            virtual size_t getMemoryUsed() override {
                return size;
            }


            ID3D11Buffer* data = nullptr;
            size_t size = 0;
        };
    }
}

#endif // !Vorb_D3DResource_h__
