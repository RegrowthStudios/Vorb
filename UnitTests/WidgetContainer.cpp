#include "stdafx.h"
#include "WidgetContainer.h"
#include "UIUtils.h"
#include "Widget.h"

#include <include/UI/InputDispatcher.h>
#include <include/utils.h>

vui::WidgetContainer::WidgetContainer() {
    UIUtils::addFlag(&this->m_widgetFlags, WidgetFlags::CLIPPING_ENABLED);
}

vui::WidgetContainer::WidgetContainer(const nString& name) {
    UIUtils::addFlag(&this->m_widgetFlags, WidgetFlags::CLIPPING_ENABLED);

    this->m_name = name;
}

vui::WidgetContainer::~WidgetContainer() {
}

void vui::WidgetContainer::enable() {
    if (!isEnabled()) {
        UIUtils::addFlag(&this->m_widgetFlags, WidgetFlags::ENABLED);

        vui::InputDispatcher::mouse.onButtonDown += makeDelegate(*this, &WidgetContainer::onMouseDown);
        vui::InputDispatcher::mouse.onButtonUp += makeDelegate(*this, &WidgetContainer::onMouseUp);
        vui::InputDispatcher::mouse.onMotion += makeDelegate(*this, &WidgetContainer::onMouseMove);
        vui::InputDispatcher::mouse.onFocusLost += makeDelegate(*this, &WidgetContainer::onMouseFocusLost);
    }

    // Enable all children
    for (auto& w : m_widgets) w->enable();
}

void vui::WidgetContainer::disable() {
    if (isEnabled()) {
        UIUtils::removeFlag(&this->m_widgetFlags, WidgetFlags::ENABLED);

        vui::InputDispatcher::mouse.onButtonDown -= makeDelegate(*this, &WidgetContainer::onMouseDown);
        vui::InputDispatcher::mouse.onButtonUp -= makeDelegate(*this, &WidgetContainer::onMouseUp);
        vui::InputDispatcher::mouse.onMotion -= makeDelegate(*this, &WidgetContainer::onMouseMove);
        vui::InputDispatcher::mouse.onFocusLost -= makeDelegate(*this, &WidgetContainer::onMouseFocusLost);
    }

    // Disable all children
    for (auto& w : m_widgets) w->disable();
}

void vui::WidgetContainer::dispose() {
    // Dispose all children
    for (auto& w : m_widgets) w->dispose();
    std::vector<Widget*>().swap(m_widgets);

    disable();
}

void vui::WidgetContainer::addWidget(Widget* child) {
    m_widgets.push_back(child);
    child->m_parent = this;
    child->update(0x00, 1.0);
}

void vui::WidgetContainer::removeWidget(Widget* child) {
    for (auto& it = m_widgets.begin(); it != m_widgets.end(); it++) {
        if (*it == child) {
            m_widgets.erase(it);
        }
    }
}

void vui::WidgetContainer::computeClipRect(const f32v4& parentClipRect) {
    if (isClippingEnabled()) {
        f32v2 pos = m_position;
        f32v2 dims = m_dimensions;

        computeClipping(parentClipRect, pos, dims);

        if (dims.x < 0) dims.x = 0;
        if (dims.y < 0) dims.y = 0;

        m_clipRect = f32v4(pos.x, pos.y, dims.x, dims.y);
    }
    else {
        m_clipRect = parentClipRect;
    }

    computeChildClipRects();
}

void vui::WidgetContainer::computeChildClipRects() {
    for (auto& w : m_widgets) {
        w->computeClipRect(m_clipRect);
    }
}

void vui::WidgetContainer::update(int flags, f32 dt) {

}

void vui::WidgetContainer::updateChilds(int flags) {
    for (auto& w : m_widgets) {
        w->update(flags, 1.0);
    }
}

const bool vui::WidgetContainer::isEnabled()  {
    return UIUtils::checkFlag(&this->m_widgetFlags, WidgetFlags::ENABLED);
}

bool vui::WidgetContainer::isMouseIn() {
    return UIUtils::checkFlag(&this->m_widgetFlags, WidgetFlags::HOVER);
}

const bool vui::WidgetContainer::isClippingEnabled() {
    return UIUtils::checkFlag(&this->m_widgetFlags, WidgetFlags::CLIPPING_ENABLED);
}

void vui::WidgetContainer::setClippingEnabled(bool enabled) {
    UIUtils::addFlag(&this->m_widgetFlags, WidgetFlags::CLIPPING_ENABLED);

    update(0x00, 1.0);
}

void vui::WidgetContainer::onMouseDown(Sender s, const MouseButtonEvent& e) {
    if (isEnabled()) return;

    if (isMouseIn()) {
        UIUtils::addFlag(&this->m_widgetFlags, WidgetFlags::CLICK);

        MouseDown(e);
    }
}

void vui::WidgetContainer::onMouseUp(Sender s, const MouseButtonEvent& e) {
    if (isEnabled()) return;

    if (isMouseIn()) {
        MouseUp(e);

        if (UIUtils::checkFlag(&this->m_widgetFlags, WidgetFlags::CLICK)) MouseClick(e);
    }

    UIUtils::removeFlag(&this->m_widgetFlags, WidgetFlags::CLICK);
}

void vui::WidgetContainer::onMouseMove(Sender s, const MouseMotionEvent& e) {
    if (isEnabled()) return;

    if (UIUtils::isInBounds(f32v2(e.x, e.y), this->m_position, this->m_dimensions)) {
        if (!isMouseIn()) {
            UIUtils::addFlag(&this->m_widgetFlags, WidgetFlags::HOVER);

            MouseEnter(e);
        }

        MouseMove(e);
    } else if (isMouseIn()) {
        UIUtils::removeFlag(&this->m_widgetFlags, WidgetFlags::HOVER);

        MouseLeave(e);
    }
}

void vui::WidgetContainer::onMouseFocusLost(Sender s, const MouseEvent& e) {
    if (isEnabled()) return;

    if (isMouseIn()) {
        UIUtils::removeFlag(&this->m_widgetFlags, WidgetFlags::HOVER);

        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;

        MouseLeave(ev);
    }
}