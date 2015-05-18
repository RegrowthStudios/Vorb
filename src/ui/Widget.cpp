#include "stdafx.h"
#include "UI/Widget.h"
#include "UI/InputDispatcher.h"
#include "UI/UIRenderer.h"

vui::Widget::Widget() : IWidgetContainer() {
    enable();
    m_anchor = {};
}

vui::Widget::Widget(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : IWidgetContainer(name, destRect) {
    enable();
}

vui::Widget::Widget(IWidgetContainer* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Widget(name, destRect) {
    setParent(parent);
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::dispose() {
    removeDrawables();
    IWidgetContainer::dispose();
}

void vui::Widget::addDrawables(UIRenderer* renderer) {
    m_renderer = renderer;
    for (auto& w : m_widgets) {
        w->addDrawables(m_renderer);
    }
}

void vui::Widget::removeDrawables() {
    if (m_renderer) {
        m_renderer->remove(this);
        m_renderer = nullptr;
    }
    for (auto& w : m_widgets) {
        w->removeDrawables();
    }
}

void vui::Widget::updatePosition() {
    f32v2 newPos = m_relativePosition;
    if (m_parent) {
        // Handle percentages
        if (m_positionPercentage.x >= 0.0f) {
            newPos.x = m_parent->getWidth() * m_positionPercentage.x;
        }
        if (m_positionPercentage.y >= 0.0f) {
            newPos.y = m_parent->getHeight() * m_positionPercentage.y;
        }
        m_relativePosition = newPos;
        newPos += m_parent->getPosition();
    }
    m_position = newPos;
    
    // Update relative dimensions
    updateDimensions();

    if (m_parent) computeClipRect(m_parent->getClipRect());
    
    updateChildPositions();
}

void vui::Widget::setAnchor(const AnchorStyle& anchor) {
    m_anchor = anchor;
}

void vui::Widget::setDock(const DockStyle& dock) {
    if (m_parent) {
        m_parent->setChildDock(this, dock);
    } else {
        m_dock = dock;
    }
}

void vui::Widget::setParent(IWidgetContainer* parent) {
    if (m_parent) m_parent->removeWidget(this);
    if (parent) parent->addWidget(this);
}

void vui::Widget::updateDimensions() {
    if (m_parent) {
        f32v2 newDims = m_dimensions;
        if (m_dimensionsPercentage.x > 0.0f) {
            newDims.x = m_dimensionsPercentage.x * m_parent->getWidth();
        }
        if (m_dimensionsPercentage.y > 0.0f) {
            newDims.y = m_dimensionsPercentage.y * m_parent->getHeight();
        }
        if (newDims != m_dimensions) {
            setDimensions(newDims);
        }
    }
}