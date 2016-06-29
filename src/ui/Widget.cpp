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

//vui::Widget::Widget(IWidgetContainer* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Widget(name, destRect) {
//    setParent(parent);
//}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::dispose() {
    removeDrawables();
    IWidgetContainer::dispose();
}

bool vui::Widget::addWidget(Widget* widget) {
    widget->addDrawables(m_renderer);
    return IWidgetContainer::addWidget(widget, this);
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
    m_position = processRawValue(m_rawPosition);
    // TODO(Matthew): Determine if this is a valid replacement of old m_relativePosition.
    m_relativePosition = m_position;

    switch (m_positionType) {
    case vui::PositionType::STATIC:
    case vui::PositionType::RELATIVE:
        if (m_parentWidget) {
            m_position += m_parentWidget->getPosition();
        } else if (m_parentForm) {
            m_position += m_parentForm->getPosition();
        }
        break;
    case vui::PositionType::FIXED:
        if (m_parentForm) {
            m_position += m_parentForm->getPosition();
        }
        break;
    case vui::PositionType::ABSOLUTE:
        const IWidgetContainer* firstPositionedParent = getFirstPositionedParent();
        if (firstPositionedParent) {
            m_position += firstPositionedParent->getPosition();
        }
        break;
    }
    
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

    // TODO(Matthew): Update overflow/clipping.
}

f32v2 vui::Widget::processRawValue(f32v2 rawValue, vui::UnitType units) {
    f32v2 result;
    switch (units) {
    case vui::UnitType::PIXEL:
        result = rawValue;
        break;
    case vui::UnitType::PERCENTAGE:
        switch (m_positionType) {
        case vui::PositionType::STATIC:
        case vui::PositionType::RELATIVE:
            if (m_parentWidget) {
                result = rawValue * m_parentWidget->getDimensions();
            } else if (m_parentForm) {
                result = rawValue * m_parentForm->getDimensions();
            }
            break;
        case vui::PositionType::FIXED:
            if (m_parentForm) {
                result = rawValue * m_parentForm->getDimensions();
            }
            break;
        case vui::PositionType::ABSOLUTE:
            const IWidgetContainer* firstPositionedParent = getFirstPositionedParent();
            if (firstPositionedParent) {
                result = rawValue * firstPositionedParent->getDimensions();
            }
            break;
        }
        break;
    case vui::UnitType::FORM_HEIGHT_PERC:
        if (m_parentForm) {
            result = rawValue * m_parentForm->getHeight();
        }
        break;
    case vui::UnitType::FORM_WIDTH_PERC:
        if (m_parentForm) {
            result = rawValue * m_parentForm->getWidth();
        }
        break;
    case vui::UnitType::FORM_MAX_PERC:
        if (m_parentForm) {
            f32v2 formDims = m_parentForm->getDimensions();
            if (formDims.x > formDims.y) {
                result = rawValue * formDims.x;
            } else {
                result = rawValue * formDims.y;
            }
        }
        break;
    case vui::UnitType::FORM_MIN_PERC:
        if (m_parentForm) {
            f32v2 formDims = m_parentForm->getDimensions();
            if (formDims.x > formDims.y) {
                result = rawValue * formDims.y;
            } else {
                result = rawValue * formDims.x;
            }
        }
        break;
    default: // Shouldn't happen.
        result = rawValue;
        break;
    }
    return result;
}
