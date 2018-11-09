#include "Vorb/stdafx.h"
#include "Vorb/ui/UIRenderer.h"

#include "Vorb/ui/GameWindow.h"

vui::UIRenderer::UIRenderer() {
    // Empty
}

vui::UIRenderer::~UIRenderer() {
    // Empty
}

void vui::UIRenderer::init(vg::SpriteFont* defaultFont /*= nullptr*/,
                           vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_defaultFont = defaultFont;
    if (spriteBatch) {
        m_sb = spriteBatch;
    } else {
        m_defaultSb.init();
        m_sb = &m_defaultSb;
    }
}

void vui::UIRenderer::dispose() {
    m_defaultSb.dispose();
}

void vui::UIRenderer::add(const DrawFunc& drawFunc) {
    drawFunc(m_sb);
}

void vui::UIRenderer::render(const f32m4& mWorld, const f32m4& mCamera, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->end(vg::SpriteSortMode::NONE);
    m_sb->render(mWorld, mCamera, ss, ds, rs, shader);
}

void vui::UIRenderer::render(const f32m4& mWorld, const f32v2& screenSize, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->end(vg::SpriteSortMode::NONE);
    m_sb->render(mWorld, screenSize, ss, ds, rs, shader);
}

void vui::UIRenderer::render(const f32v2& screenSize, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->end(vg::SpriteSortMode::NONE);
    m_sb->render(screenSize, ss, ds, rs, shader);
}
