//
// GpuMemory.h
// Vorb Engine
//
// Created by Ben Arnold on 20 Oct 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file GpuMemory.h
 * @brief This file provides a wrapper around VRAM and openGL object uploads.
 */

#pragma once

#ifndef Vorb_GpuMemory_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_GpuMemory_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <unordered_map>

#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "GLEnums.h"
#include "gtypes.h"
#include "SamplerState.h"

namespace vorb {
    namespace graphics {
        enum class BufferTarget : VGEnum;
        enum class BufferUsageHint : VGEnum;
        class BitmapResource;

        // TODO(Ben): Flesh this out
        class GpuMemory {
        public:

            /// Uploads a texture to the GPU.
            /// @param data: The bitmap data.
            /// @param width: The width of the data
            /// @param height: The height of the data
            /// @param texturePixelType: Format of the resource pixels.
            /// @param samplingParameters: The texture sampler parameters.
            /// @param internalFormat: Internal pixel data format.
            /// @param textureFormat: Format of uploaded pixels.
            /// @param mipmapLevels: The max number of mipmap levels.
            static void uploadTexture(VGTexture texture,
                                      const void* data,
                                      ui32 width,
                                      ui32 height,
                                      TexturePixelType texturePixelType = TexturePixelType::UNSIGNED_BYTE,
                                      TextureTarget textureTarget = TextureTarget::TEXTURE_2D,
                                      SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                                      TextureInternalFormat internalFormat = TextureInternalFormat::RGBA,
                                      TextureFormat textureFormat = TextureFormat::RGBA,
                                      i32 mipmapLevels = INT_MAX);

            /// Uploads a texture to the GPU.
            /// @param data: The bitmap data.
            /// @param width: The width of the data
            /// @param height: The height of the data
            /// @param texturePixelType: Format of the resource pixels.
            /// @param samplingParameters: The texture sampler parameters.
            /// @param internalFormat: Internal pixel data format.
            /// @param textureFormat: Format of uploaded pixels.
            /// @param mipmapLevels: The max number of mipmap levels.
            static VGTexture uploadTexture(const void* data,
                                           ui32 width,
                                           ui32 height,
                                           TexturePixelType texturePixelType = TexturePixelType::UNSIGNED_BYTE,
                                           TextureTarget textureTarget = TextureTarget::TEXTURE_2D,
                                           SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                                           TextureInternalFormat internalFormat = TextureInternalFormat::RGBA,
                                           TextureFormat textureFormat = TextureFormat::RGBA,
                                           i32 mipmapLevels = INT_MAX) {
                VGTexture textureID;
                glGenTextures(1, &textureID);
                uploadTexture(textureID, data, width, height, texturePixelType,
                              textureTarget, samplingParameters,
                              internalFormat, textureFormat, mipmapLevels);
                return textureID;
            }

            /// Uploads a texture to the GPU.
            /// @param res: The bitmap resource.
            /// @param texturePixelType: Format of the resource pixels.
            /// @param samplingParameters: The texture sampler parameters.
            /// @param internalFormat: Internal pixel data format.
            /// @param textureFormat: Format of uploaded pixels.
            /// @param mipmapLevels: The max number of mipmap levels.
            static void uploadTexture(VGTexture texture,
                                      const BitmapResource* res,
                                      TexturePixelType texturePixelType = TexturePixelType::UNSIGNED_BYTE,
                                      TextureTarget textureTarget = TextureTarget::TEXTURE_2D,
                                      SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                                      TextureInternalFormat internalFormat = TextureInternalFormat::RGBA,
                                      TextureFormat textureFormat = TextureFormat::RGBA,
                                      i32 mipmapLevels = INT_MAX);
            /// Uploads a texture to the GPU.
            /// @param res: The bitmap resource.
            /// @param texturePixelType: Format of the resource pixels.
            /// @param samplingParameters: The texture sampler parameters.
            /// @param internalFormat: Internal pixel data format.
            /// @param textureFormat: Format of uploaded pixels.
            /// @param mipmapLevels: The max number of mipmap levels.
            /// @return The texture ID.
            static VGTexture uploadTexture(const BitmapResource* res,
                                           TexturePixelType texturePixelType = TexturePixelType::UNSIGNED_BYTE,
                                           TextureTarget textureTarget = TextureTarget::TEXTURE_2D,
                                           SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                                           TextureInternalFormat internalFormat = TextureInternalFormat::RGBA,
                                           TextureFormat textureFormat = TextureFormat::RGBA,
                                           i32 mipmapLevels = INT_MAX) {
                // Create one OpenGL texture
                VGTexture textureID;
                glGenTextures(1, &textureID);
                uploadTexture(textureID, res, texturePixelType,
                              textureTarget, samplingParameters,
                              internalFormat, textureFormat, mipmapLevels);
                return textureID;
            }

