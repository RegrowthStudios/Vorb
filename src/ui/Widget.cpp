#include "stdafx.h"
#include "UI/Widget.h"
#include "UI/InputDispatcher.h"

vui::Widget::Widget() :
    MouseClick(this),
    MouseDown(this),
    MouseUp(this),
    MouseEnter(this),
    MouseLeave(this),
    MouseMove(this) {
    m_anchor = {};
    m_style = {};
    m_dock = DockStyle::NONE;
    enable();
    vui::InputDispatcher::mouse.onButtonDown += makeDelegate(*this, &Widget::onMouseDown);
    vui::InputDispatcher::mouse.onButtonUp += makeDelegate(*this, &Widget::onMouseUp);
    vui::InputDispatcher::mouse.onMotion += makeDelegate(*this, &Widget::onMouseMove);
}

vui::Widget::Widget(const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : Widget() {
    m_name = name;
    m_destRect = destRect;
}

vui::Widget::Widget(Widget* parent, const nString& name, const ui32v4& destRect /*= ui32v4(0)*/) : Widget(name, destRect) {
    parent->addChild(this);
    m_parent = parent;
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::dispose() {
    vui::InputDispatcher::mouse.onButtonDown -= makeDelegate(*this, &Widget::onMouseDown);
    vui::InputDispatcher::mouse.onButtonUp -= makeDelegate(*this, &Widget::onMouseUp);
    vui::InputDispatcher::mouse.onMotion -= makeDelegate(*this, &Widget::onMouseMove);
}

bool vui::Widget::addChild(Widget* child) {
    m_widgets.push_back(child);
    return true; // TODO(Ben): Is this needed?
}

void vui::Widget::enable() {
    if (!m_isEnabled) {
        m_isEnabled = true;
    }
}

void vui::Widget::disable() {
    if (m_isEnabled) {
        m_isEnabled = false;
        m_isClicking = false;
    }
}

bool vui::Widget::isInBounds(f32 x, f32 y) {
    return (x >= m_destRect.x && x <= m_destRect.x + m_destRect.z &&
            y >= m_destRect.y && y <= m_destRect.y + m_destRect.w);
}

void vui::Widget::onMouseDown(Sender s, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseDown(e);
        m_isClicking = true;
    }   
}

void vui::Widget::onMouseUp(Sender s, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseUp(e);
        if (m_isClicking) MouseClick(e);
    }
    m_isClicking = false;
}

void vui::Widget::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (!m_isEnabled) return;
    if (isInBounds(e.x, e.y)) {
        if (!m_isMouseIn) {
            m_isMouseIn = true;
            MouseEnter(e);
        }
        MouseMove(e);
    } else if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseLeave(e);
    }
}
