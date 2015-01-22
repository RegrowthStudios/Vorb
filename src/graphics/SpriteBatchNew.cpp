#include "stdafx.h"
#include "graphics/SpriteBatchNew.h"

#include "graphics/GLStates.h"
#include "graphics/GLProgram.h"
#include "graphics/SpriteFont.h"

#include "graphics/SpriteBatchShader.inl"

vg::VertexSpriteBatch::VertexSpriteBatch(const f32v3& pos, const f32v2& uv, const f32v4& uvr, const color4& color) :
position(pos),
uv(uv),
uvRect(uvr),
color(color) {
    // Empty
}
vg::VertexSpriteBatch::VertexSpriteBatch() {
    // Empty
}

vg::SpriteGlyph::SpriteGlyph() {
    // Empty
}
vg::SpriteGlyph::SpriteGlyph(VGTexture texID, f32 d) :
texture(texID),
depth(d) {
    // Empty
}

vg::SpriteBatch::SpriteBatch(bool isDynamic /*= true*/, bool doInit /*= false*/) :
_bufUsage(isDynamic ? vg::BufferUsageHint::DYNAMIC_DRAW : vg::BufferUsageHint::STATIC_DRAW) {
    if (doInit) init();
}
vg::SpriteBatch::~SpriteBatch() {
    _batchRecycler.freeAll();
    _glyphRecycler.freeAll();
}

void vg::SpriteBatch::init() {
    createProgram();
    createVertexArray();
    createPixelTexture();
}
void vg::SpriteBatch::dispose() {
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }
    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }

    if (_texPixel != 0) {
        glDeleteTextures(1, &_texPixel);
        _texPixel = 0;
    }
}

void vg::SpriteBatch::begin() {
    if (_glyphs.size() > 0) {
        // Why Would This Ever Happen?
        for (auto glyph : _glyphs) _glyphRecycler.recycle(glyph);
        std::vector<SpriteGlyph*>().swap(_glyphs);
    }
    for (auto batch : _batches) _batchRecycler.recycle(batch);
    std::vector<SpriteBatchCall*>().swap(_batches);
    std::vector<VertexSpriteBatch>().swap(m_verts);
}
void vg::SpriteBatch::end(SpriteSortMode ssm /*= SpriteSortMode::Texture*/) {
    sortGlyphs(ssm);
    generateBatches();
}

CALLEE_DELETE vg::SpriteGlyph* vg::SpriteBatch::draw(VGTexture tex, OPT f32v4* uvRect, OPT f32v2* uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, const f32& rotation, const color4& tint, f32 depth /*= 0.0f*/) {
    // Account for flipping
    static ui32v4 offs[4] = {
        ui32v4(0, 1, 2, 3),
        ui32v4(1, 0, 3, 2),
        ui32v4(2, 3, 0, 1),
        ui32v4(3, 2, 1, 0)
    };
    ui32 flip = (size.x < 0 ? 0x01 : 0x00) | (size.y < 0 ? 0x02 : 0x00);
    ui32v4& vertInds = offs[flip];

    // Check OPT args
    f32v4& uvr = (uvRect != nullptr) ? *uvRect : f32v4(0, 0, 1, 1);
    f32v2& uvt = (uvTiling != nullptr) ? *uvTiling : f32v2(1, 1);
    
    // Generate a glyph
    SpriteGlyph* g = _glyphRecycler.create();
    g->texture = tex == 0 ? _texPixel : tex;
    g->depth = depth;
    g->vOff = m_verts.size();
    g->numVerts = 4;

    { // Add 4 vertices to the end of the vector
        VertexSpriteBatch nv[4];
        m_verts.insert(m_verts.end(), nv, std::end(nv));
    }

    f32 rxx = (f32)cos(-rotation);
    f32 rxy = (f32)sin(-rotation);
    f32 cl = size.x * (-offset.x);
    f32 cr = size.x * (1 - offset.x);
    f32 ct = size.y * (-offset.y);
    f32 cb = size.y * (1 - offset.y);

    // Top left
    VertexSpriteBatch& vert = m_verts[g->vOff + vertInds.x];
    vert.position.x = (cl * rxx) + (ct * rxy) + position.x;
    vert.position.y = (cl * -rxy) + (ct * rxx) + position.y;
    vert.position.z = depth;
    vert.uv.x = 0;
    vert.uv.y = 0;
    vert.uvRect = uvr;
    vert.color = tint;

    // Top right
    vert = m_verts[g->vOff + vertInds.y];
    vert.position.x = (cr * rxx) + (ct * rxy) + position.x;
    vert.position.y = (cr * -rxy) + (ct * rxx) + position.y;
    vert.position.z = depth;
    vert.uv.x = uvt.x;
    vert.uv.y = 0;
    vert.uvRect = uvr;
    vert.color = tint;

    // Bottom left
    vert = m_verts[g->vOff + vertInds.z];
    vert.position.x = (cl * rxx) + (cb * rxy) + position.x;
    vert.position.y = (cl * -rxy) + (cb * rxx) + position.y;
    vert.position.z = depth;
    vert.uv.x = 0;
    vert.uv.y = uvt.y;
    vert.uvRect = uvr;
    vert.color = tint;

    // Bottom right
    vert = m_verts[g->vOff + vertInds.w];
    vert.position.x = (cr * rxx) + (cb * rxy) + position.x;
    vert.position.y = (cr * -rxy) + (cb * rxx) + position.y;
    vert.position.z = depth;
    vert.uv.x = uvt.x;
    vert.uv.y = uvt.y;
    vert.uvRect = uvr;
    vert.color = tint;

    _glyphs.push_back(g);

    return g;
}

