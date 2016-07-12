#include "stdafx.h"
#include "UI/Widget.h"
#include "UI/InputDispatcher.h"
#include "UI/UIRenderer.h"

#include <iostream>

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

//void vui::Widget::updateDrawableOrderState() {
//    for (auto& w : m_widgets.get_container()) {
//        w->removeDrawables();
//        w->setNeedsDrawableReload(false);
//        w->addDrawables(m_renderer);
//        w->updateDrawableOrderState();
//    }
//}
//
//void vui::Widget::updateSpatialState() {
//    updateMaxSize(); // TODO(Matthew): Updating children too many times. Should be just the once.
//
//    updateMinSize();
//
//    vui::IWidgetContainer::updateSpatialState();
//}
//
//void vui::Widget::updateTransitionState() {
//    updateTargetMaxSize();
//
//    updateTargetMinSize();
//
//    vui::IWidgetContainer::updateTransitionState();
//}

void vui::Widget::update(f32 dt /*= 0.0f*/) {
    //m_targetPosition.currentTime += dt; // TODO(Matthew): Optional callback on transition completion?
    //if (m_targetPosition.currentTime <= m_targetPosition.finalTime) {
    //    m_position = m_targetPosition.calculateCurrent();
    //    m_needsDrawableReload = true;
    //}
    //m_targetDimensions.currentTime += dt;
    //if (m_targetDimensions.currentTime <= m_targetDimensions.finalTime) {
    //    m_dimensions = m_targetDimensions.calculateCurrent();
    //    m_needsDrawableReload = true;
    //}
    //m_targetMaxSize.currentTime += dt;
    //if (m_targetMaxSize.currentTime <= m_targetMaxSize.finalTime) {
    //    m_maxSize = m_targetMaxSize.calculateCurrent();
    //    m_needsDrawableReload = true;
    //}
    //m_targetMinSize.currentTime += dt;
    //if (m_targetMinSize.currentTime <= m_targetMinSize.finalTime) {
    //    m_minSize = m_targetMinSize.calculateCurrent();
    //    m_needsDrawableReload = true;
    //}
    //applyMinMaxSizesToDimensions(m_dimensions);
    //updateChildSpatialStates(); // TODO(Matthew): Optimise this.
    //if (m_needsDrawableReload) { // TODO(Matthew): Some choppiness is appearing. Investigate.
    //    updateDrawableSpatialState();
    //    m_needsDrawableReload = false;
    //}
    //
    //// TODO(Matthew): For auto scaling widgets, may need to propogate updates up as well as down (i.e. propogate down for sizing not-auto-scaled widgets then propogate back upwards to update the auto-scaled widgets).
    //
    //// Next update children.
    //for (auto& w : m_widgets.get_container()) {
    //    // Check if we need to reload the drawables
    //    if (w->needsDrawableReload()) {
    //        w->removeDrawables();
    //        w->setNeedsDrawableReload(false);
    //        w->addDrawables(m_renderer);
    //    }
    //    w->update(dt);
    //}


}

//void vui::Widget::setDockingOptions(const DockingOptions& options) { 
//    m_dockingOptions = options;
//
//    // Reprocess docking size.
//    updateDockingSize();
//
//    // Reprocess docking of all sibling widgets and self.
//    if (m_parentWidget) {
//        m_parentWidget->updateDockingState();
//        m_parentWidget->updateChildClippingStates();
//    } else if (m_parentForm) {
//        m_parentForm->updateDockingState();
//        m_parentForm->updateChildClippingStates();
//    }
//}
//
//void vui::Widget::setRawDockingSize(const Length& size) {
//    m_dockingOptions.size = size;
//
//    // Reprocess docking size.
//    updateDockingSize();
//
//    // Reprocess docking of all sibling widgets and self.
//    if (m_parentWidget) {
//        m_parentWidget->updateDockingState();
//        m_parentWidget->updateChildClippingStates();
//    } else if (m_parentForm) {
//        m_parentForm->updateDockingState();
//        m_parentForm->updateChildClippingStates();
//    }
//}
//
//void vui::Widget::setDockingStyle(const DockingStyle& style) {
//    m_dockingOptions.style = style;
//
//    // Reprocess docking size.
//    updateDockingSize();
//
//    // Reprocess docking of all sibling widgets and self.
//    if (m_parentWidget) {
//        m_parentWidget->updateDockingState();
//        m_parentWidget->updateChildClippingStates();
//    } else if (m_parentForm) {
//        m_parentForm->updateDockingState();
//        m_parentForm->updateChildClippingStates();
//    }
//}

