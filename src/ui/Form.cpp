#include "stdafx.h"
#include "ui/Form.h"
#include "ui/Widget.h"
#include "ui/GameWindow.h"

vui::Form::Form() {
    // Empty
}

vui::Form::~Form() {
    // Empty
}

void vui::Form::init(const GameWindow* ownerWindow, ui32v4 destRect, vg::SpriteFont* defaultFont /*= nullptr*/, vg::SpriteBatch* spriteBatch /*= nullptr*/) {
    m_ownerWindow = ownerWindow;
    m_position.x = destRect.x;
    m_position.y = destRect.y;
    m_dimensions.x = destRect.z;
    m_dimensions.y = destRect.w;
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
    for (auto& w : m_widgets) w->update(dt);
}

void vui::Form::draw() {
    glViewport(m_position.x, m_position.y, m_dimensions.x, m_dimensions.y);
    m_renderer.draw(m_dimensions);
    glViewport(0, 0, m_ownerWindow->getWidth(), m_ownerWindow->getHeight());
}

void vui::Form::dispose() {
    IWidgetContainer::dispose();
    m_renderer.dispose();
}
