#include "stdafx.h"
#include "graphics/GpuMemory.h"

#include <GL/glew.h>

#include "graphics/ImageIO.h"
#include "graphics/SamplerState.h"
#include "graphics/ImageIO.h"
#include "utils.h"

#define RGBA_BYTES 4

ui32 vg::GpuMemory::m_totalVramUsage = 0;
ui32 vg::GpuMemory::m_textureVramUsage = 0;
ui32 vg::GpuMemory::m_bufferVramUsage = 0;

std::unordered_map<VGTexture, ui32> vg::GpuMemory::m_textures;
std::unordered_map<VGBuffer, ui32> vg::GpuMemory::m_buffers;

void vg::GpuMemory::uploadTexture(VGTexture texture,
                                  const vg::BitmapResource* res,
                                  TexturePixelType texturePixelType /*= TexturePixelType::UNSIGNED_BYTE*/,
                                  vg::TextureTarget textureTarget /*= vg::TextureTarget::TEXTURE_2D*/,
                                  vg::SamplerState* samplingParameters /*= &SamplerState::LINEAR_CLAMP_MIPMAP*/,
                                  vg::TextureInternalFormat internalFormat /* = vg::TextureInternalFormat::RGBA*/,
                                  vg::TextureFormat textureFormat /* = vg::TextureFormat::RGBA */,
                                  i32 mipmapLevels /* = INT_MAX */) {
    // Determine The Maximum Number Of Mipmap Levels Available
    i32 maxMipmapLevels = 0;
    i32 size = MIN(res->width, res->height);
    while (size > 1) {
        maxMipmapLevels++;
        size >>= 1;
    }

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(static_cast<GLenum>(textureTarget), texture);
    switch (textureTarget) {
        case TextureTarget::TEXTURE_1D:
        case TextureTarget::PROXY_TEXTURE_1D:
            glTexImage1D((VGEnum)textureTarget, 0, (VGEnum)internalFormat, res->width, 0, (VGEnum)textureFormat, (VGEnum)texturePixelType, res->bytesUI8);
            break;
        default:
            glTexImage2D((VGEnum)textureTarget, 0, (VGEnum)internalFormat, res->width, res->height, 0, (VGEnum)textureFormat, (VGEnum)texturePixelType, res->bytesUI8);
            break;
    }
    // Setup Texture Sampling Parameters
    samplingParameters->set((VGEnum)textureTarget);

    // Get the number of mipmaps for this image
    mipmapLevels = MIN(mipmapLevels, maxMipmapLevels);

    // Create Mipmaps If Necessary
    if (mipmapLevels > 0) {
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glTexParameteri((VGEnum)textureTarget, GL_TEXTURE_MAX_LOD, mipmapLevels);
        glTexParameteri((VGEnum)textureTarget, GL_TEXTURE_MAX_LEVEL, mipmapLevels);
        glEnable((VGEnum)textureTarget);
        glGenerateMipmap((VGEnum)textureTarget);
    }

    // Calculate memory usage
    ui32 vramUsage = res->width * res->height * RGBA_BYTES;

    // If this texture already exists, change its vram usage
    auto it = m_textures.find(texture);
    if (it != m_textures.end()) {
        // Reduce total vram usage
        m_totalVramUsage -= it->second;
        m_textureVramUsage -= it->second;
    }
    // Store the texture and its memory usage
    m_textures[texture] = vramUsage;
    m_totalVramUsage += vramUsage;
    m_textureVramUsage += vramUsage;
    // Unbind texture
    glBindTexture((VGEnum)textureTarget, texture);
}

void vg::GpuMemory::freeTexture(VGTexture& textureID) {
    
    // See if the texture was uploaded through GpuMemory
    auto it = m_textures.find(textureID);
    if (it != m_textures.end()) {
        // Reduce total vram usage
        m_totalVramUsage -= it->second;
        m_textureVramUsage -= it->second;
        m_textures.erase(it);
    }

    // Delete the texture
    glDeleteTextures(1, &textureID);
    textureID = 0;
}

void vg::GpuMemory::uploadBufferData(VGBuffer bufferID, BufferTarget target, ui32 bufferSize,
                                     const void* data,
                                     BufferUsageHint usage /* = BufferUsageHint::STATIC_DRAW */)
{
    // Orphan the buffer by default
    glBufferData(static_cast<GLenum>(target), bufferSize, nullptr, static_cast<GLenum>(usage));
    glBufferSubData(static_cast<GLenum>(target), 0, bufferSize, data);

    // Track the VRAM usage
    auto it = m_buffers.find(bufferID);
    if (it != m_buffers.end()) {
        ui32 memoryDiff = bufferSize - it->second;
        m_totalVramUsage += memoryDiff;
        m_bufferVramUsage += memoryDiff;
        it->second = bufferSize;
    } else {
        // Start tracking the buffer object if it is not tracked
        m_buffers[bufferID] = bufferSize;
        m_totalVramUsage += bufferSize;
        m_bufferVramUsage += bufferSize;
    }
}

void vg::GpuMemory::freeBuffer(VGBuffer& bufferID)
{
    // Reduce our memory counters
    auto it = m_buffers.find(bufferID);
    if (it != m_buffers.end()) {
        m_totalVramUsage -= it->second;
        m_bufferVramUsage -= it->second;
        m_buffers.erase(it);
    }

    // Delete the buffer
    glDeleteBuffers(1, &bufferID);
    bufferID = 0;
}

void vg::GpuMemory::changeTextureMemory(ui32 s) {
    m_textureVramUsage += s;
    m_totalVramUsage += s;
}
void vg::GpuMemory::changeBufferMemory(ui32 s) {
    m_bufferVramUsage += s;
    m_totalVramUsage += s;
}

ui32 vg::GpuMemory::getFormatSize(ui32 format) {
    switch (format) {
    case GL_RGBA4:
    case GL_RGBA16:
        return 2;
    case GL_RGBA8:
        return 4;
    default:
        return 0;
    }
}
