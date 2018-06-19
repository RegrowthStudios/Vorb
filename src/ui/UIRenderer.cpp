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

void vui::UIRenderer::add(const Widget* widget, const DrawFunc& drawFunc, const RefreshFunc& refreshFunc) {
    m_drawableFuncs.push_back({ widget, drawFunc, refreshFunc });
}

bool vui::UIRenderer::remove(const Widget* widget) {
    bool rv = false;
    // Loop through and remove all // TODO(Ben): OPTIMIZE THIS CRAP
    for (i32 i = (i32)m_drawableFuncs.size() - 1; i >= 0; i--) {
        if (m_drawableFuncs[i].owner == widget) {
            m_drawableFuncs.erase(m_drawableFuncs.begin() + i);
            rv = true;
        }
    }
    return rv;
}

void vui::UIRenderer::dispose() {
    m_defaultSb.dispose();
    std::vector<DrawableFuncs>().swap(m_drawableFuncs);
}

void vui::UIRenderer::draw(const f32m4& mWorld, const f32m4& mCamera, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->begin();
    for (auto& f : m_drawableFuncs) f.drawFunc(m_sb);
    m_sb->end(vg::SpriteSortMode::BACK_TO_FRONT);
    m_sb->render(mWorld, mCamera, ss, ds, rs, shader);
}

void vui::UIRenderer::draw(const f32m4& mWorld, const f32v2& screenSize, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->begin();
    for (auto& f : m_drawableFuncs) f.drawFunc(m_sb);
    m_sb->end(vg::SpriteSortMode::BACK_TO_FRONT);
    m_sb->render(mWorld, screenSize, ss, ds, rs, shader);
}

void vui::UIRenderer::draw(const f32v2& screenSize, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->begin();
    for (auto& f : m_drawableFuncs) f.drawFunc(m_sb);
    m_sb->end(vg::SpriteSortMode::BACK_TO_FRONT);
    m_sb->render(screenSize, ss, ds, rs, shader);
}
