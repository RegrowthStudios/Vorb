#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/CheckBox.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/widgets/Viewport.h"

vui::CheckBox::CheckBox() :
    TextWidget(),
    m_boxColor(color::DarkGray),
    m_boxHoverColor(color::AliceBlue),
    m_boxCheckedColor(color::LightGray),
    m_boxCheckedHoverColor(color::AliceBlue),
    m_checkedTexture(0),
    m_uncheckedTexture(0),
    m_checkedHoverTexture(0),
    m_uncheckedHoverTexture(0),
    m_isChecked(false) {
    m_flags.needsDrawableRecalculation = true;
}

vui::CheckBox::CheckBox(const CheckBox& widget) :
    TextWidget(widget),
    m_drawableRect(widget.m_drawableRect),
    m_boxColor(widget.m_boxColor),
    m_boxHoverColor(widget.m_boxHoverColor),
    m_boxCheckedColor(widget.m_boxCheckedColor),
    m_boxCheckedHoverColor(widget.m_boxCheckedHoverColor),
    m_checkedTexture(widget.m_checkedTexture),
    m_uncheckedTexture(widget.m_uncheckedTexture),
    m_checkedHoverTexture(widget.m_checkedHoverTexture),
    m_uncheckedHoverTexture(widget.m_uncheckedHoverTexture),
    m_isChecked(widget.m_isChecked) {
    ValueChange.setSender(this);
}

vui::CheckBox::CheckBox(CheckBox&& widget) :
    TextWidget(std::forward<CheckBox>(widget)),
    ValueChange(std::move(widget.ValueChange)),
    m_drawableRect(widget.m_drawableRect),
    m_boxColor(widget.m_boxColor),
    m_boxHoverColor(widget.m_boxHoverColor),
    m_boxCheckedColor(widget.m_boxCheckedColor),
    m_boxCheckedHoverColor(widget.m_boxCheckedHoverColor),
    m_checkedTexture(widget.m_checkedTexture),
    m_uncheckedTexture(widget.m_uncheckedTexture),
    m_checkedHoverTexture(widget.m_checkedHoverTexture),
    m_uncheckedHoverTexture(widget.m_uncheckedHoverTexture),
    m_isChecked(widget.m_isChecked) {
    ValueChange.setSender(this);
}

vui::CheckBox& vui::CheckBox::operator=(const CheckBox& rhs) {
    TextWidget::operator=(rhs);

    m_drawableRect = rhs.m_drawableRect;
    m_boxColor = rhs.m_boxColor;
    m_boxHoverColor = rhs.m_boxHoverColor;
    m_boxCheckedColor = rhs.m_boxCheckedColor;
    m_boxCheckedHoverColor = rhs.m_boxCheckedHoverColor;
    m_checkedTexture = rhs.m_checkedTexture;
    m_uncheckedTexture = rhs.m_uncheckedTexture;
    m_checkedHoverTexture = rhs.m_checkedHoverTexture;
    m_uncheckedHoverTexture = rhs.m_uncheckedHoverTexture;
    m_isChecked = rhs.m_isChecked;

    ValueChange.setSender(this);

    return *this;
}

vui::CheckBox& vui::CheckBox::operator=(CheckBox&& rhs) {
    TextWidget::operator=(std::forward<CheckBox>(rhs));

    ValueChange = std::move(rhs.ValueChange);
    m_drawableRect = rhs.m_drawableRect;
    m_boxColor = rhs.m_boxColor;
    m_boxHoverColor = rhs.m_boxHoverColor;
    m_boxCheckedColor = rhs.m_boxCheckedColor;
    m_boxCheckedHoverColor = rhs.m_boxCheckedHoverColor;
    m_checkedTexture = rhs.m_checkedTexture;
    m_uncheckedTexture = rhs.m_uncheckedTexture;
    m_checkedHoverTexture = rhs.m_checkedHoverTexture;
    m_uncheckedHoverTexture = rhs.m_uncheckedHoverTexture;
    m_isChecked = rhs.m_isChecked;

    ValueChange.setSender(this);

    return *this;
}

void vui::CheckBox::initBase() {
    TextWidget::initBase();

    m_isModifiedMap["boxHoverColor"]         = false;
    m_isModifiedMap["boxCheckedHoverColor"]  = false;
    m_isModifiedMap["checkedHoverTexture"]   = false;
    m_isModifiedMap["uncheckedHoverTexture"] = false;

    ValueChange.setSender(this);
}

void vui::CheckBox::addDrawables(UIRenderer& renderer) {
    // Add the checkbox rect.
    renderer.add(makeDelegate(&m_drawableRect, &DrawableRect::draw));

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

    if (!m_isModifiedMap["checkedHoverTexture"]) setCheckedHoverTexture(texture);

    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setUncheckedTexture(VGTexture texture) {
    m_uncheckedTexture = texture;

    if (!m_isModifiedMap["uncheckedHoverTexture"]) setUncheckedHoverTexture(texture);

    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setCheckedHoverTexture(VGTexture texture) {
    m_checkedHoverTexture = texture;

    m_isModifiedMap["checkedHoverTexture"] = true;

    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setUncheckedHoverTexture(VGTexture texture) {
    m_uncheckedHoverTexture = texture;

    m_isModifiedMap["uncheckedHoverTexture"] = true;

    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setBoxColor(const color4& color) {
    m_boxColor = color;

    if (!m_isModifiedMap["boxHoverColor"]) setBoxHoverColor(color);

    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setBoxHoverColor(const color4& color) {
    m_boxHoverColor = color;

    m_isModifiedMap["boxHoverColor"] = true;

    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setBoxCheckedColor(const color4& color) {
    m_boxCheckedColor = color;

    if (!m_isModifiedMap["boxCheckedHoverColor"]) setBoxCheckedHoverColor(color);

    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setBoxCheckedHoverColor(const color4& color) {
    m_boxCheckedHoverColor = color;

    m_isModifiedMap["boxCheckedHoverColor"] = true;

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
    } else {
        if (m_isChecked) {
            m_drawableRect.setColor(m_boxCheckedColor);
        } else {
            m_drawableRect.setColor(m_boxColor);
        }
    }
}

void vui::CheckBox::onMouseUp(Sender, const MouseButtonEvent& e) {
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

void vui::CheckBox::onMouseMove(Sender, const MouseMotionEvent& e) {
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
