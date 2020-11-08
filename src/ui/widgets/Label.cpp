#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/Label.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/widgets/Viewport.h"

vui::Label::Label() :
    TextWidget(),
    m_labelColor(color::Transparent),
    m_labelHoverColor(color::Transparent),
    m_labelTexture(0),
    m_labelHoverTexture(0) {
    m_flags.needsDrawableRecalculation = true;
}

vui::Label::Label(const Label& widget) :
    TextWidget(widget),
    m_drawableRect(widget.m_drawableRect),
    m_labelColor(widget.m_labelColor),
    m_labelHoverColor(widget.m_labelHoverColor),
    m_labelTexture(widget.m_labelTexture),
    m_labelHoverTexture(widget.m_labelHoverTexture) {
    // Empty.
}

vui::Label::Label(Label&& widget) :
    TextWidget(std::forward<Label>(widget)),
    m_drawableRect(widget.m_drawableRect),
    m_labelColor(widget.m_labelColor),
    m_labelHoverColor(widget.m_labelHoverColor),
    m_labelTexture(widget.m_labelTexture),
    m_labelHoverTexture(widget.m_labelHoverTexture) {
    // Empty.
}

vui::Label& vui::Label::operator=(const Label& rhs) {
    TextWidget::operator=(rhs);

    m_drawableRect = rhs.m_drawableRect;
    m_labelColor = rhs.m_labelColor;
    m_labelHoverColor = rhs.m_labelHoverColor;
    m_labelTexture = rhs.m_labelTexture;
    m_labelHoverTexture = rhs.m_labelHoverTexture;

    return *this;
}

vui::Label& vui::Label::operator=(Label&& rhs) {
    TextWidget::operator=(std::forward<Label>(rhs));

    m_drawableRect = rhs.m_drawableRect;
    m_labelColor = rhs.m_labelColor;
    m_labelHoverColor = rhs.m_labelHoverColor;
    m_labelTexture = rhs.m_labelTexture;
    m_labelHoverTexture = rhs.m_labelHoverTexture;

    return *this;
}

void vui::Label::addDrawables(UIRenderer& renderer) {
    // Add the label rect.
    renderer.add(makeDelegate(&m_drawableRect, &DrawableRect::draw));

    // Add the text <- after checkbox to be rendererd on top!
    TextWidget::addDrawables(renderer);
}

void vui::Label::setLabelColor(const color4& color) {
    m_labelColor = color;

    if (!m_isModifiedMap["hoverColor"]) setLabelHoverColor(color);

    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::setLabelHoverColor(const color4& color) {
    m_labelHoverColor = color;

    m_isModifiedMap["hoverColor"] = true;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::setLabelTexture(VGTexture texture) {
    m_labelTexture = texture;

    if (!m_isModifiedMap["hoverTexture"]) setLabelHoverTexture(texture);

    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::setLabelHoverTexture(VGTexture texture) {
    m_labelHoverTexture = texture;

    m_isModifiedMap["hoverTexture"] = true;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Label::initBase() {
    TextWidget::initBase();

    m_isModifiedMap["hoverColor"]   = false;
    m_isModifiedMap["hoverTexture"] = false;
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
