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

void vui::Form::init(IGameScreen* ownerScreen, const ui32v4& destRect, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_ownerIGameScreen = ownerScreen;
    m_position.x = (f32)destRect.x;
    m_position.y = (f32)destRect.y;
    m_dimensions.x = (f32)destRect.z;
    m_dimensions.y = (f32)destRect.w;
    m_renderer.init(defaultFont, spriteBatch);
}

bool vui::Form::addWidget(Widget* widget) {
    if (IWidgetContainer::addWidget(widget)) {
        widget->addDrawables(&m_renderer);
        return true;
    }
    return false;
}

bool vui::Form::removeWidget(Widget* widget) {
    if (IWidgetContainer::removeWidget(widget)) {
        widget->removeDrawables(&m_renderer);
        return true;
    }
    return false;
}

void vui::Form::updatePosition() {
    m_position = m_relativePosition;
    // Update child positions
    for (auto& w : m_widgets) {
        w->updatePosition();
    }
}

void vui::Form::update(f32 dt /*= 1.0f*/) {
    if (!m_isEnabled) return;
    for (auto& w : m_widgets) {
        // Check if we need to reload the drawables
        if (w->needsDrawableReload()) {
            w->removeDrawables(&m_renderer);
            w->setNeedsDrawableReload(false);
            w->addDrawables(&m_renderer);
        }
        w->update(dt);
    }
}

void vui::Form::draw() {
    if (!m_isEnabled) return;
    m_renderer.draw(m_dimensions);
}

void vui::Form::dispose() {
    IWidgetContainer::dispose();
    m_renderer.dispose();
}

void vui::Form::enable() {
    if (!m_isEnabled) {
        m_isEnabled = true;
        for (auto& w : m_widgets) w->enable();
    }
}

void vui::Form::disable() {
    if (m_isEnabled) {
        m_isEnabled = false;
        for (auto& w : m_widgets) w->disable();
    }
}