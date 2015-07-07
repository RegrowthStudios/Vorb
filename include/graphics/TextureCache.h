// 
//  TextureCache.h
//  Vorb Engine
//
//  Created by Ben Arnold on 20 Oct 2014
//  Copyright 2014 Regrowth Studios
//  All Rights Reserved
//  
//  Summary:
//  This file provides an implementation of a TextureCache
//  which handles loading and cacheing of textures.
//

#pragma once

#ifndef TEXTURECACHE_H_
#define TEXTURECACHE_H_

#include "ImageIO.h"
#include "SamplerState.h"
#include "Texture.h"
#include "gtypes.h"
#include "GLEnums.h"
#include "../io/Path.h"
#include "../VorbPreDecl.inl"

DECL_VIO(class IOManager)
#ifdef VORB_USING_SCRIPT
DECL_VSCRIPT(class Environment)
#endif

namespace vorb {
    namespace graphics {
        class TextureCache {
        public:
            TextureCache();
            TextureCache(vio::IOManager* ioManager);
            ~TextureCache();

            /// Finds a texture if it exists in the cache
            /// @param filePath: The path of the texture
            /// @return the texture ID or 0 if it doesn't exist
            Texture findTexture(const vio::Path& filePath);

            /// Returns the file path associated with the texture
            /// @param textureID: The ID of the texture
            /// @return The filepath or empty string if it doesn't exist
            vio::Path getTexturePath(ui32 textureID);

            /// Loads and uploads a png texture and adds it to the cache or returns
            /// an existing texture ID if it already exists in the cache
            /// @param filePath: The file path of the texture
            /// @param TextureTarget: The texture target
            /// @param samplingParameters: The texture sampler parameters
            /// @param internalFormat: Internal format of the pixel data
            /// @param textureFormat: Format of uploaded pixels
            /// @param mipmapLevels: The max number of mipmap levels
            /// @param flipV: When true, texture will flip across horizontal.
            /// @return The texture.
            Texture addTexture(const vio::Path& filePath,
                               vg::TextureTarget textureTarget = vg::TextureTarget::TEXTURE_2D,
                               SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                               vg::TextureInternalFormat internalFormat = vg::TextureInternalFormat::RGBA,
                               vg::TextureFormat textureFormat = vg::TextureFormat::RGBA,
                               i32 mipmapLevels = INT_MAX,
                               bool flipV = false);

            /// Loads and uploads a png texture and adds it to the cache or returns
            /// an existing texture ID if it already exists in the cache. Also
            /// returns the BitmapResource in the rvBitmap parameter
            /// @param filePath: The file path of the texture
            /// @param rvBitmap: Bitmap data to be filled
            /// @param rvFormat: Format of the returned data
            /// @param TextureTarget: The texture target
            /// @param samplingParameters: The texture sampler parameters
            /// @param internalFormat: Internal format of the pixel data
            /// @param textureFormat: Format of uploaded pixels
            /// @param mipmapLevels: The max number of mipmap levels
            /// @param flipV: When true, texture will flip across horizontal.
            /// @return The texture.
            Texture addTexture(const vio::Path& filePath,
                               OUT vg::BitmapResource& rvBitmap,
                               vg::ImageIOFormat rvFormat,
                               vg::TextureTarget textureTarget = vg::TextureTarget::TEXTURE_2D,
                               SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                               vg::TextureInternalFormat internalFormat = vg::TextureInternalFormat::RGBA,
                               vg::TextureFormat textureFormat = vg::TextureFormat::RGBA,
                               i32 mipmapLevels = INT_MAX,
                               bool flipV = false);

            /// Uploads a png texture and adds it to the cache
            /// an existing texture ID if it already exists in the cache
            /// @param filePath: The path of the texture
            /// @param rs: The bitmap resource
            /// @param texturePixelType: Format of the resource pixels.
            /// @param TextureTarget: The texture target
            /// @param samplingParameters: The texture sampler parameters
            /// @param internalFormat : Internal format of the pixel data
            /// @param textureFormat: Format of uploaded pixels
            /// @return The texture.
            Texture addTexture(const vio::Path& filePath,
                               const vg::BitmapResource* rs,
                               TexturePixelType texturePixelType = TexturePixelType::UNSIGNED_BYTE,
                               vg::TextureTarget textureTarget = vg::TextureTarget::TEXTURE_2D,
                               SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                               vg::TextureInternalFormat internalFormat = vg::TextureInternalFormat::RGBA,
                               vg::TextureFormat textureFormat = vg::TextureFormat::RGBA,
                               i32 mipmapLevels = INT_MAX);

            /// Adds a texture to the cache
            /// @param filePath: The path of the texture
            /// @param textureID: The opengGL texture ID
            void addTexture(const vio::Path& filePath, const Texture& texture);

            /// Frees a texture from the cache
            /// @param filePath: The path of the texture to free
            void freeTexture(const vio::Path& filePath);

            /// Frees a texture from the cache
            /// @param textureID: The ID of the texture to free. It will be set to 0
            void freeTexture(VGTexture& texture);

            /// Frees a texture from the cache
            /// @param textureID: The texture to free. texture.ID will be set to 0
            void freeTexture(Texture& texture);

            /// Frees all textures
            void dispose();

#ifdef VORB_USING_SCRIPT
            /*! @brief Registers the texture cache functions with a script environment
             * Does not set any namespaces.
             * @param env: The scripting environment. 
             */
            void registerTextureCache(vscript::Environment& env);
        private:
            /// Texture loading function used by scripts
            VGTexture scriptLoadTexture(nString filePath,
                             vg::TextureTarget textureTarget,
                             SamplerState* samplingParameters,
                             vg::TextureInternalFormat internalFormat,
                             vg::TextureFormat textureFormat,
                             i32 mipmapLevels);
            /// Easy texture loading function
            VGTexture scriptLoadTextureDefault(nString filePath);
            /// Texture freeing function used by scripts
            void scriptFreeTexture(VGTexture texture);
#endif
        private:
            /// Inserts a texture into the cache
            /// @param filePath: The path of the texture to insert
            /// #param texture: The texture to insert
            void insertTexture(const vio::Path& filePath, const Texture& texture);

            /// Resolves the full path for a texture if m_ioManager exists
            /// @param path: The path to resolve
            /// @param fullPath: resulting path. Will be equal to path if m_ioManager == nullptr
            void resolvePath(const vio::Path& path, OUT vio::Path& fullPath);

            bool m_ownsIoManager = nullptr; ///< True when the cache should deallocate the iomanager
            vio::IOManager* m_ioManager = nullptr; ///< Handles the IO

            /// We store two maps here so that users can free textures using either the ID or filePath
            std::unordered_map <vio::Path, Texture> _textureStringMap; ///< Textures store here keyed on filename
            std::map <ui32, vio::Path> _textureIdMap; ///< Textures are stored here keyed on ID
        };
    }
}
namespace vg = vorb::graphics;

#endif // TEXTURECACHE_H_

