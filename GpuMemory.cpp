#include "stdafx.h"
#include "GpuMemory.h"

#include "ImageLoader.h"
#include "utils.h"

#include <GL/glew.h>

#define RGBA_BYTES 4

ui32 vg::GpuMemory::m_totalVramUsage = 0;
ui32 vg::GpuMemory::m_textureVramUsage = 0;
ui32 vg::GpuMemory::m_bufferVramUsage = 0;

std::unordered_map<VGTexture, ui32> vg::GpuMemory::m_textures;
std::unordered_map<VGBuffer, ui32> vg::GpuMemory::m_buffers;

void vg::GpuMemory::uploadTexture(VGTexture texture,
                          const ui8* pixels,
                          ui32 width,
                          ui32 height,
                          SamplerState* samplingParameters,
                          vg::TextureInternalFormat internalFormat /* = vg::TextureInternalFormat::RGBA*/,
                          vg::TextureFormat textureFormat /* = vg::TextureFormat::RGBA */,
                          i32 mipmapLevels /* = INT_MAX */) {
    // Determine The Maximum Number Of Mipmap Levels Available
    i32 maxMipmapLevels = 0;
    i32 size = MIN(width, height);
    while (size > 1) {
        maxMipmapLevels++;
        size >>= 1;
    }

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, (VGEnum)internalFormat, width, height, 0, (VGEnum)textureFormat, GL_UNSIGNED_BYTE, pixels);

    // Setup Texture Sampling Parameters
    samplingParameters->set(GL_TEXTURE_2D);

    // Get the number of mipmaps for this image
    mipmapLevels = MIN(mipmapLevels, maxMipmapLevels);

    // Create Mipmaps If Necessary
    if (mipmapLevels > 0) {
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, mipmapLevels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevels);
        glEnable(GL_TEXTURE_2D);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Calculate memory usage
    ui32 vramUsage = width * height * RGBA_BYTES;

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