void vg::SpriteBatch::renderBatch(f32m4 mWorld, f32m4 mCamera, /*const BlendState* bs = nullptr,*/ const SamplerState* ss /*= nullptr*/, const DepthState* ds /*= nullptr*/, const RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    //if (bs == nullptr) bs = BlendState::PremultipliedAlphaBlend;
    if (ds == nullptr) ds = &DepthState::NONE;
    if (rs == nullptr) rs = &RasterizerState::CULL_NONE;
    if (ss == nullptr) ss = &SamplerState::LINEAR_WRAP;
    if (shader == nullptr) shader = _program;

    // Setup The Shader
    //bs->set();
    ds->set();
    rs->set();

    shader->use();

    glUniformMatrix4fv(shader->getUniform("World"), 1, false, (f32*)&mWorld);
    glUniformMatrix4fv(shader->getUniform("VP"), 1, false, (f32*)&mCamera);

    glBindVertexArray(_vao);

    // Draw All The Batches
    i32 bc = _batches.size();
    for (int i = 0; i < bc; i++) {
        SpriteBatchCall* batch = _batches[i];

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(shader->getUniform("SBTex"), 0);
        glBindTexture(GL_TEXTURE_2D, batch->texture);
        ss->setObject(0);

        glDrawArrays(GL_TRIANGLES, batch->indexOffset, batch->indices);
    }

    glBindVertexArray(0);
    glBindSampler(0, 0);

    shader->unuse();
}
void vg::SpriteBatch::renderBatch(f32m4 mWorld, const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss /*= nullptr*/, const DepthState* ds /*= nullptr*/, const RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    f32m4 mCamera(
        2.0f / screenSize.x, 0, 0, 0,
        0, -2.0f / screenSize.y, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
        );
    renderBatch(mWorld, mCamera, /*bs, */ ss, ds, rs, shader);
}
void vg::SpriteBatch::renderBatch(const f32v2& screenSize, /*const BlendState* bs = nullptr,*/ const SamplerState* ss /*= nullptr*/, const DepthState* ds /*= nullptr*/, const RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    f32m4 mIdentity(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
        );
    renderBatch(mIdentity, screenSize, /*bs, */ ss, ds, rs, shader);
}

void vg::SpriteBatch::sortGlyphs(SpriteSortMode ssm) {
    if (_glyphs.size() < 1) return;

    switch (ssm) {
    case SpriteSortMode::TEXTURE:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), SSMTexture);
        break;
    case SpriteSortMode::FRONT_TO_BACK:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), SSMFrontToBack);
        break;
    case SpriteSortMode::BACK_TO_FRONT:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), SSMBackToFront);
        break;
    default:
        break;
    }
}
void vg::SpriteBatch::generateBatches() {
    if (_glyphs.size() < 1) return;

    // Create Arrays
    VertexSpriteBatch* verts = new VertexSpriteBatch[6 * _glyphs.size()];
    int vi = 0;

    SpriteBatchCall* call = _batchRecycler.create();
    call->texture = _glyphs[0]->texture;
    call->indexOffset = 0;
    call->indices = 6;
    _batches.push_back(call);
    verts[vi++] = m_verts[_glyphs[0]->vOff];
    verts[vi++] = m_verts[_glyphs[0]->vOff + 2];
    verts[vi++] = m_verts[_glyphs[0]->vOff + 3];
    verts[vi++] = m_verts[_glyphs[0]->vOff + 3];
    verts[vi++] = m_verts[_glyphs[0]->vOff + 1];
    verts[vi++] = m_verts[_glyphs[0]->vOff];
    _glyphRecycler.recycle(_glyphs[0]);

    int gc = _glyphs.size();
    for (int i = 1; i < gc; i++) {
        SpriteGlyph* glyph = _glyphs[i];
        call = call->append(glyph, _batches, &_batchRecycler);
        verts[vi++] = m_verts[glyph->vOff];
        verts[vi++] = m_verts[glyph->vOff + 2];
        verts[vi++] = m_verts[glyph->vOff + 3];
        verts[vi++] = m_verts[glyph->vOff + 3];
        verts[vi++] = m_verts[glyph->vOff + 1];
        verts[vi++] = m_verts[glyph->vOff];
        _glyphRecycler.recycle(_glyphs[i]);
    }
    std::vector<SpriteGlyph*>().swap(_glyphs);

    // Set The Buffer Data
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    if (gc > _glyphCapacity) {
        _glyphCapacity = gc * 2;
        glBufferData(GL_ARRAY_BUFFER, _glyphCapacity * 6 * sizeof(VertexSpriteBatch), nullptr, (VGEnum)_bufUsage);
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, gc * 6 * sizeof(VertexSpriteBatch), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] verts;
}

