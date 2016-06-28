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

    if (m_parent) computeClipRect(m_parent->getClipRect());
    
    updateChildPositions();
}

// TODO(Matthew): Better solution? Even with caching seems a tad overkill.
//                Maybe move parent out to IWidgetContainer?
const vui::Form* vui::Widget::getParentForm() const {
    const IWidgetContainer* widgetContainer = m_parent;
    IWidgetContainer* form = new Form();
    while (typeid(&widgetContainer) != typeid(&form)) {
        const Widget* widget = static_cast<const Widget*>(widgetContainer);
        widgetContainer = widget->getParent();
    }
    delete form;
    return static_cast<const Form*>(widgetContainer);
}

// TODO(Matthew): Better solution?
const vui::IWidgetContainer* vui::Widget::getFirstPositionedParent() const {
    const IWidgetContainer* widgetContainer = m_parent;
    while (true) {
        const Widget* widget = dynamic_cast<const Widget*>(widgetContainer);

        // Breakout if the widgetContainer is the first parent that is not a Widget or derivative.
        if (widget == nullptr) {
            break;
        }

        // Breakout if the widget is positioned.
        vui::PositionType positionType = widget->getPositionType();
        if (positionType == vui::PositionType::ABSOLUTE
            || positionType == vui::PositionType::RELATIVE) {
            break;
        }

        widgetContainer = widget->getParent();
    }
    return widgetContainer;
}

void vui::Widget::setAnchor(const AnchorStyle& anchor) {
    m_anchor = anchor;
}

void vui::Widget::setParent(IWidgetContainer* parent) {
    if (m_parent) m_parent->removeWidget(this);
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
    /*if (m_parent) {      
        if (m_dimensionsPercentage.x > 0.0f) {
            newDims.x = m_dimensionsPercentage.x * m_parent->getWidth();
        }
        if (m_dimensionsPercentage.y > 0.0f) {
            newDims.y = m_dimensionsPercentage.y * m_parent->getHeight();
        } 
    }*/

    // Check min/max size
    /*if (newDims.x < m_minSize.x) {
        newDims.x = m_minSize.x;
    } else if (newDims.x > m_maxSize.x) {
        newDims.x = m_maxSize.x;
    }
    if (newDims.y < m_minSize.y) {
        newDims.y = m_minSize.y;
    } else if (newDims.y > m_maxSize.y) {
        newDims.y = m_maxSize.y;
    }*/

    switch (m_rawDimensions.second) {
    case vui::UnitType::PIXEL:
        newDims = m_rawDimensions.first;
    case vui::UnitType::PERCENTAGE:
        switch (m_positionType) {
        case vui::PositionType::STATIC:
        case vui::PositionType::RELATIVE:
            newDims = m_rawDimensions.first * getParent()->getDimensions();
        case vui::PositionType::FIXED_TO_FORM:
            newDims = m_rawDimensions.first * getParentForm()->getDimensions();
        case vui::PositionType::FIXED_TO_WINDOW:
            // Find out a reasonable way to get window size.
        case vui::PositionType::ABSOLUTE:
            newDims = m_rawDimensions.first * getFirstPositionedParent()->getDimensions();
        }
    case vui::UnitType::FORM_HEIGHT_PERC:
        newDims = m_rawDimensions.first * getParentForm()->getHeight();
    case vui::UnitType::FORM_WIDTH_PERC:
        newDims = m_rawDimensions.first * getParentForm()->getWidth();
    case vui::UnitType::FORM_MAX_PERC:
        f32v2 formDims = getParentForm()->getDimensions();
        if (formDims.x > formDims.y) {
            newDims = m_rawDimensions.first * formDims.x;
        } else {
            newDims = m_rawDimensions.first * formDims.y;
        }
    case vui::UnitType::FORM_MIN_PERC:
        f32v2 formDims = getParentForm()->getDimensions();
        if (formDims.x > formDims.y) {
            newDims = m_rawDimensions.first * formDims.y;
        } else {
            newDims = m_rawDimensions.first * formDims.x;
        }
    case vui::UnitType::WINDOW_HEIGHT_PERC:
    case vui::UnitType::WINDOW_WIDTH_PERC:
    case vui::UnitType::WINDOW_MAX_PERC:
    case vui::UnitType::WINDOW_MIN_PERC:
        // Find out a reasonable way to get window size.
    }

    // Only set if it changed
    if (newDims != m_dimensions) {
        IWidgetContainer::setDimensions(newDims);
    }
}