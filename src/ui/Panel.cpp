#include "Vorb/stdafx.h"
#include "Vorb/ui/Panel.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/utils.h"

const int SLIDER_VAL_MAX = 10000;

vui::Panel::Panel() : Panel("") {
    // Empty
}

vui::Panel::Panel(const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/) : Widget(name, dimensions) {
    addWidget(&m_sliders.horizontal);
    addWidget(&m_sliders.vertical);

    m_sliders.horizontal.ValueChange += makeDelegate(*this, &Panel::onSliderValueChange);
    m_sliders.vertical.ValueChange   += makeDelegate(*this, &Panel::onSliderValueChange);

    m_flags.needsDrawableRecalculation = true;
}

vui::Panel::Panel(const nString& name, const Length2& position, const Length2& size) : Widget(name, position, size) {
    addWidget(&m_sliders.horizontal);
    addWidget(&m_sliders.vertical);

    m_sliders.horizontal.ValueChange += makeDelegate(*this, &Panel::onSliderValueChange);
    m_sliders.vertical.ValueChange   += makeDelegate(*this, &Panel::onSliderValueChange);

    m_flags.needsDrawableRecalculation = true;
}

vui::Panel::Panel(IWidget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Panel(name, destRect) {
    parent->addWidget(this);
}

vui::Panel::Panel(IWidget* parent, const nString& name, const Length2& position, const Length2& size) : Panel(name, position, size) {
    parent->addWidget(this);
}

vui::Panel::~Panel() {
    // Empty
}

void vui::Panel::addDrawables() {
    // Make copy.
    m_drawnRect = m_drawableRect;
    // Register the drawable.
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &Panel::refreshDrawables));
}

void vui::Panel::refreshDrawables() {
    m_drawnRect = m_drawableRect;
}

bool vui::Panel::addWidget(IWidget* child) {
    bool rv = IWidget::addWidget(child);

    m_flags.needsDrawableRecalculation = true;

    return rv;
}

