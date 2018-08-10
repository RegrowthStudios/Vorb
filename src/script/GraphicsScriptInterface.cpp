#include "Vorb/stdafx.h"
#include "Vorb/script/GraphicsScriptInterface.h"
#include "Vorb/graphics/SamplerState.h"
#include "Vorb/graphics/GLEnums.h"
#include "Vorb/graphics/SpriteBatch.h"

void vscript::GraphicsScriptInterface::registerGraphics(vscript::Environment& env) {
    env.setNamespaces();
    textureCache.registerTextureCache(env);
    registerConstants(env);
    registerFunctions(env);
}

void vscript::GraphicsScriptInterface::dispose() {
    textureCache.dispose();
}

void vscript::GraphicsScriptInterface::registerConstants(vscript::Environment& env) {
    // Enums
    vg::SamplerState::registerStates(env);
    { // vg::TextureInternalFormat
        env.setNamespaces("TextureInternalFormat");
        env.addValue("NONE", vg::TextureInternalFormat::NONE);
        env.addValue("ALPHA", vg::TextureInternalFormat::ALPHA);
        env.addValue("COMPRESSED_ALPHA", vg::TextureInternalFormat::COMPRESSED_ALPHA);
        env.addValue("COMPRESSED_INTENSITY", vg::TextureInternalFormat::COMPRESSED_INTENSITY);
        env.addValue("COMPRESSED_LUMINANCE", vg::TextureInternalFormat::COMPRESSED_LUMINANCE);
        env.addValue("COMPRESSED_LUMINANCE_ALPHA", vg::TextureInternalFormat::COMPRESSED_LUMINANCE_ALPHA);
        env.addValue("COMPRESSED_RED", vg::TextureInternalFormat::COMPRESSED_RED);
        env.addValue("COMPRESSED_RED_RGTC1", vg::TextureInternalFormat::COMPRESSED_RED_RGTC1);
        env.addValue("COMPRESSED_RG", vg::TextureInternalFormat::COMPRESSED_RG);
        env.addValue("COMPRESSED_RGB", vg::TextureInternalFormat::COMPRESSED_RGB);
        env.addValue("COMPRESSED_RGBA", vg::TextureInternalFormat::COMPRESSED_RGBA);
        env.addValue("COMPRESSED_RGBA_BPTC_UNORM", vg::TextureInternalFormat::COMPRESSED_RGBA_BPTC_UNORM);
        env.addValue("COMPRESSED_RGB_BPTC_SIGNED_FLOAT", vg::TextureInternalFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT);
        env.addValue("COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT", vg::TextureInternalFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT);
        env.addValue("COMPRESSED_RG_RGTC2", vg::TextureInternalFormat::COMPRESSED_RG_RGTC2);
        env.addValue("COMPRESSED_SIGNED_RED_RGTC1", vg::TextureInternalFormat::COMPRESSED_SIGNED_RED_RGTC1);
        env.addValue("COMPRESSED_SIGNED_RG_RGTC2", vg::TextureInternalFormat::COMPRESSED_SIGNED_RG_RGTC2);
        env.addValue("COMPRESSED_SLUMINANCE", vg::TextureInternalFormat::COMPRESSED_SLUMINANCE);
        env.addValue("COMPRESSED_SLUMINANCE_ALPHA", vg::TextureInternalFormat::COMPRESSED_SLUMINANCE_ALPHA);
        env.addValue("COMPRESSED_SRGB", vg::TextureInternalFormat::COMPRESSED_SRGB);
        env.addValue("COMPRESSED_SRGB_ALPHA", vg::TextureInternalFormat::COMPRESSED_SRGB_ALPHA);
        env.addValue("DEPTH24_STENCIL8", vg::TextureInternalFormat::DEPTH24_STENCIL8);
        env.addValue("DEPTH32F_STENCIL8", vg::TextureInternalFormat::DEPTH32F_STENCIL8);
        env.addValue("DEPTH_COMPONENT", vg::TextureInternalFormat::DEPTH_COMPONENT);
        env.addValue("DEPTH_COMPONENT16", vg::TextureInternalFormat::DEPTH_COMPONENT16);
        env.addValue("DEPTH_COMPONENT24", vg::TextureInternalFormat::DEPTH_COMPONENT24);
        env.addValue("DEPTH_COMPONENT32", vg::TextureInternalFormat::DEPTH_COMPONENT32);
        env.addValue("DEPTH_COMPONENT32F", vg::TextureInternalFormat::DEPTH_COMPONENT32F);
        env.addValue("DEPTH_STENCIL", vg::TextureInternalFormat::DEPTH_STENCIL);
        env.addValue("FLOAT_32_UNSIGNED_INT_24_8_REV", vg::TextureInternalFormat::FLOAT_32_UNSIGNED_INT_24_8_REV);
        env.addValue("LUMINANCE", vg::TextureInternalFormat::LUMINANCE);
        env.addValue("LUMINANCE_ALPHA", vg::TextureInternalFormat::LUMINANCE_ALPHA);
        env.addValue("R11F_G11F_B10F", vg::TextureInternalFormat::R11F_G11F_B10F);
        env.addValue("R16", vg::TextureInternalFormat::R16);
        env.addValue("R16F", vg::TextureInternalFormat::R16F);
        env.addValue("R16I", vg::TextureInternalFormat::R16I);
        env.addValue("R16_SNORM", vg::TextureInternalFormat::R16_SNORM);
        env.addValue("R16UI", vg::TextureInternalFormat::R16UI);
        env.addValue("R32F", vg::TextureInternalFormat::R32F);
        env.addValue("R32I", vg::TextureInternalFormat::R32I);
        env.addValue("R32UI", vg::TextureInternalFormat::R32UI);
        env.addValue("R3_G3_B2", vg::TextureInternalFormat::R3_G3_B2);
        env.addValue("R8", vg::TextureInternalFormat::R8);
        env.addValue("R8I", vg::TextureInternalFormat::R8I);
        env.addValue("R8_SNORM", vg::TextureInternalFormat::R8_SNORM);
        env.addValue("R8UI", vg::TextureInternalFormat::R8UI);
        env.addValue("RG16", vg::TextureInternalFormat::RG16);
        env.addValue("RG16F", vg::TextureInternalFormat::RG16F);
        env.addValue("RG16I", vg::TextureInternalFormat::RG16I);
        env.addValue("RG16_SNORM", vg::TextureInternalFormat::RG16_SNORM);
        env.addValue("RG16UI", vg::TextureInternalFormat::RG16UI);
        env.addValue("RG32F", vg::TextureInternalFormat::RG32F);
        env.addValue("RG32I", vg::TextureInternalFormat::RG32I);
        env.addValue("RG32UI", vg::TextureInternalFormat::RG32UI);
        env.addValue("RG8", vg::TextureInternalFormat::RG8);
        env.addValue("RG8I", vg::TextureInternalFormat::RG8I);
        env.addValue("RG8_SNORM", vg::TextureInternalFormat::RG8_SNORM);
        env.addValue("RG8UI", vg::TextureInternalFormat::RG8UI);
        env.addValue("RGB", vg::TextureInternalFormat::RGB);
        env.addValue("RGB10", vg::TextureInternalFormat::RGB10);
        env.addValue("RGB10_A2", vg::TextureInternalFormat::RGB10_A2);
        env.addValue("RGB10_A2UI", vg::TextureInternalFormat::RGB10_A2UI);
        env.addValue("RGB12", vg::TextureInternalFormat::RGB12);
        env.addValue("RGB16", vg::TextureInternalFormat::RGB16);
        env.addValue("RGB16F", vg::TextureInternalFormat::RGB16F);
        env.addValue("RGB16I", vg::TextureInternalFormat::RGB16I);
        env.addValue("RGB16_SNORM", vg::TextureInternalFormat::RGB16_SNORM);
        env.addValue("RGB16UI", vg::TextureInternalFormat::RGB16UI);
        env.addValue("RGB32F", vg::TextureInternalFormat::RGB32F);
        env.addValue("RGB32I", vg::TextureInternalFormat::RGB32I);
        env.addValue("RGB32UI", vg::TextureInternalFormat::RGB32UI);
        env.addValue("RGB4", vg::TextureInternalFormat::RGB4);
        env.addValue("RGB5", vg::TextureInternalFormat::RGB5);
        env.addValue("RGB5_A1", vg::TextureInternalFormat::RGB5_A1);
        env.addValue("RGB8", vg::TextureInternalFormat::RGB8);
        env.addValue("RGB8I", vg::TextureInternalFormat::RGB8I);
        env.addValue("RGB8_SNORM", vg::TextureInternalFormat::RGB8_SNORM);
        env.addValue("RGB8UI", vg::TextureInternalFormat::RGB8UI);
        env.addValue("RGB9_E5", vg::TextureInternalFormat::RGB9_E5);
        env.addValue("RGBA", vg::TextureInternalFormat::RGBA);
        env.addValue("RGBA12", vg::TextureInternalFormat::RGBA12);
        env.addValue("RGBA16", vg::TextureInternalFormat::RGBA16);
        env.addValue("RGBA16F", vg::TextureInternalFormat::RGBA16F);
        env.addValue("RGBA16I", vg::TextureInternalFormat::RGBA16I);
        env.addValue("RGBA16_SNORM", vg::TextureInternalFormat::RGBA16_SNORM);
        env.addValue("RGBA16UI", vg::TextureInternalFormat::RGBA16UI);
        env.addValue("RGBA2", vg::TextureInternalFormat::RGBA2);
        env.addValue("RGBA32F", vg::TextureInternalFormat::RGBA32F);
        env.addValue("RGBA32I", vg::TextureInternalFormat::RGBA32I);
        env.addValue("RGBA32UI", vg::TextureInternalFormat::RGBA32UI);
        env.addValue("RGBA4", vg::TextureInternalFormat::RGBA4);
        env.addValue("RGBA8", vg::TextureInternalFormat::RGBA8);
        env.addValue("RGBA8I", vg::TextureInternalFormat::RGBA8I);
        env.addValue("RGBA8_SNORM", vg::TextureInternalFormat::RGBA8_SNORM);
        env.addValue("RGBA8UI", vg::TextureInternalFormat::RGBA8UI);
        env.addValue("SLUMINANCE", vg::TextureInternalFormat::SLUMINANCE);
        env.addValue("SLUMINANCE8", vg::TextureInternalFormat::SLUMINANCE8);
        env.addValue("SLUMINANCE8_ALPHA8", vg::TextureInternalFormat::SLUMINANCE8_ALPHA8);
        env.addValue("SLUMINANCE_ALPHA", vg::TextureInternalFormat::SLUMINANCE_ALPHA);
        env.addValue("SRGB", vg::TextureInternalFormat::SRGB);
        env.addValue("SRGB8", vg::TextureInternalFormat::SRGB8);
        env.addValue("SRGB8_ALPHA8", vg::TextureInternalFormat::SRGB8_ALPHA8);
        env.addValue("SRGB_ALPHA", vg::TextureInternalFormat::SRGB_ALPHA);
        env.addValue("STENCIL_INDEX1", vg::TextureInternalFormat::STENCIL_INDEX1);
        env.addValue("STENCIL_INDEX4", vg::TextureInternalFormat::STENCIL_INDEX4);
        env.addValue("STENCIL_INDEX8", vg::TextureInternalFormat::STENCIL_INDEX8);
        env.addValue("STENCIL_INDEX16", vg::TextureInternalFormat::STENCIL_INDEX16);
        env.addValue("ONE", vg::TextureInternalFormat::ONE);
        env.addValue("TWO", vg::TextureInternalFormat::TWO);
        env.addValue("THREE", vg::TextureInternalFormat::THREE);
        env.addValue("FOUR", vg::TextureInternalFormat::FOUR);
    }
    { // vg::TextureFormat
        env.setNamespaces("TextureFormat");
        env.addValue("NONE", vg::TextureFormat::NONE);
        env.addValue("ALPHA", vg::TextureFormat::ALPHA);
        env.addValue("ALPHA_INTEGER", vg::TextureFormat::ALPHA_INTEGER);
        env.addValue("BGR", vg::TextureFormat::BGR);
        env.addValue("BGRA", vg::TextureFormat::BGRA);
        env.addValue("BGRA_INTEGER", vg::TextureFormat::BGRA_INTEGER);
        env.addValue("BGR_INTEGER", vg::TextureFormat::BGR_INTEGER);
        env.addValue("BLUE", vg::TextureFormat::BLUE);
        env.addValue("BLUE_INTEGER", vg::TextureFormat::BLUE_INTEGER);
        env.addValue("COLOR_INDEX", vg::TextureFormat::COLOR_INDEX);
        env.addValue("DEPTH_COMPONENT", vg::TextureFormat::DEPTH_COMPONENT);
        env.addValue("DEPTH_STENCIL", vg::TextureFormat::DEPTH_STENCIL);
        env.addValue("GREEN", vg::TextureFormat::GREEN);
        env.addValue("GREEN_INTEGER", vg::TextureFormat::GREEN_INTEGER);
        env.addValue("LUMINANCE", vg::TextureFormat::LUMINANCE);
        env.addValue("LUMINANCE_ALPHA", vg::TextureFormat::LUMINANCE_ALPHA);
        env.addValue("RED", vg::TextureFormat::RED);
        env.addValue("RED_INTEGER", vg::TextureFormat::RED_INTEGER);
        env.addValue("RG", vg::TextureFormat::RG);
        env.addValue("RGB", vg::TextureFormat::RGB);
        env.addValue("RGBA", vg::TextureFormat::RGBA);
        env.addValue("RGBA_INTEGER", vg::TextureFormat::RGBA_INTEGER);
        env.addValue("RGB_INTEGER", vg::TextureFormat::RGB_INTEGER);
        env.addValue("RG_INTEGER", vg::TextureFormat::RG_INTEGER);
        env.addValue("STENCIL_INDEX", vg::TextureFormat::STENCIL_INDEX);
        env.addValue("UNSIGNED_INT", vg::TextureFormat::UNSIGNED_INT);
        env.addValue("UNSIGNED_SHORT", vg::TextureFormat::UNSIGNED_SHORT);
    }
    { // vg::TexturePixelType
        env.setNamespaces("TexturePixelType");
        env.addValue("NONE", vg::TexturePixelType::NONE);
        env.addValue("BYTE", vg::TexturePixelType::BYTE);
        env.addValue("FLOAT", vg::TexturePixelType::FLOAT);
        env.addValue("FLOAT_32_UNSIGNED_INT_24_8_REV", vg::TexturePixelType::FLOAT_32_UNSIGNED_INT_24_8_REV);
        env.addValue("HALF_FLOAT", vg::TexturePixelType::HALF_FLOAT);
        env.addValue("INT", vg::TexturePixelType::INT);
        env.addValue("SHORT", vg::TexturePixelType::SHORT);
        env.addValue("UNSIGNED_BYTE", vg::TexturePixelType::UNSIGNED_BYTE);
        env.addValue("UNSIGNED_BYTE_3_3_2", vg::TexturePixelType::UNSIGNED_BYTE_3_3_2);
        env.addValue("UNSIGNED_INT", vg::TexturePixelType::UNSIGNED_INT);
        env.addValue("UNSIGNED_INT_10_10_10_2", vg::TexturePixelType::UNSIGNED_INT_10_10_10_2);
        env.addValue("UNSIGNED_INT_24_8", vg::TexturePixelType::UNSIGNED_INT_24_8);
        env.addValue("UNSIGNED_INT_8_8_8_8", vg::TexturePixelType::UNSIGNED_INT_8_8_8_8);
        env.addValue("UNSIGNED_SHORT", vg::TexturePixelType::UNSIGNED_SHORT);
        env.addValue("UNSIGNED_SHORT_4_4_4_4", vg::TexturePixelType::UNSIGNED_SHORT_4_4_4_4);
        env.addValue("UNSIGNED_SHORT_5_5_5_1", vg::TexturePixelType::UNSIGNED_SHORT_5_5_5_1);
        env.addValue("UNSIGNED_SHORT_5_6_5", vg::TexturePixelType::UNSIGNED_SHORT_5_6_5);
    }
    { // vg::TextureTarget
        env.setNamespaces("TextureTarget");
        env.addValue("NONE", vg::TextureTarget::NONE);
        env.addValue("PROXY_TEXTURE_1D", vg::TextureTarget::PROXY_TEXTURE_1D);
        env.addValue("PROXY_TEXTURE_1D_ARRAY", vg::TextureTarget::PROXY_TEXTURE_1D_ARRAY);
        env.addValue("PROXY_TEXTURE_2D", vg::TextureTarget::PROXY_TEXTURE_2D);
        env.addValue("PROXY_TEXTURE_2D_ARRAY", vg::TextureTarget::PROXY_TEXTURE_2D_ARRAY);
        env.addValue("PROXY_TEXTURE_2D_MULTISAMPLE", vg::TextureTarget::PROXY_TEXTURE_2D_MULTISAMPLE);
        env.addValue("PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY", vg::TextureTarget::PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY);
        env.addValue("PROXY_TEXTURE_3D", vg::TextureTarget::PROXY_TEXTURE_3D);
        env.addValue("PROXY_TEXTURE_CUBE_MAP", vg::TextureTarget::PROXY_TEXTURE_CUBE_MAP);
        env.addValue("PROXY_TEXTURE_CUBE_MAP_ARRAY", vg::TextureTarget::PROXY_TEXTURE_CUBE_MAP_ARRAY);
        env.addValue("PROXY_TEXTURE_RECTANGLE", vg::TextureTarget::PROXY_TEXTURE_RECTANGLE);
        env.addValue("TEXTURE_1D", vg::TextureTarget::TEXTURE_1D);
        env.addValue("TEXTURE_1D_ARRAY", vg::TextureTarget::TEXTURE_1D_ARRAY);
        env.addValue("TEXTURE_2D", vg::TextureTarget::TEXTURE_2D);
        env.addValue("TEXTURE_2D_ARRAY", vg::TextureTarget::TEXTURE_2D_ARRAY);
        env.addValue("TEXTURE_2D_MULTISAMPLE", vg::TextureTarget::TEXTURE_2D_MULTISAMPLE);
        env.addValue("TEXTURE_2D_MULTISAMPLE_ARRAY", vg::TextureTarget::TEXTURE_2D_MULTISAMPLE_ARRAY);
        env.addValue("TEXTURE_3D", vg::TextureTarget::TEXTURE_3D);
        env.addValue("TEXTURE_BASE_LEVEL", vg::TextureTarget::TEXTURE_BASE_LEVEL);
        env.addValue("TEXTURE_BINDING_CUBE_MAP", vg::TextureTarget::TEXTURE_BINDING_CUBE_MAP);
        env.addValue("TEXTURE_BUFFER", vg::TextureTarget::TEXTURE_BUFFER);
        env.addValue("TEXTURE_CUBE_MAP", vg::TextureTarget::TEXTURE_CUBE_MAP);
        env.addValue("TEXTURE_CUBE_MAP_ARRAY", vg::TextureTarget::TEXTURE_CUBE_MAP_ARRAY);
        env.addValue("TEXTURE_CUBE_MAP_NEGATIVE_X", vg::TextureTarget::TEXTURE_CUBE_MAP_NEGATIVE_X);
        env.addValue("TEXTURE_CUBE_MAP_NEGATIVE_Y", vg::TextureTarget::TEXTURE_CUBE_MAP_NEGATIVE_Y);
        env.addValue("TEXTURE_CUBE_MAP_NEGATIVE_Z", vg::TextureTarget::TEXTURE_CUBE_MAP_NEGATIVE_Z);
        env.addValue("TEXTURE_CUBE_MAP_POSITIVE_X", vg::TextureTarget::TEXTURE_CUBE_MAP_POSITIVE_X);
        env.addValue("TEXTURE_CUBE_MAP_POSITIVE_Y", vg::TextureTarget::TEXTURE_CUBE_MAP_POSITIVE_Y);
        env.addValue("TEXTURE_CUBE_MAP_POSITIVE_Z", vg::TextureTarget::TEXTURE_CUBE_MAP_POSITIVE_Z);
        env.addValue("TEXTURE_MAX_LEVEL", vg::TextureTarget::TEXTURE_MAX_LEVEL);
        env.addValue("TEXTURE_MAX_LOD", vg::TextureTarget::TEXTURE_MAX_LOD);
        env.addValue("TEXTURE_MIN_LOD", vg::TextureTarget::TEXTURE_MIN_LOD);
        env.addValue("TEXTURE_RECTANGLE", vg::TextureTarget::TEXTURE_RECTANGLE);
    }
    { // vg::GradientType
        env.setNamespaces("GradientType");
        env.addValue("NONE", vg::GradientType::NONE);
        env.addValue("HORIZONTAL", vg::GradientType::HORIZONTAL);
        env.addValue("VERTICAL", vg::GradientType::VERTICAL);
        env.addValue("LEFT_DIAGONAL", vg::GradientType::LEFT_DIAGONAL);
        env.addValue("RIGHT_DIAGONAL", vg::GradientType::RIGHT_DIAGONAL);
    }

    env.setNamespaces();
}

// TODO: Complete this and reevaluate parameter attribute.
void vscript::GraphicsScriptInterface::registerFunctions(vscript::Environment& env VORB_UNUSED) {
    // Empty for now
}
