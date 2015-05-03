#include "stdafx.h"
#include "UI/IButton.h"
#include "UI/UIRenderer.h"

vorb::ui::IButton::~IButton() {
    // Empty
}

void vorb::ui::IButton::addDrawables(UIRenderer* renderer) {
    // Add the rect
    renderer->add(this,
                  makeDelegate(m_drawableRect, &DrawableRect::draw),
                  makeDelegate(*this, &IButton::refreshDrawable));
    // Add the text
    renderer->add(this,
                  makeDelegate(m_drawableText, &DrawableText::draw),
                  makeDelegate(*this, &IButton::refreshDrawable));
}

void vorb::ui::IButton::removeDrawables(UIRenderer* renderer) {
    renderer->remove(this);
}

void vorb::ui::IButton::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);
}

void vorb::ui::IButton::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableRect.setHeight(height);
}

void vorb::ui::IButton::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableRect.setPosition(position);
    m_drawableText.setPosition(position);
}

void vorb::ui::IButton::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableRect.setWidth(width);
}

void vorb::ui::IButton::setX(f32 x) {
    Widget::setX(x);
    m_drawableRect.setX(x);
    m_drawableText.setX(x);
}

void vorb::ui::IButton::setY(f32 y) {
    Widget::setY(y);
    m_drawableRect.setX(y);
    m_drawableText.setX(y);
}

void vorb::ui::IButton::refreshDrawable() {
    // TODO(Ben): Implement
}
