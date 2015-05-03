#include "stdafx.h"
#include "UI/UIRenderer.h"

vorb::ui::UIRenderer::UIRenderer() {
    // Empty
}

vorb::ui::UIRenderer::~UIRenderer() {
    // Empty
}

void vorb::ui::UIRenderer::add(const Widget* widget, const DrawFunc& drawFunc, const RefreshFunc& refreshFunc) {
    ui32 index = m_drawFuncs.size();
    m_widgetLookup[widget].push_back(index);
    m_drawFuncs.push_back(drawFunc);
    m_refreshFuncs.push_back(refreshFunc);
}

bool vorb::ui::UIRenderer::remove(const Widget* widget) {
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

void vorb::ui::UIRenderer::dispose() {
    // TODO(Ben): Implement
}

void vorb::ui::UIRenderer::draw(vg::SpriteBatch* sb) {
    for (auto& f : m_drawFuncs) f(sb);
}
