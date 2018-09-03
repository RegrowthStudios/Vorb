#include "Vorb/stdafx.h"
#include "Vorb/ui/Slider.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/utils.h"

vui::Slider::Slider() : Widget() {
    ValueChange.setSender(this);
    updateColor();
    m_drawableSlide.setDimensions(f32v2(30.0f, 30.0f));
}

vui::Slider::Slider(const nString& name, VORB_UNUSED const f32v4& destRect /*= f32v4(0)*/) : Slider() {
    m_name = name;
    // setDestRect(destRect); 
    // updatePosition();
    m_drawableBar.setDimensions(getSize());
    m_drawableBar.setPosition(getPosition());
}

vui::Slider::Slider(IWidget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Slider(name, destRect) {
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

// void vui::Slider::updatePosition() {
//     Widget::updatePosition();

//     m_drawableBar.setPosition(m_position);
//     m_drawableBar.setDimensions(m_dimensions);
//     m_drawableBar.setClipRect(m_clipRect);
//     m_drawableSlide.setClipRect(m_clipRect);

//     updateSlidePosition();
// }

void vui::Slider::setSlideDimensions(const f32v2& dimensions) {
    m_drawableSlide.setDimensions(dimensions);
    // updatePosition();
}

// void vui::Slider::setHeight(f32 height) {
//     Widget::setHeight(height);
//     updatePosition();
// }

// void vui::Slider::setPosition(const f32v2& position) {
//     Widget::setPosition(position);
//     updatePosition();
// }

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

// void vui::Slider::setWidth(f32 width) {
//     Widget::setWidth(width);
//     updatePosition();
// }

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
    // updatePosition();
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
        // updatePosition();
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
    const f32v2& barDims = getSize();
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

// void vui::Slider::computeClipRect(const f32v4& parentClipRect /*= f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX)*/) {
//     if (m_isClippingEnabled) {
//         f32v2 pos = m_position;
//         f32v2 dims = m_dimensions;
//         f32v2 slideDims = m_drawableSlide.getSize();
//         pos -= slideDims * 0.5f;
//         dims += slideDims * 2.0f;

//         computeClipping(parentClipRect, pos, dims);
//         if (dims.x < 0) dims.x = 0;
//         if (dims.y < 0) dims.y = 0;
//         m_clipRect = f32v4(pos.x, pos.y, dims.x, dims.y);
//     } else {
//         m_clipRect = parentClipRect;
//     }
//     computeChildClipRects();
// }

void vui::Slider::onMouseDown(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (m_isMouseIn) {
        MouseDown(e);
        m_isClicking = true;
    }
}

void vui::Slider::onMouseUp(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (m_isMouseIn) {
        MouseUp(e);
        if (m_isClicking) MouseClick(e);
    }
    m_isClicking = false;
}

void vui::Slider::onMouseMove(Sender s VORB_UNUSED, const MouseMotionEvent& e) {

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
            v = glm::clamp((e.y - pos.y) / dims.y, 0.0f, 1.0f);
        } else {
            v = glm::clamp((e.x - pos.x) / dims.x, 0.0f, 1.0f);
        }
        // TODO(Ben): Faster round
        int newValue = (int)round(v * (m_max - m_min)) + m_min;
        if (newValue != m_value) {
            setValue(newValue);
        }
    }
}
