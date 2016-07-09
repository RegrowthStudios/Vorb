#include "stdafx.h"
#include "ui/CheckBox.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"

vui::CheckBox::CheckBox() : Widget() {
    ValueChange.setSender(this);
    updateColor();
    setTextAlign(vg::TextAlign::LEFT);
}

vui::CheckBox::CheckBox(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : CheckBox() {
    m_name = name;
    setDestRect(destRect);
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    updateTextPosition();
}

vui::CheckBox::CheckBox(Widget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : CheckBox(name, destRect) {
    parent->addWidget(this);
}

vui::CheckBox::CheckBox(Form* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : CheckBox(name, destRect) {
    parent->addWidget(this);
}

vui::CheckBox::~CheckBox() {
    // Empty
}

void vui::CheckBox::addDrawables(UIRenderer* renderer) {
    Widget::addDrawables(renderer);
    // Make copies
    m_drawnText = m_drawableText;
    m_drawnRect = m_drawableRect;
    // Use renderer default font if we dont have a font
    m_defaultFont = renderer->getDefaultFont();
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &CheckBox::refreshDrawables));


    // Add the text 
    renderer->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &CheckBox::refreshDrawables));
}

void vui::CheckBox::updatePosition() {
    Widget::updatePosition();

    updateTextPosition();
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    m_drawableRect.setClipRect(m_clipRect);
    m_drawableText.setClipRect(m_clipRect);
}

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

void vui::CheckBox::setDimensions(const f32v2& dimensions, bool update /*= true*/) {
    Widget::setDimensions(dimensions, update);
    m_drawableRect.setDimensions(dimensions);
    updateTextPosition();
}

void vui::CheckBox::setFont(const vg::SpriteFont* font) {
    m_drawableText.setFont(font);
    refreshDrawables();
}

void vui::CheckBox::setHeight(f32 height, bool update /*= true*/) {
    Widget::setHeight(height, update);
    m_drawableRect.setHeight(height);
    updateTextPosition();
}

void vui::CheckBox::setPosition(const f32v2& position, bool update /*= true*/) {
    Widget::setPosition(position, update);
    m_drawableRect.setPosition(position);
    updateTextPosition();
}

void vui::CheckBox::setCheckedTexture(VGTexture texture) {
    m_checkedTexture = texture;
    refreshDrawables();
}

void vui::CheckBox::setUncheckedTexture(VGTexture texture) {
    m_uncheckedTexture = texture;
    refreshDrawables();
}

void vui::CheckBox::setWidth(f32 width, bool update /*= true*/) {
    Widget::setWidth(width, update);
    m_drawableRect.setWidth(width);
    updateTextPosition();
}

void vui::CheckBox::setX(f32 x, bool update /*= true*/) {
    Widget::setX(x, update);
    m_drawableRect.setX(x);
    updateTextPosition();
}

void vui::CheckBox::setY(f32 y, bool update /*= true*/) {
    Widget::setY(y, update);
    m_drawableRect.setX(y);
    updateTextPosition();
}

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
        if (m_isMouseIn) {
            m_drawableRect.setColor(m_boxCheckedHoverColor);
        } else {
            m_drawableRect.setColor(m_boxCheckedColor);
        }
    } else {
        if (m_isMouseIn) {
            m_drawableRect.setColor(m_boxHoverColor);
        } else {
            m_drawableRect.setColor(m_boxColor);
        }
    }
    refreshDrawables();
}

void vui::CheckBox::updateTextPosition() {
    const f32v2& dims = getDimensions();
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


void vui::CheckBox::computeClipRect() {
    f32v4 parentClipRect = f32v4(-FLT_MAX / 2.0f, -FLT_MAX / 2.0f, FLT_MAX, FLT_MAX);
    if (m_parentWidget) {
        parentClipRect = m_parentWidget->getClipRect();
    } else if (m_parentForm) {
        parentClipRect = m_parentForm->getClipRect();
    }

    m_clipRect = parentClipRect;
}

void vui::CheckBox::onMouseUp(Sender s, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseUp(e);
        if (m_isClicking) {
            MouseClick(e);
            toggleChecked();
        }
    }
    m_isClicking = false;
    updateColor();
}

void vui::CheckBox::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (!m_isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
        if (!m_isMouseIn) {
            m_isMouseIn = true;
            MouseEnter(e);
            updateColor();
        }
        MouseMove(e);
    } else {
        if (m_isMouseIn) {
            m_isMouseIn = false;
            MouseLeave(e);
            updateColor();
        }
    }
}
