#include "stdafx.h"
#include "ui/IWidgetContainer.h"
#include "UI/InputDispatcher.h"
#include "UI/Widget.h"

vui::IWidgetContainer::IWidgetContainer() :
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
    vui::InputDispatcher::mouse.onButtonDown += makeDelegate(*this, &IWidgetContainer::onMouseDown);
    vui::InputDispatcher::mouse.onButtonUp += makeDelegate(*this, &IWidgetContainer::onMouseUp);
    vui::InputDispatcher::mouse.onMotion += makeDelegate(*this, &IWidgetContainer::onMouseMove);
}

vui::IWidgetContainer::IWidgetContainer(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : IWidgetContainer() {
    m_name = name;
    setDestRect(destRect);
}

vui::IWidgetContainer::~IWidgetContainer() {
    // Empty
}

void vui::IWidgetContainer::dispose() {
    vui::InputDispatcher::mouse.onButtonDown -= makeDelegate(*this, &IWidgetContainer::onMouseDown);
    vui::InputDispatcher::mouse.onButtonUp -= makeDelegate(*this, &IWidgetContainer::onMouseUp);
    vui::InputDispatcher::mouse.onMotion -= makeDelegate(*this, &IWidgetContainer::onMouseMove);
}

bool vui::IWidgetContainer::addWidget(Widget* child) {
    m_widgets.push_back(child);
    child->m_parent = this;
    child->updatePosition();
    return true; // TODO(Ben): Is this needed?
}

bool vui::IWidgetContainer::removeWidget(Widget* child) {
    for (auto& it = m_widgets.begin(); it != m_widgets.end(); it++) {
        if (*it == child) {
            m_widgets.erase(it);
            return true;
        }
    }
    return false;
}

void vui::IWidgetContainer::enable() {
    m_isEnabled = true;
}

void vui::IWidgetContainer::disable() {
    m_isEnabled = false;
    m_isClicking = false;
}

bool vui::IWidgetContainer::isInBounds(f32 x, f32 y) {
    return (x >= m_position.x && x <= m_position.x + m_dimensions.x &&
            y >= m_position.y && y <= m_position.y + m_dimensions.y);
}

void vui::IWidgetContainer::setDestRect(const f32v4& destRect) {
    m_relativePosition.x = destRect.x;
    m_relativePosition.y = destRect.y;
    m_dimensions.x = destRect.z;
    m_dimensions.y = destRect.w;
    updatePosition();
}

void vui::IWidgetContainer::onMouseDown(Sender s, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseDown(e);
        m_isClicking = true;
    }
}

void vui::IWidgetContainer::onMouseUp(Sender s, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseUp(e);
        if (m_isClicking) MouseClick(e);
    }
    m_isClicking = false;
}

void vui::IWidgetContainer::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (!m_isEnabled) return;
    if (isInBounds((f32)e.x, (f32)e.y)) {
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
