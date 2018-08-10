#include "Vorb/stdafx.h"
#include "Vorb/ui/IWidgetContainer.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/utils.h"

vui::IWidgetContainer::IWidgetContainer() :
    MouseClick(this),
    MouseDown(this),
    MouseUp(this),
    MouseEnter(this),
    MouseLeave(this),
    MouseMove(this) {
    m_style = {};
    enable();
}

vui::IWidgetContainer::IWidgetContainer(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : IWidgetContainer() {
    m_name = name;
    setDestRect(destRect);
}

vui::IWidgetContainer::~IWidgetContainer() {
    // Empty
}

void vui::IWidgetContainer::dispose() {
    for (auto& w : m_widgets) {
        w->dispose();
    }
    std::vector<Widget*>().swap(m_widgets);
    for (int i = 0; i < 5; i++) std::vector<Widget*>().swap(m_dockedWidgets[i]);

    disable();
    
}

bool vui::IWidgetContainer::addWidget(Widget* child) {
    m_widgets.push_back(child);
    child->m_parent = this;
    child->updatePosition();
    return true; // TODO(Ben): Is this needed?
}

bool vui::IWidgetContainer::removeWidget(Widget* child) {
    for (auto it = m_widgets.begin(); it != m_widgets.end(); it++) {
        if (*it == child) {
            if (removeChildFromDock(child)) recalculateDockedWidgets();
            m_widgets.erase(it);
            return true;
        }
    }
    return false;
}

void vui::IWidgetContainer::enable() {
    if (!m_isEnabled) {
        m_isEnabled = true;
        vui::InputDispatcher::mouse.onButtonDown += makeDelegate(*this, &IWidgetContainer::onMouseDown);
        vui::InputDispatcher::mouse.onButtonUp += makeDelegate(*this, &IWidgetContainer::onMouseUp);
        vui::InputDispatcher::mouse.onMotion += makeDelegate(*this, &IWidgetContainer::onMouseMove);
        vui::InputDispatcher::mouse.onFocusLost += makeDelegate(*this, &IWidgetContainer::onMouseFocusLost);
    }
    // Enable all children
    for (auto& w : m_widgets) w->enable();
}

void vui::IWidgetContainer::disable() {
    if (m_isEnabled) {
        m_isEnabled = false;
        vui::InputDispatcher::mouse.onButtonDown -= makeDelegate(*this, &IWidgetContainer::onMouseDown);
        vui::InputDispatcher::mouse.onButtonUp -= makeDelegate(*this, &IWidgetContainer::onMouseUp);
        vui::InputDispatcher::mouse.onMotion -= makeDelegate(*this, &IWidgetContainer::onMouseMove);
        vui::InputDispatcher::mouse.onFocusLost -= makeDelegate(*this, &IWidgetContainer::onMouseFocusLost);
    }
    m_isClicking = false;
    // Disable all children
    for (auto& w : m_widgets) w->disable();
}

bool vui::IWidgetContainer::isInBounds(f32 x, f32 y) const {
    return (x >= m_position.x && x < m_position.x + m_dimensions.x &&
            y >= m_position.y && y < m_position.y + m_dimensions.y);
}

void vui::IWidgetContainer::setChildDock(Widget* widget, DockStyle dockStyle) {
    removeChildFromDock(widget);
    // Add to new dock
    widget->m_dock = dockStyle;
    int dockIndex = (int)dockStyle - 1;
    if (dockIndex != -1) {
        m_dockedWidgets[dockIndex].push_back(widget);
    }
    recalculateDockedWidgets();
}

void vui::IWidgetContainer::setDestRect(const f32v4& destRect) {
    m_relativePosition.x = destRect.x;
    m_relativePosition.y = destRect.y;
    m_dimensions.x = destRect.z;
    m_dimensions.y = destRect.w;
    updatePosition();
}

bool vui::IWidgetContainer::removeChildFromDock(Widget* widget) {
    int dockIndex = (int)widget->m_dock - 1;
    // Remove it from its current dock
    if (dockIndex != -1) {
        for (auto w = m_dockedWidgets[dockIndex].begin(); w != m_dockedWidgets[dockIndex].end(); w++) {
            if (*w == widget) {
                m_dockedWidgets[dockIndex].erase(w);
                // Subtract size
                switch (widget->m_dock) {
                    case DockStyle::LEFT:
                    case DockStyle::RIGHT:
                        m_dockSizes[dockIndex] -= widget->getWidth();
                        break;
                    case DockStyle::BOTTOM:
                    case DockStyle::TOP:
                        m_dockSizes[dockIndex] -= widget->getHeight();
                        break;
                    default:
                        break;
                }
                break;
            }
        }
        widget->m_dock = DockStyle::NONE;
        return true;
    }
    return false;
}

void vui::IWidgetContainer::recalculateDockedWidgets() {
    // Bottom
    m_dockSizes[2] = 0.0f;
    for (auto& w : m_dockedWidgets[2]) {
        w->setWidth(m_dimensions.x);
        m_dockSizes[2] += w->getHeight();
        w->setPosition(f32v2(0.0f, m_dimensions.y - m_dockSizes[2]));
    }
    // Top
    m_dockSizes[3] = 0.0f;
    for (auto& w : m_dockedWidgets[3]) {
        w->setWidth(m_dimensions.x);
        w->setPosition(f32v2(0.0f, m_dockSizes[3]));
        m_dockSizes[3] += w->getHeight();
    }
    // Left
    m_dockSizes[0] = 0.0f;
    for (auto& w : m_dockedWidgets[0]) {
        w->setHeight(m_dimensions.y - m_dockSizes[2] - m_dockSizes[3]);
        w->setPosition(f32v2(m_dockSizes[0], m_dockSizes[3]));
        m_dockSizes[0] += w->getWidth();
    }
    // Right
    m_dockSizes[1] = 0.0f;
    for (auto& w : m_dockedWidgets[1]) {
        w->setHeight(m_dimensions.y - m_dockSizes[2] - m_dockSizes[3]);
        m_dockSizes[1] += w->getWidth();
        w->setPosition(f32v2(m_dimensions.x - m_dockSizes[1], m_dockSizes[3]));
    }
    // Fill
    for (auto& w : m_dockedWidgets[4]) {
        w->setHeight(m_dimensions.y - m_dockSizes[2] - m_dockSizes[3]);
        w->setWidth(m_dimensions.x - m_dockSizes[0] - m_dockSizes[1]);
        w->setPosition(f32v2(m_dockSizes[0], m_dockSizes[3]));
    }
}

void vui::IWidgetContainer::computeClipRect(const f32v4& parentClipRect /*= f32v4(-FLT_MAX / 2.0f, -FLT_MAX / 2.0f, FLT_MAX, FLT_MAX)*/) {
    if (m_isClippingEnabled) {
        f32v2 pos = m_position;
        f32v2 dims = m_dimensions;
        computeClipping(parentClipRect, pos, dims);
        if (dims.x < 0) dims.x = 0;
        if (dims.y < 0) dims.y = 0;
        m_clipRect = f32v4(pos.x, pos.y, dims.x, dims.y);
    } else {
        m_clipRect = parentClipRect;
    }
    computeChildClipRects();
}

void vui::IWidgetContainer::computeChildClipRects() {
    for (auto& w : m_widgets) {
        w->computeClipRect(m_clipRect);
    }
}

void vui::IWidgetContainer::updateChildPositions() {
    for (auto& w : m_widgets) {
        w->updatePosition();
    }
}

void vui::IWidgetContainer::onMouseDown(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseDown(e);
        m_isClicking = true;
    }
}

void vui::IWidgetContainer::onMouseUp(Sender s VORB_UNUSED, const MouseButtonEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        MouseUp(e);
        if (m_isClicking) MouseClick(e);
    }
    m_isClicking = false;
}

void vui::IWidgetContainer::onMouseMove(Sender s VORB_UNUSED, const MouseMotionEvent& e) {
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

void vui::IWidgetContainer::onMouseFocusLost(Sender s VORB_UNUSED, const MouseEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);
    }
}