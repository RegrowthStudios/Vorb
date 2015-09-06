#include "stdafx.h"
#include "Widget.h"
#include "UIUtils.h"

#include <include/ui/InputDispatcher.h>
#include <include/ui/UIRenderer.h>

vui::Widget::Widget() : WidgetContainer() {
    enable();
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::dispose() {
    removeDrawables();
    WidgetContainer::dispose();
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

void vui::Widget::setParent(WidgetContainer* parent) {
    if (m_parent) m_parent->removeWidget(this);
    if (parent) parent->addWidget(this);
}

void vui::Widget::setNeedsDrawableReload(bool needsReload) {
    UIUtils::addFlag(&this->m_widgetFlags, WidgetFlags::DRAWABLE_RELOAD);
}

const volatile bool vui::Widget::needsDrawableReload() {
    return UIUtils::checkFlag(&this->m_widgetFlags, WidgetFlags::DRAWABLE_RELOAD);
}

f32v2 vui::Widget::getWidgetAlignOffset(WidgetAlign align, f32v2 dimensions) {
    switch (align) {
    case WidgetAlign::LEFT:
        return f32v2(0, -dimensions.y * 0.5f);
    case WidgetAlign::TOP_LEFT:
        return f32v2(0.0f);
    case WidgetAlign::TOP:
        return f32v2(-dimensions.x * 0.5f, 0.0f);
    case WidgetAlign::TOP_RIGHT:
        return f32v2(-dimensions.x, 0.0f);
    case WidgetAlign::RIGHT:
        return f32v2(-dimensions.x, -dimensions.y * 0.5f);
    case WidgetAlign::BOTTOM_RIGHT:
        return f32v2(-dimensions.x, -dimensions.y);
    case WidgetAlign::BOTTOM:
        return f32v2(-dimensions.x * 0.5f, -dimensions.y);
    case WidgetAlign::BOTTOM_LEFT:
        return f32v2(0.0f, -dimensions.y);
    case WidgetAlign::CENTER:
        return f32v2(-dimensions.x * 0.5f, -dimensions.y * 0.5f);
    }

    return f32v2(0.0f);
}

void vui::Widget::update(int flags, f32 dt) {
    // Update relative positions
    if (UIUtils::checkFlag(&flags, 0x01)) {
        f32v2 newPos = m_relativePosition;

        if (m_parent) {
            m_relativePosition = newPos;
            newPos += m_parent->getPosition();
        }

        newPos += getWidgetAlignOffset(m_align, m_dimensions);
        m_position = newPos;
    };

    // Update relative dimensions
    if (UIUtils::checkFlag(&flags, 0x02)) {
        f32v2 newDims = m_dimensions;

        // Check min/max size
        if (newDims.x < m_minSize.x) {
            newDims.x = m_minSize.x;
        }
        else if (newDims.x > m_maxSize.x) {
            newDims.x = m_maxSize.x;
        }

        if (newDims.y < m_minSize.y) {
            newDims.y = m_minSize.y;
        }
        else if (newDims.y > m_maxSize.y) {
            newDims.y = m_maxSize.y;
        }

        // Only set if it changed
        if (newDims != m_dimensions) {
            setDimensions(newDims);
        }
    };

    if (m_parent) computeClipRect(m_parent->getClipRect());

    updateChilds(0x01 | 0x02);
}