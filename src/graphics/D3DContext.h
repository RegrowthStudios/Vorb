//
// D3DContext.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DContext.h
 * @brief 
 */

#pragma once

#ifndef Vorb_D3DContext_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DContext_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "graphics/IContext.h"

struct ID3D11Device;

namespace vorb {
    namespace graphics {
        class D3DContext : public IContext {
        public:
            virtual IBuffer* createBuffer(const BufferDescription& desc) override;
            virtual IBuffer* createBuffer(const CBufferDescription* desc) override;


        private:
            ID3D11Device* device;
        };
    }
}

#endif // !Vorb_D3DContext_h__