void vui::Widget::setMaxSize(const f32v2& maxSize) {
    m_rawMaxSize = { maxSize.x, maxSize.y, { UnitType::PIXEL, UnitType::PIXEL } };
    m_maxSize = maxSize;
    m_pendingUpdates |= UpdateFlag::MAX_SIZE;
}

void vui::Widget::setMinSize(const f32v2& minSize) {
    m_rawMinSize = { minSize.x, minSize.y, { UnitType::PIXEL, UnitType::PIXEL } };
    m_minSize = minSize;
    m_pendingUpdates |= UpdateFlag::MIN_SIZE;
}

//void vui::Widget::setZIndex(ui16 zIndex) {
//    m_zIndex = zIndex;
//    if (m_parentWidget) {
//        m_parentWidget->updateZIndexState(this);
//    } else if (m_parentForm) {
//        m_parentForm->updateZIndexState(this);
//    }
//}

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

void vui::Widget::setParentForm(Form* parent) {
    if (m_parentForm) m_parentForm->removeWidget(this);
    m_parentForm = parent;
    for (auto& w : m_widgets) {
        w->setParentForm(parent);
    }
    m_pendingUpdates = UpdateFlag::ALL;
}

void vui::Widget::setParentWidget(Widget* parent) { // TODO(Matthew): Should also update drawables.
    if (m_parentWidget) m_parentWidget->removeWidget(this);
    m_parentWidget = parent;
    setParentForm(parent->getParentForm());
}

//f32v2 vui::Widget::getWidgetAlignOffset() {
//    switch (m_align) {
//        case WidgetAlign::LEFT:
//            return f32v2(0, -m_dimensions.y * 0.5f);
//        case WidgetAlign::TOP_LEFT:
//            return f32v2(0.0f);
//        case WidgetAlign::TOP:
//            return f32v2(-m_dimensions.x * 0.5f, 0.0f);
//        case WidgetAlign::TOP_RIGHT:
//            return f32v2(-m_dimensions.x, 0.0f);
//        case WidgetAlign::RIGHT:
//            return f32v2(-m_dimensions.x, -m_dimensions.y * 0.5f);
//        case WidgetAlign::BOTTOM_RIGHT:
//            return f32v2(-m_dimensions.x, -m_dimensions.y);
//        case WidgetAlign::BOTTOM:
//            return f32v2(-m_dimensions.x * 0.5f, -m_dimensions.y);
//        case WidgetAlign::BOTTOM_LEFT:
//            return f32v2(0.0f, -m_dimensions.y);
//        case WidgetAlign::CENTER:
//            return f32v2(-m_dimensions.x * 0.5f, -m_dimensions.y * 0.5f);
//    }
//    return f32v2(0.0f); // Should never happen
//}

