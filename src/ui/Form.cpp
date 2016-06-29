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

void vui::Form::init(const nString& name, IGameScreen* ownerScreen, const f32v4& destRect, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_name = name;
    m_ownerIGameScreen = ownerScreen;
    m_position.x = destRect.x;
    m_position.y = destRect.y;
    m_dimensions.x = destRect.z;
    m_dimensions.y = destRect.w;
    m_renderer.init(defaultFont, spriteBatch);
}

bool vui::Form::addWidget(Widget* widget) {
    if (IWidgetContainer::addWidget(widget, this)) {
        widget->addDrawables(&m_renderer);
        return true;
    }
    return false;
}

bool vui::Form::removeWidget(Widget* widget) {
    if (IWidgetContainer::removeWidget(widget)) {
        return true;
    }
    return false;
}

void vui::Form::updatePosition() {
    computeClipRect();
    
    updateChildPositions();
}

void vui::Form::update(f32 dt /*= 1.0f*/) {
    if (!m_isEnabled) return;
    for (auto& w : m_widgets) {
        // Check if we need to reload the drawables
        if (w->needsDrawableReload()) {
            w->removeDrawables();
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
