#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/Slider.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/widgets/Viewport.h"
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
    Widget::initBase();

    m_isModifiedMap["sliderHoverColor"] = false;

    ValueChange.setSender(this);
}

void vui::Slider::addDrawables(UIRenderer& renderer) {
    // Add the bar rect.
    renderer.add(makeDelegate(&m_drawableBar, &DrawableRect::draw));

    // Add the slide rect <- after the bar rect to draw on top!
    renderer.add(makeDelegate(&m_drawableSlide, &DrawableRect::draw));
}

void vui::Slider::updateDimensions(f32 dt) {
    Widget::updateDimensions(dt);
    m_dock.state = DockState::NONE;
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
}

void vui::Slider::setBarTexture(VGTexture texture) {
    m_drawableBar.setTexture(texture);
}

void vui::Slider::setBarColor(const color4& color) {
    m_barColor = color;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Slider::setSlideColor(const color4& color) {
    m_slideColor = color;

    if (!m_isModifiedMap["sliderHoverColor"]) setSlideHoverColor(color);

    m_flags.needsDrawableRecalculation = true;
}

void vui::Slider::setSlideHoverColor(const color4& color) {
    m_slideHoverColor = color;

    m_isModifiedMap["sliderHoverColor"] = true;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Slider::setValue(i32 value) {
    i32 old = m_value;

    m_value = glm::clamp(value, m_min, m_max);

    if (old != m_value) {
        ValueChange(m_value);

        m_flags.needsDrawableRecalculation = true;
    }
}

void vui::Slider::setRange(i32 min, i32 max) {
    m_min = min;
    m_max = max;

    setValue(m_value);
}

void vui::Slider::setMin(i32 min) {
    m_min = min;

    setValue(m_value);
}

void vui::Slider::setMax(i32 max) {
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
        newPos.y = barPos.y + scale * (barSize.y - slideSize.y);
    } else {
        newPos.x = barPos.x + scale * (barSize.x - slideSize.x);
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

void vui::Slider::onMouseDown(Sender, const MouseButtonEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn || isInSlideBounds((f32)e.x, (f32)e.y)) {
        MouseDown(e);
        m_flags.isClicking = true;
        m_clickPoint = f32v2(e.x, e.y);
        m_clickValue = getValueScaled();
    }
}

void vui::Slider::onMouseMove(Sender, const MouseMotionEvent& e) {
    if (isInSlideBounds((f32)e.x, (f32)e.y)) {
        if (!m_flags.isMouseIn) {
            m_flags.isMouseIn = true;
            MouseEnter(e);

            updateColor();
        }
        MouseMove(e);
    } else {
        if (m_flags.isMouseIn) {
            m_flags.isMouseIn = false;
            MouseLeave(e);

            updateColor();
        }
    }

    // Check value change
    if (m_flags.isClicking) {
        const f32v2& barSize = m_drawableBar.getSize();

        const f32v2& slideSize = m_drawableSlide.getSize();

        f32 v = 0.0f; // Normalised position of slide in bar after mouse move.
        if (m_isVertical) {
            f32 newSlidePos = glm::clamp((f32)e.y - m_clickPoint.y, -m_clickValue * (barSize.y - slideSize.y), barSize.y - slideSize.y - m_clickValue * (barSize.y - slideSize.y));

            v = (newSlidePos + m_clickValue * (barSize.y - slideSize.y)) / (barSize.y - slideSize.y);
        } else {
            f32 newSlidePos = glm::clamp((f32)e.x - m_clickPoint.x, -m_clickValue * (barSize.x - slideSize.x), barSize.x - slideSize.x - m_clickValue * (barSize.x - slideSize.x));

            v = (newSlidePos + m_clickValue * (barSize.x - slideSize.x)) / (barSize.x - slideSize.x);
        }

        // TODO(Ben): Faster round
        i32 newValue = (i32)round(v * (f32)(m_max - m_min)) + m_min;
        if (newValue != m_value) {
            setValue(newValue);
        }
    }
}
