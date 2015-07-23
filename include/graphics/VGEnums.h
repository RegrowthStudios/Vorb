//
// VGEnums.h
// Vorb Engine
//
// Created by Cristian Zaloj on 26 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file VGEnums.h
 * @brief 
 *
 *
 */

#pragma once

#ifndef Vorb_VGEnums_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VGEnums_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        enum class ComparisonMode {
            NEVER         = 0x00,
            EQUAL         = 0x01,
            GREATER       = 0x02,
            LESS          = 0x04,
            NOT_EQUAL     = LESS | GREATER,
            LESS_EQUAL    = LESS | EQUAL,
            GREATER_EQUAL = GREATER | EQUAL,
            ALWAYS        = LESS | GREATER | EQUAL,
            MAX_VALUE
        };
        ENUM_CLASS_OPS_INL(ComparisonMode, std::underlying_type<ComparisonMode>::type);

        enum class MemoryFormat {
            UNKNOWN = 0,
            A8_UNORM,
            DEPTH24_UNORM_STENCIL8_UINT,
            DEPTH32_FLOAT_STENCIL8_UINT,
            DEPTH16_UNORM,
            DEPTH24_UNORM,
            DEPTH32_FLOAT,
            X32_FLOAT_X8X24_TYPELESS,
            R11G11B10_FLOAT,
            R16_TYPELESS,
            R16_FLOAT,
            R16_SINT,
            R16_SNORM,
            R16_UINT,
            R32_FLOAT,
            R32_SINT,
            R32_UINT,
            R8_TYPELESS,
            R8_SINT,
            R8_SNORM,
            R8_UINT,
            R16G16_TYPELESS,
            R16G16_FLOAT,
            R16G16_SINT,
            R16G16_SNORM,
            R16G16_UINT,
            R32G32_FLOAT,
            R32G32_SINT,
            R32G32_UINT,
            R8G8_TYPELESS,
            R8G8_SINT,
            R8G8_SNORM,
            R8G8_UINT,
            R10G10B10A2_TYPELESS,
            R10G10B10A2_UINT,
            R32G32B32_FLOAT,
            R32G32B32_SINT,
            R32G32B32_UINT,
            R9G9B9E5_FLOAT_SHAREDEXP,
            R16G16B16A16_TYPELESS,
            R16G16B16A16_FLOAT,
            R16G16B16A16_SINT,
            R16G16B16A16_SNORM,
            R16G16B16A16_UINT,
            R32G32B32A32_FLOAT,
            R32G32B32A32_SINT,
            R32G32B32A32_UINT,
            R8G8B8A8_TYPELESS,
            R8G8B8A8_SINT,
            R8G8B8A8_SNORM,
            R8G8B8A8_UINT,
            R8G8B8A8_UNORM_SRGB,
            MAX_VALUE
        };

        enum class TextureAddressMode {
            WRAP,
            CLAMP,
            CLAMP_ABSOLUTE,
            BORDER,
            MIRROR,
            MAX_VALUE
        };
        enum class TextureFilterMode {
            POINT,
            LINEAR,
            MAX_VALUE
        };
        enum class TextureSampleMode {
            NORMAL,
            COMPARISON,
            MINIMUM,
            MAXIMUM,
            MAX_VALUE
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_VGEnums_h__
