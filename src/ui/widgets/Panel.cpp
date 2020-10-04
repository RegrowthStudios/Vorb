#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/Panel.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/widgets/Viewport.h"
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
    m_backHoverColor(color::Transparent),
    m_texture(0),
    m_hoverTexture(0) {
    m_flags.needsDrawableRecalculation = true;
}

vui::Panel::~Panel() {
    // Empty
}

void vui::Panel::initBase() {
    m_sliders.horizontal.init(this, getName() + "_horizontal_slider");
    m_sliders.vertical.init(this, getName() + "_vertical_slider");

    m_sliders.horizontal.setIgnoreOffset(true);
    m_sliders.vertical.setIgnoreOffset(true);

    m_sliders.horizontal.setZIndex(IWidget::Z_INDEX_MAX);
    m_sliders.vertical.setZIndex(IWidget::Z_INDEX_MAX);
}

void vui::Panel::enable() {
    if (!m_flags.isEnabled) {
        m_sliders.horizontal.ValueChange += makeDelegate(this, &Panel::onSliderValueChange);
        m_sliders.vertical.ValueChange   += makeDelegate(this, &Panel::onSliderValueChange);
    }

    IWidget::enable();

    m_sliders.horizontal.disable();
    m_sliders.vertical.disable();
}

void vui::Panel::disable(bool thisOnly /*= false*/) {
    if (m_flags.isEnabled) {
        m_sliders.horizontal.ValueChange -= makeDelegate(this, &Panel::onSliderValueChange);
        m_sliders.vertical.ValueChange   -= makeDelegate(this, &Panel::onSliderValueChange);
    }

    IWidget::disable(thisOnly);
}

void vui::Panel::addDrawables(UIRenderer& renderer) {
    // Add the panel rect.
    renderer.add(makeDelegate(&m_drawableRect, &DrawableRect::draw));
}

