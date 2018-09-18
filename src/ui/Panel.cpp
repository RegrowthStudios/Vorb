#include "Vorb/stdafx.h"
#include "Vorb/ui/Panel.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/utils.h"

const int SLIDER_VAL_MAX = 10000;

vui::Panel::Panel() : Widget() {
    updateColor();
    addWidget(&m_sliders[0]);
    addWidget(&m_sliders[1]);
    m_sliders[0].ValueChange += makeDelegate(*this, &Panel::onSliderValueChange);
    m_sliders[1].ValueChange += makeDelegate(*this, &Panel::onSliderValueChange);
    updateSliders();
}

// vui::Panel::Panel(const nString& name, VORB_UNUSED const f32v4& destRect /*= f32v4(0)*/) : Panel() {
//     m_name = name;
//     // setDestRect(destRect);
//     m_drawableRect.setPosition(getPosition());
//     m_drawableRect.setDimensions(getSize());
// }

// vui::Panel::Panel(IWidget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Panel(name, destRect) {
//     parent->addWidget(this);
// }

vui::Panel::~Panel() {
    // Empty
}

void vui::Panel::addDrawables() {
    // Make copy
    m_drawnRect = m_drawableRect;
    // Add the rect
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &Panel::refreshDrawables));
}

void vui::Panel::refreshDrawables() {
    m_drawnRect = m_drawableRect;
}

bool vui::Panel::addWidget(IWidget* child) {
    bool rv = IWidget::addWidget(child);
    updateSliders();
    return rv;
}

void vui::Panel::updateDimensions(f32 dt /*= 1.0f*/) {
    Widget::updateDimensions(dt);

    // Use child offset for auto-scroll
    m_position -= m_childOffset;
    // Update child positions but skip sliders
    for (size_t i = 2; i < m_widgets.size(); i++) {
        m_widgets[i]->updatePosition();
    }
    m_position += m_childOffset;

    if (m_autoScroll) {
        m_sliders[0].updatePosition();
        m_sliders[1].updatePosition();
    }

    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getSize());
    m_drawableRect.setClipRect(m_clipRect);
    refreshDrawables();
}

void vui::Panel::setTexture(VGTexture texture) {
    m_drawableRect.setTexture(texture);
    refreshDrawables();
}

void vui::Panel::setColor(const color4& color) {
    m_backColor = color;

    updateColor();

    m_flags.needsDrawableRefresh = true;
}

void vui::Panel::setHoverColor(const color4& color) {
    m_backHoverColor = color;

    updateColor();

    m_flags.needsDrawableRefresh = true;
}

void vui::Panel::setAutoScroll(bool autoScroll) {
    if (autoScroll != m_autoScroll) {
        m_autoScroll = autoScroll;

        m_flags.needsDrawableRecalculation = true;
    }
}

void vui::Panel::updateColor() {
    if (m_flags.isMouseIn) {
        m_drawableRect.setColor(m_backHoverColor);
    } else {
        m_drawableRect.setColor(m_backColor);
    }
}

void vui::Panel::updateSliders() {
    // bool needsHorizontal = false;
    // bool needsVertical = false;
    // // Check which scroll bars we need
    // maxX = -FLT_MAX;
    // maxY = -FLT_MAX;
    // minX = FLT_MAX;
    // minY = FLT_MAX;
    // if (m_autoScroll) {
    //     int a = 0;
    //     // Skip sliders
    //     for (size_t i = 2; i < m_widgets.size(); i++) {
    //         auto& w = m_widgets[i];
    //         const f32v2& pos = w->getRelativePosition();
    //         const f32v2& dims = w->getSize();
    //         if (pos.x < minX) {
    //             minX = pos.x;
    //         }
    //         if (pos.y < minY) {
    //             minY = pos.y;
    //         }
    //         if (pos.x + dims.x > maxX) {
    //             maxX = pos.x + dims.x;
    //         }
    //         if (pos.y + dims.y > maxY) {
    //             maxY = pos.y + dims.y;
    //         }
    //         a++;
    //     }
    //     if ((maxX > m_dimensions.x) || (minX < 0.0f)) {
    //         needsHorizontal = true;
    //     }
    //     if ((maxY > m_dimensions.y) || (minY < 0.0f)) {
    //         needsVertical = true;
    //     }
    // }
    // if (minX > 0.0f) minX = 0.0f;
    // if (maxX < m_dimensions.x) {
    //     maxX = m_dimensions.x;
    // }
    // if (minY > 0.0f) minY = 0.0f;
    // if (maxY < m_dimensions.y) {
    //     maxY = m_dimensions.y;
    // }

    // if (needsHorizontal) {
    //     m_sliders[0].enable();
    //     m_sliders[0].setPosition(f32v2(0.0f, m_dimensions.y - m_sliderWidth));
    //     if (needsVertical) {
    //         m_sliders[0].setDimensions(f32v2(m_dimensions.x - m_sliderWidth, m_sliderWidth));
    //     } else {
    //         m_sliders[0].setDimensions(f32v2(m_dimensions.x, m_sliderWidth));
    //     }
    //     m_sliders[0].setSlideDimensions(f32v2(m_sliderWidth));
    //     m_sliders[0].setRange(0, SLIDER_VAL_MAX);
    //     m_sliders[0].setIsVertical(false);
    // } else {
    //     m_sliders[0].setDimensions(f32v2(0.0f));
    //     m_sliders[0].setSlideDimensions(f32v2(0.0f));
    //     m_sliders[0].disable();
    // }
    // if (needsVertical) {
    //     m_sliders[1].enable();
    //     m_sliders[1].setPosition(f32v2(m_dimensions.x - m_sliderWidth, 0));
    //     if (needsHorizontal) {
    //         m_sliders[1].setDimensions(f32v2(m_sliderWidth, m_dimensions.y - m_sliderWidth));
    //     } else {
    //         m_sliders[1].setDimensions(f32v2(m_sliderWidth, m_dimensions.y));
    //     }
    //     m_sliders[1].setSlideDimensions(f32v2(m_sliderWidth));
    //     m_sliders[1].setRange(0, SLIDER_VAL_MAX);
    //     m_sliders[1].setIsVertical(true);
    // } else {
    //     m_sliders[1].setDimensions(f32v2(0.0f));
    //     m_sliders[1].setSlideDimensions(f32v2(0.0f));
    //     m_sliders[1].disable();
    // }
}

void vui::Panel::onMouseMove(Sender s VORB_MAYBE_UNUSED, const MouseMotionEvent& e) {
    if (!m_flags.isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
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
}

void vui::Panel::onMouseFocusLost(Sender s VORB_MAYBE_UNUSED, const MouseEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        m_flags.isMouseIn = false;
        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);
        updateColor();
    }
}

void vui::Panel::onSliderValueChange(Sender s, int v) {
    if (m_autoScroll) {
        f32 r = (f32)v / SLIDER_VAL_MAX;
        if ((Slider*)s == &m_sliders[0]) {
            // Horizontal
            f32 range = maxX - minX - m_size.x + m_sliderWidth;
            if (m_sliders[1].isEnabled()) range += m_sliderWidth;
            m_childOffset.x = minX + range * r;
        } else {
            // Vertical
            f32 range = maxY - minY - m_size.y + m_sliderWidth;
            if (m_sliders[0].isEnabled()) range += m_sliderWidth;
            m_childOffset.y = minY + range * r;
        }
    }
    m_flags.needsDimensionUpdate = true;
}