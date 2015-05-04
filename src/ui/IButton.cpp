#include "stdafx.h"
#include "UI/IButton.h"
#include "UI/UIRenderer.h"
#include "graphics/SpriteFont.h"

vui::IButton::IButton() : Widget(){
   // Empty
}

vui::IButton::IButton(const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : IButton() {
    m_name = name;
    m_destRect = destRect;
    m_drawableText.setPosition(getPosition());
    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
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

    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawnRect, &DrawableRect::draw),
                  makeDelegate(*this, &IButton::refreshDrawable));

    // Use renderer default font if we dont have a font
    if (!m_drawnText.getFont()) m_drawnText.setFont(renderer->getDefaultFont());
    // Add the text 
    renderer->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &IButton::refreshDrawable));  
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

void vui::IButton::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);
    updateTextPosition();
}

void vui::IButton::updateTextPosition() {
    const f32v2& dims = getDimensions();
    const f32v2& pos = getPosition();
    const vg::TextAlign& textAlign = getTextAlign();

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
}

void vui::IButton::refreshDrawable() {
    // TODO(Ben): Implement
}