#include "Vorb/stdafx.h"
#include "Vorb/ui/CheckBox.h"
#include "Vorb/ui/MouseInputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"

vui::CheckBox::CheckBox() : Widget() {
    ValueChange.setSender(this);
    updateColor();
    setTextAlign(vg::TextAlign::LEFT);
}

// vui::CheckBox::CheckBox(const nString& name, VORB_UNUSED const f32v4& destRect /*= f32v4(0)*/) : CheckBox() {
//     m_name = name;
//     // setDestRect(destRect);
//     m_drawableRect.setPosition(getPosition());
//     m_drawableRect.setDimensions(getSize());
//     updateTextPosition();
// }

// vui::CheckBox::CheckBox(IWidget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : CheckBox(name, destRect) {
//     parent->addWidget(this);
//     m_parent = parent;
// }

vui::CheckBox::~CheckBox() {
    // Empty
}

void vui::CheckBox::addDrawables() {
    Widget::addDrawables();
    // Make copies
    m_drawnText = m_drawableText;
    m_drawnRect = m_drawableRect;
    // Use renderer default font if we dont have a font
    m_defaultFont = m_renderer->getDefaultFont();
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the rect
    m_renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &CheckBox::refreshDrawables));


    // Add the text 
    m_renderer->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &CheckBox::refreshDrawables));
}

// void vui::CheckBox::updatePosition() {
//     Widget::updatePosition();

//     updateTextPosition();
//     m_drawableRect.setPosition(getPosition());
//     m_drawableRect.setDimensions(getSize());
//     m_drawableRect.setClipRect(m_clipRect);
//     m_drawableText.setClipRect(m_clipRect);
// }

void vui::CheckBox::check() {
    if (!m_isChecked) {
        m_isChecked = true;
        ValueChange(m_isChecked);
        refreshDrawables();
    }
}

void vui::CheckBox::unCheck() {
    if (m_isChecked) {
        m_isChecked = false;
        ValueChange(m_isChecked);
        refreshDrawables();
    }
}

void vui::CheckBox::toggleChecked() {
    m_isChecked = !m_isChecked;
    ValueChange(m_isChecked);
    refreshDrawables();
}

// void vui::CheckBox::setDimensions(const f32v2& dimensions) {
//     Widget::setDimensions(dimensions);
//     m_drawableRect.setDimensions(dimensions);
//     updateTextPosition();
// }

void vui::CheckBox::setFont(const vg::SpriteFont* font) {
    m_drawableText.setFont(font);
    refreshDrawables();
}

// void vui::CheckBox::setHeight(f32 height) {
//     Widget::setHeight(height);
//     m_drawableRect.setHeight(height);
//     updateTextPosition();
// }

// void vui::CheckBox::setPosition(const f32v2& position) {
//     Widget::setPosition(position);
//     m_drawableRect.setPosition(position);
//     updateTextPosition();
// }

void vui::CheckBox::setCheckedTexture(VGTexture texture) {
    m_checkedTexture = texture;
    refreshDrawables();
}

void vui::CheckBox::setUncheckedTexture(VGTexture texture) {
    m_uncheckedTexture = texture;
    refreshDrawables();
}

// void vui::CheckBox::setWidth(f32 width) {
//     Widget::setWidth(width);
//     m_drawableRect.setWidth(width);
//     updateTextPosition();
// }

// void vui::CheckBox::setX(f32 x) {
//     Widget::setX(x);
//     m_drawableRect.setX(x);
//     updateTextPosition();
// }

// void vui::CheckBox::setY(f32 y) {
//     Widget::setY(y);
//     m_drawableRect.setX(y);
//     updateTextPosition();
// }

void vui::CheckBox::setBoxColor(const color4& color) {
    m_boxColor = color;
    updateColor();
}

void vui::CheckBox::setBoxHoverColor(const color4& color) {
    m_boxHoverColor = color;
    updateColor();
}

void vui::CheckBox::setBoxCheckedColor(const color4& color) {
    m_boxCheckedColor = color;
    updateColor();
}

void vui::CheckBox::setBoxCheckedHoverColor(const color4& color) {
    m_boxCheckedHoverColor = color;
    updateColor();
}

void vui::CheckBox::setText(const nString& text) {
    m_drawableText.setText(text);
    refreshDrawables();
}

void vui::CheckBox::setTextColor(const color4& color) {
    m_drawableText.setColor(color);
    updateColor();
}

void vui::CheckBox::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);
    refreshDrawables();
}

void vui::CheckBox::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);
    refreshDrawables();
}

void vui::CheckBox::setChecked(bool checked) {
    if (m_isChecked != checked) {
        m_isChecked = checked;
        ValueChange(m_isChecked);
        updateColor();
    }
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
    refreshDrawables();
}

void vui::CheckBox::updateTextPosition() {
    const f32v2& dims = getSize();
    const f32v2& pos = getPosition();
    const vg::TextAlign& textAlign = getTextAlign();

    // TODO(Ben): Padding
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

    refreshDrawables();
}

void vui::CheckBox::refreshDrawables() {
    // Use renderer default font if we don't have a font
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);
        m_drawnText = m_drawableText;
        m_drawableText.setFont(nullptr);
    } else {
        m_drawnText = m_drawableText;
    }

    m_drawableRect.setTexture(m_isChecked ? m_checkedTexture : m_uncheckedTexture);
    m_drawnRect = m_drawableRect;
}


// void vui::CheckBox::computeClipRect(const f32v4& parentClipRect /*= f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX)*/) {
//     m_clipRect = parentClipRect;
//     computeChildClipRects();
// }

void vui::CheckBox::onMouseUp(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
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

void vui::CheckBox::onMouseMove(Sender s VORB_UNUSED, const MouseMotionEvent& e) {
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