//void vui::Widget::updatePosition() {
//    if (m_dockingOptions.style == DockingStyle::NONE) {
//        m_position = processRawValues(m_rawPosition);
//        // TODO(Matthew): Determine if this is a valid replacement of old m_relativePosition.
//        m_relativePosition = m_position;
//
//        // Calculate and apply the relative-to-parent position component of the widget.
//        m_position += calculateRelativeToParentShift();
//    }
//}
//
//void vui::Widget::updateTargetPosition() {
//    //// Only update transition data if a transition is in process of completing.
//    //if (m_targetPosition.currentTime < m_targetPosition.finalTime) {
//        m_targetPosition.initialLength = processRawValues(m_targetPosition.rawInitialLength);
//        m_targetPosition.targetLength = processRawValues(m_targetPosition.rawTargetLength);
//        // Calculate and apply the relative-to-parent position component of the widget.
//        f32v2 relativeShift = calculateRelativeToParentShift();
//        m_targetPosition.initialLength += relativeShift;
//        m_targetPosition.targetLength += relativeShift;
//    //}
//}
//
//void vui::Widget::updateDimensions() {
//    if (m_dockingOptions.style == DockingStyle::NONE) {
//        // Process raw dimensions.
//        m_dimensions = processRawValues(m_rawDimensions);
//
//        // Check against min/max size.
//        applyMinMaxSizesToDimensions(m_dimensions);
//    }
//}
//
//void vui::Widget::updateTargetDimensions() {
//    //if (m_targetRawDimensions.currentTime < m_targetRawDimensions.finalTime) {
//        m_targetPosition.initialLength = processRawValues(m_targetDimensions.rawInitialLength);
//        applyMinMaxSizesToDimensions(m_targetPosition.initialLength);
//        m_targetPosition.targetLength = processRawValues(m_targetDimensions.rawTargetLength);
//        applyMinMaxSizesToDimensions(m_targetPosition.targetLength);
//    //}
//}
//
//void vui::Widget::updateTargetMaxSize() {
//    m_targetMaxSize.initialLength = processRawValues(m_targetMaxSize.rawInitialLength);
//    m_targetMaxSize.targetLength = processRawValues(m_targetMaxSize.rawTargetLength);
//}
//
//void vui::Widget::updateTargetMinSize() {
//    m_targetMinSize.initialLength = processRawValues(m_targetMinSize.rawInitialLength);
//    m_targetMinSize.targetLength = processRawValues(m_targetMinSize.rawTargetLength);
//}
//
//void vui::Widget::updateDockingSize() {
//    switch (m_dockingOptions.style) {
//    case DockingStyle::LEFT:
//        m_processedDockingSize = processRawValue(f32v2(m_dockingOptions.size.x, 0.0f), m_dockingOptions.size.units.x).x;
//        break;
//    case DockingStyle::TOP:
//        m_processedDockingSize = processRawValue(f32v2(0.0f, m_dockingOptions.size.x), m_dockingOptions.size.units.x).y;
//        break;
//    case DockingStyle::BOTTOM:
//        m_processedDockingSize = processRawValue(f32v2(m_dockingOptions.size.x, 0.0f), m_dockingOptions.size.units.x).x;
//        break;
//    case DockingStyle::RIGHT:
//        m_processedDockingSize = processRawValue(f32v2(0.0f, m_dockingOptions.size.x), m_dockingOptions.size.units.x).y;
//        break;
//    case DockingStyle::FILL:
//    case DockingStyle::NONE:
//    default:
//        break;
//    }
//}

void vui::Widget::computeClipRect() {
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

f32v2 vui::Widget::processRawValues(const Length2& rawValues) {
    return IWidgetContainer::processRawValues(this, rawValues);
}

f32v2 vui::Widget::processRawValue(const f32v2& rawValue, const UnitType& units) {
    return IWidgetContainer::processRawValue(this, rawValue, units);
}

f32v2 vui::Widget::calculateRelativeToParentShift() {
    f32v2 relativeShift = f32v2(0.0f);
    switch (m_positionType) {
    case vui::PositionType::STATIC:
    case vui::PositionType::RELATIVE:
        if (m_parentWidget) {
            relativeShift = m_parentWidget->getPosition();
        } else if (m_parentForm) {
            relativeShift = m_parentForm->getPosition();
        }
        break;
    case vui::PositionType::FIXED:
        if (m_parentForm) {
            relativeShift = m_parentForm->getPosition();
        }
        break;
    case vui::PositionType::ABSOLUTE:
        const IWidgetContainer* firstPositionedParent = getFirstPositionedParent();
        if (firstPositionedParent) {
            relativeShift = firstPositionedParent->getPosition();
        }
        break;
    }
    return relativeShift;
}

void vui::Widget::applyMinMaxSizesToDimensions(f32v2& dimensions) {
    if (dimensions.x < m_minSize.x) {
        dimensions.x = m_minSize.x;
    } else if (dimensions.x > m_maxSize.x) {
        dimensions.x = m_maxSize.x;
    }
    if (dimensions.y < m_minSize.y) {
        dimensions.y = m_minSize.y;
    } else if (dimensions.y > m_maxSize.y) {
        dimensions.y = m_maxSize.y;
    }
}
