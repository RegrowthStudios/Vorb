#include "Vorb/stdafx.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/GameWindow.h"

vui::Viewport::Viewport(const GameWindow* window /*= nullptr*/) : IWidget() {
    m_window = window;
}

vui::Viewport::~Viewport() {
    // Empty
}

void vui::Viewport::init(const nString& name, const f32v4& destRect, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    vui::InputDispatcher::window.onResize += makeDelegate(*this, &Viewport::onResize);
    
    m_name = name;
    m_position.x = destRect.x;
    m_position.y = destRect.y;
    m_size.x = destRect.z;
    m_size.y = destRect.w;
    m_renderer.init(defaultFont, spriteBatch);
}

// TODO(Matthew): Clarify roles of {add|remove}Widget and setParent - rewrite such that either both pathways are public and viable endpoints or such that one is protected.
// TODO(Matthew): Clarify role of flags and update functions so that updates are propagated to descendants ONCE - rewrite such that Viewport provides an easy entry point for updates.
bool vui::Viewport::addWidget(IWidget* widget) {
    if (IWidget::addWidget(widget)) {
        widget->m_viewport = this;
        if (!widget->getRenderer()) widget->setRenderer(&m_renderer);
        return true;
    }
    return false;
}

bool vui::Viewport::removeWidget(IWidget* widget) {
    if (IWidget::removeWidget(widget)) {
        widget->setRenderer(nullptr);
        return true;
    }
    return false;
}

void vui::Viewport::draw() {
    if (!m_flags.isEnabled) return;
    m_renderer.draw(m_size);
}

void vui::Viewport::dispose() {
    IWidget::dispose();
    m_renderer.dispose();
}

void vui::Viewport::onResize(Sender s VORB_MAYBE_UNUSED, const WindowResizeEvent& e VORB_MAYBE_UNUSED) {
    m_flags.needsDimensionUpdate = true;
}
