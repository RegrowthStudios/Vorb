#include "stdafx.h"
#include "graphics/D3DMap.h"

DXGI_FORMAT vg::formatMapDXGI[(size_t)vg::MemoryFormat::MAX_VALUE] {};

void vorb::graphics::fillDXGIMAP(DXGI_FORMAT* f) {
    f[(size_t)vg::MemoryFormat::UNKNOWN] = DXGI_FORMAT_UNKNOWN;
    f[(size_t)vg::MemoryFormat::A8_UNORM] = DXGI_FORMAT_A8_UNORM;
    f[(size_t)vg::MemoryFormat::DEPTH24_UNORM_STENCIL8_UINT] = DXGI_FORMAT_D24_UNORM_S8_UINT;
    f[(size_t)vg::MemoryFormat::DEPTH32_FLOAT_STENCIL8_UINT] = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    f[(size_t)vg::MemoryFormat::DEPTH16_UNORM] = DXGI_FORMAT_D16_UNORM;
    f[(size_t)vg::MemoryFormat::DEPTH24_UNORM] = DXGI_FORMAT_D24_UNORM_S8_UINT;
    f[(size_t)vg::MemoryFormat::DEPTH32_FLOAT] = DXGI_FORMAT_D32_FLOAT;
    f[(size_t)vg::MemoryFormat::X32_FLOAT_X8X24_TYPELESS] = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    f[(size_t)vg::MemoryFormat::R11G11B10_FLOAT] = DXGI_FORMAT_R11G11B10_FLOAT;
    f[(size_t)vg::MemoryFormat::R16_TYPELESS] = DXGI_FORMAT_R16_TYPELESS;
    f[(size_t)vg::MemoryFormat::R16_FLOAT] = DXGI_FORMAT_R16_FLOAT;
    f[(size_t)vg::MemoryFormat::R16_SINT] = DXGI_FORMAT_R16_SINT;
    f[(size_t)vg::MemoryFormat::R16_SNORM] = DXGI_FORMAT_R16_SNORM;
    f[(size_t)vg::MemoryFormat::R16_UINT] = DXGI_FORMAT_R16_UINT;
    f[(size_t)vg::MemoryFormat::R32_FLOAT] = DXGI_FORMAT_R32_FLOAT;
    f[(size_t)vg::MemoryFormat::R32_SINT] = DXGI_FORMAT_R32_SINT;
    f[(size_t)vg::MemoryFormat::R32_UINT] = DXGI_FORMAT_R32_UINT;
    f[(size_t)vg::MemoryFormat::R8_TYPELESS] = DXGI_FORMAT_R8_TYPELESS;
    f[(size_t)vg::MemoryFormat::R8_SINT] = DXGI_FORMAT_R8_SINT;
    f[(size_t)vg::MemoryFormat::R8_SNORM] = DXGI_FORMAT_R8_SNORM;
    f[(size_t)vg::MemoryFormat::R8_UINT] = DXGI_FORMAT_R8_UINT;
    f[(size_t)vg::MemoryFormat::R16G16_TYPELESS] = DXGI_FORMAT_R16G16_TYPELESS;
    f[(size_t)vg::MemoryFormat::R16G16_FLOAT] = DXGI_FORMAT_R16G16_FLOAT;
    f[(size_t)vg::MemoryFormat::R16G16_SINT] = DXGI_FORMAT_R16G16_SINT;
    f[(size_t)vg::MemoryFormat::R16G16_SNORM] = DXGI_FORMAT_R16G16_SNORM;
    f[(size_t)vg::MemoryFormat::R16G16_UINT] = DXGI_FORMAT_R16G16_UINT;
    f[(size_t)vg::MemoryFormat::R32G32_FLOAT] = DXGI_FORMAT_R32G32_FLOAT;
    f[(size_t)vg::MemoryFormat::R32G32_SINT] = DXGI_FORMAT_R32G32_SINT;
    f[(size_t)vg::MemoryFormat::R32G32_UINT] = DXGI_FORMAT_R32G32_UINT;
    f[(size_t)vg::MemoryFormat::R8G8_TYPELESS] = DXGI_FORMAT_R8G8_TYPELESS;
    f[(size_t)vg::MemoryFormat::R8G8_SINT] = DXGI_FORMAT_R8G8_SINT;
    f[(size_t)vg::MemoryFormat::R8G8_SNORM] = DXGI_FORMAT_R8G8_SNORM;
    f[(size_t)vg::MemoryFormat::R8G8_UINT] = DXGI_FORMAT_R8G8_UINT;
    f[(size_t)vg::MemoryFormat::R10G10B10A2_TYPELESS] = DXGI_FORMAT_R10G10B10A2_TYPELESS;
    f[(size_t)vg::MemoryFormat::R10G10B10A2_UINT] = DXGI_FORMAT_R10G10B10A2_UINT;
    f[(size_t)vg::MemoryFormat::R32G32B32_FLOAT] = DXGI_FORMAT_R32G32B32_FLOAT;
    f[(size_t)vg::MemoryFormat::R32G32B32_SINT] = DXGI_FORMAT_R32G32B32_SINT;
    f[(size_t)vg::MemoryFormat::R32G32B32_UINT] = DXGI_FORMAT_R32G32B32_UINT;
    f[(size_t)vg::MemoryFormat::R9G9B9E5_FLOAT_SHAREDEXP] = DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_TYPELESS] = DXGI_FORMAT_R16G16B16A16_TYPELESS;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_FLOAT] = DXGI_FORMAT_R16G16B16A16_FLOAT;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_SINT] = DXGI_FORMAT_R16G16B16A16_SINT;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_SNORM] = DXGI_FORMAT_R16G16B16A16_SNORM;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_UINT] = DXGI_FORMAT_R16G16B16A16_UINT;
    f[(size_t)vg::MemoryFormat::R32G32B32A32_FLOAT] = DXGI_FORMAT_R32G32B32A32_FLOAT;
    f[(size_t)vg::MemoryFormat::R32G32B32A32_SINT] = DXGI_FORMAT_R32G32B32A32_SINT;
    f[(size_t)vg::MemoryFormat::R32G32B32A32_UINT] = DXGI_FORMAT_R32G32B32A32_UINT;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_TYPELESS] = DXGI_FORMAT_R8G8B8A8_TYPELESS;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_SINT] = DXGI_FORMAT_R8G8B8A8_SINT;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_SNORM] = DXGI_FORMAT_R8G8B8A8_SNORM;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_UINT] = DXGI_FORMAT_R8G8B8A8_UINT;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_UNORM_SRGB] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
}