void vui::Panel::setTexture(VGTexture texture) {
    m_texture = texture;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Panel::setHoverTexture(VGTexture texture) {
    m_hoverTexture = texture;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Panel::setAutoScroll(bool autoScroll) {
    if (autoScroll != m_autoScroll) {
        m_autoScroll = autoScroll;

        m_flags.needsDrawableRecalculation = true;
    }
}

void vui::Panel::setSliderWidth(f32 width) {
    if (width != m_sliderWidth) {
        m_sliderWidth = width;

        m_flags.needsDrawableRecalculation = true;
    }
}

void vui::Panel::setColor(const color4& color) {
    m_backColor = color;

    updateColor();
}

void vui::Panel::setHoverColor(const color4& color) {
    m_backHoverColor = color;

    updateColor();
}

void vui::Panel::updateDimensions(f32 dt) {
    Widget::updateDimensions(dt);

    if (m_autoScroll) {
        if (m_sliders.horizontal.isEnabled()) {
            // r == 0: offset_l = -(m_minX)
            // r == 1: offset_r = -(m_maxX - m_position.x - m_size.x)
            // We want a formula that combines these offsets:
            //    offset = offset_l + r(offset_r - offset_l)

            f32 r = m_sliders.horizontal.getValueScaled();

            f32 offset = -m_minX + r * ( m_minX - m_maxX + m_size.x );
            setChildOffsetX(offset);
        }

        if (m_sliders.vertical.isEnabled()) {
            // r == 0: offset_l = -(m_minY)
            // r == 1: offset_r = -(m_maxY - m_position.y - m_size.y)
            // We want a formula that combines these offsets:
            //    offset = offset_l + r(offset_r - offset_l)

            f32 r = m_sliders.vertical.getValueScaled();

            f32 offset = -m_minY + r * ( m_minY - m_maxY + m_size.y );
            setChildOffsetY(offset);
        }
    }

    // We might want updateSliders here. This won't work immediately for docked panels as the update model we use means docked & undocked widgets aren't treated equally.
    // For docked widgets at initialisation its likely only the parents needsDockRecalculation flag will be set but not this panel's needsDimensionUpdate.
    // There is a further bug that makes rendering the slide of the slider bar strange - it appears to change length when moved about.
}

void vui::Panel::calculateDrawables() {
    updateSliders();

    m_drawableRect.setPosition(getPaddedPosition());
    m_drawableRect.setSize(getPaddedSize());
    m_drawableRect.setClipRect(m_clipRect);
    m_drawableRect.setTexture(m_flags.isMouseIn ? m_hoverTexture : m_texture);

    updateColor();
}

void vui::Panel::updateColor() {
    if (m_flags.isMouseIn) {
        m_drawableRect.setColor(m_backHoverColor);
    } else {
        m_drawableRect.setColor(m_backColor);
    }
}

// TODO(Matthew): We ideally want to be putting sliders outside of panel area, so as to not have panel contents overlapping them (for things like hover).
//                  Alternatively, provide an isInSliderBounds function (/isMouseOverSlider function) in Panel for other children to check against.
void vui::Panel::updateSliders() {
    // We need to figure which of the two sliders are needed.
    bool needsHorizontal = false;
    bool needsVertical   = false;

    // Reset min and max coord points.
    m_maxX = m_maxY = -FLT_MAX;
    m_minX = m_minY =  FLT_MAX;

    // If we're automatically adding scrollbars, then we want to know the extreme points of the child widgets.
    if (m_autoScroll) {
        // Determine min and max coords of child widgets.
        for (auto& child : m_widgets) {
            // Skip sliders.
            if (child == &m_sliders.horizontal ||
                child == &m_sliders.vertical) continue;

            // Skip disabled widgets.
            if (!child->isEnabled()) continue;

            // We need to update the child's dimensions now, as we might otherwise get screwy scroll bars as the child isn't up-to-date with parent changes.
            {
                WidgetFlags oldFlags = child->getFlags();
                child->setFlags({
                    oldFlags.isClicking,
                    oldFlags.isEnabled,
                    oldFlags.isMouseIn,
                    oldFlags.ignoreOffset,
                    true,  // needsDimensionUpdate
                    false, // needsZIndexReorder
                    false, // needsDockRecalculation
                    false, // needsClipRectRecalculation
                    false  // needsDrawableRecalculation
                });

                child->update(0.0f);

                WidgetFlags newFlags = child->getFlags();
                child->setFlags({
                    newFlags.isClicking,
                    newFlags.isEnabled,
                    newFlags.isMouseIn,
                    newFlags.ignoreOffset,
                    false, // needsDimensionUpdate
                    oldFlags.needsZIndexReorder         || newFlags.needsZIndexReorder,
                    oldFlags.needsDockRecalculation     || newFlags.needsDockRecalculation,
                    oldFlags.needsClipRectRecalculation || newFlags.needsClipRectRecalculation,
                    oldFlags.needsDrawableRecalculation || newFlags.needsDrawableRecalculation
                });
            }

            const f32v2& position = child->getRelativePosition();
            const f32v2& size     = child->getSize();

            f32v2 correctedPos = position - m_childOffset;

            if (correctedPos.x < m_minX) {
                m_minX = correctedPos.x;
            }
            if (correctedPos.y < m_minY) {
                m_minY = correctedPos.y;
            }
            if (correctedPos.x + size.x > m_maxX) {
                m_maxX = correctedPos.x + size.x;
            }
            if (correctedPos.y + size.y > m_maxY) {
                m_maxY = correctedPos.y + size.y;
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

    // Set up horizontal slider.
    if (needsHorizontal) {
        if (!m_sliders.horizontal.isEnabled()) {
            m_sliders.horizontal.enable();

            m_sliders.horizontal.setPositionType(PositionType::RELATIVE_TO_PARENT);

            m_sliders.horizontal.setLeft(0.0f);
            m_sliders.horizontal.setRight(0.0f);

            m_sliders.horizontal.setRange(0, SLIDER_VAL_MAX);
            m_sliders.horizontal.setIsVertical(false);
        }

        if (m_flipHorizontal) {
            m_sliders.horizontal.setTop(0.0f);
            m_sliders.horizontal.setRawBottom({ m_size.y - m_sliderWidth, { DimensionType::PIXEL } });
        } else {
            m_sliders.horizontal.setRawTop({ m_size.y - m_sliderWidth, { DimensionType::PIXEL } });
            m_sliders.horizontal.setBottom(0.0f);
        }

        m_sliders.horizontal.setSlideSize(f32v2(m_size.x * m_size.x / (m_maxX - m_minX), m_sliderWidth));

        m_sliders.horizontal.setNeedsDimensionUpdate(true);
    } else {
        m_sliders.horizontal.disable();
    }

    // Set up vertical slider.
    if (needsVertical) {
        if (!m_sliders.vertical.isEnabled()) {
            m_sliders.vertical.enable();

            m_sliders.vertical.setPositionType(PositionType::RELATIVE_TO_PARENT);

            m_sliders.vertical.setTop(0.0f);
            m_sliders.vertical.setBottom(0.0f);

            m_sliders.vertical.setRange(0, SLIDER_VAL_MAX);
            m_sliders.vertical.setIsVertical(true);
        }

        if (m_flipVertical) {
            m_sliders.vertical.setLeft(0.0f);
            m_sliders.vertical.setRawRight({ m_size.x - m_sliderWidth, { DimensionType::PIXEL } });
        } else {
            m_sliders.vertical.setRawLeft({ m_size.x - m_sliderWidth, { DimensionType::PIXEL } });
            m_sliders.vertical.setRight(0.0f);
        }

        m_sliders.vertical.setSlideSize(f32v2(m_sliderWidth, m_size.y * m_size.y / (m_maxY - m_minY)));

        m_sliders.vertical.setNeedsDimensionUpdate(true);
    } else {
        m_sliders.vertical.disable();
    }
}

void vui::Panel::onMouseMove(Sender, const MouseMotionEvent& e) {
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

void vui::Panel::onMouseFocusLost(Sender, const MouseEvent& e) {
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

void vui::Panel::onSliderValueChange(Sender, i32) {
    m_flags.needsDimensionUpdate = true;
}