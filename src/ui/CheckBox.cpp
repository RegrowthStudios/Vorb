#include "Vorb/stdafx.h"
#include "Vorb/ui/CheckBox.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"

vui::CheckBox::CheckBox() :
    TextWidget(),
    m_boxColor(color::DarkGray),
    m_boxHoverColor(color::AliceBlue),
    m_boxCheckedColor(color::LightGray),
    m_boxCheckedHoverColor(color::AliceBlue),
    m_textColor(color::Black),
    m_textHoverColor(color::Black),
    m_checkedTexture(0),
    m_uncheckedTexture(0),
    m_checkedHoverTexture(0),
    m_uncheckedHoverTexture(0),
    m_isChecked(false) {
    m_flags.needsDrawableRecalculation = true;
}

vui::CheckBox::~CheckBox() {
    // Empty
}

void vui::CheckBox::initBase() {
    ValueChange.setSender(this);
}

void vui::CheckBox::addDrawables(UIRenderer& renderer) {
    // Add the checkbox rect.
    renderer.add(makeDelegate(m_drawableRect, &DrawableRect::draw));

    // Add the text <- after checkbox to be rendererd on top!
    TextWidget::addDrawables(renderer);
}

void vui::CheckBox::check() {
    setChecked(true);
}

void vui::CheckBox::uncheck() {
    setChecked(false);
}

void vui::CheckBox::toggleChecked() {
    setChecked(!m_isChecked);
}

void vui::CheckBox::setCheckedTexture(VGTexture texture) {
    m_checkedTexture = texture;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setUncheckedTexture(VGTexture texture) {
    m_uncheckedTexture = texture;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setCheckedHoverTexture(VGTexture texture) {
    m_checkedHoverTexture = texture;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setUncheckedHoverTexture(VGTexture texture) {
    m_uncheckedHoverTexture = texture;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setBoxColor(const color4& color) {
    m_boxColor = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setBoxHoverColor(const color4& color) {
    m_boxHoverColor = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setBoxCheckedColor(const color4& color) {
    m_boxCheckedColor = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setBoxCheckedHoverColor(const color4& color) {
    m_boxCheckedHoverColor = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setTextColor(const color4& color) {
    m_textColor = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setTextHoverColor(const color4& color) {
    m_textHoverColor = color;
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setChecked(bool checked) {
    if (m_isChecked != checked) {
        m_isChecked = checked;

        ValueChange(m_isChecked);
    
        m_flags.needsDrawableRecalculation = true;
    }
}

void vui::CheckBox::calculateDrawables() {
    m_drawableRect.setPosition(getPaddedPosition());
    m_drawableRect.setSize(getPaddedSize());
    m_drawableRect.setClipRect(m_clipRect);

    if (m_flags.isMouseIn) {
        m_drawableRect.setTexture(m_isChecked ? m_checkedHoverTexture : m_uncheckedHoverTexture);
    } else {
        m_drawableRect.setTexture(m_isChecked ? m_checkedTexture : m_uncheckedTexture);
    }

    updateColor();

    TextWidget::calculateDrawables();
}

void vui::CheckBox::updateColor() {
    if (m_flags.isMouseIn) {
        if (m_isChecked) {
            m_drawableRect.setColor(m_boxCheckedHoverColor);
        } else {
            m_drawableRect.setColor(m_boxHoverColor);
        }
        m_drawableText.setColor(m_textHoverColor);
    } else {
        if (m_isChecked) {
            m_drawableRect.setColor(m_boxCheckedColor);
        } else {
            m_drawableRect.setColor(m_boxColor);
        }
        m_drawableText.setColor(m_textColor);
    }
}

void vui::CheckBox::onMouseUp(Sender s VORB_MAYBE_UNUSED, const MouseButtonEvent& e) {
    if (!m_flags.isEnabled) return;
    if (m_flags.isMouseIn) {
        MouseUp(e);
        if (m_flags.isClicking) {
            MouseClick(e);
            toggleChecked();
        }
    }
    m_flags.isClicking = false;
    updateColor();
}

void vui::CheckBox::onMouseMove(Sender s VORB_MAYBE_UNUSED, const MouseMotionEvent& e) {
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
