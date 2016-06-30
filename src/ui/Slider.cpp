#include "stdafx.h"
#include "ui/Slider.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"
#include "utils.h"

vui::Slider::Slider() : Widget() {
    ValueChange.setSender(this);
    updateColor();
    m_drawableSlide.setDimensions(f32v2(30.0f, 30.0f));
}

vui::Slider::Slider(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Slider() {
    m_name = name;
    setDestRect(destRect); 
    updatePosition();
    m_drawableBar.setDimensions(getDimensions());
    m_drawableBar.setPosition(getPosition());
}

vui::Slider::Slider(Widget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Slider(name, destRect) {
    parent->addWidget(this);
}

vui::Slider::Slider(Form* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Slider(name, destRect) {
    parent->addWidget(this);
}

vui::Slider::~Slider() {
    // Empty
}

void vui::Slider::addDrawables(UIRenderer* renderer) {
    Widget::addDrawables(renderer);
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

void vui::Slider::updatePosition() {
    Widget::updatePosition();

    m_drawableBar.setPosition(m_position);
    m_drawableBar.setDimensions(m_dimensions);
    m_drawableBar.setClipRect(m_clipRect);
    m_drawableSlide.setClipRect(m_clipRect);

    updateSlidePosition();
}

void vui::Slider::setSlideDimensions(const f32v2& dimensions) {
    m_drawableSlide.setDimensions(dimensions);
    updatePosition();
}

void vui::Slider::setHeight(f32 height) {
    Widget::setHeight(height);
    updatePosition();
}

void vui::Slider::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    updatePosition();
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
    updatePosition();
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
    m_value = vmath::clamp(value, m_min, m_max);
    if (old != m_value) ValueChange(m_value);
    updatePosition();
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

void vui::Slider::setIsVertical(bool isVertical) {
    if (isVertical != m_isVertical) {
        m_isVertical = isVertical;
        updatePosition();
    }
}

bool vui::Slider::isInSlideBounds(f32 x, f32 y) const {
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
    if (m_isVertical) {
        newPos.x = barPos.x + barDims.x * 0.5f - dims.x * 0.5f;
        newPos.y = barPos.y + scale * barDims.y - dims.y * 0.5f;
    } else {
        newPos.x = barPos.x + scale * barDims.x - dims.x * 0.5f;
        newPos.y = barPos.y + barDims.y * 0.5f - dims.y * 0.5f;
    }
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

void vui::Slider::computeClipRect() {
    f32v4 parentClipRect = f32v4(-FLT_MAX / 2.0f, -FLT_MAX / 2.0f, FLT_MAX, FLT_MAX);
    if (m_parentWidget) {
        parentClipRect = m_parentWidget->getClipRect();
    } else if (m_parentForm) {
        parentClipRect = m_parentForm->getClipRect();
    }

    f32v2 position = m_position;
    f32v2 dimensions = m_dimensions;
    f32v2 slideDimensions = m_drawableSlide.getDimensions();
    position -= slideDimensions * 0.5f;
    dimensions += slideDimensions * 2.0f;

    computeClipping(parentClipRect, position, dimensions);

    if (dimensions.x < 0) dimensions.x = 0;
    if (dimensions.y < 0) dimensions.y = 0;

    m_clipRect = f32v4(position.x, position.y, dimensions.x, dimensions.y);
    if (!m_clippingOptions.left) {
        m_clipRect.x = parentClipRect.x;
    }
    if (!m_clippingOptions.top) {
        m_clipRect.y = parentClipRect.y;
    }
    if (!m_clippingOptions.right) {
        m_clipRect.z = parentClipRect.z;
    }
    if (!m_clippingOptions.bottom) {
        m_clipRect.w = parentClipRect.w;
    }

    computeChildClipRects();
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

    if (isInSlideBounds((f32)e.x, (f32)e.y)) {
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
        float v;
        if (m_isVertical) {
            v = vmath::clamp((e.y - pos.y) / dims.y, 0.0f, 1.0f);
        } else {
            v = vmath::clamp((e.x - pos.x) / dims.x, 0.0f, 1.0f);
        }
        // TODO(Ben): Faster round
        int newValue = (int)round(v * (m_max - m_min)) + m_min;
        if (newValue != m_value) {
            setValue(newValue);
        }
    }
}
