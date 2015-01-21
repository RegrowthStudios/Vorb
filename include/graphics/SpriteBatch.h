///
/// SpriteBatch.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 17 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Batches together 2D shapes
///

#pragma once

#ifndef SpriteBatch_h__
#define SpriteBatch_h__

#include "gtypes.h"
#include "GLEnums.h"
#include "PtrRecycler.hpp"
#include "VorbPreDecl.inl"

class SamplerState;
class RasterizerState;
class DepthState;
DECL_VG(class, GLProgram)

namespace vorb {
    namespace core {
        namespace graphics {
            class SpriteBatch;

            struct VertexSpriteBatch {
            public:
                VertexSpriteBatch();
                VertexSpriteBatch(const f32v3& pos, const f32v2& uv, const f32v4& uvr, const color4& color);

                f32v3 position { 0, 0, 0 };
                f32v2 uv { 0, 0 };
                f32v4 uvRect { 0, 0, 1, 1 };
                color4 color { 0, 0, 0, 255 };
            };

            enum class SpriteSortMode {
                NONE,
                FRONT_TO_BACK,
                BACK_TO_FRONT,
                TEXTURE
            };

            class SpriteGlyph {
                friend class SpriteBatch;
            public:
                SpriteGlyph();
                SpriteGlyph(VGTexture texID, f32 d);

                VGTexture texture = 0;
                f32 depth = 0.0f;
            private:
                ui32 vOff = 0;
                ui32 numVerts = 0;
            };

            class SpriteBatch {
            public:
                SpriteBatch(bool isDynamic = true, bool init = false);
                ~SpriteBatch();

                void init();
                void dispose();

                void begin();
                void end(SpriteSortMode ssm = SpriteSortMode::TEXTURE);

                CALLEE_DELETE SpriteGlyph* draw(VGTexture tex, OPT f32v4* uvRect, OPT f32v2* uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, const f32& rotation, const color4& tint, f32 depth = 0.0f);

                void renderBatch(f32m4 mWorld, f32m4 mCamera, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
                void renderBatch(f32m4 mWorld, const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
                void renderBatch(const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);

                void sortGlyphs(SpriteSortMode ssm);
                void generateBatches();

                void applyScissorTest(f32v2 rectMin, f32v2 rectMax, SpriteGlyph* glyph);

                static void disposeProgram();
            private:
                static bool SSMTexture(SpriteGlyph* g1, SpriteGlyph* g2) {
                    return g1->texture < g2->texture;
                }
                static bool SSMFrontToBack(SpriteGlyph* g1, SpriteGlyph* g2) {
                    return g1->depth < g2->depth;
                }
                static bool SSMBackToFront(SpriteGlyph* g1, SpriteGlyph* g2) {
                    return g1->depth > g2->depth;
                }

                void createProgram();
                void createVertexArray();
                void createPixelTexture();

                class SpriteBatchCall {
                public:
                    VGTexture texture = 0;
                    i32 indices = 0;
                    i32 indexOffset = 0;

                    void set(i32 iOff, VGTexture texID, std::vector<SpriteBatchCall*>& calls);
                    SpriteBatchCall* append(SpriteGlyph* g, std::vector<SpriteBatchCall*>& calls, PtrRecycler<SpriteBatchCall>* recycler);
                };

                // Glyph Information
                std::vector<SpriteGlyph*> _glyphs;
                PtrRecycler<SpriteGlyph> _glyphRecycler;
                std::vector<VertexSpriteBatch> m_verts;

                // Render Batches
                vg::BufferUsageHint _bufUsage = vg::BufferUsageHint::STATIC_DRAW;
                VGVertexArray _vao = 0;
                VGVertexBuffer _vbo = 0;
                i32 _glyphCapacity = _INITIAL_GLYPH_CAPACITY;
                std::vector<SpriteBatchCall*> _batches;
                PtrRecycler<SpriteBatchCall> _batchRecycler;

                // Custom Shader
                static vg::GLProgram* _program;

                // Default White Pixel Texture
                VGTexture _texPixel = 0;

                static const i32 _INITIAL_GLYPH_CAPACITY = 32;
            };
        }
    }
}
namespace vg = vorb::core::graphics;

#endif // SpriteBatch_h__
