#include "stdafx.h"
#include "UI/UIRenderer.h"

vui::UIRenderer::UIRenderer() {
    // Empty
}

vui::UIRenderer::~UIRenderer() {
    // Empty
}

void vui::UIRenderer::init(vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_defaultFont = defaultFont;
    if (spriteBatch) {
        m_sb = spriteBatch;
    } else {
        m_sb = &m_defaultSb;
    }
    m_sb->init();
}

void vui::UIRenderer::add(const Widget* widget, const DrawFunc& drawFunc, const RefreshFunc& refreshFunc) {
    ui32 index = m_drawFuncs.size();
    m_widgetLookup[widget].push_back(index);
    m_drawFuncs.push_back(drawFunc);
    m_refreshFuncs.push_back(refreshFunc);
}

bool vui::UIRenderer::remove(const Widget* widget) {
    auto& it = m_widgetLookup.find(widget);
    if (it == m_widgetLookup.end()) return false;
    // Loop through and remove all // TODO(Ben): OPTIMIZE THIS CRAP
    for (auto& id : it->second) {
        m_drawFuncs.erase(m_drawFuncs.begin() + id);
        m_refreshFuncs.erase(m_refreshFuncs.begin() + id);
    }
    m_widgetLookup.erase(it);
    return true;
}

void vui::UIRenderer::dispose() {
    m_defaultSb.dispose();
}

void vui::UIRenderer::draw(f32m4 mWorld, f32m4 mCamera, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->begin();
    for (auto& f : m_drawFuncs) f(m_sb);
    m_sb->end(vg::SpriteSortMode::BACK_TO_FRONT);
    m_sb->renderBatch(mWorld, mCamera, ss, ds, rs, shader);
}

void vui::UIRenderer::draw(f32m4 mWorld, const f32v2& screenSize, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->begin();
    for (auto& f : m_drawFuncs) f(m_sb);
    m_sb->end(vg::SpriteSortMode::BACK_TO_FRONT);
    m_sb->renderBatch(mWorld, screenSize, ss, ds, rs, shader);
}

void vui::UIRenderer::draw(const f32v2& screenSize, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/) {
    m_sb->begin();
    for (auto& f : m_drawFuncs) f(m_sb);
    m_sb->end(vg::SpriteSortMode::BACK_TO_FRONT);
    m_sb->renderBatch(screenSize, ss, ds, rs, shader);
}
