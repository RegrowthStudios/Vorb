#include "stdafx.h"
#include "ui/Panel.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"
#include "utils.h"

vui::Panel::Panel() : Widget() {
    m_sliders[0] = new Slider;
    m_sliders[1] = new Slider;
    updateColor();
    addWidget(m_sliders[0]);
    addWidget(m_sliders[1]);
    updateSliders();
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

    // Add child drawables
    m_sliders[0]->addDrawables(renderer);
    m_sliders[1]->addDrawables(renderer);

    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &Panel::refreshDrawables));

}

void vui::Panel::removeDrawables() {
    Widget::removeDrawables();
    m_sliders[0]->removeDrawables();
    m_sliders[1]->removeDrawables();
}

void vui::Panel::updatePosition() {
    // Use child offset for auto-scroll
    m_position += m_childOffset;
    Widget::updatePosition();
    m_position -= m_childOffset;

    if (m_autoScroll) {
        m_sliders[0]->updatePosition();
        m_sliders[1]->updatePosition();
    }

    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    m_drawableRect.setClipRect(m_clipRect);
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
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableRect.setHeight(height);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableRect.setPosition(m_position);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableRect.setWidth(width);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setX(f32 x) {
    Widget::setX(x);
    m_drawableRect.setX(m_position.x);
    updateSliders();
    refreshDrawables();
}

void vui::Panel::setY(f32 y) {
    Widget::setY(y);
    m_drawableRect.setX(m_position.y);
    updateSliders();
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

void vui::Panel::setAutoScroll(bool autoScroll) {
    if (autoScroll != m_autoScroll) {
        m_autoScroll = autoScroll;
        updateSliders();
        updatePosition();
    }
}

void vui::Panel::updateColor() {
    if (m_isMouseIn) {
        m_drawableRect.setColor(m_backHoverColor);
    } else {
        m_drawableRect.setColor(m_backColor);
    }
    refreshDrawables();
}

void vui::Panel::updateSliders() {
    if (m_autoScroll) {
        // Horizontal scroll bar
        m_sliders[0]->enable();
        m_sliders[0]->setPosition(f32v2(0, m_dimensions.y - m_sliderWidth));
        m_sliders[0]->setDimensions(f32v2(m_dimensions.x - m_sliderWidth, m_sliderWidth));
        m_sliders[0]->setSlideDimensions(f32v2(m_sliderWidth));
        m_sliders[0]->setRange(0, 10000);
        m_sliders[0]->setIsVertical(false);
        // Vertical scroll bar
        m_sliders[1]->enable();
        m_sliders[1]->setPosition(f32v2(m_dimensions.x - m_sliderWidth, 0));
        m_sliders[1]->setDimensions(f32v2(m_sliderWidth, m_dimensions.y - m_sliderWidth));
        m_sliders[1]->setSlideDimensions(f32v2(m_sliderWidth));
        m_sliders[1]->setRange(0, 10000);
        m_sliders[1]->setIsVertical(true);
    } else {
        for (int i = 0; i < 2; i++) {
            m_sliders[i]->setDimensions(f32v2(0.0f));
            m_sliders[i]->setSlideDimensions(f32v2(0.0f));
            m_sliders[i]->disable();
        }
    }
}

void vui::Panel::refreshDrawables() {
    m_drawnRect = m_drawableRect;
}

void vui::Panel::computeClipRect(const f32v4& parentClipRect /*= f32v4(FLT_MIN / 2.0f, FLT_MIN / 2.0f, FLT_MAX, FLT_MAX)*/) {
    f32v2 pos = m_position - m_childOffset;
    f32v2 dims = m_dimensions;
    computeClipping(parentClipRect, pos, dims);
    if (dims.x < 0) dims.x = 0;
    if (dims.y < 0) dims.y = 0;
    m_clipRect = f32v4(pos.x, pos.y, dims.x, dims.y);
    computeChildClipRects();
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
