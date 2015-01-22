///
/// SpriteBatch.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 17 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef SpriteBatch_h__
#define SpriteBatch_h__

#include "../PtrRecycler.hpp"
#include "../Vorb.h"
#include "gtypes.h"

class DepthState;
class RasterizerState;
class SamplerState;
class SpriteFont;
DECL_VG(class GLProgram)

struct VertexSpriteBatch {
public:
    VertexSpriteBatch();
    VertexSpriteBatch(const f32v3& pos, const f32v2& uv, const f32v4& uvr, const color4& color);

    f32v3 position;
    f32v2 uv;
    f32v4 uvRect;
    color4 color;
};

enum class SpriteSortMode {
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

class SpriteGlyph {
public:
    SpriteGlyph();
    SpriteGlyph(VGTexture texID, f32 d);

    VGTexture textureID;
    f32 depth;

    VertexSpriteBatch vtl;
    VertexSpriteBatch vtr;
    VertexSpriteBatch vbl;
    VertexSpriteBatch vbr;
};

class SpriteBatch {
public:
    SpriteBatch(bool isDynamic = true, bool init = false);
    ~SpriteBatch();

    void init();
    void dispose();

    void begin();

    void draw(VGTexture tex, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 offset, f32v2 size, f32 rotation, const color4& tint, f32 depth = 0.0f);
    void draw(VGTexture tex, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 offset, f32v2 size, const color4& tint, f32 depth = 0.0f);
    void draw(VGTexture tex, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);
    void draw(VGTexture tex, f32v4* uvRect, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);
    void draw(VGTexture tex, f32v2 position, f32v2 size, const color4& tint, f32 depth = 0.0f);
    
    void drawString(SpriteFont* font, const cString s, f32v2 position, f32v2 scaling, const color4& tint, f32 depth = 0.0f);
    void drawString(SpriteFont* font, const cString s, f32v2 position, f32 desiredHeight, f32 scaleX, const color4& tint, f32 depth = 0.0f);
    
    void end(SpriteSortMode ssm = SpriteSortMode::TEXTURE);

    void renderBatch(f32m4 mWorld, f32m4 mCamera, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
    void renderBatch(f32m4 mWorld, const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
    void renderBatch(const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss = nullptr, const DepthState* ds = nullptr, const RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);

    void sortGlyphs(SpriteSortMode ssm);
    void generateBatches();

    static void disposeProgram();
private:
    static bool SSMTexture(SpriteGlyph* g1, SpriteGlyph* g2) {
        return g1->textureID < g2->textureID;
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
        ui32 textureID;
        i32 indices;
        i32 indexOffset;

        void set(i32 iOff, ui32 texID, std::vector<SpriteBatchCall*>& calls);
        SpriteBatchCall* append(SpriteGlyph* g, std::vector<SpriteBatchCall*>& calls, PtrRecycler<SpriteBatchCall>* recycler);
    };

    // Glyph Information
    std::vector<SpriteGlyph*> _glyphs;
    PtrRecycler<SpriteGlyph> _glyphRecycler;

    // Render Batches
    ui32 _bufUsage;
    ui32 _vao, _vbo;
    i32 _glyphCapacity;
    std::vector<SpriteBatchCall*> _batches;
    PtrRecycler<SpriteBatchCall> _batchRecycler;

    // Custom Shader
    static vg::GLProgram* _program;

    // Default White Pixel Texture
    ui32 _texPixel;

    static const i32 _INITIAL_GLYPH_CAPACITY = 32;
};

#endif // SpriteBatch_h__
