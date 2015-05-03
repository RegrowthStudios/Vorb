#include "stdafx.h"
#include "UI/IButton.h"
#include "UI/UIRenderer.h"

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
    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawableRect, &DrawableRect::draw),
                  makeDelegate(*this, &IButton::refreshDrawable));
    // Add the text
    if (!m_drawableText.getFont()) {
        // Use renderer default font if we dont have a font
        m_drawableText.setFont(renderer->getDefaultFont());
        renderer->add(this,
                      makeDelegate(m_drawableText, &DrawableText::draw),
                      makeDelegate(*this, &IButton::refreshDrawable));
        m_drawableText.setFont(nullptr);
    } else {
        renderer->add(this,
                      makeDelegate(m_drawableText, &DrawableText::draw),
                      makeDelegate(*this, &IButton::refreshDrawable));
    }
}

void vui::IButton::removeDrawables(UIRenderer* renderer) {
    renderer->remove(this);
}

void vui::IButton::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);
}

void vui::IButton::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableRect.setHeight(height);
}

void vui::IButton::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableRect.setPosition(position);
    m_drawableText.setPosition(position);
}

void vui::IButton::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableRect.setWidth(width);
}

void vui::IButton::setX(f32 x) {
    Widget::setX(x);
    m_drawableRect.setX(x);
    m_drawableText.setX(x);
}

void vui::IButton::setY(f32 y) {
    Widget::setY(y);
    m_drawableRect.setX(y);
    m_drawableText.setX(y);
}

void vui::IButton::refreshDrawable() {
    // TODO(Ben): Implement
}
