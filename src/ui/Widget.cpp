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

//TODO(Matthew): Stop using m_relativePosition and instead update via raw position data.
void vui::Widget::updatePosition() {
    //f32v2 newPos = m_relativePosition;
    //if (m_parent) {
    //    // Handle percentages
    //    if (m_positionPercentage.x >= 0.0f) {
    //        newPos.x = m_parent->getWidth() * m_positionPercentage.x;
    //    }
    //    if (m_positionPercentage.y >= 0.0f) {
    //        newPos.y = m_parent->getHeight() * m_positionPercentage.y;
    //    }
    //    m_relativePosition = newPos;
    //    newPos += m_parent->getPosition();
    //}
    //newPos += getWidgetAlignOffset();
    
    m_position = m_relativePosition;
    
    // Update relative dimensions
    updateDimensions();

    if (m_parentWidget) {
        computeClipRect(m_parentWidget->getClipRect());
    } else if (m_parentForm) {
        computeClipRect(m_parentForm->getClipRect());
    }
        
    updateChildPositions();
}

const vui::IWidgetContainer* vui::Widget::getFirstPositionedParent() const {
    const IWidgetContainer* firstPositionedParent = m_parentForm;
    const Widget* widget = m_parentWidget;
    while (widget != nullptr) {
        vui::PositionType positionType = widget->getPositionType();
        if (positionType == vui::PositionType::ABSOLUTE
            || positionType == vui::PositionType::RELATIVE) {
            // Widget is "positioned"; break out.
            firstPositionedParent = widget;
            break;
        }

        widget = widget->getParentWidget();
    }
    return firstPositionedParent;
}

void vui::Widget::setParentWidget(Widget* parent) {
    if (m_parentWidget) m_parentWidget->removeWidget(this);
    if (parent) parent->addWidget(this);
}

void vui::Widget::setPosition(const f32v2& position) {
    m_rawPosition = std::pair<f32v2, vui::UnitType>(position, vui::UnitType::PIXEL);
    IWidgetContainer::setPosition(position);
}

void vui::Widget::setDimensions(const f32v2& dimensions) {
    m_rawDimensions = std::pair<f32v2, vui::UnitType>(dimensions, vui::UnitType::PIXEL);
    IWidgetContainer::setDimensions(dimensions);
}

void vui::Widget::setMaxSize(const f32v2& maxSize) {
    m_rawMaxSize = std::pair<f32v2, vui::UnitType>(maxSize, vui::UnitType::PIXEL);
    m_maxSize = maxSize;
    updateDimensions();
}

void vui::Widget::setMinSize(const f32v2& minSize) {
    m_rawMinSize = std::pair<f32v2, vui::UnitType>(minSize, vui::UnitType::PIXEL);
    m_minSize = minSize;
    updateDimensions();
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
    // Process raw dimensions.
    f32v2 newDims = processRawValue(m_rawDimensions);

    // Check against min/max size.
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

    // Only set if dimensions changed
    if (newDims != m_dimensions) {
        IWidgetContainer::setDimensions(newDims);
    }
}

f32v2 vui::Widget::processRawValue(f32v2 rawValue, vui::UnitType units) {
    f32v2 result;
    switch (units) {
    case vui::UnitType::PIXEL:
        result = rawValue;
    case vui::UnitType::PERCENTAGE:
        switch (m_positionType) {
        case vui::PositionType::STATIC:
        case vui::PositionType::RELATIVE:
            if (m_parentWidget) {
                result = rawValue * m_parentWidget->getDimensions();
            } else if (m_parentForm) {
                result = rawValue * m_parentForm->getDimensions();
            }
        case vui::PositionType::FIXED:
            if (m_parentForm) {
                result = rawValue * m_parentForm->getDimensions();
            }
        case vui::PositionType::ABSOLUTE:
            const IWidgetContainer* firstPositionedParent = getFirstPositionedParent();
            if (firstPositionedParent) {
                result = rawValue * firstPositionedParent->getDimensions();
            }
        }
    case vui::UnitType::FORM_HEIGHT_PERC:
        if (m_parentForm) {
            result = rawValue * m_parentForm->getHeight();
        }
    case vui::UnitType::FORM_WIDTH_PERC:
        if (m_parentForm) {
            result = rawValue * m_parentForm->getWidth();
        }
    case vui::UnitType::FORM_MAX_PERC:
        if (m_parentForm) {
            f32v2 formDims = m_parentForm->getDimensions();
            if (formDims.x > formDims.y) {
                result = rawValue * formDims.x;
            } else {
                result = rawValue * formDims.y;
            }
        }
    case vui::UnitType::FORM_MIN_PERC:
        if (m_parentForm) {
            f32v2 formDims = m_parentForm->getDimensions();
            if (formDims.x > formDims.y) {
                result = rawValue * formDims.y;
            } else {
                result = rawValue * formDims.x;
            }
        }
    default: // Shouldn't happen.
        result = rawValue;
    }
    return result;
}
