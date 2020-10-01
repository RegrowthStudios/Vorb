//
// FontCache.h
// Vorb Engine
//
// Created by Matthew Marshall on 01 Oct 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file FontCache.h
 * @brief Provides an implementation of a class which handles loading and caching of fonts.
 */

#pragma once

#ifndef Vorb_FontCache_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FontCache_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>
#include <unordered_map>

#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "ImageIO.h"
#include "SamplerState.h"
#include "Texture.h"
#include "gtypes.h"
#include "GLEnums.h"
#include "../io/Path.h"
#include "../VorbPreDecl.inl"

DECL_VG(class SpriteFont)
DECL_VIO(class IOManagerBase)

namespace vorb {
    namespace graphics {
        class FontCache {
            using FontMap = std::unordered_map<nString, SpriteFont*>;
        public:
            FontCache();
            ~FontCache();

            /*!
             * \brief Initialises the texture cache with the provided IO manager.
             *
             * \param ioManager The IO manager for loading fonts from disk.
             * \param ownsManager If true, this texture cache will manage the memory of the IO manager.
             */
            void init(vio::IOManagerBase* ioManager, bool ownsManager = false);

            /*! \brief Frees all cached fonts and any management resources this instance owns.
             */
            void dispose();

            /*!
             * \brief Gets a font, loading it if it does not yet exist in cache.
             *
             * \param filepath The path of the font to look for.
             * \param size The size of the font as cached.
             * \param cs The starting character of the font as cached.
             * \param ce The final character of the font as cached.
             *
             * \return Pointer to the font if found, nullptr otherwise.
             */
            CALLEE_DELETE SpriteFont* getFont(const vio::Path& filepath, ui32 size, char cs, char ce);
            /*!
             * \brief Gets a font, loading it if it does not yet exist in cache.
             *
             * \param filepath The path of the font to look for.
             * \param size The size of the font as cached.
             *
             * \return Pointer to the font if found, nullptr otherwise.
             */
            CALLEE_DELETE SpriteFont* getFont(const vio::Path& filepath, ui32 size);

            /*!
             * \brief Gets a font if it exists in the cache.
             *
             * \param filepath The path of the font to look for.
             * \param size The size of the font as cached.
             * \param cs The starting character of the font as cached.
             * \param ce The final character of the font as cached.
             *
             * \return Pointer to the font if found, nullptr otherwise.
             */
            CALLEE_DELETE SpriteFont* tryGetFont(const vio::Path& filepath, ui32 size, char cs, char ce);
            /*!
             * \brief Gets a font if it exists in the cache.
             *
             * \param filepath The path of the font to look for.
             * \param size The size of the font as cached.
             *
             * \return Pointer to the font if found, nullptr otherwise.
             */
            CALLEE_DELETE SpriteFont* tryGetFont(const vio::Path& filepath, ui32 size);

            /*!
             * \brief Frees a font from the cache.
             *
             * \param filepath The path of the font to free.
             * \param size The size of the font as cached.
             * \param cs The starting character of the font as cached.
             * \param ce The final character of the font as cached.
             *
             * \return True if freed, false otherwise.
             */
            bool freeFont(const vio::Path& filepath, ui32 size, char cs, char ce);
            /*!
             * \brief Frees a font from the cache.
             *
             * \param font The ID of the font to free. It will be set to 0.
             * \param size The size of the font as cached.
             *
             * \return True if freed, false otherwise.
             */
            bool freeFont(const vio::Path& filepath, ui32 size);
        private:
            VORB_NON_COPYABLE(FontCache);

            /*!
             * \brief Produce a uniquely identifying string of the combination of values provided.
             *
             * \param filepath The path of a font.
             * \param size The size of a font.
             * \param cs The starting character of a font.
             * \param ce The final character of a font.
             *
             * \return Uniquely identifying string.
             */
            nString makeKey(const vio::Path& filepath, ui32 size, char cs, char ce);

            bool                m_ownsIOManager; ///< True when the cache should deallocate the IO manager.
            vio::IOManagerBase* m_ioManager;     ///< Handles the IO of textures.

            FontMap m_fontMap; ///< Fonts stored here keyed on filename, size, and char range.
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_FontCache_h__
