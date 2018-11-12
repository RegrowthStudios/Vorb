//
// TextureCache.h
// Vorb Engine
//
// Created by Ben Arnold on 20 Oct 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file TextureCache.h
 * @brief Provides an implementation of a class which handles loading and caching of textures.
 */

#pragma once

#ifndef Vorb_TextureCache_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TextureCache_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>
#include <unordered_map>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "ImageIO.h"
#include "SamplerState.h"
#include "Texture.h"
#include "gtypes.h"
#include "GLEnums.h"
#include "../io/Path.h"
#include "../VorbPreDecl.inl"

DECL_VIO(class IOManager)

namespace vorb {
    namespace graphics {
        class TextureCache {
            using TexturePathMap = std::unordered_map<vio::Path, Texture>;
            using PathIDMap      = std::map<ui32, vio::Path>;
        public:
            TextureCache();
            ~TextureCache();

            /*!
             * \brief Initialises the texture cache with the provided IO manager.
             *
             * \param ioManager The IO manager for loading textures from disk.
             * \param ownsManager If true, this texture cache will manage the memory of the IO manager.
             */
            void init(vio::IOManager* ioManager, bool ownsManager = false);

            /*!
             * \brief Frees all cached textures and any management resources this instance owns.
             */
            void dispose();

            /*!
             * \brief Finds a texture if it exists in the cache.
             *
             * \param filePath The path of the texture to look for.
             *
             * \return The ID of the texture if found, 0 otherwise.
             */
            Texture findTexture(const vio::Path& filePath);

            /*!
             * \brief Returns the filepath associated with the texture.
             *
             * \param textureID the ID of the texture to get the path of.
             *
             * \return The filepath of the texture if it exists, an empty string otherwise.
             */
            vio::Path getTexturePath(ui32 textureID);

            /*!
             * \brief Loads from disk, and uploads to GPU a PNG texture and adds it to the cache if it is not already present.
             *
             * \param filePath The filepath of the texture.
             * \param textureTarget The texture target.
             * \param samplingParameters The texture sampler parameters.
             * \param internalFormat The internal format of the pixel data.
             * \param textureFormat The format of the uploaded pixels.
             * \param mipmapLevels The max number of mipmap levels.
             * \param flipV: When true, texture will flip across horizontal.
             *
             * \return The stored texture.
             */
            Texture addTexture(         const vio::Path& filePath,
                                       vg::TextureTarget textureTarget      = vg::TextureTarget::TEXTURE_2D,
                                           SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                               vg::TextureInternalFormat internalFormat     = vg::TextureInternalFormat::RGBA,
                                       vg::TextureFormat textureFormat      = vg::TextureFormat::RGBA,
                                                     i32 mipmapLevels       = INT_MAX,
                                                    bool flipV              = false);
            /*!
             * \brief Loads from disk, and uploads to GPU a PNG texture and adds it to the cache if it is not already present.
             * Returns a populated bitmap resource of the texture.
             *
             * \param filePath The filepath of the texture.
             * \param rvBitmap The bitmap resource to be filled by the texture.
             * \param rvFormat The format of returned pixels.
             * \param textureTarget The texture target.
             * \param samplingParameters The texture sampler parameters.
             * \param internalFormat The internal format of the pixel data.
             * \param textureFormat The format of the uploaded pixels.
             * \param mipmapLevels The max number of mipmap levels.
             * \param flipV: When true, texture will flip across horizontal.
             *
             * \return The stored texture.
             */
            Texture addTexture(         const vio::Path& filePath,
                                 OUT vg::BitmapResource& rvBitmap,
                                       vg::ImageIOFormat rvFormat,
                                       vg::TextureTarget textureTarget      = vg::TextureTarget::TEXTURE_2D,
                                           SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                               vg::TextureInternalFormat internalFormat     = vg::TextureInternalFormat::RGBA,
                                       vg::TextureFormat textureFormat      = vg::TextureFormat::RGBA,
                                                     i32 mipmapLevels       = INT_MAX,
                                                    bool flipV              = false);
            /*!
             * \brief Uploads a PNG texture and adds it to the cache if it is not already present.
             *
             * \param filePath The filepath of the texture.
             * \param rs The bitmap resource of the texture.
             * \param texturePixelType The format of the resource's pixels.
             * \param textureTarget The texture target.
             * \param samplingParameters The texture sampler parameters.
             * \param internalFormat The internal format of the pixel data.
             * \param textureFormat The format of the uploaded pixels.
             * \param mipmapLevels The max number of mipmap levels.
             *
             * \return The stored texture.
             */
            Texture addTexture(         const vio::Path& filePath,
                               const vg::BitmapResource* rs,
                                        TexturePixelType texturePixelType   = TexturePixelType::UNSIGNED_BYTE,
                                       vg::TextureTarget textureTarget      = vg::TextureTarget::TEXTURE_2D,
                                           SamplerState* samplingParameters = &SamplerState::LINEAR_CLAMP_MIPMAP,
                               vg::TextureInternalFormat internalFormat     = vg::TextureInternalFormat::RGBA,
                                       vg::TextureFormat textureFormat      = vg::TextureFormat::RGBA,
                                                     i32 mipmapLevels       = INT_MAX);
            /*!
             * \brief Adds existing texture with filepath specified to the cache.
             *
             * \param filePath The filepath of the texture.
             * \param texture The texture to be added to the cache.
             */
            void addTexture(const vio::Path& filePath, const Texture& texture);

            /*!
             * \brief Frees a texture from the cache.
             *
             * \param filePath The path of the texture to free.
             */
            bool freeTexture(const vio::Path& filePath);
            /*!
             * \brief Frees a texture from the cache.
             *
             * \param texture The ID of the texture to free. It will be set to 0.
             */
            bool freeTexture(VGTexture& texture);
            /*!
             * \brief Frees a texture from the cache.
             *
             * \param texture The texture to free, its ID will be set to 0.
             */
            bool freeTexture(Texture& texture);
        private:
            VORB_NON_COPYABLE(TextureCache);

            /*!
             * \brief Inserts a texture into the cache.
             *
             * \param filePath The path of the texture to insert.
             * \param texture The texture to insert.
             */
            void insertTexture(const vio::Path& filePath, const Texture& texture);

            /*!
             * \brief Resolves the full path for a texture if m_ioManager exists.
             *
             * \param path The path to resolve.
             * \param fullPath The resolved full path. Guaranteed to be equal to path if m_ioManager == nullptr.
             */
            void resolvePath(const vio::Path& path, OUT vio::Path& fullPath);

            bool            m_ownsIOManager; ///< True when the cache should deallocate the IO manager.
            vio::IOManager* m_ioManager;     ///< Handles the IO of textures.

            // We store two maps here so that users can free textures using either the ID or filePath
            TexturePathMap m_texturePathMap; ///< Textures store here keyed on filename
            PathIDMap      m_pathIDMap;     ///< Textures are stored here keyed on ID
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_TextureCache_h__
