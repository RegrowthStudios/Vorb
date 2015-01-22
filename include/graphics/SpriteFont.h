///
/// SpriteFont.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 15 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Bitmap glyph font container
///

#pragma once

#ifndef SpriteFont_h__
#define SpriteFont_h__

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

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

        class SpriteFont {
            friend class SpriteBatch;
        public:
            SpriteFont(const cString font, ui32 size, char cs, char ce);
            SpriteFont(const cString font, ui32 size) : SpriteFont(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR) {
                // Empty
            }
            void dispose();

            const ui32& getFontHeight() const {
                return _fontHeight;
            }

            static void getInstalledFonts(std::map<nString, nString>& fontFileDictionary);

            f32v2 measure(const cString s);

        private:
            void draw(SpriteBatch* batch, const cString s, f32v2 position, f32v2 scaling, color4 tint, f32 depth);

            static std::vector<ui32>* createRows(i32v4* rects, ui32 rectsLength, ui32 r, ui32 padding, ui32& w);

            ui32 _regStart, _regLength;
            CharGlyph* _glyphs;
            ui32 _fontHeight;

            VGTexture _texID;
        };
    }
}
namespace vg = vorb::graphics;

#endif // SpriteFont_h__