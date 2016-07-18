#include "stdafx.h"
#include "ui/IWidgetContainer.h"
#include "UI/InputDispatcher.h"
#include "UI/Widget.h"
#include "utils.h"

vui::IWidgetContainer::IWidgetContainer() :
    MouseClick(this),
    MouseDown(this),
    MouseUp(this),
    MouseEnter(this),
    MouseLeave(this),
    MouseMove(this) {
    //m_zIndexCompare = [](Widget* w1, Widget* w2) { return w1->getZIndex() < w2->getZIndex(); };
    m_widgets = SortedVector<Widget*, true, bool(*)(Widget*, Widget*)>(m_zIndexCompare);
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
    SortedVector<Widget*, true, bool(*)(Widget*, Widget*)>().swap(m_widgets);

    disable();
}

bool vui::IWidgetContainer::removeWidget(Widget* child) {
    for (auto& it = m_widgets.begin(); it != m_widgets.end(); it++) {
        if (*it == child) {
            m_widgets.erase(it);
            child->setParentForm(nullptr);
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

//void vui::IWidgetContainer::updateZIndexState(Widget* changedChild /*= nullptr*/) {
//    if (changedChild) {
//        auto& it = m_widgets.find(changedChild);
//        m_widgets.erase(it);
//        m_widgets.insert(changedChild);
//    } else {
//        m_widgets.sort();
//    }
//}
//
//void vui::IWidgetContainer::updateTransitionState() {
//    updateTargetPosition();
//
//    updateTargetDimensions();
//}

bool vui::IWidgetContainer::isInBounds(f32 x, f32 y) const {
    return (x >= vmath::max(m_position.x, m_clipRect.x) && x < vmath::min(m_position.x + m_dimensions.x, m_clipRect.x + m_clipRect.z) &&
        y >= vmath::max(m_position.y, m_clipRect.y) && y < vmath::min(m_position.y + m_dimensions.y, m_clipRect.y + m_clipRect.w));
}

void vui::IWidgetContainer::setDestRect(const f32v4& destRect) {
    setPosition(f32v2(destRect.x, destRect.y));
    setDimensions(f32v2(destRect.z, destRect.w));
}

bool vui::IWidgetContainer::addWidget(Widget* child, Widget* self) {
    if (m_widgets.insert(child).second) {
        child->setParentWidget(self);
        return true;
    }
    return false;
}

bool vui::IWidgetContainer::addWidget(Widget* child, Form* self) {
    if (m_widgets.insert(child).second) {
        child->setParentForm(self);
        return true;
    }
    return false;
}

void vui::IWidgetContainer::updateClipping() {
    f32v4 currRect = m_clipRect;
    computeClipRect();
    if (currRect != m_clipRect) applyUpdateFlagToChildren(UpdateFlag::CLIPPING);
}

void vui::IWidgetContainer::applyUpdateFlagToChildren(ui32 flag) {
    for (auto& w : m_widgets.get_container())
        w->applyUpdateFlag(flag);
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

void vui::IWidgetContainer::onMouseFocusLost(Sender s, const MouseEvent& e) {
    if (!m_isEnabled) return;
    if (m_isMouseIn) {
        m_isMouseIn = false;
        MouseMotionEvent ev;
        ev.x = e.x;
        ev.y = e.y;
        MouseLeave(ev);
    }
}