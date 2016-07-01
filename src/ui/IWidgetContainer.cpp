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

    disable();
}

bool vui::IWidgetContainer::addWidget(Widget* child, Widget* self) {
    m_widgets.push_back(child);
    child->m_parentWidget = self;
    child->setParentForm(this->m_parentForm);
    child->updateSpatialState();
    return true; // TODO(Ben): Is this needed?
}

bool vui::IWidgetContainer::addWidget(Widget* child, Form* self) {
    m_widgets.push_back(child);
    child->setParentForm(self);
    child->updateSpatialState();
    return true; // TODO(Ben): Is this needed?
}

bool vui::IWidgetContainer::removeWidget(Widget* child) {
    for (auto& it = m_widgets.begin(); it != m_widgets.end(); it++) {
        if (*it == child) {
            m_widgets.erase(it);
            return true;
        }
    }
    child->setParentForm(nullptr);
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

void vui::IWidgetContainer::updateSpatialState() {
    updatePosition();
    
    updateDimensions();

    computeClipRect();

    updateChildSpatialStates();
}

void vui::IWidgetContainer::updatePositionState() {
    updatePosition();

    computeClipRect();

    updateChildSpatialStates();
}

void vui::IWidgetContainer::updateDimensionState() {
    updateDimensions();

    computeClipRect();

    updateChildSpatialStates();
}

void vui::IWidgetContainer::updateClippingState() {
    computeClipRect();

    updateChildClippingStates();
}

bool vui::IWidgetContainer::isInBounds(f32 x, f32 y) const {
    return (x >= vmath::max(m_position.x, m_clipRect.x) && x < vmath::min(m_position.x + m_dimensions.x, m_clipRect.x + m_clipRect.z) &&
        y >= vmath::max(m_position.y, m_clipRect.y) && y < vmath::min(m_position.y + m_dimensions.y, m_clipRect.y + m_clipRect.w));
}

void vui::IWidgetContainer::setParentForm(Form* form) {
    m_parentForm = form;
    for (auto& w : m_widgets) {
        w->setParentForm(form);
    }
}

void vui::IWidgetContainer::setParentWidget(Widget* parent, Widget* self) {
    if (m_parentWidget) m_parentWidget->removeWidget(self);
    if (parent) parent->addWidget(self);
}

void vui::IWidgetContainer::setDestRect(const f32v4& destRect) {
    setPosition(f32v2(destRect.x, destRect.y));
    setDimensions(f32v2(destRect.z, destRect.w));
}

void vui::IWidgetContainer::computeClipRect() {
    f32v4 parentClipRect = f32v4(-FLT_MAX / 2.0f, -FLT_MAX / 2.0f, FLT_MAX, FLT_MAX);
    if (m_parentWidget) {
        parentClipRect = m_parentWidget->getClipRect();
    } else if (m_parentForm) {
        parentClipRect = m_parentForm->getClipRect();
    }

    f32v2 position = m_position;
    f32v2 dimensions = m_dimensions;

    computeClipping(parentClipRect, position, dimensions);


    if (dimensions.x < 0.0f) dimensions.x = 0.0f;
    if (dimensions.y < 0.0f) dimensions.y = 0.0f;
    
    m_clipRect = f32v4(position.x, position.y, dimensions.x, dimensions.y);
    if (!m_clippingOptions.left) {
        m_clipRect.x = parentClipRect.x;
    }
    if (!m_clippingOptions.top) {
        m_clipRect.y = parentClipRect.y;
    }
    if (!m_clippingOptions.right) {
        m_clipRect.z = (parentClipRect.x + parentClipRect.z) - m_clipRect.x;
        if (m_clipRect.z < 0.0f) m_clipRect.z = 0.0f;
    }
    if (!m_clippingOptions.bottom) {
        m_clipRect.w = (parentClipRect.y + parentClipRect.w) - m_clipRect.y;
        if (m_clipRect.w < 0.0f) m_clipRect.w = 0.0f;
    }
}

void vui::IWidgetContainer::updateChildSpatialStates() {
    for (auto& w : m_widgets) {
        w->updateSpatialState();
    }
}

void vui::IWidgetContainer::updateChildClippingStates() {
    for (auto& w : m_widgets) {
        w->updateClippingState();
    }
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