#include "stdafx.h"
#include "ui/Label.h"
#include "ui/UIRenderer.h"
#include "utils.h"

vui::Label::Label() : Widget() {
    refreshDrawables();
}

vui::Label::Label(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Label() {
    m_name = name;
    setDestRect(destRect);
    updateTextPosition();
}

vui::Label::Label(Widget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Label(name, destRect) {
    parent->addWidget(this);
    m_parent = parent;
}

vui::Label::~Label() {
    // Empty
}

void vui::Label::addDrawables(UIRenderer* renderer) {
    Widget::addDrawables(renderer);
    // Make copies
    m_drawnText = m_drawableText;

    // Use renderer default font if we dont have a font
    m_defaultFont = renderer->getDefaultFont();
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the text 
    renderer->add(this,
                  makeDelegate(m_drawnText, &DrawableText::draw),
                  makeDelegate(*this, &Label::refreshDrawables));
}

void vui::Label::updatePosition() {
    Widget::updatePosition();
    updateTextPosition();
}

void vui::Label::setDestRect(const f32v4& destRect) {
    vui::Widget::setDestRect(destRect);
    refreshDrawables();
}

void vui::Label::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    updateTextPosition();
}

void vui::Label::setFont(const vorb::graphics::SpriteFont* font) {
    m_font = font;
    updateTextPosition();
}

void vui::Label::setHeight(f32 height) {
    Widget::setHeight(height);
    updateTextPosition();
}

void vui::Label::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    updateTextPosition();
}

void vui::Label::setWidth(f32 width) {
    Widget::setWidth(width);
    updateTextPosition();
}

void vui::Label::setX(f32 x) {
    Widget::setX(x);
    updateTextPosition();
}

void vui::Label::setY(f32 y) {
    Widget::setY(y);
    updateTextPosition();
}

void vui::Label::setText(const nString& text) {
    m_drawableText.setText(text);
    updateTextPosition();
}

void vui::Label::setTextColor(const color4& color) {
    m_drawableText.setColor(color);
    refreshDrawables();
}

void vui::Label::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);
    updateTextPosition();
}

void vui::Label::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);
    updateTextPosition();
}

void vui::Label::updateTextPosition() {
    const f32v2& dims = getDimensions();
    const f32v2& pos = getPosition();
    const vg::TextAlign& textAlign = getTextAlign();

    m_drawableText.setClipRect(m_clipRect);

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

void vui::Label::refreshDrawables() {
    // Use renderer default font if we don't have a font
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);
        m_drawnText = m_drawableText;
        m_drawableText.setFont(nullptr);
    } else {
        m_drawnText = m_drawableText;
    }
}
