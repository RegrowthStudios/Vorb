#include "Vorb/stdafx.h"
#include "Vorb/graphics/TextureCache.h"

#include "Vorb/io/IOManager.h"
#include "Vorb/graphics/GpuMemory.h"

vg::TextureCache::TextureCache() :
    m_ownsIOManager(false),
    m_ioManager(nullptr) {
    // Empty
}

vg::TextureCache::~TextureCache() {
    dispose();
}

void vg::TextureCache::init(vio::IOManager* ioManager, bool ownsManager /*= false*/) {
    m_ioManager     = ioManager;
    m_ownsIOManager = ownsManager;
}

void vg::TextureCache::dispose() {
    for (auto tex : m_texturePathMap) {
        GpuMemory::freeTexture(tex.second.id);
    }
    TexturePathMap().swap(m_texturePathMap);
    PathIDMap().swap(m_pathIDMap);

    if (m_ownsIOManager) {
        delete m_ioManager;
        m_ioManager = nullptr;
    }
}

vg::Texture vg::TextureCache::findTexture(const vio::Path& filePath) {
    auto it = m_texturePathMap.find(filePath);
    if (it != m_texturePathMap.end()) {
        return it->second;
    }

    return Texture();
}

vio::Path vg::TextureCache::getTexturePath(ui32 textureID) {
    auto it = m_pathIDMap.find(textureID);
    if (it != m_pathIDMap.end()) {
        return it->second;
    }

    return "";
}

vg::Texture vg::TextureCache::addTexture(         const vio::Path& filePath,
                                                 vg::TextureTarget textureTarget      /* = vg::TextureTarget::TEXTURE_2D*/,
                                                     SamplerState* samplingParameters /* = &SamplerState::LINEAR_CLAMP_MIPMAP */,
                                         vg::TextureInternalFormat internalFormat     /* = vg::TextureInternalFormat::RGBA */,
                                                 vg::TextureFormat textureFormat      /* = vg::TextureFormat::RGBA */,
                                                               i32 mipmapLevels       /* = INT_MAX */,
                                                              bool flipV              /* = false*/) {
    // Get absolute path of texture.
    vio::Path texPath; 
    resolvePath(filePath, texPath);

    // Check if the texture is already cached.
    Texture texture = findTexture(texPath);
    if (texture.id) return texture;

    // Load the pixel data.
    vg::ScopedBitmapResource rs(vg::ImageIO().load(texPath.getString(), vg::ImageIOFormat::RGBA_UI8, flipV));
    if (!rs.data) return Texture();
    texture.width = rs.width;
    texture.height = rs.height;
    texture.textureTarget = textureTarget;

    // Upload the texture through GpuMemory.
    texture.id = GpuMemory::uploadTexture(&rs,
                                          TexturePixelType::UNSIGNED_BYTE,
                                          textureTarget,
                                          samplingParameters,
                                          internalFormat,
                                          textureFormat,
                                          mipmapLevels);

    // Store the texture in the cache.
    insertTexture(texPath, texture);
    return texture;
}

vg::Texture vg::TextureCache::addTexture(         const vio::Path& filePath,
                                           OUT vg::BitmapResource& rvBitmap,
                                                 vg::ImageIOFormat rvFormat,
                                                 vg::TextureTarget textureTarget      /* = vg::TextureTarget::TEXTURE_2D*/,
                                                     SamplerState* samplingParameters /* = &SamplerState::LINEAR_CLAMP_MIPMAP */,
                                         vg::TextureInternalFormat internalFormat     /* = vg::TextureInternalFormat::RGBA */,
                                                 vg::TextureFormat textureFormat      /* = vg::TextureFormat::RGBA */,
                                                               i32 mipmapLevels       /* = INT_MAX */,
                                                              bool flipV              /* = false*/) {
    // Get absolute path of texture.
    vio::Path texPath;
    resolvePath(filePath, texPath);

    // Check if the texture is already cached.
    Texture texture = findTexture(texPath);
    if (texture.id) return texture;

    // Load the pixel data.
    rvBitmap = vg::ImageIO().load(texPath.getString(), rvFormat, flipV);
    if (!rvBitmap.data) return Texture();
    texture.width = rvBitmap.width;
    texture.height = rvBitmap.height;
    texture.textureTarget = textureTarget;
    
    // Determine pixel type.
    // TODO(Ben): This is not a comprehensive list of pixel types.
    TexturePixelType pixelType;
    switch (rvFormat) {
        case ImageIOFormat::RGB_F32:
        case ImageIOFormat::RGB_F64:
        case ImageIOFormat::RGBA_F32:
        case ImageIOFormat::RGBA_F64:
            pixelType = TexturePixelType::FLOAT;          break;
        case ImageIOFormat::RGB_UI16:
        case ImageIOFormat::RGBA_UI16:
            pixelType = TexturePixelType::UNSIGNED_SHORT; break;
        default:
            pixelType = TexturePixelType::UNSIGNED_BYTE;  break;       
    }

    // Upload the texture through GpuMemory.
    texture.id = GpuMemory::uploadTexture(&rvBitmap,
                                          pixelType,
                                          textureTarget,
                                          samplingParameters,
                                          internalFormat,
                                          textureFormat,
                                          mipmapLevels);

    // Store the texture in the cache.
    insertTexture(texPath, texture);
    return texture;
}

