//
// SpriteBatch.h
// Vorb Engine
//
// Created by Cristian Zaloj on 17 Jan 2015
// Refactored by Benjamin Arnold on 3 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file SpriteBatch.h
* @brief 
* Defines the SpriteBatch for easy and efficient
* 2D rendering of Sprites.
*
*/

#pragma once

#ifndef Vorb_SpriteBatch_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SpriteBatch_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "../PtrRecycler.hpp"
#include "../VorbPreDecl.inl"
#include "SpriteFont.h"
#include "gtypes.h"

#include <cfloat> // For FTL_MAX

#define CLIP_RECT_DEFAULT f32v4(-FLT_MIN / 2.0f, -FLT_MIN / 2.0f, FLT_MAX, FLT_MAX)

namespace vorb {
    namespace graphics {
        class DepthState;
        class GLProgram;
        class RasterizerState;
        class SamplerState;

        /// Sorting mode for SpriteBatch sprites
        enum class SpriteSortMode {
            NONE,
            FRONT_TO_BACK,
            BACK_TO_FRONT,
            TEXTURE
        };

        enum class GradientType {
            NONE,
            HORIZONTAL,
            VERTICAL,
            LEFT_DIAGONAL,
            RIGHT_DIAGONAL
        };

        class SpriteBatch {
        public:
            SpriteBatch(bool isDynamic = true, bool init = false);
            ~SpriteBatch();

            void init();
            void dispose();

            void begin();

            void draw(VGTexture t, f32v4* uvRect, f32v2* uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, f32 rotation, const color4& tint1, const color4& tint2, GradientType grad, f32 depth = 0.0f);
            void draw(VGTexture t, f32v4* uvRect, f32v2* uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, f32 rotation, const color4& tint, f32 depth = 0.0f);
            void draw(VGTexture t, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 offset, f32v2 size, const color4& tint, f32 depth = 0.0f);
            void draw(VGTexture t, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);
            void draw(VGTexture t, f32v4* uvRect, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);
            void draw(VGTexture t, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);

            void drawString(const SpriteFont* font, const cString s, f32v2 position, f32v2 scaling, const color4& tint, TextAlign textAlign = TextAlign::TOP_LEFT, f32 depth = 0.0f, const f32v4& clipRect = CLIP_RECT_DEFAULT, bool shouldWrap = true);
            void drawString(const SpriteFont* font, const cString s, f32v2 position, f32 desiredHeight, f32 scaleX, const color4& tint, TextAlign textAlign = TextAlign::TOP_LEFT, f32 depth = 0.0f, const f32v4& clipRect = CLIP_RECT_DEFAULT, bool shouldWrap = true);

            void end(SpriteSortMode ssm = SpriteSortMode::TEXTURE);

            void render(f32m4 mWorld, f32m4 mCamera, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
            void render(f32m4 mWorld, const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
            void render(const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);

            void sortGlyphs(SpriteSortMode ssm);
            void generateBatches();

            static void disposeProgram();
        private:
            struct Glyph; struct Vertex;
            typedef void(SpriteBatch::*QuadBuildFunc)(const Glyph*, Vertex*);

            struct Glyph {
                Glyph(QuadBuildFunc f, VGTexture tex, const f32v4& uvRect, const f32v2& uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, f32 rotation, const color4& tint, f32 depth);
                Glyph(QuadBuildFunc f, VGTexture tex, const f32v4& uvRect, const f32v2& uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, f32 rotation, const color4& tint1, const color4& tint2, GradientType grad, f32 depth);
                VGTexture tex;
                f32v4 uvRect;
                f32v2 uvTiling;
                f32v2 position;
                f32v2 offset;
                f32v2 size;
                f32 rotation;
                color4 tint1;
                color4 tint2;
                GradientType grad;
                f32 depth;
                QuadBuildFunc func;
            };
            struct Vertex {
            public:
                Vertex() {};
                Vertex(const f32v3& pos, const f32v2& uv, const f32v4& uvr, const color4& color);

                f32v3 position;
                f32v2 uv;
                f32v4 uvRect;
                color4 color;
            };
            class Batch {
            public:
                void set(ui32 iOff, ui32 texID);
                ui32 textureID;
                ui32 indices;
                ui32 indexOffset;
            };

            /// Sorting functions
            static bool SSMTexture(Glyph* g1, Glyph* g2) { return g1->tex < g2->tex; }
            static bool SSMFrontToBack(Glyph* g1, Glyph* g2) { return g1->depth < g2->depth; }
            static bool SSMBackToFront(Glyph* g1, Glyph* g2) { return g1->depth > g2->depth; }

            /// Quad builders
            void buildQuad(const Glyph* g, Vertex* verts);
            void buildQuadOffset(const Glyph* g, Vertex* verts);
            void buildQuadRotated(const Glyph* g, Vertex* verts);    

            /// For color gradients
            void calcColor(Vertex& vtl, Vertex& vtr, Vertex& vbl, Vertex& vbr, const Glyph* g);

            std::vector<Glyph> m_glyphs; ///< Glyph data
            std::vector<Glyph*> m_glyphPtrs; ///< Pointers to glyphs for fast sorting

            ui32 m_bufUsage; ///< Buffer usage hint
            VGVertexArray m_vao = 0; ///< Vertex Array Object
            VGBuffer m_vbo = 0; ///< Vertex Buffer Object
            VGBuffer m_ibo = 0; ///< Index Buffer Object
            ui32 m_indexCapacity = 0; ///< Current capacity of the m_ibo
            std::vector<Batch> m_batches; ///< Vector of batches for rendering

            static vg::GLProgram* m_program; ///< Shader handle

            ui32 m_texPixel; ///< Default White Pixel Texture
        };
    }
}
namespace vg = vorb::graphics;

#undef CLIP_RECT_DEFAULT

#endif // !Vorb_SpriteBatch_h__
