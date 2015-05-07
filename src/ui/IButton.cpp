#include "stdafx.h"
#include "ui/IButton.h"
#include "ui/MouseInputDispatcher.h"
#include "ui/UIRenderer.h"

vui::IButton::IButton() : Widget() {
    updateColor();
}

vui::IButton::IButton(const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : IButton() {
    m_name = name;
    m_destRect = destRect;
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    updateTextPosition();
}

vui::IButton::IButton(Widget* parent, const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : IButton(name, destRect) {
    parent->addChild(this);
    m_parent = parent;
}

vui::IButton::~IButton() {
    // Empty
}

void vui::IButton::addDrawables(UIRenderer* renderer) {
    // Make copies
    m_drawnText = m_drawableText;
    m_drawnRect = m_drawableRect;
    // Use renderer default font if we dont have a font
    m_defaultFont = renderer->getDefaultFont();
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &IButton::refreshDrawables));

    
    // Add the text 
    renderer->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &IButton::refreshDrawables));  
}

void vui::IButton::removeDrawables(UIRenderer* renderer) {
    renderer->remove(this);
}

void vui::IButton::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);
    updateTextPosition();
}

void vui::IButton::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableRect.setHeight(height);
    updateTextPosition();
}

void vui::IButton::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableRect.setPosition(position);
    updateTextPosition();
}

void vui::IButton::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableRect.setWidth(width);
    updateTextPosition();
}

void vui::IButton::setX(f32 x) {
    Widget::setX(x);
    m_drawableRect.setX(x);
    updateTextPosition();
}

void vui::IButton::setY(f32 y) {
    Widget::setY(y);
    m_drawableRect.setX(y);
    updateTextPosition();
}

void vui::IButton::setBackColor(const color4& color) {
    m_backColor = color;
    updateColor();
}

void vui::IButton::setBackHoverColor(const color4& color) {
    m_backHoverColor = color;
    updateColor();
}

void vui::IButton::setTextColor(const color4& color) {
    m_textColor = color;
    updateColor();
}

void vui::IButton::setTextHoverColor(const color4& color) {
    m_textHoverColor = color;
    updateColor();
}

void vui::IButton::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);
    refreshDrawables();
}

void vui::IButton::updateColor() {
    if (m_isMouseIn) {
        m_drawableRect.setColor(m_backHoverColor);
        m_drawableText.setColor(m_textHoverColor);
    } else {
        m_drawableRect.setColor(m_backColor);
        m_drawableText.setColor(m_textColor);
    }
    refreshDrawables();
}

void vui::IButton::updateTextPosition() {
    const f32v2& dims = getDimensions();
    const f32v2& pos = getPosition();
    const vg::TextAlign& textAlign = getTextAlign();
    m_drawableText.setClipRect(getDestRect());

    // TODO(Ben): Padding
    switch (textAlign) {
        case vg::TextAlign::LEFT:
            m_drawableText.setPosition(pos + f32v2(0.0f, dims.y / 2.0f));
            break;
        case vg::TextAlign::TOP_LEFT:
            m_drawableText.setPosition(pos);
            break;
        case vg::TextAlign::TOP:
            m_drawableText.setPosition(pos + f32v2(dims.x / 2.0f, 0.0f));
            break;
        case vg::TextAlign::TOP_RIGHT:
            m_drawableText.setPosition(pos + f32v2(dims.x, 0.0f));
            break;
        case vg::TextAlign::RIGHT:
            m_drawableText.setPosition(pos + f32v2(dims.x, dims.y / 2.0f));
            break;
        case vg::TextAlign::BOTTOM_RIGHT:
            m_drawableText.setPosition(pos + f32v2(dims.x, dims.y));
            break;
        case vg::TextAlign::BOTTOM:
            m_drawableText.setPosition(pos + f32v2(dims.x / 2.0f, dims.y));
            break;
        case vg::TextAlign::BOTTOM_LEFT:
            m_drawableText.setPosition(pos + f32v2(0.0f, dims.y));
            break;
        case vg::TextAlign::CENTER:
            m_drawableText.setPosition(pos + dims / 2.0f);
            break;
    }
    refreshDrawables();
}

void vui::IButton::refreshDrawables() {

    // Use renderer default font if we don't have a font
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);
        m_drawnText = m_drawableText;
        m_drawableText.setFont(nullptr);
    } else {
        m_drawnText = m_drawableText;
    }
    
    m_drawnRect = m_drawableRect;
}

void vui::IButton::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (isInBounds(e.x, e.y)) {  
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
