#include "Vorb/stdafx.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/widgets/Viewport.h"
#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/GameWindow.h"

vui::Viewport::Viewport(const GameWindow* window /*= nullptr*/) :
    Widget(),
    m_window(window) {
    m_clipping = Clipping{ ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN, ClippingState::HIDDEN };
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
    IWidget::dispose();

    m_renderer.dispose();

    m_window = nullptr;
}

void vui::Viewport::enable() {
    if (!m_flags.isEnabled) {
        vui::InputDispatcher::window.onResize += makeDelegate(this, &Viewport::onResize);
    }

    Widget::enable();
}

void vui::Viewport::disable() {
    if (m_flags.isEnabled) {
        vui::InputDispatcher::window.onResize -= makeDelegate(this, &Viewport::onResize);
    }

    Widget::disable();
}

void vui::Viewport::update(f32 dt /*= 0.0f*/) {
    if (!m_flags.isEnabled) return;
    IWidget::update(dt);
    IWidget::updateDescendants(dt);
}

void vui::Viewport::draw() {
    if (!m_flags.isEnabled) return;
    m_renderer.prepare();

    addDescendantDrawables(m_renderer);

    m_renderer.render(f32v2(m_window->getViewportDims()));
}

void vui::Viewport::setGameWindow(const GameWindow* window) {
    m_window = window;
    
    m_flags.needsDimensionUpdate = true;
}

void vui::Viewport::onResize(Sender, const WindowResizeEvent&) {
    m_flags.needsDimensionUpdate = true;
    markDescendantsToUpdateDimensions();
}
