#include "Vorb/stdafx.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/GameWindow.h"

vui::Viewport::Viewport(const GameWindow* window /*= nullptr*/) : IWidget() {
    m_viewport = this;
    m_window   = window;
}

vui::Viewport::~Viewport() {
    // Empty
}

// TODO(Matthew): Support raw lengths - derive from Widget rather than IWidget.
void vui::Viewport::init(const nString& name, const f32v4& destRect, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    vui::InputDispatcher::window.onResize += makeDelegate(*this, &Viewport::onResize);
    
    m_name = name;

    m_position.x = destRect.x;
    m_position.y = destRect.y;
    m_size.x = destRect.z;
    m_size.y = destRect.w;

    m_renderer.init(defaultFont, spriteBatch);
}

void vui::Viewport::dispose() {
    vui::InputDispatcher::window.onResize -= makeDelegate(*this, &Viewport::onResize);

    IWidget::dispose();
    m_renderer.dispose();
    m_window = nullptr;
}

void vui::Viewport::update(f32 dt /*= 1.0f*/) {
    IWidget::update(dt);
    IWidget::updateDescendants(dt);
}

void vui::Viewport::draw() {
    if (!m_flags.isEnabled) return;
    m_renderer.draw(m_size);
}

void vui::Viewport::setGameWindow(const GameWindow* window) {
    m_window = window;
    
    m_flags.needsDimensionUpdate = true;
}

void vui::Viewport::onResize(Sender s VORB_MAYBE_UNUSED, const WindowResizeEvent& e VORB_MAYBE_UNUSED) {
    m_flags.needsDimensionUpdate = true;
}
