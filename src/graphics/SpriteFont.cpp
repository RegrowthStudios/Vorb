#include "Vorb/stdafx.h"
#include "Vorb/graphics/SpriteFont.h"

#include <boost/filesystem.hpp>

#if defined(VORB_IMPL_FONT_SDL)
//#if defined(VORB_OS_WINDOWS)
//#include <TTF/SDL_ttf.h>
//#else
#include <SDL_ttf/SDL_ttf.h>
//#endif
#endif

#include "Vorb/graphics/GraphicsDevice.h"
#include "Vorb/graphics/ImageIO.h"
#include "Vorb/graphics/SpriteBatch.h"
#include "Vorb/utils.h"
#include <iostream>

// X Offset multipliers for vg::TextAlign
const f32 X_OFF_MULTS[9] = {
    0.0f, // LEFT
    0.0f, // TOP_LEFT
    -0.5f, // TOP
    -1.0f, // TOP_RIGHT
    -1.0f, // RIGHT
    -1.0f, // BOTTOM_RIGHT
    -0.5f, // BOTTOM
    0.0f, // BOTTOM_LEFT
    -0.5f // CENTER
};

// Used for alignment
struct GlyphToRender {
    GlyphToRender(size_t gi, f32 x) : gi(gi), x(x) {}
    size_t gi;
    f32 x;
};

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
    m_fontHeight = TTF_FontHeight(f);
    SDL_Color fg = { 255, 255, 255, 255 };
