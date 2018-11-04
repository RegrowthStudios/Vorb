#include "Vorb/stdafx.h"
#include "Vorb/ui/Slider.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/utils.h"

vui::Slider::Slider() :
    Widget(),
    m_barColor(color::LightGray),
    m_slideColor(color::DarkGray),
    m_slideHoverColor(color::LightSlateGray),
    m_value(0),
    m_min(0),
    m_max(10),
    m_isVertical(false) {
    m_drawableSlide.setSize(f32v2(30.0f, 30.0f));


    m_flags.needsDrawableRecalculation = true;
}

vui::Slider::~Slider() {
    // Empty
}

void vui::Slider::initBase() {
    ValueChange.setSender(this);
}

void vui::Slider::addDrawables() {
    if (!m_viewport) return;

    // Make copies
    m_drawnBar  = m_drawableBar;
    m_drawnSlide = m_drawableSlide;
  
    // Add the bar
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnBar, &DrawableRect::draw),
                  makeDelegate(*this, &Slider::refreshDrawables));

    // Add the slide 
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnSlide, &DrawableRect::draw),
                  makeDelegate(*this, &Slider::refreshDrawables));
}

void vui::Slider::refreshDrawables() {
    m_drawnBar   = m_drawableBar;
    m_drawnSlide = m_drawableSlide;
}

void vui::Slider::calculateDrawables() {
    m_drawableBar.setPosition(getPaddedPosition());
    m_drawableBar.setSize(getPaddedSize());
    m_drawableBar.setClipRect(m_clipRect);

    m_slideSize = processLength(m_rawSlideSize);
    m_drawableSlide.setSize(m_slideSize);
    m_drawableSlide.setClipRect(m_clipRect);
    updateSlidePosition();

    updateColor();

    m_flags.needsDrawableRefresh = true;
}

void vui::Slider::setSlideSize(const Length2& size) {
    m_rawSlideSize = size;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Slider::setSlideSize(const f32v2& size) {
    m_slideSize = size;
    m_rawSlideSize = { size.x, size.y, { DimensionType::PIXEL, DimensionType::PIXEL } };

    m_flags.needsDrawableRecalculation = true;
}

void vui::Slider::setSlideTexture(VGTexture texture) {
    m_drawableSlide.setTexture(texture);

    m_flags.needsDrawableRefresh = true;
}

void vui::Slider::setBarTexture(VGTexture texture) {
    m_drawableBar.setTexture(texture);

    m_flags.needsDrawableRefresh = true;
}

void vui::Slider::setBarColor(const color4& color) {
    m_barColor = color;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Slider::setSlideColor(const color4& color) {
    m_slideColor = color;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Slider::setSlideHoverColor(const color4& color) {
    m_slideHoverColor = color;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Slider::setValue(int value) {
    int old = m_value;

    m_value = glm::clamp(value, m_min, m_max);

    if (old != m_value) {
        ValueChange(m_value);

        m_flags.needsDrawableRecalculation = true;
    }
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

        m_flags.needsDrawableRecalculation = true;
    }
}

bool vui::Slider::isInSlideBounds(f32 x, f32 y) const {
    const f32v2& pos  = m_drawableSlide.getPosition();
    const f32v2& size = m_drawableSlide.getSize();

    return (x >= pos.x && x <= pos.x + size.x &&
            y >= pos.y && y <= pos.y + size.y);
}

void vui::Slider::updateSlidePosition() {
    f32 scale = getValueScaled();

    const f32v2& barPos  = getPosition();
    const f32v2& barSize = getSize();

    const f32v2& slideSize = m_drawableSlide.getSize();
   
    f32v2 newPos;
    if (m_isVertical) {
        newPos.x = barPos.x + barSize.x * 0.5f - slideSize.x * 0.5f;
        newPos.y = barPos.y + scale * barSize.y - slideSize.y * 0.5f;
    } else {
        newPos.x = barPos.x + scale * barSize.x - slideSize.x * 0.5f;
        newPos.y = barPos.y + barSize.y * 0.5f - slideSize.y * 0.5f;
    }
    m_drawableSlide.setPosition(newPos);
}

void vui::Slider::updateColor() {
    m_drawableBar.setColor(m_barColor);
    if (m_flags.isMouseIn) {
        m_drawableSlide.setColor(m_slideHoverColor);
    } else {
        m_drawableSlide.setColor(m_slideColor);
    }
}

void vui::Slider::onMouseDown(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (m_flags.isMouseIn) {
        MouseDown(e);
        m_flags.isClicking = true;
    }
}

void vui::Slider::onMouseUp(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (m_flags.isMouseIn) {
        MouseUp(e);
        if (m_flags.isClicking) MouseClick(e);
    }
    m_flags.isClicking = false;
}

void vui::Slider::onMouseMove(Sender s VORB_UNUSED, const MouseMotionEvent& e) {
    if (isInSlideBounds((f32)e.x, (f32)e.y)) {
        if (!m_flags.isMouseIn) {
            m_flags.isMouseIn = true;
            MouseEnter(e);

            updateColor();

            m_flags.needsDrawableRefresh = true;
        }
        MouseMove(e);
    } else {
        if (m_flags.isMouseIn) {
            m_flags.isMouseIn = false;
            MouseLeave(e);

            updateColor();

            m_flags.needsDrawableRefresh = true;
        }
    }

    // Check value change
    if (m_flags.isClicking) {
        const f32v2& pos  = m_drawableBar.getPosition();
        const f32v2& size = m_drawableBar.getSize();
        float v;
        if (m_isVertical) {
            v = glm::clamp((e.y - pos.y) / size.y, 0.0f, 1.0f);
        } else {
            v = glm::clamp((e.x - pos.x) / size.x, 0.0f, 1.0f);
        }
        // TODO(Ben): Faster round
        int newValue = (int)round(v * (m_max - m_min)) + m_min;
        if (newValue != m_value) {
            setValue(newValue);
        }
    }
}
