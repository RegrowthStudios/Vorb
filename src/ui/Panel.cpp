#include "stdafx.h"
#include "ui/Panel.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"

vui::Panel::Panel() : Widget() {
    updateColor();
}

vui::Panel::Panel(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Panel() {
    m_name = name;
    setDestRect(destRect);
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
}

vui::Panel::Panel(IWidgetContainer* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Panel(name, destRect) {
    parent->addWidget(this);
}

vui::Panel::~Panel() {
    // Empty
}

void vui::Panel::addDrawables(UIRenderer* renderer) {
    Widget::addDrawables(renderer);
    // Make copy
    m_drawnRect = m_drawableRect;
    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &Panel::refreshDrawables));
}

void vui::Panel::updatePosition() {
    Widget::updatePosition();
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    refreshDrawables();
}

void vui::Panel::setTexture(VGTexture texture) {
    m_drawableRect.setTexture(texture);
    refreshDrawables();
}

void vui::Panel::setDestRect(const f32v4& destRect) {
    vui::Widget::setDestRect(destRect);
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    refreshDrawables();
}

void vui::Panel::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);
    refreshDrawables();
}

void vui::Panel::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableRect.setHeight(height);
    refreshDrawables();
}

void vui::Panel::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableRect.setPosition(m_position);
    refreshDrawables();
}

void vui::Panel::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableRect.setWidth(width);
    refreshDrawables();
}

void vui::Panel::setX(f32 x) {
    Widget::setX(x);
    m_drawableRect.setX(m_position.x);
    refreshDrawables();
}

void vui::Panel::setY(f32 y) {
    Widget::setY(y);
    m_drawableRect.setX(m_position.y);
    refreshDrawables();
}

void vui::Panel::setColor(const color4& color) {
    m_backColor = color;
    updateColor();
}

void vui::Panel::setHoverColor(const color4& color) {
    m_backHoverColor = color;
    updateColor();
}

void vui::Panel::updateColor() {
    if (m_isMouseIn) {
        m_drawableRect.setColor(m_backHoverColor);
    } else {
        m_drawableRect.setColor(m_backColor);
    }
    refreshDrawables();
}

void vui::Panel::refreshDrawables() {
    m_drawnRect = m_drawableRect;
}

void vui::Panel::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (!m_isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
        if (!m_isMouseIn) {
            m_isMouseIn = true;
            MouseEnter(e);
            updateColor();
        }
        MouseMove(e);
    } else {
        if (m_isMouseIn) {
            m_isMouseIn = false;
            MouseLeave(e);
            updateColor();
        }
    }
}

void vui::Panel::onMouseFocusLost(Sender s, const MouseEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);
        updateColor();
    }
}
