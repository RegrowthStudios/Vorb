#include "stdafx.h"
#include "graphics/D3DMap.h"

DXGI_FORMAT vg::mapD3D::format[(size_t)vg::MemoryFormat::MAX_VALUE] {};
D3D11_PRIMITIVE_TOPOLOGY vg::mapD3D::topology[(size_t)vg::PrimitiveType::MAX_VALUE] {};
const cString vg::mapD3D::semanticName[(size_t)vg::Semantic::MAX_VALUE] {};
D3D11_TEXTURE_ADDRESS_MODE vg::mapD3D::addressMode[(size_t)TextureAddressMode::MAX_VALUE] {};
D3D11_COMPARISON_FUNC vg::mapD3D::comparisonMode[(size_t)ComparisonMode::MAX_VALUE] {};

void vorb::graphics::mapD3D::fill() {
    fillFormat(format);
    fillTopology(topology);
    fillSemanticName(semanticName);
    fillAddressMode(addressMode);
    fillComparisonMode(comparisonMode);
}
void vorb::graphics::mapD3D::fillFormat(DXGI_FORMAT* f) {
    f[(size_t)vg::MemoryFormat::UNKNOWN]                     = DXGI_FORMAT_UNKNOWN;
    f[(size_t)vg::MemoryFormat::A8_UNORM]                    = DXGI_FORMAT_A8_UNORM;
    f[(size_t)vg::MemoryFormat::DEPTH24_UNORM_STENCIL8_UINT] = DXGI_FORMAT_D24_UNORM_S8_UINT;
    f[(size_t)vg::MemoryFormat::DEPTH32_FLOAT_STENCIL8_UINT] = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    f[(size_t)vg::MemoryFormat::DEPTH16_UNORM]               = DXGI_FORMAT_D16_UNORM;
    f[(size_t)vg::MemoryFormat::DEPTH24_UNORM]               = DXGI_FORMAT_D24_UNORM_S8_UINT;
    f[(size_t)vg::MemoryFormat::DEPTH32_FLOAT]               = DXGI_FORMAT_D32_FLOAT;
    f[(size_t)vg::MemoryFormat::X32_FLOAT_X8X24_TYPELESS]    = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    f[(size_t)vg::MemoryFormat::R11G11B10_FLOAT]             = DXGI_FORMAT_R11G11B10_FLOAT;
    f[(size_t)vg::MemoryFormat::R16_TYPELESS]                = DXGI_FORMAT_R16_TYPELESS;
    f[(size_t)vg::MemoryFormat::R16_FLOAT]                   = DXGI_FORMAT_R16_FLOAT;
    f[(size_t)vg::MemoryFormat::R16_SINT]                    = DXGI_FORMAT_R16_SINT;
    f[(size_t)vg::MemoryFormat::R16_SNORM]                   = DXGI_FORMAT_R16_SNORM;
    f[(size_t)vg::MemoryFormat::R16_UINT]                    = DXGI_FORMAT_R16_UINT;
    f[(size_t)vg::MemoryFormat::R32_FLOAT]                   = DXGI_FORMAT_R32_FLOAT;
    f[(size_t)vg::MemoryFormat::R32_SINT]                    = DXGI_FORMAT_R32_SINT;
    f[(size_t)vg::MemoryFormat::R32_UINT]                    = DXGI_FORMAT_R32_UINT;
    f[(size_t)vg::MemoryFormat::R8_TYPELESS]                 = DXGI_FORMAT_R8_TYPELESS;
    f[(size_t)vg::MemoryFormat::R8_SINT]                     = DXGI_FORMAT_R8_SINT;
    f[(size_t)vg::MemoryFormat::R8_SNORM]                    = DXGI_FORMAT_R8_SNORM;
    f[(size_t)vg::MemoryFormat::R8_UINT]                     = DXGI_FORMAT_R8_UINT;
    f[(size_t)vg::MemoryFormat::R16G16_TYPELESS]             = DXGI_FORMAT_R16G16_TYPELESS;
    f[(size_t)vg::MemoryFormat::R16G16_FLOAT]                = DXGI_FORMAT_R16G16_FLOAT;
    f[(size_t)vg::MemoryFormat::R16G16_SINT]                 = DXGI_FORMAT_R16G16_SINT;
    f[(size_t)vg::MemoryFormat::R16G16_SNORM]                = DXGI_FORMAT_R16G16_SNORM;
    f[(size_t)vg::MemoryFormat::R16G16_UINT]                 = DXGI_FORMAT_R16G16_UINT;
    f[(size_t)vg::MemoryFormat::R32G32_FLOAT]                = DXGI_FORMAT_R32G32_FLOAT;
    f[(size_t)vg::MemoryFormat::R32G32_SINT]                 = DXGI_FORMAT_R32G32_SINT;
    f[(size_t)vg::MemoryFormat::R32G32_UINT]                 = DXGI_FORMAT_R32G32_UINT;
    f[(size_t)vg::MemoryFormat::R8G8_TYPELESS]               = DXGI_FORMAT_R8G8_TYPELESS;
    f[(size_t)vg::MemoryFormat::R8G8_SINT]                   = DXGI_FORMAT_R8G8_SINT;
    f[(size_t)vg::MemoryFormat::R8G8_SNORM]                  = DXGI_FORMAT_R8G8_SNORM;
    f[(size_t)vg::MemoryFormat::R8G8_UINT]                   = DXGI_FORMAT_R8G8_UINT;
    f[(size_t)vg::MemoryFormat::R10G10B10A2_TYPELESS]        = DXGI_FORMAT_R10G10B10A2_TYPELESS;
    f[(size_t)vg::MemoryFormat::R10G10B10A2_UINT]            = DXGI_FORMAT_R10G10B10A2_UINT;
    f[(size_t)vg::MemoryFormat::R32G32B32_FLOAT]             = DXGI_FORMAT_R32G32B32_FLOAT;
    f[(size_t)vg::MemoryFormat::R32G32B32_SINT]              = DXGI_FORMAT_R32G32B32_SINT;
    f[(size_t)vg::MemoryFormat::R32G32B32_UINT]              = DXGI_FORMAT_R32G32B32_UINT;
    f[(size_t)vg::MemoryFormat::R9G9B9E5_FLOAT_SHAREDEXP]    = DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_TYPELESS]       = DXGI_FORMAT_R16G16B16A16_TYPELESS;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_FLOAT]          = DXGI_FORMAT_R16G16B16A16_FLOAT;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_SINT]           = DXGI_FORMAT_R16G16B16A16_SINT;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_SNORM]          = DXGI_FORMAT_R16G16B16A16_SNORM;
    f[(size_t)vg::MemoryFormat::R16G16B16A16_UINT]           = DXGI_FORMAT_R16G16B16A16_UINT;
    f[(size_t)vg::MemoryFormat::R32G32B32A32_FLOAT]          = DXGI_FORMAT_R32G32B32A32_FLOAT;
    f[(size_t)vg::MemoryFormat::R32G32B32A32_SINT]           = DXGI_FORMAT_R32G32B32A32_SINT;
    f[(size_t)vg::MemoryFormat::R32G32B32A32_UINT]           = DXGI_FORMAT_R32G32B32A32_UINT;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_TYPELESS]           = DXGI_FORMAT_R8G8B8A8_TYPELESS;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_SINT]               = DXGI_FORMAT_R8G8B8A8_SINT;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_SNORM]              = DXGI_FORMAT_R8G8B8A8_SNORM;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_UINT]               = DXGI_FORMAT_R8G8B8A8_UINT;
    f[(size_t)vg::MemoryFormat::R8G8B8A8_UNORM_SRGB]         = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
}
void vorb::graphics::mapD3D::fillTopology(D3D11_PRIMITIVE_TOPOLOGY* f) {
    f[(size_t)vg::PrimitiveType::LINES]                      = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    f[(size_t)vg::PrimitiveType::LINES_ADJACENCY]            = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
    f[(size_t)vg::PrimitiveType::LINE_STRIP]                 = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
    f[(size_t)vg::PrimitiveType::LINE_STRIP_ADJACENCY]       = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
    f[(size_t)vg::PrimitiveType::POINTS]                     = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
    f[(size_t)vg::PrimitiveType::TRIANGLES]                  = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    f[(size_t)vg::PrimitiveType::TRIANGLES_ADJACENCY]        = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
    f[(size_t)vg::PrimitiveType::TRIANGLE_STRIP]             = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    f[(size_t)vg::PrimitiveType::TRIANGLE_STRIP_ADJACENCY]   = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
}
void vorb::graphics::mapD3D::fillSemanticName(const cString* f) {
    f[(size_t)vg::Semantic::INVALID]                         = "INVALID";
    f[(size_t)vg::Semantic::COLOR]                           = "COLOR";
    f[(size_t)vg::Semantic::POSITION]                        = "POSITION";
    f[(size_t)vg::Semantic::TEXCOORD]                        = "TEXCOORD";
    f[(size_t)vg::Semantic::NORMAL]                          = "NORMAL";
    f[(size_t)vg::Semantic::BINORMAL]                        = "BINORMAL";
    f[(size_t)vg::Semantic::TANGENT]                         = "TANGENT";
    f[(size_t)vg::Semantic::FOG]                             = "FOG";
    f[(size_t)vg::Semantic::BLENDINDICES]                    = "BLENDINDICES";
    f[(size_t)vg::Semantic::BLENDWEIGHT]                     = "BLENDWEIGHT";
    f[(size_t)vg::Semantic::PSIZE]                           = "PSIZE";
    f[(size_t)vg::Semantic::TESSFACTOR]                      = "TESSFACTOR";
}
void vorb::graphics::mapD3D::fillAddressMode(D3D11_TEXTURE_ADDRESS_MODE* f) {
    f[(size_t)vg::TextureAddressMode::WRAP]                  = D3D11_TEXTURE_ADDRESS_WRAP;
    f[(size_t)vg::TextureAddressMode::CLAMP]                 = D3D11_TEXTURE_ADDRESS_CLAMP;
    f[(size_t)vg::TextureAddressMode::CLAMP_ABSOLUTE]        = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
    f[(size_t)vg::TextureAddressMode::BORDER]                = D3D11_TEXTURE_ADDRESS_BORDER;
    f[(size_t)vg::TextureAddressMode::MIRROR]                = D3D11_TEXTURE_ADDRESS_MIRROR;
}
void vorb::graphics::mapD3D::fillComparisonMode(D3D11_COMPARISON_FUNC* f) {
    f[(size_t)vg::ComparisonMode::NEVER]                     = D3D11_COMPARISON_NEVER;
    f[(size_t)vg::ComparisonMode::EQUAL]                     = D3D11_COMPARISON_EQUAL;
    f[(size_t)vg::ComparisonMode::GREATER]                   = D3D11_COMPARISON_GREATER;
    f[(size_t)vg::ComparisonMode::LESS]                      = D3D11_COMPARISON_LESS;
    f[(size_t)vg::ComparisonMode::NOT_EQUAL]                 = D3D11_COMPARISON_NOT_EQUAL;
    f[(size_t)vg::ComparisonMode::LESS_EQUAL]                = D3D11_COMPARISON_LESS_EQUAL;
    f[(size_t)vg::ComparisonMode::GREATER_EQUAL]             = D3D11_COMPARISON_GREATER_EQUAL;
    f[(size_t)vg::ComparisonMode::ALWAYS]                    = D3D11_COMPARISON_ALWAYS;
}

