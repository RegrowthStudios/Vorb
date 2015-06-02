//
// D3DDescCompile.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DDescCompile.h
 * @brief 
 */

#pragma once

#ifndef Vorb_D3DDescCompile_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DDescCompile_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <d3d11.h>

namespace vorb {
    namespace graphics {
        struct BufferDescription;

        struct CBufferDescription : public D3D11_BUFFER_DESC {
            // Empty
        };

        void fill(CBufferDescription& v, const BufferDescription& desc);
    }
}

#endif // !Vorb_D3DDescCompile_h__
