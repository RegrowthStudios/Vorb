///
/// GBuffer.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 21 Nov 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// A special render target used for deferred rendering
///

#pragma once

#ifndef GBuffer_h__
#define GBuffer_h__

#include "GLEnums.h"
#include "gtypes.h"

/************************************************************************/
/* GBuffer breakdown                                                    */
/* -----------------------------------------------------------------    */
/* | Diffuse R     | Diffuse G     | Diffuse B     | Light Model   |    */
/* -----------------------------------------------------------------    */
/* | Normal X      | Normal Y      | Normal Z      | Specular Pow  |    */
/* -----------------------------------------------------------------    */
/* | Depth                         |                               |    */
/* -----------------------------------------------------------------    */
/* | Light R       | Light G       | Light B       |  X X X X X X  |    */
/* -----------------------------------------------------------------    */
/************************************************************************/
#define GBUFFER_INTERNAL_FORMAT_COLOR vg::TextureInternalFormat::RGBA16F
#define GBUFFER_INTERNAL_FORMAT_NORMAL vg::TextureInternalFormat::RGBA16F
#define GBUFFER_INTERNAL_FORMAT_DEPTH vg::TextureInternalFormat::RG32F
#define GBUFFER_INTERNAL_FORMAT_LIGHT vg::TextureInternalFormat::RGB16F

namespace vorb {
    namespace graphics {
        /// Geometry and light render target for deferred rendering
        class GBuffer {
        public:
            /// Name-defined textures stored in a GBuffer
            struct TextureIDs {
            public:
                VGTexture color; ///< RGBA-16f colorRGB-specularAlpha texture
                VGTexture normal; ///< RGBA-16f normal texture
                VGTexture depth; ///< RG-32f texture
                VGTexture light; ///< RGB-16f texture
            };

            /// Set up a GBuffer with a certain size
            /// @param w: Width in pixels of each target
            /// @param h: Height in pixels of each target
            GBuffer(ui32 w = 0, ui32 h = 0);
            /// Set up a GBuffer with a certain size
            /// @param s: Size in pixels of each target
            GBuffer(ui32v2 s) : GBuffer(s.x, s.y) {
                // Empty
            }

            /// Create the value-based render targets
            /// @return Self
            GBuffer& init();
            /// Attach a depth buffer to this GBuffer
            /// @param depthFormat: Precision used for depth buffer
            /// @return Self
            GBuffer& initDepth(TextureInternalFormat depthFormat = TextureInternalFormat::DEPTH_COMPONENT32);
            /// Attack a depth and stencil buffer to this GBuffer
            /// @param depthFormat: Precision used for depth and stencil buffer
            /// @return Self
            GBuffer& initDepthStencil(TextureInternalFormat depthFormat = TextureInternalFormat::DEPTH24_STENCIL8);

            void initTarget(const ui32v2& _size, const ui32& texID, const vg::TextureInternalFormat& format, const ui32& attachment);
            /// Destroy all render targets
            void dispose();

            /// Set up the geometry targets to be active
            void useGeometry();
            /// Set up the light target to be active
            void useLight();

            /// @return OpenGL texture IDs
            const GBuffer::TextureIDs& getTextureIDs() const {
                return m_tex;
            }

            /// @return Size of the GBuffer in pixels (W,H)
            const ui32v2& getSize() const {
                return m_size;
            }
            /// @return Width of the GBuffer in pixels
            const ui32& getWidth() const {
                return m_size.x;
            }
            /// @return Height of the GBuffer in pixels
            const ui32& getHeight() const {
                return m_size.y;
            }

        private:
            ui32v2 m_size; ///< The width and height of the GBuffer

            VGFramebuffer m_fboGeom; ///< The rendering target for geometry
            VGFramebuffer m_fboLight; ///< The rendering target for light
            union {
                TextureIDs m_tex; ///< Named texture targets
                VGTexture m_textures[4]; ///< All 3 textures
            };
            VGTexture m_texDepth = 0; ///< Depth texture of GBuffer
        };
    }
}
namespace vg = vorb::graphics;

#endif // GBuffer_h__