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
    newPos += getWidgetAlignOffset();
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

f32v2 vui::Widget::getWidgetAlignOffset() {
    switch (m_align) {
        case WidgetAlign::LEFT:
            return f32v2(0, -m_dimensions.y * 0.5f);
        case WidgetAlign::TOP_LEFT:
            return f32v2(0.0f);
        case WidgetAlign::TOP:
            return f32v2(-m_dimensions.x * 0.5f, 0.0f);
        case WidgetAlign::TOP_RIGHT:
            return f32v2(-m_dimensions.x, 0.0f);
        case WidgetAlign::RIGHT:
            return f32v2(-m_dimensions.x, -m_dimensions.y * 0.5f);
        case WidgetAlign::BOTTOM_RIGHT:
            return f32v2(-m_dimensions.x, -m_dimensions.y);
        case WidgetAlign::BOTTOM:
            return f32v2(-m_dimensions.x * 0.5f, -m_dimensions.y);
        case WidgetAlign::BOTTOM_LEFT:
            return f32v2(0.0f, -m_dimensions.y);
        case WidgetAlign::CENTER:
            return f32v2(-m_dimensions.x * 0.5f, -m_dimensions.y * 0.5f);
    }
    return f32v2(0.0f); // Should never happen
}

void vui::Widget::updateDimensions() {
    f32v2 newDims = m_dimensions;
    // Check parent relative dimensions
    if (m_parent) {      
        if (m_dimensionsPercentage.x > 0.0f) {
            newDims.x = m_dimensionsPercentage.x * m_parent->getWidth();
        }
        if (m_dimensionsPercentage.y > 0.0f) {
            newDims.y = m_dimensionsPercentage.y * m_parent->getHeight();
        } 
    }
    // Check min/max size
    if (newDims.x < m_minSize.x) {
        newDims.x = m_minSize.x;
    } else if (newDims.x > m_maxSize.x) {
        newDims.x = m_maxSize.x;
    }
    if (newDims.y < m_minSize.y) {
        newDims.y = m_minSize.y;
    } else if (newDims.y > m_maxSize.y) {
        newDims.y = m_maxSize.y;
    }
    // Only set if it changed
    if (newDims != m_dimensions) {
        setDimensions(newDims);
    }
}