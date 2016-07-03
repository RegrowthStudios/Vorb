//
// SpriteFont.h
// Vorb Engine
//
// Created by Cristian Zaloj on 15 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file SpriteFont.h
 * 
 * \brief Bitmap glyph font container.
 */

#pragma once

#ifndef Vorb_SpriteFont_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SpriteFont_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>
#include <vector>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "gtypes.h"

namespace vorb {
    namespace graphics {
        class SpriteBatch;

        struct CharGlyph {
        public:
            char character;
            f32v4 uvRect;
            f32v2 size;
        };

        enum class TextAlign {
            LEFT,
            TOP_LEFT,
            TOP,
            TOP_RIGHT,
            RIGHT,
            BOTTOM_RIGHT,
            BOTTOM,
            BOTTOM_LEFT,
            CENTER,
        };

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

        class SpriteFont {
            friend class SpriteBatch;
        public:
            void init(const cString font, ui32 size, char cs, char ce);
            void init(const cString font, ui32 size) {
                init(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
            }
            void dispose();

            const ui32& getFontHeight() const {
                return m_fontHeight;
            }

            static void getInstalledFonts(std::map<nString, nString>& fontFileDictionary);

            f32v2 measure(const cString s) const;

        private:
            void draw(SpriteBatch* batch, const cString s, const f32v2& position, const f32v2& scaling, const color4& tint, TextAlign align, f32 depth, const f32v4& clipRect, bool shouldWrap) const;
            f32 getInitialYOffset(TextAlign textAlign) const;
            f32 getYOffset(size_t numRows, TextAlign align) const;

            static std::vector<ui32>* createRows(i32v4* rects, ui32 rectsLength, ui32 r, ui32 padding, ui32& w);

            ui32 m_regStart = FIRST_PRINTABLE_CHAR;
            ui32 m_regLength = (LAST_PRINTABLE_CHAR - FIRST_PRINTABLE_CHAR + 1);
            CharGlyph* m_glyphs = nullptr;
            ui32 m_fontHeight = 0;

            VGTexture m_texID = 0;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_SpriteFont_h__
