//
// D3DMap.h
// Vorb Engine
//
// Created by Cristian Zaloj on 26 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DMap.h
 * @brief 
 *
 *
 */

#pragma once

#ifndef Vorb_D3DMap_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DMap_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include <d3d11.h>

#include "graphics/VGEnums.h"
#include "graphics/GLEnums.h"

namespace vorb {
    namespace graphics {
        namespace mapD3D {
            extern D3D11_COMPARISON_FUNC comparisonMode[(size_t)ComparisonMode::MAX_VALUE];
            extern DXGI_FORMAT format[(size_t)MemoryFormat::MAX_VALUE];
            extern D3D11_PRIMITIVE_TOPOLOGY topology[(size_t)PrimitiveType::MAX_VALUE];
            extern const cString semanticName[(size_t)Semantic::MAX_VALUE];
            extern D3D11_TEXTURE_ADDRESS_MODE addressMode[(size_t)TextureAddressMode::MAX_VALUE];
            extern D3D11_STENCIL_OP stencilOp[(size_t)StencilOperation::MAX_VALUE];
            extern D3D11_BLEND blendMultiplier[(size_t)BlendMultiplier::MAX_VALUE];
            extern D3D11_BLEND_OP blendOp[(size_t)BlendOperation::MAX_VALUE];
            extern D3D11_CULL_MODE cullMode[(size_t)FaceCulling::MAX_VALUE];

            void fill();
            void fillComparisonMode(D3D11_COMPARISON_FUNC* f);
            void fillFormat(DXGI_FORMAT* f);
            void fillTopology(D3D11_PRIMITIVE_TOPOLOGY* f);
            void fillSemanticName(const cString* f);
            void fillAddressMode(D3D11_TEXTURE_ADDRESS_MODE* f);
            void fillStencilOp(D3D11_STENCIL_OP* f);
            void fillBlendMultiplier(D3D11_BLEND* f);
            void fillBlendOp(D3D11_BLEND_OP* f);
            void fillCullMode(D3D11_CULL_MODE* f);
        }
    }
}

#endif // !Vorb_D3DMap_h__
