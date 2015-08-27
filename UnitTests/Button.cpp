#include "stdafx.h"
#include "Button.h"
#include "UIUtils.h"

#include <include/ui/MouseInputDispatcher.h>
#include <include/ui/UIRenderer.h>

vui::Button::Button() : Widget() {

}

vui::Button::Button(const nString& name) : Button() {
    this->m_name = name;
}

vui::Button::~Button() {
    // Empty
}

void vui::Button::addDrawables(UIRenderer* renderer) {
    Widget::addDrawables(renderer);

    m_drawableRect.setColor1(color4(1, 1, 1));
    m_drawableRect.setColor2(color4(1, 1, 1));
    m_drawableRect.setGradientType(vg::GradientType::NONE);
    m_drawableText.setColor(color4(0, 0, 0));

    // Make copies
    m_drawnText = m_drawableText;
    m_drawnRect = m_drawableRect;

    // Use renderer default font if we dont have a font
    m_defaultFont = renderer->getDefaultFont();
    if (!m_drawnText.getFont()) m_drawnText.setFont(m_defaultFont);

    // Add the rect
    renderer->add(this,
        makeDelegate(m_drawnRect, &DrawableRect::draw),
        makeDelegate(*this, &Button::refreshDrawables));

    // Add the text 
    renderer->add(this,
        makeDelegate(m_drawnText, &DrawableText::draw),
        makeDelegate(*this, &Button::refreshDrawables));
}

void vui::Button::update(int flags, f32 dt) {
    Widget::update(flags, dt);

    m_drawableRect.setPosition(getPosition());
    m_drawableRect.setDimensions(getDimensions());
    m_drawableRect.setClipRect(m_clipRect);

    updateTextPosition();
}

void vui::Button::setFont(const vorb::graphics::SpriteFont* font) {
    m_font = font;

    update(0x00 | 0x01, 1.0);
}

void vui::Button::setDimensions(const f32v2& dimensions) {
    Widget::setDimensions(dimensions);
    m_drawableRect.setDimensions(dimensions);

    update(0x00 | 0x01, 1.0);
}

void vui::Button::setWidth(f32 width) {
    Widget::setWidth(width);
    m_drawableRect.setWidth(width);

    update(0x00 | 0x01, 1.0);
}

void vui::Button::setHeight(f32 height) {
    Widget::setHeight(height);
    m_drawableRect.setHeight(height);

    update(0x00 | 0x01, 1.0);
}

void vui::Button::setPosition(const f32v2& position) {
    Widget::setPosition(position);
    m_drawableRect.setPosition(m_position);

    update(0x00 | 0x01, 1.0);
}

void vui::Button::setX(f32 x) {
    Widget::setX(x);
    m_drawableRect.setX(m_position.x);

    update(0x00 | 0x01, 1.0);
}

void vui::Button::setY(f32 y) {
    Widget::setY(y);
    m_drawableRect.setX(m_position.y);

    update(0x00 | 0x01, 1.0);
}

void vui::Button::setText(const nString& text) {
    m_drawableText.setText(text);

    updateTextPosition();
}

void vui::Button::setTextAlign(vg::TextAlign textAlign) {
    m_drawableText.setTextAlign(textAlign);

    updateTextPosition();
}

void vui::Button::setTextScale(const f32v2& textScale) {
    m_drawableText.setTextScale(textScale);

    updateTextPosition();
}

void vui::Button::updateTextPosition() {
    const f32v2& dims = getDimensions();
    const f32v2& pos = getPosition();
    const vg::TextAlign& textAlign = getTextAlign();

    m_drawableText.setClipRect(m_clipRect);

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

void vui::Button::refreshDrawables() {
    // Use renderer default font if we don't have a font
    if (!m_drawableText.getFont()) {
        m_drawableText.setFont(m_defaultFont);
        m_drawnText = m_drawableText;
        m_drawableText.setFont(nullptr);
    }
    else {
        m_drawnText = m_drawableText;
    }

    m_drawableRect.setClipRect(m_clipRect);
    m_drawnRect = m_drawableRect;
}

void vui::Button::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (!isEnabled()) return;

    if (UIUtils::isInBounds(f32v2(e.x, e.y), this->m_position, this->m_dimensions)) {
        if (!isMouseIn()) {
            UIUtils::addFlag(&this->m_widgetFlags, WidgetFlags::HOVER);

            MouseEnter(e);
        }

        MouseMove(e);
    }
    else if (isMouseIn()) {
        UIUtils::removeFlag(&this->m_widgetFlags, WidgetFlags::HOVER);

        MouseLeave(e);
    }
}

void vui::Button::onMouseFocusLost(Sender s, const MouseEvent& e) {
    if (!isEnabled()) return;

    if (isMouseIn()) {
        UIUtils::removeFlag(&this->m_widgetFlags, WidgetFlags::HOVER);

        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;

        MouseLeave(ev);
    }
}