#endif
    m_regStart = cs;
    m_regLength = ce - cs + 1;
    ui32 padding = size / 8;

    // First Measure All The Regions
    i32v4* glyphRects = new i32v4[m_regLength];
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
    while (rows <= m_regLength) {
        h = rows * (padding + m_fontHeight) + padding;
        auto gr = createRows(glyphRects, m_regLength, rows, padding, w);

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
    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
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
        ly += m_fontHeight + padding;
    }

    // Draw The Unsupported Glyph
    ui32 rs = padding - 1;
    ui32* pureWhiteSquare = new ui32[rs * rs];
    memset(pureWhiteSquare, 0xffffffffu, rs * rs * sizeof(ui32));
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
    delete[] pureWhiteSquare;
    pureWhiteSquare = nullptr;

    // Create SpriteBatch Glyphs
    m_glyphs = new CharGlyph[m_regLength + 1];
    for (i = 0; i < m_regLength; i++) {
        m_glyphs[i].character = (char)(cs + i);
        m_glyphs[i].size = f32v2((f32)glyphRects[i].z, (f32)glyphRects[i].w);
        m_glyphs[i].uvRect = f32v4(
            (f32)glyphRects[i].x / (f32)bestWidth,
            (f32)glyphRects[i].y / (f32)bestHeight,
            (f32)glyphRects[i].z / (f32)bestWidth,
            (f32)glyphRects[i].w / (f32)bestHeight
            );
    }
    m_glyphs[m_regLength].character = ' ';
    m_glyphs[m_regLength].size = m_glyphs[0].size;
    m_glyphs[m_regLength].uvRect = f32v4(0.0f, 0.0f, (f32)rs / (f32)bestWidth, (f32)rs / (f32)bestHeight);

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
    if (m_texID != 0) {
        glDeleteTextures(1, &m_texID);
        m_texID = 0;
    }
    if (m_glyphs) {
        m_glyphs = nullptr;
        delete[] m_glyphs;
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

f32v2 vg::SpriteFont::measure(const cString s) const {
    f32v2 size((f32)0, (f32)m_fontHeight);
    float cw = 0;
    for (int si = 0; s[si] != 0; si++) {
        char c = s[si];
        if (s[si] == '\n') {
            size.y += m_fontHeight;
            if (size.x < cw)
                size.x = cw;
            cw = 0;
        } else {
            // Check For Correct Glyph
            size_t gi = c - m_regStart;
            if (gi >= m_regLength)
                gi = m_regLength;
            cw += m_glyphs[gi].size.x;
        }
    }
    if (size.x < cw)
        size.x = cw;
    return size;
}

void vg::SpriteFont::draw(SpriteBatch* batch, const cString s, const f32v2& position, const f32v2& scaling, const color4& tint, TextAlign align, f32 depth, const f32v4& clipRect, bool shouldWrap) const {
    f32v2 pos = position;
    pos.y += getInitialYOffset(align) * scaling.y;
    if (pos.x < clipRect.x) pos.x = clipRect.x;
    f32 gx = 0.0f;
    std::vector <std::vector<GlyphToRender> > rows(1); // Rows of glyphs
    std::vector <f32> rightEdges(1, 0.0f); // Right edge positions for rows
    for (int si = 0; s[si] != 0; si++) {
        char c = s[si];
        if (s[si] == '\n') {
            // Go to new row on newlines
            rightEdges.back() = gx;
            rightEdges.push_back(0.0f);
            rows.emplace_back();
            gx = 0.0f;
        } else {
            bool useGlyph = true;
            // Check For Correct Glyph
            size_t gi = c - m_regStart;
            if (gi >= m_regLength) gi = m_regLength;

            // Get glyph width
            f32 gWidth = m_glyphs[gi].size.x * scaling.x;

            // Check for wrapping
            if (shouldWrap) { 
                bool isOut;
                switch (align) {
                    case vg::TextAlign::TOP:
                    case vg::TextAlign::CENTER:
                    case vg::TextAlign::BOTTOM:
                        isOut = ((pos.x + (gx + gWidth) / 2.0f > clipRect.x + clipRect.z)); break;
                    case vg::TextAlign::TOP_RIGHT:
                    case vg::TextAlign::RIGHT:
                    case vg::TextAlign::BOTTOM_RIGHT:
                        isOut = ((pos.x - gx - gWidth < clipRect.x)); break;
                    default:
                        isOut = ((pos.x + gx + gWidth > clipRect.x + clipRect.z)); break;
                }
         
                // If the glyph is out of the clip rect, may need to go to new row
                if (isOut) {
                    // TODO(Ben): Check input clipping characters
                    if (c == ' ') {
                        useGlyph = false;
                    } else {
                        // Count the word size
                        int numChars = 0;
                        while (s[si - numChars] != ' ' && si - numChars != 0) numChars++;

                        if (si - numChars > 0) {
                            for (int i = 0; i < numChars; i++) {
                                gx -= m_glyphs[si - i].size.x * scaling.x;
                            }
                            si -= (numChars + 1); // -1 to counter ++ later.
                            rightEdges.back() = gx;
                            gx = 0.0f;
                            useGlyph = false; // TODO(Ben): Is this right?
                        } else {
                            rightEdges.back() = gx;
                        }
                    }
                    // Go to new row
                    rightEdges.push_back(0.0f);
                    rows.emplace_back();
                    gx = 0.0f;
                }
            }
            // Add glyph to the row
            if (useGlyph) {
                rows.back().emplace_back(gi, gx);
                gx += gWidth;
            }
        }
    }
    rightEdges.back() = gx;
    // Get y offset
    f32 yOff = getYOffset(rows.size(), align) * scaling.y;
    // Render each row
    for (size_t y = 0; y < rows.size(); y++) {
        f32 rightEdge = rightEdges[y];
        for (auto& g : rows[y]) {   
            f32v2 position = pos + f32v2(g.x + rightEdges[y] * X_OFF_MULTS[(int)align], yOff + y * m_fontHeight * scaling.y);
            f32v2 size = m_glyphs[g.gi].size * scaling;
            f32v4 uvRect = m_glyphs[g.gi].uvRect;
            // Clip the glyphs with clipRect
            computeClipping(clipRect, position, size, uvRect);
            // Don't draw the glyph if its too small after clipping
            if (size.x > 0.0f && size.y > 0.0f) {
                batch->draw(m_texID, &uvRect, position, size, tint, depth);
            }
        }
    }
}

f32 vg::SpriteFont::getInitialYOffset(TextAlign textAlign) const {
    // No need to measure top left
    if (textAlign == vg::TextAlign::TOP_LEFT) return 0.0f;
    switch (textAlign) {
        case vg::TextAlign::LEFT:
            return -(f32)m_fontHeight / 2.0f;
        case vg::TextAlign::TOP_LEFT:
            return 0.0f;
        case vg::TextAlign::TOP:
            return 0.0f;
        case vg::TextAlign::TOP_RIGHT:
            return 0.0f;
        case vg::TextAlign::RIGHT:
            return -(f32)m_fontHeight / 2.0f;
        case vg::TextAlign::BOTTOM_RIGHT:
            return -(f32)m_fontHeight;
        case vg::TextAlign::BOTTOM:
            return -(f32)m_fontHeight;
        case vg::TextAlign::BOTTOM_LEFT:
            return -(f32)m_fontHeight;
        case vg::TextAlign::CENTER:
            return -(f32)m_fontHeight / 2.0f;
        default:
            return 0.0f; // Should never happen
    }
    return 0.0f;
}

f32 vg::SpriteFont::getYOffset(size_t numRows, vg::TextAlign align) const {
    switch (align) {
        case vg::TextAlign::TOP_LEFT:
        case vg::TextAlign::TOP:
        case vg::TextAlign::TOP_RIGHT:
            return 0.0f;
        case vg::TextAlign::LEFT:
        case vg::TextAlign::CENTER:
        case vg::TextAlign::RIGHT:
            return -((f32)(numRows - 1) * m_fontHeight / 2.0f);
        default:
            return -((f32)(numRows - 1) * m_fontHeight);
    }
    return 0.0f; // Should never happen
}
