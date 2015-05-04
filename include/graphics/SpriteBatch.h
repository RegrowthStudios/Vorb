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
#include "gtypes.h"

namespace vorb {
    namespace graphics {
        class DepthState;
        class GLProgram;
        class RasterizerState;
        class SamplerState;
        class SpriteFont;

        /// Sorting mode for SpriteBatch sprites
        enum class SpriteSortMode {
            NONE,
            FRONT_TO_BACK,
            BACK_TO_FRONT,
            TEXTURE
        };

        class SpriteBatch {
        public:
            SpriteBatch(bool isDynamic = true, bool init = false);
            ~SpriteBatch();

            void init();
            void dispose();

            void begin();

            void draw(VGTexture tex, f32v4* uvRect, f32v2* uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, f32 rotation, const color4& tint, f32 depth = 0.0f);
            void draw(VGTexture tex, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 offset, f32v2 size, const color4& tint, f32 depth = 0.0f);
            void draw(VGTexture tex, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);
            void draw(VGTexture tex, f32v4* uvRect, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);
            void draw(VGTexture tex, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);

            void drawString(const SpriteFont* font, const cString s, f32v2 position, f32v2 scaling, const color4& tint, f32 depth = 0.0f);
            void drawString(const SpriteFont* font, const cString s, f32v2 position, f32 desiredHeight, f32 scaleX, const color4& tint, f32 depth = 0.0f);

            void end(SpriteSortMode ssm = SpriteSortMode::TEXTURE);

            void renderBatch(f32m4 mWorld, f32m4 mCamera, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
            void renderBatch(f32m4 mWorld, const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
            void renderBatch(const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);

            void sortGlyphs(SpriteSortMode ssm);
            void generateBatches();

            static void disposeProgram();
        private:
            struct Glyph {
                Glyph(VGTexture tex, const f32v4& uvRect, const f32v2& uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, f32 rotation, const color4& tint, f32 depth);
                VGTexture tex;
                f32v4 uvRect;
                f32v2 uvTiling;
                f32v2 position;
                f32v2 offset;
                f32v2 size;
                f32 rotation;
                color4 tint;
                f32 depth;
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

            static bool SSMTexture(Glyph* g1, Glyph* g2) {
                return g1->tex < g2->tex;
            }
            static bool SSMFrontToBack(Glyph* g1, Glyph* g2) {
                return g1->depth < g2->depth;
            }
            static bool SSMBackToFront(Glyph* g1, Glyph* g2) {
                return g1->depth > g2->depth;
            }

            void createVertexArray();
            void createPixelTexture();

            class Batch {
            public:          
                void set(ui32 iOff, ui32 texID);
                ui32 textureID;
                ui32 indices;
                ui32 indexOffset;
            };

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

#endif // !Vorb_SpriteBatch_h__
