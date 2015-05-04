#include "stdafx.h"
#include "graphics/SpriteBatch.h"

#include "graphics/DepthState.h"
#include "graphics/GLEnums.h"
#include "graphics/GLProgram.h"
#include "graphics/RasterizerState.h"
#include "graphics/SamplerState.h"
#include "graphics/SpriteFont.h"
#include "SpriteBatchShader.inl"

#define VERTS_PER_QUAD 4
#define INDICES_PER_QUAD 6

vg::GLProgram* vg::SpriteBatch::m_program = nullptr;

vg::SpriteBatch::Vertex::Vertex(const f32v3& pos, const f32v2& uv, const f32v4& uvr, const ColorRGBA8& color) :
    position(pos),
    uv(uv),
    uvRect(uvr),
    color(color) {
    // Empty
}

vg::SpriteBatch::Glyph::Glyph(VGTexture tex, const f32v4& uvRect, const f32v2& uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, f32 rotation, const color4& tint, f32 depth) :
    tex(tex),
    uvRect(uvRect),
    uvTiling(uvTiling),
    position(position),
    offset(offset),
    size(size),
    rotation(rotation),
    tint(tint),
    depth(depth) {
    // Empty
}


vg::SpriteBatch::SpriteBatch(bool isDynamic /*= true*/, bool doInit /*= false*/) :
    m_bufUsage(isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) {
    if (doInit) init();
}
vg::SpriteBatch::~SpriteBatch() {
    // Empty
}

void vg::SpriteBatch::init() {
    createProgram();
    createVertexArray();
    createPixelTexture();
}
void vg::SpriteBatch::dispose() {
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_ibo != 0) {
        glDeleteBuffers(1, &m_ibo);
        m_ibo = 0;
        m_indexCapacity = 0;
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }

    if (m_texPixel != 0) {
        glDeleteTextures(1, &m_texPixel);
        m_texPixel = 0;
    }
}

void vg::SpriteBatch::begin() {
    m_glyphs.clear();
    std::vector<Batch>().swap(m_batches);
}

void vg::SpriteBatch::draw(ui32 t, f32v4* uvRect, f32v2* uvTiling, const f32v2& position, const f32v2& offset, const f32v2& size, f32 rotation, const ColorRGBA8& tint, f32 depth /*= 0.0f*/) {
    m_glyphs.emplace_back(t == 0 ? m_texPixel : t,
                          uvRect != nullptr ? *uvRect : f32v4(0, 0, 1, 1),
                          uvTiling != nullptr ? *uvTiling : f32v2(1, 1),
                          position,
                          offset,
                          size,
                          rotation,
                          tint,
                          depth);
}
void vg::SpriteBatch::draw(ui32 t, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 offset, f32v2 size, const ColorRGBA8& tint, f32 depth /*= 0.0f*/) {
    m_glyphs.emplace_back(t == 0 ? m_texPixel : t,
                          uvRect != nullptr ? *uvRect : f32v4(0, 0, 1, 1),
                          uvTiling != nullptr ? *uvTiling : f32v2(1, 1),
                          position,
                          offset,
                          size,
                          0.0f,
                          tint,
                          depth);
}
void vg::SpriteBatch::draw(ui32 t, f32v4* uvRect, f32v2* uvTiling, f32v2 position, f32v2 size, const ColorRGBA8& tint, f32 depth /*= 0.0f*/) {
    m_glyphs.emplace_back(t == 0 ? m_texPixel : t,
                          uvRect != nullptr ? *uvRect : f32v4(0, 0, 1, 1),
                          uvTiling != nullptr ? *uvTiling : f32v2(1, 1),
                          position,
                          f32v2(0.0f),
                          size,
                          0.0f,
                          tint,
                          depth);
}
void vg::SpriteBatch::draw(ui32 t, f32v4* uvRect, f32v2 position, f32v2 size, const ColorRGBA8& tint, f32 depth /*= 0.0f*/) {
    m_glyphs.emplace_back(t == 0 ? m_texPixel : t,
                          uvRect != nullptr ? *uvRect : f32v4(0, 0, 1, 1),
                          f32v2(1, 1),
                          position,
                          f32v2(0.0f),
                          size,
                          0.0f,
                          tint,
                          depth);
}
void vg::SpriteBatch::draw(ui32 t, f32v2 position, f32v2 size, const ColorRGBA8& tint, f32 depth /*= 0.0f*/) {
    m_glyphs.emplace_back(t == 0 ? m_texPixel : t,
                          f32v4(0, 0, 1, 1),
                          f32v2(1, 1),
                          position,
                          f32v2(0.0f),
                          size,
                          0.0f,
                          tint,
                          depth);
}

