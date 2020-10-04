#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/Label.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/widgets/Viewport.h"

vui::Label::Label() :
    TextWidget(),
    m_labelColor(color::Transparent),
    m_labelHoverColor(color::Transparent),
    m_labelTexture(0),
    m_labelHoverTexture(0),
    m_textColor(color::DarkGray),
    m_textHoverColor(color::AliceBlue) {
    m_flags.needsDrawableRecalculation = true;
}

vui::Label::~Label() {
    // Empty
}

void vui::Label::addDrawables(UIRenderer& renderer) {
    // Add the label rect.
    renderer.add(makeDelegate(&m_drawableRect, &DrawableRect::draw));

    // Add the text <- after checkbox to be rendererd on top!
    TextWidget::addDrawables(renderer);
}

void vui::Label::setLabelColor(const color4& color) {
    m_labelColor = color;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::setLabelHoverColor(const color4& color) {
    m_labelHoverColor = color;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::setLabelTexture(VGTexture texture) {
    m_labelTexture = texture;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::setLabelHoverTexture(VGTexture texture) {
    m_labelHoverTexture = texture;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::calculateDrawables() {
    m_drawableRect.setPosition(getPaddedPosition());
    m_drawableRect.setSize(getPaddedSize());
    m_drawableRect.setClipRect(m_clipRect);
    m_drawableRect.setTexture(m_flags.isMouseIn ? m_labelHoverTexture : m_labelTexture);

    updateColor();

    TextWidget::calculateDrawables();
}

void vui::Label::updateColor() {
    if (m_flags.isMouseIn) {
        m_drawableRect.setColor(m_labelHoverColor);
    } else {
        m_drawableRect.setColor(m_labelColor);
    }
}

void vui::Label::onMouseMove(Sender, const MouseMotionEvent& e) {
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
