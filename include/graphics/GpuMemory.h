// 
//  GpuMemory.h
//  Vorb Engine
//
//  Created by Ben Arnold on 20 Oct 2014
//  Copyright 2014 Regrowth Studios
//  All Rights Reserved
//  
//  This file provides a wrapper around VRAM and
//  openGL object uploads.
//

#pragma once

#ifndef GpuMemory_h__
#define GpuMemory_h__

#include "GLEnums.h"
#include "gtypes.h"

namespace vorb {
    namespace graphics {
        enum class BufferTarget : VGEnum;
        enum class BufferUsageHint : VGEnum;
        class SamplerState;
        class BitmapResource;

        // TODO(Ben): Flesh this out
        class GpuMemory {
        public:
            /// Uploads a texture to the GPU.
            /// @param pixels: The image pixels
            /// @param width: Width of the texture in pixels
            /// @param height: Height of the texture in pixels
            /// @param samplingParameters: The texture sampler parameters
            /// @param internalFormat: Internal pixel data format
            /// @param textureFormat: Format of uploaded pixels
            /// @param mipmapLevels: The max number of mipmap levels
            static void uploadTexture(VGTexture texture,
                                      const vg::BitmapResource* res,
                                      vg::SamplerState* samplingParameters,
                                      vg::TextureInternalFormat internalFormat = vg::TextureInternalFormat::RGBA,
                                      vg::TextureFormat textureFormat = vg::TextureFormat::RGBA,
                                      i32 mipmapLevels = INT_MAX);

            /// Uploads a texture to the GPU.
            /// @param pixels: The image pixels
            /// @param width: Width of the texture in pixels
            /// @param height: Height of the texture in pixels
            /// @param samplingParameters: The texture sampler parameters
            /// @param internalFormat: Internal pixel data format
            /// @param textureFormat: Format of uploaded pixels
            /// @param mipmapLevels: The max number of mipmap levels
            /// @return The texture ID
            static VGTexture uploadTexture(const vg::BitmapResource* res,
                                           SamplerState* samplingParameters,
                                           vg::TextureInternalFormat internalFormat = vg::TextureInternalFormat::RGBA,
                                           vg::TextureFormat textureFormat = vg::TextureFormat::RGBA,
                                           i32 mipmapLevels = INT_MAX) {
                // Create one OpenGL texture
                VGTexture textureID;
                glGenTextures(1, &textureID);
                uploadTexture(textureID, res, samplingParameters,
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

#endif // GpuMemory_h__