void vg::SpriteBatch::drawString(const SpriteFont* font, const cString s, f32v2 position, f32v2 scaling, const ColorRGBA8& tint, f32 depth /*= 0.0f*/) {
    if (s == nullptr) s = "";
    font->draw(this, s, position, scaling, tint, depth);
}
void vg::SpriteBatch::drawString(const SpriteFont* font, const cString s, f32v2 position, f32 desiredHeight, f32 scaleX, const ColorRGBA8& tint, f32 depth /*= 0.0f*/) {
    if (s == nullptr) s = "";
    f32v2 scaling(desiredHeight / (f32)font->getFontHeight());
    scaling.x *= scaleX;
    font->draw(this, s, position, scaling, tint, depth);
}

void vg::SpriteBatch::end(SpriteSortMode ssm /*= SpriteSortMode::Texture*/) { 
    // Set pointers for fast sort
    m_glyphPtrs.resize(m_glyphs.size());
    for (int i = 0; i < m_glyphs.size(); i++) m_glyphPtrs[i] = &m_glyphs[i];

    sortGlyphs(ssm);
    generateBatches();
}

void vg::SpriteBatch::renderBatch(f32m4 mWorld, f32m4 mCamera, /*const BlendState* bs = nullptr,*/ const SamplerState* ss /*= nullptr*/, const DepthState* ds /*= nullptr*/, const RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    //if (bs == nullptr) bs = BlendState::PremultipliedAlphaBlend;
    if (ds == nullptr) ds = &DepthState::NONE;
    if (rs == nullptr) rs = &RasterizerState::CULL_NONE;
    if (ss == nullptr) ss = &SamplerState::LINEAR_WRAP;
    if (shader == nullptr) shader = m_program;

    // Setup The Shader
    //bs->set();
    ds->set();
    rs->set();

    shader->use();

    glUniformMatrix4fv(shader->getUniform("World"), 1, false, &mWorld[0][0]);
    glUniformMatrix4fv(shader->getUniform("VP"), 1, false, &mCamera[0][0]);

    glBindVertexArray(m_vao); // TODO(Ben): This wont work with custom shaders!

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(shader->getUniform("SBTex"), 0);
    // Draw All The Batches
    for (auto& b : m_batches) {

        glBindTexture(GL_TEXTURE_2D, b.textureID);
        ss->set(GL_TEXTURE_2D);

        glDrawElements(GL_TRIANGLES, b.indices, GL_UNSIGNED_INT, (const GLvoid*)(b.indexOffset * sizeof(ui32)));
    }

    glBindVertexArray(0);

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
    f32m4 mIdentity(1.0f);
    renderBatch(mIdentity, screenSize, /*bs, */ ss, ds, rs, shader);
}

