#include "Vorb/stdafx.h"
#include "Vorb/ui/CheckBox.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"

vui::CheckBox::CheckBox() :
    Widget(),
    m_boxColor(color::DarkGray),
    m_boxHoverColor(color::AliceBlue),
    m_boxCheckedColor(color::LightGray),
    m_boxCheckedHoverColor(color::AliceBlue),
    m_textColor(color::Black),
    m_textHoverColor(color::Black),
    m_defaultFont(nullptr),
    m_checkedTexture(0),
    m_uncheckedTexture(0),
    m_isChecked(false) {
    m_flags.needsDrawableRecalculation = true;
}

vui::CheckBox::~CheckBox() {
    // Empty
}

void vui::CheckBox::initBase() {
    ValueChange.setSender(this);
}

void vui::CheckBox::addDrawables() {
    // Make copies
    m_drawnText = m_drawableText;
    m_drawnRect = m_drawableRect;
    // Use renderer default font if we dont have a font
    m_defaultFont = m_viewport->getRenderer()->getDefaultFont();
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the rect
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &CheckBox::refreshDrawables));


    // Add the text 
    m_viewport->getRenderer()->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &CheckBox::refreshDrawables));
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

void vui::CheckBox::setFont(const vg::SpriteFont* font) {
    m_drawableText.setFont(font);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::CheckBox::setCheckedTexture(VGTexture texture) {
    m_checkedTexture = texture;
    
    m_flags.needsDrawableRefresh = true;
}

void vui::CheckBox::setUncheckedTexture(VGTexture texture) {
    m_uncheckedTexture = texture;
    
    m_flags.needsDrawableRefresh = true;
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

void vui::CheckBox::setText(const nString& text) {
    m_drawableText.setText(text);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::CheckBox::setTextColor(const color4& color) {
    m_drawableText.setColor(color);
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);
    
    m_flags.needsDrawableRecalculation = true;
}

void vui::CheckBox::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);
    
    m_flags.needsDrawableRefresh = true;
}

void vui::CheckBox::setChecked(bool checked) {
    if (m_isChecked != checked) {
        m_isChecked = checked;

        ValueChange(m_isChecked);
    
        m_flags.needsDrawableRecalculation = true;
    }
}

void vui::CheckBox::calculateDrawables() {
    m_drawableRect.setPosition(m_position);
    m_drawableRect.setSize(m_size);
    m_drawableRect.setClipRect(m_clipRect);
    m_drawableText.setClipRect(m_clipRect);
    m_drawableRect.setTexture(m_isChecked ? m_checkedTexture : m_uncheckedTexture);

    updateColor();

    updateTextPosition();

    m_flags.needsDrawableRefresh = true;
}

void vui::CheckBox::updateColor() {
    if (m_isChecked) {
        if (m_flags.isMouseIn) {
            m_drawableRect.setColor(m_boxCheckedHoverColor);
        } else {
            m_drawableRect.setColor(m_boxCheckedColor);
        }
    } else {
        if (m_flags.isMouseIn) {
            m_drawableRect.setColor(m_boxHoverColor);
        } else {
            m_drawableRect.setColor(m_boxColor);
        }
    }
}

void vui::CheckBox::updateTextPosition() {
    const f32v2& dims = getSize();
    const f32v2& pos  = getPosition();
    const vg::TextAlign& textAlign = getTextAlign();

    switch (textAlign) {
        case vg::TextAlign::LEFT:
            m_drawableText.setPosition(pos + f32v2(dims.x, dims.y / 2.0f));
            break;
        case vg::TextAlign::TOP_LEFT:
            m_drawableText.setPosition(pos + f32v2(dims.x, dims.y));
            break;
        case vg::TextAlign::TOP:
            m_drawableText.setPosition(pos + f32v2(dims.x / 2.0f, dims.y));
            break;
        case vg::TextAlign::TOP_RIGHT:
            m_drawableText.setPosition(pos + f32v2(0.0f, dims.y));
            break;
        case vg::TextAlign::RIGHT:
            m_drawableText.setPosition(pos + f32v2(0.0f, dims.y / 2.0f));
            break;
        case vg::TextAlign::BOTTOM_RIGHT:
            m_drawableText.setPosition(pos);
            break;
        case vg::TextAlign::BOTTOM:
            m_drawableText.setPosition(pos + f32v2(dims.x / 2.0f, 0.0f));
            break;
        case vg::TextAlign::BOTTOM_LEFT:
            m_drawableText.setPosition(pos + f32v2(dims.x, 0.0f));
            break;
        case vg::TextAlign::CENTER:
            m_drawableText.setPosition(pos + dims / 2.0f);
            break;
    }
}

void vui::CheckBox::refreshDrawables() {
    // Use renderer default font if we don't have a font.
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);

        m_drawnText = m_drawableText;

        m_drawableText.setFont(nullptr);
    } else {
        m_drawnText = m_drawableText;
    }

    m_drawnRect = m_drawableRect;
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
