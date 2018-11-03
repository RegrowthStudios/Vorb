#include "Vorb/stdafx.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/GameWindow.h"

vui::Viewport::Viewport(const GameWindow* window /*= nullptr*/) : Widget() {
    m_window   = window;
}

vui::Viewport::~Viewport() {
    // Empty
}

void vui::Viewport::init(const nString& name, const f32v4& dimensions, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    Widget::init(name, dimensions);

    m_viewport = this;
    updateDescendantViewports();

    m_positionType = PositionType::STATIC_TO_WINDOW;

    m_renderer.init(defaultFont, spriteBatch);
}

void vui::Viewport::init(const nString& name, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    Widget::init(name, position, size);

    m_viewport = this;
    updateDescendantViewports();

    m_positionType = PositionType::STATIC_TO_WINDOW;

    m_renderer.init(defaultFont, spriteBatch);
}

void vui::Viewport::dispose() {
    vui::InputDispatcher::window.onResize -= makeDelegate(*this, &Viewport::onResize);

    IWidget::dispose();
    m_renderer.dispose();
    m_window = nullptr;
}

void vui::Viewport::enable() {
    if (!m_flags.isEnabled) {
        vui::InputDispatcher::window.onResize += makeDelegate(*this, &Viewport::onResize);
    }

    Widget::enable();
}

void vui::Viewport::disable() {
    if (m_flags.isEnabled) {
        vui::InputDispatcher::window.onResize -= makeDelegate(*this, &Viewport::onResize);
    }

    Widget::disable();
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
