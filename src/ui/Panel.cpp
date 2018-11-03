#include "Vorb/stdafx.h"
#include "Vorb/ui/Panel.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/utils.h"

const int SLIDER_VAL_MAX = 10000;

vui::Panel::Panel() :
    Widget(),
    m_minX(FLT_MAX),
    m_minY(FLT_MAX),
    m_maxX(-FLT_MAX),
    m_maxY(-FLT_MAX),
    m_autoScroll(false),
    m_flipHorizontal(false),
    m_flipVertical(false),
    m_sliderWidth(15.0f),
    m_backColor(color::Transparent),
    m_backHoverColor(color::Transparent) {
    m_flags.needsDrawableRecalculation = true;
}

vui::Panel::~Panel() {
    // Empty
}

void vui::Panel::initBase() {
    m_sliders.horizontal.init(this, getName() + "_horizontal_slider");
    m_sliders.vertical.init(this, getName() + "_vertical_slider");
}

void vui::Panel::enable() {
    if (!m_flags.isEnabled) {
        m_sliders.horizontal.ValueChange += makeDelegate(*this, &Panel::onSliderValueChange);
        m_sliders.vertical.ValueChange   += makeDelegate(*this, &Panel::onSliderValueChange);
    }

    IWidget::enable();
}

void vui::Panel::disable() {
    if (m_flags.isEnabled) {
        m_sliders.horizontal.ValueChange -= makeDelegate(*this, &Panel::onSliderValueChange);
        m_sliders.vertical.ValueChange   -= makeDelegate(*this, &Panel::onSliderValueChange);
    }

    IWidget::disable();
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
    m_drawableRect.setPosition(getPaddedPosition());
    m_drawableRect.setSize(getPaddedSize());
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
        // Determine min and max coords of child widgets.
        for (auto& child : m_widgets) {
            // Skip sliders.
            if (child == &m_sliders.horizontal ||
                child == &m_sliders.vertical) continue;

            // Skip disabled widgets.
            if (!child->isEnabled()) continue;

            const f32v2& position = child->getRelativePosition();
            const f32v2& size     = child->getSize();

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

    // Note that while this looks like it might iteratively push the sliders further and further out, the processed values of position and padding are recalculated any time we also come here to update sliders - so as long as this never touches the raw values of position and padding we're good!

    // Set up horizontal slider.
    Slider& slider = m_sliders.horizontal;
    if (needsHorizontal) {
        slider.enable();

        slider.setPositionType(PositionType::RELATIVE_TO_PARENT);
        if (m_flipHorizontal) {
            m_position.x += m_sliderWidth;
            m_padding.x  += m_sliderWidth;

            slider.setLeft(-1.0f * m_sliderWidth);
            slider.setRawRight({ 1.0f, { DimensionType::PARENT_WIDTH_PERCENTAGE } });
        } else {
            m_size.x    -= m_sliderWidth;
            m_padding.z += m_sliderWidth;

            slider.setRawLeft({ 1.0f, { DimensionType::PARENT_WIDTH_PERCENTAGE } });
            slider.setRight(-1.0f * m_sliderWidth);
        }
        slider.setTop(0.0f);
        slider.setBottom(0.0f);

        slider.setSlideSize(f32v2(m_sliderWidth));
        slider.setRange(0, SLIDER_VAL_MAX);
        slider.setIsVertical(false);
    } else {
        slider.setPositionType(PositionType::STATIC_TO_PARENT);
        slider.setSize(f32v2(0.0f));
        slider.setSlideSize(f32v2(0.0f));
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

            slider.setTop(-1.0f * m_sliderWidth);
            slider.setRawBottom({ 1.0f, { DimensionType::PARENT_HEIGHT_PERCENTAGE } });
        } else {
            m_size.y    -= m_sliderWidth;
            m_padding.w += m_sliderWidth;

            slider.setRawTop({ 1.0f, { DimensionType::PARENT_HEIGHT_PERCENTAGE } });
            slider.setBottom(-1.0f * m_sliderWidth);
        }
        slider.setLeft(0.0f);
        slider.setRight(0.0f);

        slider.setSlideSize(f32v2(m_sliderWidth));
        slider.setRange(0, SLIDER_VAL_MAX);
        slider.setIsVertical(false);
    } else {
        slider.setPositionType(PositionType::STATIC_TO_PARENT);
        slider.setSize(f32v2(0.0f));
        slider.setSlideSize(f32v2(0.0f));
        slider.disable();
    }
}

void vui::Panel::onMouseMove(Sender, const MouseMotionEvent& e) {
    if (!m_flags.isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
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
}

void vui::Panel::onMouseFocusLost(Sender, const MouseEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        m_flags.isMouseIn = false;

        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);

        updateColor();

        m_flags.needsDrawableRefresh = true;
    }
}

void vui::Panel::onSliderValueChange(Sender s, int v) {
    if (m_autoScroll) {
        f32 r = (f32)v / SLIDER_VAL_MAX;
        if ((Slider*)s == &m_sliders.horizontal) {
            // Horizontal
            f32 range = m_maxX - m_minX - m_size.x + m_sliderWidth;
            if (m_sliders.vertical.isEnabled()) range += m_sliderWidth;
            setChildOffsetX(m_minX + range * r);
        } else {
            // Vertical
            f32 range = m_maxY - m_minY - m_size.y + m_sliderWidth;
            if (m_sliders.horizontal.isEnabled()) range += m_sliderWidth;
            setChildOffsetY(m_minY + range * r);
        }
    }
    m_flags.needsDimensionUpdate = true;
}