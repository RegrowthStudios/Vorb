//
// GBuffer.h
// Vorb Engine
//
// Created by Cristian Zaloj on 27 Apr 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file GBuffer.h
 * @brief A special render target used for deferred rendering
 */

#pragma once

#ifndef Vorb_GBuffer_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_GBuffer_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

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
         /*! @brief Information that specifies size and location of a texture in the GBuffer
          */
        struct GBufferAttachment {
        public:
            vg::TextureInternalFormat format; ///< Internal format for the attachment (all must be the same size).
            vg::TextureFormat pixelFormat;
            vg::TexturePixelType pixelType;
            ui32 number; ///< Attachment index for the texture [0, MaxAttachments).
        };

        /// Geometry and light render target for deferred rendering
        class GBuffer {
        public:
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
            GBuffer& init(const Array<GBufferAttachment>& attachments, vg::TextureInternalFormat lightFormat);
            /// Attach a depth buffer to this GBuffer
            /// @param depthFormat: Precision used for depth buffer
            /// @return Self
            GBuffer& initDepth(TextureInternalFormat depthFormat = TextureInternalFormat::DEPTH_COMPONENT32);
            /// Attack a depth and stencil buffer to this GBuffer
            /// @param depthFormat: Precision used for depth and stencil buffer
            /// @return Self
            GBuffer& initDepthStencil(TextureInternalFormat depthFormat = TextureInternalFormat::DEPTH24_STENCIL8);

            void initTarget(const ui32v2& _size, const ui32& texID, const GBufferAttachment& attachment);
            /// Destroy all render targets
            void dispose();

            /// Set up the geometry targets to be active
            void useGeometry();
            /// Set up the light target to be active
            void useLight();

            /// Bind Geometry Texture
            /// @param i: Which Geometry texture to bind
            /// @param textureUnit Position to bind texture
            void bindGeometryTexture(size_t i, ui32 textureUnit);

            /// Bind Depth Texture
            /// @param textureUnit Position to bind texture
            void bindDepthTexture(ui32 textureUnit);
            
            /// Bind Light Texture
            /// @param textureUnit Position to bind texture
            void bindLightTexture(ui32 textureUnit);

            /// @return OpenGL texture IDs
            const VGTexture& getGeometryTexture(size_t i) const {
                return m_textures[i];
            }
            /// @return Light texture
            const VGTexture& getLightTexture() const {
                return m_textures[m_textures.size() - 1];
            }

            void setSize(ui32 width, ui32 height) {
                m_size.x = width;
                m_size.y = height;
            }

            void setSize(const ui32v2& size) {
                m_size = size;
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

            const VGFramebuffer& getGeometryID() const {
                return m_fboGeom;
            }
            const VGTexture& getDepthTexture() const {
                return m_texDepth;
            }
        private:
            ui32v2 m_size; ///< The width and height of the GBuffer

            VGFramebuffer m_fboGeom; ///< The rendering target for geometry
            VGFramebuffer m_fboLight; ///< The rendering target for light
            Array<VGTexture> m_textures; ///< An array of all the textures
            VGTexture m_texDepth = 0; ///< Depth texture of GBuffer
        };
    }
}
namespace vg = vorb::graphics;


#endif // !Vorb_GBuffer_h__