void vg::SpriteBatch::sortGlyphs(SpriteSortMode ssm) {
    if (m_glyphPtrs.empty()) return;

    // Sort the data
    switch (ssm) {
    case SpriteSortMode::TEXTURE:
        std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), SSMTexture);
        break;
    case SpriteSortMode::FRONT_TO_BACK:
        std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), SSMFrontToBack);
        break;
    case SpriteSortMode::BACK_TO_FRONT:
        std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), SSMBackToFront);
        break;
    default:
        break;
    }
}
void vg::SpriteBatch::generateBatches() {
    if (m_glyphPtrs.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, m_bufUsage);
        return;
    }

    // Create Arrays
    Vertex* verts = new Vertex[VERTS_PER_QUAD * m_glyphPtrs.size()];
    ui32 vi = 0;
    ui32 indexCount = 0;

    // Add  the first batch
    m_batches.emplace_back();
    m_batches.back().set(0, m_glyphPtrs[0]->tex);
   
    // Loop through all glyphs
    for (auto& g : m_glyphPtrs) {
        auto& oldBatch = m_batches.back();
        if (g->tex != oldBatch.textureID) {
            oldBatch.indices = indexCount - oldBatch.indexOffset;
            m_batches.emplace_back();
            m_batches.back().set(indexCount, g->tex);
        }
        // Apply rotation
        f32 rxx = (f32)cos(-g->rotation);
        f32 rxy = (f32)sin(-g->rotation);
        f32 cl = g->size.x * (-g->offset.x);
        f32 cr = g->size.x * (1.0f - g->offset.x);
        f32 ct = g->size.y * (-g->offset.y);
        f32 cb = g->size.y * (1.0f - g->offset.y);
        // Top Left
        Vertex& vtl = verts[vi++];
        vtl.position.x = (cl * rxx) + (ct * rxy) + g->position.x;
        vtl.position.y = (cl * -rxy) + (ct * rxx) + g->position.y;
        vtl.position.z = g->depth;
        vtl.uv.x = 0.0f;
        vtl.uv.y = 0.0f;
        vtl.uvRect = g->uvRect;
        vtl.color = g->tint;
        // Top Right
        Vertex& vtr = verts[vi++];
        vtr.position.x = (cr * rxx) + (ct * rxy) + g->position.x;
        vtr.position.y = (cr * -rxy) + (ct * rxx) + g->position.y;
        vtr.position.z = g->depth;
        vtr.uv.x = g->uvTiling.x;
        vtr.uv.y = 0.0f;
        vtr.uvRect = g->uvRect;
        vtr.color = g->tint;
        // Bottom Left
        Vertex& vbl = verts[vi++];
        vbl.position.x = (cl * rxx) + (cb * rxy) + g->position.x;
        vbl.position.y = (cl * -rxy) + (cb * rxx) + g->position.y;
        vbl.position.z = g->depth;
        vbl.uv.x = 0.0f;
        vbl.uv.y = g->uvTiling.y;
        vbl.uvRect = g->uvRect;
        vbl.color = g->tint;
        // Bottom Right
        Vertex& vbr = verts[vi++];
        vbr.position.x = (cr * rxx) + (cb * rxy) + g->position.x;
        vbr.position.y = (cr * -rxy) + (cb * rxx) + g->position.y;
        vbr.position.z = g->depth;
        vbr.uv.x = g->uvTiling.x;
        vbr.uv.y = g->uvTiling.y;
        vbr.uvRect = g->uvRect;
        vbr.color = g->tint;
        indexCount += INDICES_PER_QUAD;
    }
    m_batches.back().indices = indexCount - m_batches.back().indexOffset;

    // Upload index data if needed
    if (m_indexCapacity < indexCount) {
        ui32 start = m_indexCapacity;
        m_indexCapacity = indexCount;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        // Orphan the buffer for speed
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(ui32), nullptr, m_bufUsage);
        // Generate indices
        std::vector<ui32> indices(indexCount - start);
        ui32 i = 0;
        for (ui32 v = start; i < indexCount; v += 4u) {
            indices[i++] = v;
            indices[i++] = v + 2;
            indices[i++] = v + 3;
            indices[i++] = v + 3;
            indices[i++] = v + 1;
            indices[i++] = v;
        }
        // Set data
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start * sizeof(ui32), indexCount * sizeof(ui32), indices.data());
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // Orphan the buffer for speed
    glBufferData(GL_ARRAY_BUFFER, vi * sizeof(Vertex), nullptr, m_bufUsage);
    // Set data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vi * sizeof(Vertex), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Don't free memory, only clear it
    m_glyphs.clear();
    m_glyphPtrs.clear();

    delete[] verts;
}

void vg::SpriteBatch::createProgram() {
    // Only need to create it if it isn't cached
    if (!m_program) {

        // Allocate the program
        m_program = new vg::GLProgram(true);

        // Create the vertex shader
        m_program->addShader(vg::ShaderType::VERTEX_SHADER, impl::SPRITEBATCH_VS_SRC);

        // Create the fragment shader
        m_program->addShader(vg::ShaderType::FRAGMENT_SHADER, impl::SPRITEBATCH_FS_SRC);

        // Set the attributes
        std::vector <nString> attributes;
        attributes.push_back("vPosition");
        attributes.push_back("vTint");
        attributes.push_back("vUV");
        attributes.push_back("vUVRect");
        m_program->setAttributes(attributes);

        // Link the program
        m_program->link();

        // Init the uniforms
        m_program->initUniforms();

        // Init the attributes
        m_program->initAttributes();
    }
}

void vg::SpriteBatch::createVertexArray() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  
    m_program->enableVertexAttribArrays();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, uvRect));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void vg::SpriteBatch::createPixelTexture() {
    glGenTextures(1, &m_texPixel);
    glBindTexture(GL_TEXTURE_2D, m_texPixel);
    ui32 pix = 0xffffffffu;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
    vg::SamplerState::POINT_CLAMP.set(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void vg::SpriteBatch::disposeProgram() {
    if (m_program) {
        m_program->dispose();
        delete m_program;
        m_program = nullptr;
    }
}

void vg::SpriteBatch::Batch::set(ui32 iOff, ui32 texID) {
    textureID = texID;
    indexOffset = iOff;
}
