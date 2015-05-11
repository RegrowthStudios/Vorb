#include "stdafx.h"
#include "graphics/TextureCache.h"

#include "io/IOManager.h"
#include "graphics/GpuMemory.h"

vg::TextureCache::TextureCache() {
    // Empty
}
vg::TextureCache::TextureCache(vio::IOManager* ioManager) :
m_ioManager(ioManager) {
    // Empty
}

vg::TextureCache::~TextureCache() {
    destroy();
}

vg::Texture vg::TextureCache::findTexture(const vio::Path& filePath) {
    auto it = _textureStringMap.find(filePath);
    if (it != _textureStringMap.end()) {
        return it->second;
    }

    return Texture();
}

vio::Path vg::TextureCache::getTexturePath(ui32 textureID) {
    auto it = _textureIdMap.find(textureID);
    if (it != _textureIdMap.end()) {
        return it->second->first;
    }
    return "";
}

vg::Texture vg::TextureCache::addTexture(const vio::Path& filePath,
                                         vg::TextureTarget textureTarget /*= vg::TextureTarget::TEXTURE_2D*/,
                                         SamplerState* samplingParameters /* = &SamplerState::LINEAR_CLAMP_MIPMAP */,
                                         vg::TextureInternalFormat internalFormat /* = vg::TextureInternalFormat::RGBA */,
                                         vg::TextureFormat textureFormat /* = vg::TextureFormat::RGBA */,
                                         i32 mipmapLevels /* = INT_MAX */) {
    // Get absolute path
    vio::Path texPath; 
    resolvePath(filePath, texPath);

    // Check if its already cached
    Texture texture = findTexture(texPath);
    if (texture.id) return texture;

    // Load the pixel data
    vg::ScopedBitmapResource rs = vg::ImageIO().load(texPath.getString(), vg::ImageIOFormat::RGBA_UI8);
    if (!rs.data) return Texture();
    texture.width = rs.width;
    texture.height = rs.height;
    texture.textureTarget = textureTarget;

    // Upload the texture through GpuMemory
    texture.id = GpuMemory::uploadTexture(&rs,
                                          textureTarget,
                                          samplingParameters,
                                          internalFormat,
                                          textureFormat,
                                          mipmapLevels);

    // Store the texture in the cache
    insertTexture(texPath, texture);
    return texture;
}

vg::Texture vg::TextureCache::addTexture(const vio::Path& filePath,
                                         OUT vg::BitmapResource& rvBitmap,
                                         vg::ImageIOFormat rvFormat,
                                         vg::TextureTarget textureTarget /*= vg::TextureTarget::TEXTURE_2D*/,
                                         SamplerState* samplingParameters /* = &SamplerState::LINEAR_CLAMP_MIPMAP */,
                                         vg::TextureInternalFormat internalFormat /* = vg::TextureInternalFormat::RGBA */,
                                         vg::TextureFormat textureFormat /* = vg::TextureFormat::RGBA */,
                                         i32 mipmapLevels /* = INT_MAX */) {
    // Get absolute path
    vio::Path texPath;
    resolvePath(filePath, texPath);

    // Check if its already cached
    Texture texture = findTexture(texPath);
    if (texture.id) return texture;

    // Load the pixel data
    rvBitmap = vg::ImageIO().load(texPath.getString(), rvFormat);
    if (!rvBitmap.data) return Texture();
    texture.width = rvBitmap.width;
    texture.height = rvBitmap.height;
    texture.textureTarget = textureTarget;

    // Upload the texture through GpuMemory
    texture.id = GpuMemory::uploadTexture(&rvBitmap,
                                          textureTarget,
                                          samplingParameters,
                                          internalFormat,
                                          textureFormat,
                                          mipmapLevels);

    // Store the texture in the cache
    insertTexture(texPath, texture);
    return texture;
}

vg::Texture vg::TextureCache::addTexture(const vio::Path& filePath,
                                         const vg::BitmapResource* rs,
                                         vg::TextureTarget textureTarget /*= vg::TextureTarget::TEXTURE_2D*/,
                                         SamplerState* samplingParameters /* = &SamplerState::LINEAR_CLAMP_MIPMAP */,
                                         vg::TextureInternalFormat internalFormat /* = vg::TextureInternalFormat::RGBA */,
                                         vg::TextureFormat textureFormat /* = vg::TextureFormat::RGBA */,
                                         i32 mipmapLevels /* = INT_MAX */) {

    // Get absolute path
    vio::Path texPath;
    resolvePath(filePath, texPath);

    // Check if its already cached
    Texture texture = findTexture(texPath);
    if (texture.id) return texture;

    // Upload the texture through GpuMemory
    texture.id = GpuMemory::uploadTexture(rs,
                                          textureTarget,
                                          samplingParameters,
                                          internalFormat,
                                          textureFormat,
                                          mipmapLevels);
    texture.width = rs->width;
    texture.height = rs->height;
    texture.textureTarget = textureTarget;

    // Store the texture in the cache
    insertTexture(texPath, texture);
    return texture;
}

void vg::TextureCache::addTexture(const vio::Path& filePath, const Texture& texture) {
    insertTexture(filePath, texture);
}

void vg::TextureCache::freeTexture(const vio::Path& filePath) {
    // Get absolute path
    vio::Path texPath;
    resolvePath(filePath, texPath);
   
    // Check the cache for the texture
    auto it = _textureStringMap.find(texPath);
    if (it != _textureStringMap.end()) {
        // Erase from the set ( must be done before freeTexture )
        _textureIdMap.erase(it->second.id);
        // If we have the texture, free it
        GpuMemory::freeTexture(it->second.id);
        // Remove from the map
        _textureStringMap.erase(it);
       
    }
}

void vg::TextureCache::freeTexture(ui32& textureID) {
    auto it = _textureIdMap.find(textureID);
    if (it != _textureIdMap.end()) {
        // Free the texture
        GpuMemory::freeTexture(textureID);
        // Quickly erase from the string map since we have the iterator
        _textureStringMap.erase(it->second);
     
        _textureIdMap.erase(it);
    }
}

void vg::TextureCache::freeTexture(Texture& texture) {
    auto it = _textureIdMap.find(texture.id);
    if (it != _textureIdMap.end()) {
        // Free the texture
        GpuMemory::freeTexture(texture.id);
        // Quickly erase from the string map since we have the iterator
        _textureStringMap.erase(it->second);

        _textureIdMap.erase(it);
    }
}

void vg::TextureCache::destroy() {
    for (auto tex : _textureStringMap) {
        GpuMemory::freeTexture(tex.second.id);
    }
}

void vg::TextureCache::insertTexture(const vio::Path& filePath, const Texture& texture) {
    // We store an iterator to the map node in the _textureIdMap
    // so that we can quickly remove textures from the cache
    _textureIdMap[texture.id] = _textureStringMap.insert(std::make_pair(filePath, texture)).first;
}

void vg::TextureCache::resolvePath(const vio::Path& path, OUT vio::Path& fullPath) {
    if (m_ioManager) {
        m_ioManager->resolvePath(path, fullPath);
    } else {
        fullPath = path;
    }
}
