#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/Button.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/widgets/Viewport.h"

vui::Button::Button() :
    TextWidget(),
    m_gradBack(vg::GradientType::NONE),
    m_gradHover(vg::GradientType::NONE),
    m_backColor1(color::LightGray),
    m_backColor2(color::LightGray),
    m_backHoverColor1(color::AliceBlue),
    m_backHoverColor2(color::AliceBlue),
    m_texture(0),
    m_hoverTexture(0) {
    m_flags.needsDrawableRecalculation = true;
}

vui::Button::~Button() {
    // Empty
}

void vui::Button::addDrawables(UIRenderer& renderer) {
    // Add the button rect.
    renderer.add(makeDelegate(&m_drawableRect, &DrawableRect::draw));

    // Add the text <- after checkbox to be rendererd on top!
    TextWidget::addDrawables(renderer);
}

void vui::Button::setTexture(VGTexture texture) {
    m_texture = texture;

    if (!m_isModifiedMap["hoverTexture"]) setHoverTexture(texture);

    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setHoverTexture(VGTexture texture) {
    m_hoverTexture = texture;

    m_isModifiedMap["hoverTexture"] = true;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setBackColor(const color4& color) {
    m_backColor1 = m_backColor2 = color;

    if (!m_isModifiedMap["backHoverColor1"] && !m_isModifiedMap["backHoverColor2"]) setBackHoverColor(color);

    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setBackColorGrad(const color4& color1, const color4& color2, vg::GradientType grad) {
    m_backColor1 = color1;
    m_backColor2 = color2;
    m_gradBack = grad;

    if (!m_isModifiedMap["backHoverColor1"]
            && !m_isModifiedMap["backHoverColor2"]
            && !m_isModifiedMap["gradHover"]) setBackHoverColorGrad(color1, color2, grad);

    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setBackHoverColor(const color4& color) {
    m_backHoverColor1 = m_backHoverColor2 = color;

    m_isModifiedMap["backHoverColor1"] = true;
    m_isModifiedMap["backHoverColor2"] = true;

    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::setBackHoverColorGrad(const color4& color1, const color4& color2, vg::GradientType grad) {
    m_backHoverColor1 = color1;
    m_backHoverColor2 = color2;
    m_gradHover = grad;

    m_isModifiedMap["backHoverColor1"] = true;
    m_isModifiedMap["backHoverColor2"] = true;
    m_isModifiedMap["gradHover"]       = true;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::Button::initBase() {
    TextWidget::initBase();

    m_isModifiedMap["gradHover"]       = false;
    m_isModifiedMap["backHoverColor1"] = false;
    m_isModifiedMap["backHoverColor2"] = false;
    m_isModifiedMap["hoverTexture"]    = false;
}

void vui::Button::calculateDrawables() {
    m_drawableRect.setPosition(getPaddedPosition());
    m_drawableRect.setSize(getPaddedSize());
    m_drawableRect.setClipRect(m_clipRect);
    m_drawableRect.setTexture(m_flags.isMouseIn ? m_hoverTexture : m_texture);

    updateColor();

    TextWidget::calculateDrawables();
}

void vui::Button::updateColor() {
    if (m_flags.isMouseIn) {
        m_drawableRect.setColor1(m_backHoverColor1);
        m_drawableRect.setColor2(m_backHoverColor2);
        m_drawableRect.setGradientType(m_gradHover);
    } else {
        m_drawableRect.setColor1(m_backColor1);
        m_drawableRect.setColor2(m_backColor2);
        m_drawableRect.setGradientType(m_gradBack);
    }
}

void vui::Button::onMouseMove(Sender, const MouseMotionEvent& e) {
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

void vui::Button::onMouseFocusLost(Sender, const MouseEvent& e) {
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
