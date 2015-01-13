#include "stdafx.h"
#include "graphics/TextureCache.h"

#include "graphics/GpuMemory.h"

vg::TextureCache::TextureCache() {
    // Empty
}


vg::TextureCache::~TextureCache() {
    destroy();
}

vg::Texture vg::TextureCache::findTexture(const nString& filePath) {
    auto it = _textureStringMap.find(filePath);
    if (it != _textureStringMap.end()) {
        return it->second;
    }

    return Texture();
}

nString vg::TextureCache::getTexturePath(ui32 textureID) {
    auto it = _textureIdMap.find(textureID);
    if (it != _textureIdMap.end()) {
        return it->second->first;
    }
    return "";
}

vg::Texture vg::TextureCache::addTexture(const nString& filePath,
                                SamplerState* samplingParameters /* = &SamplerState::LINEAR_CLAMP_MIPMAP */,
                                i32 mipmapLevels /* = INT_MAX */) {

    // Check if its already cached
    Texture texture = findTexture(filePath);
    if (texture.id) return texture;

    // Buffer for the pixels
    std::vector <ui8> pixelStore;

    // Load the pixel data
    vio::ImageIO loader;
    if (!loader.loadPng(filePath, pixelStore, texture.width, texture.height)) {
        return Texture();
    }

    // Upload the texture through GpuMemory
    texture.id = GpuMemory::uploadTexture(pixelStore.data(), texture.width, texture.height, samplingParameters, mipmapLevels);

    // Store the texture in the cache
    insertTexture(filePath, texture);
    return texture;
}

vg::Texture vg::TextureCache::addTexture(const nString& filePath,
                              const ui8* pixels,
                              ui32 width,
                              ui32 height,
                              SamplerState* samplingParameters /* = &SamplerState::LINEAR_CLAMP_MIPMAP */,
                              i32 mipmapLevels /* = INT_MAX */) {
    // Check if its already cached
    Texture texture = findTexture(filePath);
    if (texture.id) return texture;

    // Upload the texture through GpuMemory
    texture.id = GpuMemory::uploadTexture(pixels, width, height, samplingParameters, mipmapLevels);
    texture.width = width;
    texture.height = height;

    // Store the texture in the cache
    insertTexture(filePath, texture);
    return texture;
}

void vg::TextureCache::addTexture(const nString& filePath, const Texture& texture) {
    insertTexture(filePath, texture);
}

void vg::TextureCache::freeTexture(const nString& filePath) {
    // Check the cache for the texture
    auto it = _textureStringMap.find(filePath);
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
    std::unordered_map <nString, Texture>().swap(_textureStringMap); ///< Textures store here keyed on filename
    std::map <ui32, std::unordered_map <nString, Texture>::iterator>().swap(_textureIdMap);
}

void vg::TextureCache::insertTexture(const nString& filePath, const Texture& texture) {
    // We store an iterator to the map node in the _textureIdMap
    // so that we can quickly remove textures from the cache
    _textureIdMap[texture.id] = _textureStringMap.insert(std::make_pair(filePath, texture)).first;
}