            /// Frees a texture and sets its ID to 0
            /// @param textureID: The texture to free. Will be set to 0.
            static void freeTexture(VGTexture& textureID);

            /// Creates an OpenGL buffer object
            /// @param vbo: The result buffer ID
            static void createBuffer(VGBuffer& bufferID) {
                glGenBuffers(1, &bufferID);
                m_buffers[bufferID] = 0;
            }

            /// Frees an OpenGL buffer object and sets the
            /// ID to 0.
            /// @param bufferID: The ID of the buffer
            static void freeBuffer(VGBuffer& bufferID);

            /// Binds a buffer
            /// @param bufferID: The ID of the buffer
            /// @param target: The desired buffer target
            static void bindBuffer(const VGBuffer& bufferID, BufferTarget target) {
                glBindBuffer(static_cast<GLenum>(target), bufferID);
            }

            /// Uploads data to a buffer. Make sure the buffer is bound using
            /// bindBuffer before uploading.
            /// @param bufferID: The ID of the buffer
            /// @param target: The desired buffer target
            /// @param bufferSize: The size of data
            /// @param data: Pointer to the buffer data
            /// @usage: The desired buffer usage
            static void uploadBufferData(VGBuffer bufferID,
                                         BufferTarget target,
                                         ui32 bufferSize,
                                         const void* data,
                                         BufferUsageHint usage = BufferUsageHint::STATIC_DRAW);

            /// Changes The Total Texture Memory Usage By A Specified Amount
            /// @param s: Amount Of Memory Change In Bytes
            static void changeTextureMemory(ui32 s);
            /// Changes The Total Buffer Memory Usage By A Specified Amount
            /// @param s: Amount Of Memory Change In Bytes
            static void changeBufferMemory(ui32 s);

            /// Gets the amount of VRAM used in bytes
            static ui32 getTotalVramUsage() {
                return m_totalVramUsage;
            }

            /// Gets the texture VRAM used in bytes
            static ui32 getTextureVramUsage() {
                return m_textureVramUsage;
            }

            /// Gets the texture VRAM used in bytes for one texture
            static ui32 getTextureVramUsage(VGTexture texture) {
                auto it = m_textures.find(texture);
                if (it != m_textures.end()) {
                    return it->second;
                }
                return 0;
            }

            /// Gets the buffer VRAM used in bytes
            static ui32 getBufferVramUsage() {
                return m_bufferVramUsage;
            }

            /// Gets the buffer VRAM used in bytes for one buffer
            static ui32 getBufferVramUsage(VGBuffer buffer) {
                auto it = m_buffers.find(buffer);
                if (it != m_buffers.end()) {
                    return it->second;
                }
                return 0;
            }

            static ui32 getFormatSize(ui32 format);
        private:
            static ui32 m_totalVramUsage; ///< The total VRAM usage by all objects
            static ui32 m_textureVramUsage; ///< The total VRAM usage by texture objects
            static ui32 m_bufferVramUsage; ///< The total VRAM usage by buffer objects

            static std::unordered_map<VGTexture, ui32> m_textures; ///< Store of texture objects
            static std::unordered_map<VGBuffer, ui32> m_buffers; ///< Store of buffer objects
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_GpuMemory_h__