void vg::SpriteBatch::createProgram() {
    // Only need to create it if it isn't cached
    if (!_program) {
        // Allocate the program
        _program = new vg::GLProgram(true);
        _program->addShader(vg::ShaderType::VERTEX_SHADER, vg::impl::SPRITEBATCH_VS_SRC);
        _program->addShader(vg::ShaderType::FRAGMENT_SHADER, vg::impl::SPRITEBATCH_FS_SRC);

        // Set the attributes
        std::vector <nString> attributes;
        attributes.push_back("vPosition");
        attributes.push_back("vTint");
        attributes.push_back("vUV");
        attributes.push_back("vUVRect");
        _program->setAttributes(attributes);

        // Finish
        _program->link();
        _program->initUniforms();
        _program->initAttributes();
    }
}

void vg::SpriteBatch::createVertexArray() {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    _glyphCapacity = _INITIAL_GLYPH_CAPACITY;
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _glyphCapacity * 6 * sizeof(VertexSpriteBatch), nullptr, (VGEnum)_bufUsage);

    _program->enableVertexAttribArrays();

    glVertexAttribPointer(_program->getAttribute("vPosition"), 3, GL_FLOAT, false, sizeof(VertexSpriteBatch), (void*)offsetof(VertexSpriteBatch, position));
    glVertexAttribPointer(_program->getAttribute("vTint"), 4, GL_UNSIGNED_BYTE, true, sizeof(VertexSpriteBatch), (void*)offsetof(VertexSpriteBatch, color));
    glVertexAttribPointer(_program->getAttribute("vUV"), 2, GL_FLOAT, false, sizeof(VertexSpriteBatch), (void*)offsetof(VertexSpriteBatch, uv));
    glVertexAttribPointer(_program->getAttribute("vUVRect"), 4, GL_FLOAT, false, sizeof(VertexSpriteBatch), (void*)offsetof(VertexSpriteBatch, uvRect));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void vg::SpriteBatch::createPixelTexture() {
    glGenTextures(1, &_texPixel);
    glBindTexture(GL_TEXTURE_2D, _texPixel);
    ui32 pix = 0xffffffffu;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void vg::SpriteBatch::disposeProgram() {
    if (_program) {
        _program->dispose();
        _program = nullptr;
    }
}

vg::GLProgram* vg::SpriteBatch::_program = nullptr;

void vg::SpriteBatch::SpriteBatchCall::set(i32 iOff, ui32 texID, std::vector<SpriteBatchCall*>& calls) {
    texture = texID;
    indices = 6;
    indexOffset = iOff;
    calls.push_back(this);
}
vg::SpriteBatch::SpriteBatchCall* vg::SpriteBatch::SpriteBatchCall::append(SpriteGlyph* g, std::vector<SpriteBatchCall*>& calls, PtrRecycler<SpriteBatchCall>* recycler) {
    if (g->texture != texture) {
        SpriteBatchCall* sbc = recycler->create();
        sbc->set(indexOffset + indices, g->texture, calls);
        return sbc;
    } else {
        indices += 6;
    }
    return this;
}

void vg::SpriteBatch::applyScissorTest(f32v2 rectMin, f32v2 rectMax, SpriteGlyph* glyph) {
    VertexSpriteBatch& vtl = m_verts[glyph->vOff + 0];
    VertexSpriteBatch& vtr = m_verts[glyph->vOff + 1];
    VertexSpriteBatch& vbl = m_verts[glyph->vOff + 2];
    VertexSpriteBatch& vbr = m_verts[glyph->vOff + 3];

    if (vtl.position.x > rectMax.x ||
        vtr.position.x < rectMin.x ||
        vtr.position.y > rectMax.y ||
        vbr.position.y < rectMin.y
        ) {

    }

    //if (vtl.position.x < rectMin.x) {
    //    vtl.position.x
    //}
}
