#include "stdafx.h"
#include "UI/Widget.h"
#include "UI/InputDispatcher.h"
#include "UI/UIRenderer.h"

vui::Widget::Widget() : IWidgetContainer(), 
    m_rawPosition({ 0.0f, 0.0f, { UnitType::PIXEL, UnitType::PIXEL } }),
    m_rawDimensions({ 0.0f, 0.0f, { UnitType::PIXEL, UnitType::PIXEL } }),
    m_rawMinSize({ 0.0f, 0.0f, { UnitType::PIXEL, UnitType::PIXEL } }),
    m_rawMaxSize({ FLT_MAX, FLT_MAX, { UnitType::PIXEL, UnitType::PIXEL } }),
    m_zIndex(0)
{
    enable();
    m_anchor = {};
}

vui::Widget::Widget(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : IWidgetContainer(name, destRect),
    m_rawPosition({ 0.0f, 0.0f, { UnitType::PIXEL, UnitType::PIXEL } }),
    m_rawDimensions({ 0.0f, 0.0f, { UnitType::PIXEL, UnitType::PIXEL } }),
    m_rawMinSize({ 0.0f, 0.0f, { UnitType::PIXEL, UnitType::PIXEL } }),
    m_rawMaxSize({ FLT_MAX, FLT_MAX, { UnitType::PIXEL, UnitType::PIXEL } }),
    m_zIndex(0)
{
    enable();
    m_anchor = {};
}

vui::Widget::Widget(Form* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Widget(name, destRect) {
    parent->addWidget(this);
}

vui::Widget::Widget(Widget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Widget(name, destRect) {
    parent->addWidget(this);
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::dispose() {
    removeDrawables();
    IWidgetContainer::dispose();
}

bool vui::Widget::addWidget(Widget* widget) {
    //widget->addDrawables(m_renderer);
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

void vui::Widget::updateDrawableOrderState() {
    for (auto& it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        (*it)->removeDrawables();
        (*it)->setNeedsDrawableReload(false);
        (*it)->addDrawables(m_renderer);
        (*it)->updateDrawableOrderState();
    }
}

void vui::Widget::setDockingOptions(const DockingOptions& options) { 
    m_dockingOptions = options;

    // Reprocess docking size.
    updateDockingSize();

    // Reprocess docking of all sibling widgets and self.
    if (m_parentWidget) {
        m_parentWidget->updateDockingState();
        m_parentWidget->updateChildClippingStates();
    } else if (m_parentForm) {
        m_parentForm->updateDockingState();
        m_parentForm->updateChildClippingStates();
    }
}

void vui::Widget::setRawDockingSize(const Length& size) {
    m_dockingOptions.size = size;

    // Reprocess docking size.
    updateDockingSize();

    // Reprocess docking of all sibling widgets and self.
    if (m_parentWidget) {
        m_parentWidget->updateDockingState();
        m_parentWidget->updateChildClippingStates();
    } else if (m_parentForm) {
        m_parentForm->updateDockingState();
        m_parentForm->updateChildClippingStates();
    }
}

void vui::Widget::setDockingStyle(const DockingStyle& style) {
    m_dockingOptions.style = style;

    // Reprocess docking size.
    updateDockingSize();

    // Reprocess docking of all sibling widgets and self.
    if (m_parentWidget) {
        m_parentWidget->updateDockingState();
        m_parentWidget->updateChildClippingStates();
    } else if (m_parentForm) {
        m_parentForm->updateDockingState();
        m_parentForm->updateChildClippingStates();
    }
}

void vui::Widget::setPosition(const f32v2& position, bool update /*= true*/) {
    m_rawPosition = { position.x, position.y, { UnitType::PIXEL, UnitType::PIXEL } };
    IWidgetContainer::setPosition(position, update);
}

void vui::Widget::setRawPosition(const f32v2& rawPosition, UnitType& units) {
    m_rawPosition = { rawPosition.x, rawPosition.y, { units, units } };
    updatePositionState();
}

void vui::Widget::setDimensions(const f32v2& dimensions) {
    m_rawDimensions = { dimensions.x, dimensions.y, { UnitType::PIXEL, UnitType::PIXEL } };
    IWidgetContainer::setDimensions(dimensions);
}

void vui::Widget::setRawDimensions(const f32v2& rawDimensions, UnitType& units) {
    m_rawDimensions = { rawDimensions.x, rawDimensions.y, { units, units } };
    updateDimensionState();
}

void vui::Widget::setMaxSize(const f32v2& maxSize) {
    m_rawMaxSize = { maxSize.x, maxSize.y, { UnitType::PIXEL, UnitType::PIXEL } };
    m_maxSize = maxSize;
    updateDimensionState();
}

void vui::Widget::setRawMaxSize(const f32v2& maxSize, UnitType& units) {
    m_rawMaxSize = { maxSize.x, maxSize.y, { units, units } };
    updateMaxSize();
}

void vui::Widget::setMinSize(const f32v2& minSize) {
    m_rawMinSize = { minSize.x, minSize.y, { UnitType::PIXEL, UnitType::PIXEL } };
    m_minSize = minSize;
    updateDimensionState();
}

void vui::Widget::setRawMinSize(const f32v2& minSize, UnitType& units) {
    m_rawMinSize = { minSize.x, minSize.y, { units, units } };
    updateMinSize();
}

void vui::Widget::setZIndex(ui16 zIndex) {
    m_zIndex = zIndex;
    if (m_parentWidget) {
        m_parentWidget->updateZIndexState(this);
    } else if (m_parentForm) {
        m_parentForm->updateZIndexState(this);
    }
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

void vui::Widget::updatePosition() {
    if (m_dockingOptions.style == DockingStyle::NONE) {
        m_position = processRawValues(m_rawPosition);
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
    }
}

void vui::Widget::updateDimensions() {
    if (m_dockingOptions.style == DockingStyle::NONE) {
        // Process raw dimensions.
        f32v2 newDims = processRawValues(m_rawDimensions);

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
}

void vui::Widget::updateDockingSize() {
    switch (m_dockingOptions.style) {
    case DockingStyle::LEFT:
        m_processedDockingSize = processRawValue(f32v2(m_dockingOptions.size.x, 0.0f), m_dockingOptions.size.units.x).x;
        break;
    case DockingStyle::TOP:
        m_processedDockingSize = processRawValue(f32v2(0.0f, m_dockingOptions.size.x), m_dockingOptions.size.units.x).y;
        break;
    case DockingStyle::BOTTOM:
        m_processedDockingSize = processRawValue(f32v2(m_dockingOptions.size.x, 0.0f), m_dockingOptions.size.units.x).x;
        break;
    case DockingStyle::RIGHT:
        m_processedDockingSize = processRawValue(f32v2(0.0f, m_dockingOptions.size.x), m_dockingOptions.size.units.x).y;
        break;
    case DockingStyle::FILL:
    case DockingStyle::NONE:
    default:
        break;
    }
}

f32v2 vui::Widget::processRawValues(const Length2& rawValues) {
    return IWidgetContainer::processRawValues(this, rawValues);
}

f32v2 vui::Widget::processRawValue(const f32v2& rawValue, const UnitType& units) {
    return IWidgetContainer::processRawValue(this, rawValue, units);
}
