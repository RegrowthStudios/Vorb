#include "Vorb/stdafx.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/widgets/Slider.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/widgets/Viewport.h"
#include "Vorb/utils.h"

#include <iostream>

vui::Slider::Slider() :
    Widget(),
    m_barColor(color::LightGray),
    m_slideColor(color::DarkGray),
    m_slideHoverColor(color::LightSlateGray),
    m_value(0),
    m_min(0),
    m_max(10),
    m_isVertical(false),
    m_naturalScroll(true),
    m_elasticScroll(true),
    m_slideStaticFriction(0.95f),
    m_slideKineticFriction(2.5f),
    m_slideWeight(1.0f),
    m_slideEnergy(0.0f),
    m_slideMaxSpeed(4.0f),
    m_slideDirection(0),
    m_scrollSensitivity(8.0f),
    m_scrollStrength(0),
    m_scrollOnParent(true) {
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

void vui::Slider::update(f32 dt /*= 0.0f*/) {
    if (m_elasticScroll) {
        f32 initialSlideVelocity = glm::sqrt(2.0f * m_slideEnergy / m_slideWeight);

        // Subtract the friction-based energy losses.
        m_slideEnergy = glm::clamp(
            m_slideEnergy - (m_slideKineticFriction * m_slideWeight * dt * glm::abs(initialSlideVelocity)) - (m_slideStaticFriction * m_slideStaticFriction * m_slideWeight * dt),
            0.0f,
            std::numeric_limits<f32>::max()
        );

        f32 newSlideSpeed = 0.0f;
        if (m_slideDirection < 0) {
            newSlideSpeed = glm::clamp(-1.0f * glm::sqrt(2.0f * m_slideEnergy / m_slideWeight), -m_slideMaxSpeed, 0.0f);
        } else if (m_slideDirection > 0) {
            newSlideSpeed = glm::clamp(glm::sqrt(2.0f * m_slideEnergy / m_slideWeight), 0.0f, m_slideMaxSpeed);
        }

        static f32 i = 0.0f;
        i += dt;
        if (i >= 0.1f) {
            if (m_scrollStrength > 0) m_scrollStrength -= 1;

            i = 0.0f;
        }

        f32 val = glm::clamp(getValueScaled() + newSlideSpeed * dt , 0.0f, 1.0f);

        // TODO(Ben): Faster round
        i32 newValue = (i32)round(val * (f32)(m_max - m_min)) + m_min;
        i32 oldValue = m_value;
        if (newValue != m_value) {
            setValue(newValue);
        }

        IWidget::update(dt);

        if (newValue != oldValue) {
            i32v2 mousePos = InputDispatcher::mouse.getPosition();
            if (isInSlideBounds((f32)mousePos.x, (f32)mousePos.y)) {
                if (!m_flags.isMouseIn) {
                    m_flags.isMouseIn = true;

                    updateColor();
                }
            } else {
                if (m_flags.isMouseIn) {
                    m_flags.isMouseIn = false;

                    updateColor();
                }
            }
        }
    } else {
        IWidget::update(dt);
    }
}

void vui::Slider::enable() {
    if (!m_flags.isEnabled) {
        vui::InputDispatcher::mouse.onWheel += makeDelegate(this, &Slider::onMouseScroll);
    }

    IWidget::enable();
}

void vui::Slider::disable(bool thisOnly /*= false*/) {
    if (m_flags.isEnabled) {
        vui::InputDispatcher::mouse.onWheel -= makeDelegate(this, &Slider::onMouseScroll);
    }

    IWidget::disable(thisOnly);
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

void vui::Slider::setNaturalScroll(bool naturalScroll) {
    m_naturalScroll = naturalScroll;
}

void vui::Slider::setElasticScroll(bool elasticScroll) {
    m_elasticScroll = elasticScroll;
}

void vui::Slider::setSlideWeight(f32 slideWeight) {
    m_slideWeight = slideWeight;
}

void vui::Slider::setSlideKineticFriction(f32 slideKineticFriction) {
    m_slideKineticFriction = slideKineticFriction;
}

void vui::Slider::setSlideStaticFriction(f32 slideStaticFriction) {
    m_slideStaticFriction = slideStaticFriction;
}

void vui::Slider::setSlideMaxSpeed(f32 slideMaxSpeed) {
    m_slideMaxSpeed = slideMaxSpeed;
}

void vui::Slider::setScrollSensitivity(f32 scrollSensitivity) {
    m_scrollSensitivity = scrollSensitivity;
}

void vui::Slider::setScrollOnParent(bool scrollOnParent) {
    m_scrollOnParent = scrollOnParent;
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
        m_slideEnergy = 0.0f;
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

void vui::Slider::onMouseScroll(Sender, const MouseWheelEvent& e) {
    // Handle scroll attempt if mouse in somewhere on scrollbar.
    if (isInBounds((f32)e.x, (f32)e.y)
        || (m_scrollOnParent && m_parent && m_parent->isMouseIn())) {
        const f32v2& barSize   = m_drawableBar.getSize();
        const f32v2& slideSize = m_drawableSlide.getSize();

        // TODO(Matthew): Check if input is convential mouse or trackpad or otherwise choose between e.dy and e.dx.

        i32 dy = m_naturalScroll ? -e.dy : e.dy;

        if (m_elasticScroll) {
            // If user indicates to go otherway to current motion, kill all motion.
            if ((dy < 0 && m_slideDirection < 0) || (dy > 0 && m_slideDirection > 0)) {
                m_slideEnergy = 0.0f;
                m_scrollStrength = 0;
            }

            m_scrollStrength += glm::abs(dy);
            m_slideDirection = dy;

            if (m_isVertical) {
                m_slideEnergy += glm::abs(m_scrollSensitivity * (f32)m_scrollStrength / (barSize.y - slideSize.y) / m_slideWeight);
            } else {
                m_slideEnergy += glm::abs(m_scrollSensitivity * (f32)m_scrollStrength / (barSize.x - slideSize.x) / m_slideWeight);
            }
        } else {
            f32 oldScaledValue = getValueScaled();
            f32 val;
            if (m_isVertical) {
                val = glm::clamp(oldScaledValue + m_scrollSensitivity * dy / (barSize.y - slideSize.y), 0.0f, 1.0f);
            } else {
                val = glm::clamp(oldScaledValue + m_scrollSensitivity * dy / (barSize.x - slideSize.x), 0.0f, 1.0f);
            }

            // TODO(Ben): Faster round
            i32 newValue = (i32)round(val * (f32)(m_max - m_min)) + m_min;
            if (newValue != m_value) {
                setValue(newValue);
            }
        }
    }
}
