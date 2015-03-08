#include "stdafx.h"
#include "graphics/SpriteFont.h"

#include <boost/filesystem.hpp>

#if defined(VORB_IMPL_FONT_SDL)
#if defined(OS_WINDOWS)
#include <TTF/SDL_ttf.h>
#else
#include <SDL2_ttf/SDL_ttf.h>
#endif
#endif

#include "graphics/GraphicsDevice.h"
#include "graphics/ImageIO.h"
#include "graphics/SpriteBatch.h"

i32 closestPow2(i32 i) {
    i--;
    i32 pi = 1;
    while (i > 0) {
        i >>= 1;
        pi <<= 1;
    }
    return pi;
}

void vg::SpriteFont::init(const cString font, ui32 size, char cs, char ce) {
#if defined(VORB_IMPL_FONT_SDL)
    TTF_Font* f = TTF_OpenFont(font, size);
    if (!f) {
        std::cerr << "Failed to open font " << font << "\n";
        std::cerr << "Error: " << TTF_GetError() << std::endl;
        return;
    }
    _fontHeight = TTF_FontHeight(f);
    SDL_Color fg = { 255, 255, 255, 255 };
#endif
    _regStart = cs;
    _regLength = ce - cs + 1;
    ui32 padding = size / 8;

    // First Measure All The Regions
    i32v4* glyphRects = new i32v4[_regLength];
    size_t i = 0;
    i32 advance;
    for (char c = cs; c <= ce; c++) {
#if defined(VORB_IMPL_FONT_SDL)
        TTF_GlyphMetrics(f, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
#endif
        glyphRects[i].z -= glyphRects[i].x;
        glyphRects[i].x = 0;
        glyphRects[i].w -= glyphRects[i].y;
        glyphRects[i].y = 0;
        i++;
    }

    // Find Best Partitioning Of Glyphs
    ui32 rows = 1, w, h, bestWidth = 0, bestHeight = 0, area = 4096 * 4096, bestRows = 0;
    std::vector<ui32>* bestPartition = nullptr;
    while (rows <= _regLength) {
        h = rows * (padding + _fontHeight) + padding;
        auto gr = createRows(glyphRects, _regLength, rows, padding, w);

        // Desire A Power Of 2 Texture
        w = closestPow2(w);
        h = closestPow2(h);

        // A Texture Must Be Feasible
        ui32 maxTextureSize = GraphicsDevice::getCurrent()->getProperties().maxTextureSize;
        if (w > maxTextureSize || h > maxTextureSize) {
            rows++;
            delete[] gr;
            continue;
        }

        // Check For Minimal Area
        if (area >= w * h) {
            if (bestPartition) delete[] bestPartition;
            bestPartition = gr;
            bestWidth = w;
            bestHeight = h;
            bestRows = rows;
            area = bestWidth * bestHeight;
            rows++;
        } else {
            delete[] gr;
            break;
        }
    }

    // Can A Bitmap Font Be Made?
    if (!bestPartition) return;

    // Create The Texture
    glGenTextures(1, &_texID);
    glBindTexture(GL_TEXTURE_2D, _texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Now Draw All The Glyphs
    ui32 ly = padding;
    for (size_t ri = 0; ri < bestRows; ri++) {
        ui32 lx = padding;
        for (size_t ci = 0; ci < bestPartition[ri].size(); ci++) {
            ui32 gi = bestPartition[ri][ci];

#if defined(VORB_IMPL_FONT_SDL)
            SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(f, (char)(cs + gi), fg);
            // Pre-multiplication Occurs Here
            ubyte* sp = (ubyte*)glyphSurface->pixels;
            ui32 cp = glyphSurface->w * glyphSurface->h * 4;
            for (size_t i = 0; i < cp; i += 4) {
                f32 a = sp[i + 3] / 255.0f;
                sp[i] = (ubyte)((f32)sp[i] * a);
                sp[i + 1] = sp[i];
                sp[i + 2] = sp[i];
            }

            // Save Glyph Image And Update Coordinates
            glTexSubImage2D(GL_TEXTURE_2D, 0, lx, ly, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
            glyphRects[gi].x = lx;
            glyphRects[gi].y = ly;
            glyphRects[gi].z = glyphSurface->w;
            glyphRects[gi].w = glyphSurface->h;

            SDL_FreeSurface(glyphSurface);
            glyphSurface = nullptr;
#endif

            lx += glyphRects[gi].z + padding;
        }
        ly += _fontHeight + padding;
    }

    // Draw The Unsupported Glyph
    ui32 rs = padding - 1;
    ui32* pureWhiteSquare = new ui32[rs * rs];
    memset(pureWhiteSquare, 0xffffffffu, rs * rs * sizeof(ui32));
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
    delete[] pureWhiteSquare;
    pureWhiteSquare = nullptr;

    // Create SpriteBatch Glyphs
    _glyphs = new CharGlyph[_regLength + 1];
    for (i = 0; i < _regLength; i++) {
        _glyphs[i].character = (char)(cs + i);
        _glyphs[i].size = f32v2(glyphRects[i].z, glyphRects[i].w);
        _glyphs[i].uvRect = f32v4(
            (f32)glyphRects[i].x / (f32)bestWidth,
            (f32)glyphRects[i].y / (f32)bestHeight,
            (f32)glyphRects[i].z / (f32)bestWidth,
            (f32)glyphRects[i].w / (f32)bestHeight
            );
    }
    _glyphs[_regLength].character = ' ';
    _glyphs[_regLength].size = _glyphs[0].size;
    _glyphs[_regLength].uvRect = f32v4(0.0f, 0.0f, (f32)rs / (f32)bestWidth, (f32)rs / (f32)bestHeight);

#ifdef DEBUG
    // Save An Image
    std::vector<ui8> pixels;
    pixels.resize(bestWidth * bestHeight * 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
    char buffer[512];
    sprintf(buffer, "SFont_%s_%s_%d.png", TTF_FontFaceFamilyName(f), TTF_FontFaceStyleName(f), size);
    vg::ImageIO().save(buffer, pixels.data(), bestWidth, bestHeight, vg::ImageIOFormat::RGBA_UI8);
#endif // DEBUG

    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] glyphRects;
    delete[] bestPartition;

#if defined(VORB_IMPL_FONT_SDL)
    TTF_CloseFont(f);
#endif
}

void vg::SpriteFont::dispose() {
    if (_texID != 0) {
        glDeleteTextures(1, &_texID);
        _texID = 0;
    }
    if (_glyphs) {
        _glyphs = nullptr;
        delete[] _glyphs;
    }
}

void vg::SpriteFont::getInstalledFonts(std::map<nString, nString>& fontFileDictionary) {
#ifdef DEBUG
    ui32 startTime = SDL_GetTicks(), searchCount = 0;
#endif // DEBUG

    boost::filesystem::path fontDirectory(getenv("SystemRoot"));
    fontDirectory /= "Fonts";
    boost::filesystem::directory_iterator dirIter(fontDirectory);
    nString fontExtTTF = ".ttf";
    nString fontExtFON = ".fon";
    while (true) {
        i32* m_impCheck = (i32*)&dirIter;
        if (*m_impCheck == 0) break;

        auto dirFile = dirIter->path();
#ifdef DEBUG
        searchCount++;
#endif // DEBUG
        if (!boost::filesystem::is_directory(dirFile)) {
            nString fExt = dirFile.extension().string();
            if (fExt == fontExtTTF || fExt == fontExtFON) {
                nString fontFile = dirFile.string();
                TTF_Font* font = TTF_OpenFont(fontFile.c_str(), 12);
                if (font) {
                    nString fontName = TTF_FontFaceFamilyName(font);
                    fontName += " - " + nString(TTF_FontFaceStyleName(font));
                    fontFileDictionary[fontName] = fontFile;

                    TTF_CloseFont(font);
                    font = nullptr;
                }
            }
        }
        dirIter++;
    }
#ifdef DEBUG
    printf("Found %d System Fonts Out Of %d Files\nTime Spent: %d ms\n", fontFileDictionary.size(), searchCount, SDL_GetTicks() - startTime);
#endif // DEBUG
    return;
}

std::vector<ui32>* vg::SpriteFont::createRows(i32v4* rects, ui32 rectsLength, ui32 r, ui32 padding, ui32& w) {
    // Blank Initialize
    std::vector<ui32>* l = new std::vector<ui32>[r]();
    ui32* cw = new ui32[r]();
    for (size_t i = 0; i < r; i++) {
        cw[i] = padding;
    }

    // Loop Through All Glyphs
    for (ui32 i = 0; i < rectsLength; i++) {
        // Find Row For Placement
        size_t ri = 0;
        for (size_t rii = 1; rii < r; rii++)
        if (cw[rii] < cw[ri]) ri = rii;

        // Add Width To That Row
        cw[ri] += rects[i].z + padding;

        // Add Glyph To The Row List
        l[ri].push_back(i);
    }

    // Find The Max Width
    w = 0;
    for (size_t i = 0; i < r; i++) {
        if (cw[i] > w) w = cw[i];
    }

    return l;
}

f32v2 vg::SpriteFont::measure(const cString s) {
    f32v2 size(0, _fontHeight);
    float cw = 0;
    for (int si = 0; s[si] != 0; si++) {
        char c = s[si];
        if (s[si] == '\n') {
            size.y += _fontHeight;
            if (size.x < cw)
                size.x = cw;
            cw = 0;
        } else {
            // Check For Correct Glyph
            size_t gi = c - _regStart;
            if (gi >= _regLength)
                gi = _regLength;
            cw += _glyphs[gi].size.x;
        }
    }
    if (size.x < cw)
        size.x = cw;
    return size;
}

void vg::SpriteFont::draw(SpriteBatch* batch, const cString s, f32v2 position, f32v2 scaling, color4 tint, f32 depth) {
    f32v2 tp = position;
    for (int si = 0; s[si] != 0; si++) {
        char c = s[si];
        if (s[si] == '\n') {
            tp.y += _fontHeight * scaling.y;
            tp.x = position.x;
        } else {
            // Check For Correct Glyph
            size_t gi = c - _regStart;
            if (gi >= _regLength)
                gi = _regLength;
            batch->draw(_texID, &_glyphs[gi].uvRect, tp, _glyphs[gi].size * scaling, tint, depth);
            tp.x += _glyphs[gi].size.x * scaling.x;
        }
    }
}
