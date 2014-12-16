///
/// GBuffer.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 21 Nov 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef GBuffer_h__
#define GBuffer_h__

#include "GLEnums.h"
#include "gtypes.h"

#define GBUFFER_INTERNAL_FORMAT_COLOR vg::TextureInternalFormat::RGBA16F
#define GBUFFER_INTERNAL_FORMAT_NORMAL vg::TextureInternalFormat::RGBA16F
#define GBUFFER_INTERNAL_FORMAT_DEPTH vg::TextureInternalFormat::RG32F

namespace vorb {
    namespace core {
		namespace graphics {
            /// 
            class GBuffer {
            public:
                /// Name-defined textures stored in a GBuffer
                struct TextureIDs {
                public:
                    VGTexture depth; ///< R-32f texture
                    VGTexture normal; ///< RGB-16f normal texture
                    VGTexture color; ///< RGBA-8f colorRGB-specularAlpha texture
                };

                /// 
                /// @param w: 
                /// @param h: 
                GBuffer(ui32 w = 0, ui32 h = 0);
                /// 
                /// @param s: 
                GBuffer(ui32v2 s) : GBuffer(s.x, s.y) {
                    // Empty
                }

                /// 
                /// @return
                GBuffer& init();
                /// 
                /// @param depthFormat: 
                /// @return 
                GBuffer& initDepth(TextureInternalFormat depthFormat = TextureInternalFormat::DEPTH_COMPONENT32);
                ///
                void dispose();

                /// 
                void use();

                /// @return OpenGL texture IDs
                const GBuffer::TextureIDs& getTextureIDs() const {
                    return m_tex;
                }

                /// @return Size of the FBO in pixels (W,H)
                const ui32v2& getSize() const {
                    return m_size;
                }
                /// @return Width of the FBO in pixels
                const ui32& getWidth() const {
                    return m_size.x;
                }
                /// @return Height of the FBO in pixels
                const ui32& getHeight() const {
                    return m_size.y;
                }

            private:
                ui32v2 m_size; ///< The width and height of the GBuffer

                VGFramebuffer m_fbo; ///< The rendering target
                union {
                    TextureIDs m_tex; ///< Named texture targets
                    VGTexture m_textures[3]; ///< All 3 textures
                };
                VGTexture m_texDepth = 0; ///< Depth texture of GBuffer
            };
		}
    }
}
namespace vg = vorb::core::graphics;

#endif // GBuffer_h__