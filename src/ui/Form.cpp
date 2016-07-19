#include "stdafx.h"
#include "ui/Form.h"
#include "ui/Widget.h"
#include "ui/GameWindow.h"

vui::Form::Form() : IWidgetContainer() {
    // Empty
}

vui::Form::~Form() {
    // Empty
}

void vui::Form::init(const nString& name, IGameScreen* ownerScreen, const GameWindow* viewport, const f32v4& destRect, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_name = name;
    m_viewport = viewport;
    m_ownerIGameScreen = ownerScreen;
    m_position.x = destRect.x;
    m_position.y = destRect.y;
    m_dimensions.x = destRect.z;
    m_dimensions.y = destRect.w;
    m_pendingUpdates |= UpdateFlag::DIMENSIONS | UpdateFlag::POSITION;
    m_renderer.init(defaultFont, spriteBatch);
}

void vui::Form::update(f32 dt /*= 0.0f*/) {
    if (!m_isEnabled) return;

    if ((m_pendingUpdates & UpdateFlag::CLIPPING) == UpdateFlag::CLIPPING)
        updateClipping();

    for (auto& w : m_widgets) {
        w->update(dt);
    }
}

void vui::Form::draw() {
    if (!m_isEnabled) return;
    m_renderer.draw(m_viewport->getViewportDims());
}

void vui::Form::dispose() {
    IWidgetContainer::dispose();
    m_renderer.dispose();
}

bool vui::Form::addWidget(Widget* child) {
    return IWidgetContainer::addWidget(child, this);
}

void vui::Form::computeClipRect() {
    m_clipRect = f32v4(-FLT_MAX / 2.0f, -FLT_MAX / 2.0f, FLT_MAX, FLT_MAX);
    if (m_clippingOptions.left) {
        m_clipRect.x = m_position.x;
    }
    if (m_clippingOptions.top) {
        m_clipRect.y = m_position.y;
    }
    if (m_clippingOptions.right) {
        m_clipRect.z = m_dimensions.x;
    }
    if (m_clippingOptions.bottom) {
        m_clipRect.w = m_dimensions.y;
    }
}