void vui::Panel::setTexture(VGTexture texture) {
    m_drawableRect.setTexture(texture);

    m_flags.needsDrawableRefresh = true;
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

void vui::Panel::updateDimensions(f32 dt) {
    Widget::updateDimensions(dt);

    updateSliders();
}

void vui::Panel::calculateDrawables() {
    m_drawableRect.setPosition(getPosition() - f32v2(getPadding().x, getPadding().y));
    m_drawableRect.setSize(getSize() + f32v2(getPadding().z, getPadding().w));
    m_drawableRect.setClipRect(m_clipRect);

    updateColor();

    m_flags.needsDrawableRefresh = true;
}

void vui::Panel::updateColor() {
    if (m_flags.isMouseIn) {
        m_drawableRect.setColor(m_backHoverColor);
    } else {
        m_drawableRect.setColor(m_backColor);
    }
}

void vui::Panel::updateSliders() {
    // We need to figure which of the two sliders are needed.
    bool needsHorizontal = false;
    bool needsVertical   = false;
    
    // Reset min and max coord points.
    m_maxX = m_maxY = -FLT_MAX;
    m_minX = m_minY =  FLT_MAX;

    // TODO(Matthew): Revisit this after figuring slider embedding - if children get altered drastically this calculation will be outdated.
    // If we're automatically adding scrollbars, then we want to know the extreme points of the child widgets.
    if (m_autoScroll) {
        // Skip sliders for determining the min and max values.
        for (size_t i = 2; i < m_widgets.size(); ++i) {
            auto& widget = m_widgets[i];
            const f32v2& position = widget->getRelativePosition();
            const f32v2& size     = widget->getSize();

            if (position.x < m_minX) {
                m_minX = position.x;
            }
            if (position.y < m_minY) {
                m_minY = position.y;
            }
            if (position.x + size.x > m_maxX) {
                m_maxX = position.x + size.x;
            }
            if (position.y + size.y > m_maxY) {
                m_maxY = position.y + size.y;
            }
        }
        if ((m_maxX > m_size.x) || (m_minX < 0.0f)) {
            needsHorizontal = true;
        }
        if ((m_maxY > m_size.y) || (m_minY < 0.0f)) {
            needsVertical = true;
        }
    }
    // If all elements sit neatly inside the panel, our min and max points should be the boundaries of the panel itself.
    if (m_minX > 0.0f)     m_minX = 0.0f;
    if (m_minY > 0.0f)     m_minY = 0.0f;
    if (m_maxX < m_size.x) m_maxX = m_size.x;
    if (m_maxY < m_size.y) m_maxY = m_size.y;

    // TODO(Matthew): Either update children then apply childOffset or make childOffset a part of IWidget that on change flags children to updateDimensions (which uses this to shift its position).

    // Note that while this looks like it might iteratively push the sliders further and further out, the processed values of position and padding are recalculated any time we also come here to update sliders - so as long as this never touches the raw values of position and padding we're good!

    // Set up horizontal slider.
    Slider& slider = m_sliders.horizontal;
    if (needsHorizontal) {
        slider.enable();

        slider.setPositionType(PositionType::RELATIVE_TO_PARENT);
        if (m_flipHorizontal) {
            m_position.x += m_sliderWidth;
            m_padding.x  += m_sliderWidth;

            slider.setRawLeft(-1.0f * m_sliderWidth);
            slider.setRawRight({ 1.0f, { DimensionType::PARENT_WIDTH_PERCENTAGE } });
        } else {
            m_size.x    -= m_sliderWidth;
            m_padding.z += m_sliderWidth;

            slider.setRawLeft({ 1.0f, { DimensionType::PARENT_WIDTH_PERCENTAGE } });
            slider.setRawRight(-1.0f * m_sliderWidth);
        }
        slider.setRawTop(0.0f);
        slider.setRawBottom(0.0f);

        slider.setSlideDimensions(f32v2(m_sliderWidth));
        slider.setRange(0, SLIDER_VAL_MAX);
        slider.setIsVertical(false);
    } else {
        slider.setPositionType(PositionType::STATIC_TO_PARENT);
        slider.setRawSize(f32v2(0.0f));
        slider.setSlideDimensions(f32v2(0.0f));
        slider.disable();
    }

    // Set up vertical slider.
    slider = m_sliders.vertical;
    if (needsVertical) {
        slider.enable();

        slider.setPositionType(PositionType::RELATIVE_TO_PARENT);
        if (m_flipVertical) {
            m_position.y += m_sliderWidth;
            m_padding.y  += m_sliderWidth;

            slider.setRawTop(-1.0f * m_sliderWidth);
            slider.setRawBottom({ 1.0f, { DimensionType::PARENT_HEIGHT_PERCENTAGE } });
        } else {
            m_size.y    -= m_sliderWidth;
            m_padding.w += m_sliderWidth;

            slider.setRawTop({ 1.0f, { DimensionType::PARENT_HEIGHT_PERCENTAGE } });
            slider.setRawBottom(-1.0f * m_sliderWidth);
        }
        slider.setRawLeft(0.0f);
        slider.setRawRight(0.0f);

        slider.setSlideDimensions(f32v2(m_sliderWidth));
        slider.setRange(0, SLIDER_VAL_MAX);
        slider.setIsVertical(false);
    } else {
        slider.setPositionType(PositionType::STATIC_TO_PARENT);
        slider.setRawSize(f32v2(0.0f));
        slider.setSlideDimensions(f32v2(0.0f));
        slider.disable();
    }
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
        if ((Slider*)s == &m_sliders.horizontal) {
            // Horizontal
            f32 range = m_maxX - m_minX - m_size.x + m_sliderWidth;
            if (m_sliders.vertical.isEnabled()) range += m_sliderWidth;
            m_childOffset.x = m_minX + range * r;
        } else {
            // Vertical
            f32 range = m_maxY - m_minY - m_size.y + m_sliderWidth;
            if (m_sliders.horizontal.isEnabled()) range += m_sliderWidth;
            m_childOffset.y = m_minY + range * r;
        }
    }
    m_flags.needsDimensionUpdate = true;
}