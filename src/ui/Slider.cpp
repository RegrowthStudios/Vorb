#include "stdafx.h"
#include "ui/Slider.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"

vui::Slider::Slider() : Widget() {
    ValueChange.setSender(this);
    updateColor();
    m_drawableSlide.setDimensions(f32v2(15.0f, 30.0f));
}

vui::Slider::Slider(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Slider() {
    m_name = name;
    setDestRect(destRect);
    m_drawableBar.setDimensions(getDimensions());
    m_drawableBar.setPosition(getPosition());
    updateSlidePosition();
}

vui::Slider::Slider(Widget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Slider(name, destRect) {
    parent->addWidget(this);
    m_parent = parent;
}

vui::Slider::~Slider() {
    // Empty
}

void vui::Slider::addDrawables(UIRenderer* renderer) {
    // Make copies
    m_drawnBar = m_drawableBar;
    m_drawnSlide = m_drawableSlide;
  
    // Add the bar
    renderer->add(this,
                  makeDelegate(m_drawnBar, &DrawableRect::draw),
                  makeDelegate(*this, &Slider::refreshDrawables));

    // Add the slide 
    renderer->add(this,
                  makeDelegate(m_drawnSlide, &DrawableRect::draw),
                  makeDelegate(*this, &Slider::refreshDrawables));
}

void vui::Slider::removeDrawables(UIRenderer* renderer) {
    renderer->remove(this);
}

void vui::Slider::setSlideDimensions(const f32v2& dimensions) {
    m_drawableSlide.setDimensions(dimensions);
    updateSlidePosition();
}

void vui::Slider::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableBar.setDimensions(dimensions);
    updateSlidePosition();
}

void vui::Slider::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableBar.setHeight(height);
    updateSlidePosition();
}

void vui::Slider::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableBar.setPosition(position);
    updateSlidePosition();
}

void vui::Slider::setSlideTexture(VGTexture texture) {
    m_drawableSlide.setTexture(texture);
    refreshDrawables();
}

void vui::Slider::setBarTexture(VGTexture texture) {
    m_drawableBar.setTexture(texture);
    refreshDrawables();
}

void vui::Slider::setBarColor(const color4& color) {
    m_barColor = color;
    updateColor();
}

void vui::Slider::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableBar.setWidth(width);
    updateSlidePosition();
}

void vui::Slider::setX(f32 x) {
    Widget::setX(x);
    m_drawableBar.setX(x);
    updateSlidePosition();
}

void vui::Slider::setY(f32 y) {
    Widget::setY(y);
    m_drawableBar.setX(y);
    updateSlidePosition();
}

void vui::Slider::setSlideColor(const color4& color) {
    m_slideColor = color;
    updateColor();
}

void vui::Slider::setSlideHoverColor(const color4& color) {
    m_slideHoverColor = color;
    updateColor();
}

void vui::Slider::setValue(int value) {
    int old = m_value;
    m_value = glm::clamp(value, m_min, m_max);
    if (old != m_value) ValueChange(m_value);
    updateSlidePosition();
}

void vui::Slider::setRange(int min, int max) {
    m_min = min;
    m_max = max;
    setValue(m_value);
}

void vui::Slider::setMin(int min) {
    m_min = min;
    setValue(m_value);
}

void vui::Slider::setMax(int max) {
    m_max = max;
    setValue(m_value);
}

bool vui::Slider::isInSlideBounds(f32 x, f32 y) {
    const f32v2& pos = m_drawableSlide.getPosition();
    const f32v2& dims = m_drawableSlide.getDimensions();
    return (x >= pos.x && x <= pos.x + dims.x &&
            y >= pos.y && y <= pos.y + dims.y);
}

void vui::Slider::updateSlidePosition() {
    f32 scale = getValueScaled();
    const f32v2& barPos = getPosition();
    const f32v2& barDims = getDimensions();
    const f32v2& dims = m_drawableSlide.getDimensions();
    f32v2 newPos;
    newPos.x = barPos.x + scale * barDims.x - dims.x * 0.5f;
    newPos.y = barPos.y + barDims.y * 0.5 - dims.y * 0.5f;
    m_drawableSlide.setPosition(newPos);
    refreshDrawables();
}

void vui::Slider::updateColor() {
    m_drawableBar.setColor(m_barColor);
    if (m_isMouseIn) {
        m_drawableSlide.setColor(m_slideHoverColor);
    } else {
        m_drawableSlide.setColor(m_slideColor);
    }
    refreshDrawables();
}

void vui::Slider::refreshDrawables() {
    m_drawnBar = m_drawableBar;
    m_drawnSlide = m_drawableSlide;
}

void vui::Slider::onMouseDown(Sender s, const MouseButtonEvent& e) {
    if (m_isMouseIn) {
        MouseDown(e);
        m_isClicking = true;
    }
}

void vui::Slider::onMouseUp(Sender s, const MouseButtonEvent& e) {
    if (m_isMouseIn) {
        MouseUp(e);
        if (m_isClicking) MouseClick(e);
    }
    m_isClicking = false;
}

void vui::Slider::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (isInSlideBounds(e.x, e.y)) {
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

    // Check value change
    if (m_isClicking) {
        const f32v2& pos = m_drawableBar.getPosition();
        const f32v2& dims = m_drawableBar.getDimensions();
        float v = glm::clamp((e.x - pos.x) / dims.x, 0.0f, 1.0f);
        // TODO(Ben): Faster round
        int newValue = round(v * (m_max - m_min)) + m_min;
        if (newValue != m_value) {
            setValue(newValue);
        }
    }
}