vg::Texture vg::TextureCache::addTexture(         const vio::Path& filePath,
                                         const vg::BitmapResource* rs,
                                                  TexturePixelType texturePixelType   /* = TexturePixelType::UNSIGNED_BYTE*/,
                                                 vg::TextureTarget textureTarget      /* = vg::TextureTarget::TEXTURE_2D*/,
                                                     SamplerState* samplingParameters /* = &SamplerState::LINEAR_CLAMP_MIPMAP */,
                                         vg::TextureInternalFormat internalFormat     /* = vg::TextureInternalFormat::RGBA */,
                                                 vg::TextureFormat textureFormat      /* = vg::TextureFormat::RGBA */,
                                                               i32 mipmapLevels       /* = INT_MAX */) {
    // Get absolute path of texture.
    vio::Path texPath;
    resolvePath(filePath, texPath);

    // Check if the texture is already cached.
    Texture texture = findTexture(texPath);
    if (texture.id) return texture;

    // Upload the texture through GpuMemory.
    texture.id = GpuMemory::uploadTexture(rs,
                                          texturePixelType,
                                          textureTarget,
                                          samplingParameters,
                                          internalFormat,
                                          textureFormat,
                                          mipmapLevels);
    texture.width = rs->width;
    texture.height = rs->height;
    texture.textureTarget = textureTarget;

    // Store the texture in the cache.
    insertTexture(texPath, texture);
    return texture;
}

void vg::TextureCache::addTexture(const vio::Path& filePath, const Texture& texture) {
    insertTexture(filePath, texture);
}

bool vg::TextureCache::freeTexture(const vio::Path& filePath) {
    // Get absolute path of the texture.
    vio::Path texPath;
    resolvePath(filePath, texPath);
   
    // Check the cache for the texture.
    auto it = m_texturePathMap.find(texPath);
    if (it != m_texturePathMap.end()) {
        // Erase from the set ( must be done before freeTexture ).
        m_pathIDMap.erase(it->second.id);
        // If we have the texture, free it.
        GpuMemory::freeTexture(it->second.id);
        // Remove from the map.
        m_texturePathMap.erase(it);

        return true;
    }
    return false;
}

bool vg::TextureCache::freeTexture(VGTexture& textureID) {
    auto it = m_pathIDMap.find(textureID);
    if (it != m_pathIDMap.end()) {
        // Free the texture.
        GpuMemory::freeTexture(textureID);
        // Quickly erase from the path map since we have the iterator.
        m_texturePathMap.erase(it->second);
        m_pathIDMap.erase(it);

        return true;
    }
    return false;
}

bool vg::TextureCache::freeTexture(Texture& texture) {
    auto it = m_pathIDMap.find(texture.id);
    if (it != m_pathIDMap.end()) {
        // Free the texture.
        GpuMemory::freeTexture(texture.id);
        // Quickly erase from the path map since we have the iterator.
        m_texturePathMap.erase(it->second);
        m_pathIDMap.erase(it);

        return true;
    }
    return false;
}

void vg::TextureCache::insertTexture(const vio::Path& filePath, const Texture& texture) {
    // We store an iterator to the map node in m_pathIDMap
    // so that we can quickly remove textures from the cache.
    m_texturePathMap.insert(std::make_pair(filePath, texture));
    m_pathIDMap[texture.id] = filePath;
}

void vg::TextureCache::resolvePath(const vio::Path& path, OUT vio::Path& fullPath) {
    if (m_ioManager) {
        m_ioManager->resolvePath(path, fullPath);
    } else {
        fullPath = path;
    }
}
