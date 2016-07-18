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

void vui::Widget::update(f32 dt /*= 0.0f*/) {
    //// TODO(Matthew): For auto scaling widgets, may need to propogate updates up as well as down (i.e. propogate down for sizing not-auto-scaled widgets then propogate back upwards to update the auto-scaled widgets).

    // Process changes to raw state not associated with docking if this widget is not docked.
    if (m_dockingOptions.style == DockingStyle::NONE) {
        if (!isPositionTransitioning()) {
            if ((m_pendingUpdates & UpdateFlag::RAW_POSITION) == UpdateFlag::RAW_POSITION)
                processRawPosition();
        } else {
            if ((m_pendingUpdates & UpdateFlag::RAW_TRANSITION_POSITION) == UpdateFlag::RAW_TRANSITION_POSITION)
                processRawPositionTransition();
            if ((m_pendingUpdates & UpdateFlag::TRANSITION_POSITION) == UpdateFlag::TRANSITION_POSITION)
                onPositionTransitionUpdate();
            processPositionTransition(dt);
        }
        if (!isDimensionsTransitioning()) {
            if ((m_pendingUpdates & UpdateFlag::RAW_DIMENSIONS) == UpdateFlag::RAW_DIMENSIONS)
                processRawDimensions();
        } else {
            if ((m_pendingUpdates & UpdateFlag::RAW_TRANSITION_DIMENSIONS) == UpdateFlag::RAW_TRANSITION_DIMENSIONS)
                processRawDimensionsTransition();
            if ((m_pendingUpdates & UpdateFlag::TRANSITION_DIMENSIONS) == UpdateFlag::TRANSITION_DIMENSIONS)
                onDimensionsTransitionUpdate();
            processDimensionsTransition(dt);
        }
        if (!isMaxSizeTransitioning()) {
            if ((m_pendingUpdates & UpdateFlag::RAW_MAX_SIZE) == UpdateFlag::RAW_MAX_SIZE)
                processRawMaxSize();
        } else {
            if ((m_pendingUpdates & UpdateFlag::RAW_TRANSITION_MAX_SIZE) == UpdateFlag::RAW_TRANSITION_MAX_SIZE)
                processRawMaxSizeTransition();
            if ((m_pendingUpdates & UpdateFlag::TRANSITION_MAX_SIZE) == UpdateFlag::TRANSITION_MAX_SIZE)
                onMaxSizeTransitionUpdate();
            processMaxSizeTransition(dt);
        }
        if (!isMinSizeTransitioning()) {
            if ((m_pendingUpdates & UpdateFlag::RAW_MIN_SIZE) == UpdateFlag::RAW_MIN_SIZE)
                processRawMinSize();
        } else {
            if ((m_pendingUpdates & UpdateFlag::RAW_TRANSITION_MIN_SIZE) == UpdateFlag::RAW_TRANSITION_MIN_SIZE)
                processRawMinSizeTransition();
            if ((m_pendingUpdates & UpdateFlag::TRANSITION_MIN_SIZE) == UpdateFlag::TRANSITION_MIN_SIZE)
                onMinSizeTransitionUpdate();
            processMinSizeTransition(dt);
        }

        if ((m_pendingUpdates & UpdateFlag::SIZE_LIMITS) > 0)
            applyMinMaxSizesToDimensions(m_dimensions);
    } else {
        if (!isDockingTransitioning()) {
            if ((m_pendingUpdates & UpdateFlag::RAW_DOCKING_SIZE) == UpdateFlag::RAW_DOCKING_SIZE)
                processRawDockingSize();
        } else {
            if ((m_pendingUpdates & UpdateFlag::RAW_TRANSITION_DOCKING) == UpdateFlag::RAW_TRANSITION_DOCKING)
                processRawDockingTransition();
            if ((m_pendingUpdates & UpdateFlag::TRANSITION_DOCKING) == UpdateFlag::RAW_TRANSITION_DOCKING)
                onDockingTransitionUpdate();
            processDockingTransition(dt);
        }

        if ((m_pendingUpdates & UpdateFlag::DOCKING) > 0)
            recalculateDocking();

        if ((m_pendingUpdates & UpdateFlag::DOCKING_SIZE) == UpdateFlag::DOCKING_SIZE)
            onDockingSizeUpdate();
        if ((m_pendingUpdates & UpdateFlag::DOCKING_STYLE) == UpdateFlag::DOCKING_STYLE)
            onDockingStyleUpdate();
    }

    if ((m_pendingUpdates & UpdateFlag::SPATIAL) > 0)
        onSpatialUpdate();

    if ((m_pendingUpdates & UpdateFlag::CLIPPING) == UpdateFlag::CLIPPING)
        recalculateClipping();

    if ((m_pendingUpdates & UpdateFlag::DRAWABLE) > 0)
        recalculateDrawables();

    if ((m_pendingUpdates & UpdateFlag::TEXT) == UpdateFlag::TEXT)
        recalculateText();

    if ((m_pendingUpdates & UpdateFlag::DRAWABLE_ORDER) == UpdateFlag::DRAWABLE_ORDER)
        recalculateDrawableOrder();

    for (auto& w : m_widgets.get_container()) {
        w->update(dt);
    }
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

void vui::Widget::processRawPosition() {
    f32v2 currPosition = m_position;

    // Process raw position.
    m_position = processRawValues(m_rawPosition);

    // TODO(Matthew): Determine if this is a valid replacement of old m_relativePosition.
    m_relativePosition = m_position;

    // Calculate and apply the relative-to-parent position component of the widget.
    m_position += calculateRelativeToParentShift();

    // If position has changed, propogate to children.
    if (m_position != currPosition) {
        m_pendingUpdates |= UpdateFlag::POSITION;
        applyUpdateFlagToChildren(UpdateFlag::POSITION);
    }
}

void vui::Widget::processRawDimensions() {
    f32v2 currDimensions = m_dimensions;

    // Process raw dimensions.
    m_dimensions = processRawValues(m_rawDimensions);

    // If dimensions has changed, propogate to children.
    if (m_dimensions != currDimensions) {
        m_pendingUpdates |= UpdateFlag::DIMENSIONS;
        applyUpdateFlagToChildren(UpdateFlag::DIMENSIONS);
    }
}

void vui::Widget::processRawMaxSize() {
    f32v2 currMaxSize = m_maxSize;

    m_maxSize = processRawValues(m_rawMaxSize);

    if (m_maxSize != currMaxSize) m_pendingUpdates |= UpdateFlag::MAX_SIZE;
}

void vui::Widget::processRawMinSize() {
    f32v2 currMinSize = m_minSize;

    m_minSize = processRawValues(m_rawMinSize);

    if (m_minSize != currMinSize) m_pendingUpdates |= UpdateFlag::MIN_SIZE;
}

void vui::Widget::processRawDockingSize() {
    f32 currDockingSize = m_dockingOptions.size;

    m_dockingOptions.size = processRawValues(m_dockingOptions.rawSize);

    if (m_dockingOptions.size != currDockingSize) m_pendingUpdates |= UpdateFlag::DOCKING_SIZE;
}

void vui::Widget::processRawPositionTransition() { // TODO(Matthew): Think about how changing only other aspects of transition should work.
    f32v2 currInitialLength = m_targetPosition.initialLength;
    f32v2 currFinalLength = m_targetPosition.finalLength;

    // Process raw values.
    m_targetPosition.initialLength = processRawValues(m_targetPosition.rawInitialLength);
    m_targetPosition.finalLength = processRawValues(m_targetPosition.rawFinalLength);

    // Apply relative shift due to parent.
    f32v2 relativeShift = calculateRelativeToParentShift();
    m_targetPosition.initialLength += relativeShift;
    m_targetPosition.finalLength += relativeShift;

    if (m_targetPosition.initialLength != currInitialLength
        || m_targetPosition.finalLength != currFinalLength)
        m_pendingUpdates |= UpdateFlag::TRANSITION_POSITION;
}

void vui::Widget::processRawDimensionsTransition() {
    f32v2 currInitialDimensions = m_targetDimensions.initialLength;
    f32v2 currFinalDimensions = m_targetDimensions.finalLength;

    // Process raw values.
    m_targetDimensions.initialLength = processRawValues(m_targetDimensions.rawInitialLength);
    m_targetDimensions.finalLength = processRawValues(m_targetDimensions.rawFinalLength);

    // If dimensions has changed, propogate to children.
    if (m_targetDimensions.initialLength != currInitialDimensions
        || m_targetDimensions.finalLength != currFinalDimensions)
        m_pendingUpdates |= UpdateFlag::TRANSITION_DIMENSIONS;
}

void vui::Widget::processRawMaxSizeTransition() {
    f32v2 currInitialMaxSize = m_targetMaxSize.initialLength;
    f32v2 currFinalMaxSize = m_targetMaxSize.finalLength;

    // Process raw values.
    m_targetMaxSize.initialLength = processRawValues(m_targetMaxSize.rawInitialLength);
    m_targetMaxSize.finalLength = processRawValues(m_targetMaxSize.rawFinalLength);

    if (m_targetMaxSize.initialLength != currInitialMaxSize
        || m_targetMaxSize.finalLength != currFinalMaxSize)
        m_pendingUpdates |= UpdateFlag::TRANSITION_MAX_SIZE;
}

void vui::Widget::processRawMinSizeTransition() {
    f32v2 currInitialMinSize = m_targetMinSize.initialLength;
    f32v2 currFinalMinSize = m_targetMinSize.finalLength;

    // Process raw values.
    m_targetMinSize.initialLength = processRawValues(m_targetMinSize.rawInitialLength);
    m_targetMinSize.finalLength = processRawValues(m_targetMinSize.rawFinalLength);

    if (m_targetMinSize.initialLength != currInitialMinSize
        || m_targetMinSize.finalLength != currFinalMinSize)
        m_pendingUpdates |= UpdateFlag::TRANSITION_MIN_SIZE;
}

void vui::Widget::processRawDockingTransition() {
    f32 currInitialDockingSize = m_targetDockingSize.initialLength;
    f32 currFinalDockingSize = m_targetDockingSize.finalLength;

    // Process raw values.
    m_targetDockingSize.initialLength = processRawValues(m_targetDockingSize.rawInitialLength);
    m_targetDockingSize.finalLength = processRawValues(m_targetDockingSize.rawFinalLength);

    if (m_targetDockingSize.initialLength != currInitialDockingSize
        || m_targetDockingSize.finalLength != currFinalDockingSize)
        m_pendingUpdates |= UpdateFlag::TRANSITION_DOCKING;
}

void vui::Widget::processPositionTransition(f32 dt) {
    m_targetPosition.currentTime += dt;
    if (m_targetPosition.currentTime <= m_targetPosition.finalTime) {
        f32v2 currPosition = m_position;

        m_position = m_targetPosition.calculateCurrent();

        // If position has changed, propogate to children.
        if (m_position != currPosition) {
            m_pendingUpdates |= UpdateFlag::POSITION;
            applyUpdateFlagToChildren(UpdateFlag::POSITION);
        }
    }
}

void vui::Widget::processDimensionsTransition(f32 dt) {
    m_targetDimensions.currentTime += dt;
    if (m_targetDimensions.currentTime <= m_targetDimensions.finalTime) {
        f32v2 currDimensions = m_dimensions;

        m_dimensions = m_targetDimensions.calculateCurrent();

        // If dimensions have changed, propogate to children.
        if (m_dimensions != currDimensions) {
            m_pendingUpdates |= UpdateFlag::DIMENSIONS;
            applyUpdateFlagToChildren(UpdateFlag::DIMENSIONS);
        }
    }
}

void vui::Widget::processMaxSizeTransition(f32 dt) {
    m_targetMaxSize.currentTime += dt;
    if (m_targetMaxSize.currentTime <= m_targetMaxSize.finalTime) {
        f32v2 currMaxSize = m_maxSize;

        m_maxSize = m_targetMaxSize.calculateCurrent();

        if (m_maxSize != currMaxSize)
            m_pendingUpdates |= UpdateFlag::MAX_SIZE;
    }
}

void vui::Widget::processMinSizeTransition(f32 dt) {
    m_targetMinSize.currentTime += dt;
    if (m_targetMinSize.currentTime <= m_targetMinSize.finalTime) {
        f32v2 currMinSize = m_minSize;

        m_minSize = m_targetMinSize.calculateCurrent();

        if (m_minSize != currMinSize)
            m_pendingUpdates |= UpdateFlag::MIN_SIZE;
    }
}

void vui::Widget::processDockingTransition(f32 dt) {
    m_targetDockingSize.currentTime += dt;
    if (m_targetDockingSize.currentTime <= m_targetDockingSize.finalTime) {
        f32 currDockingSize = m_dockingOptions.size;

        m_dockingOptions.size = m_targetDockingSize.calculateCurrent();

        if (m_dockingOptions.size != currDockingSize)
            m_pendingUpdates |= UpdateFlag::DOCKING_SIZE;
    }
}

inline void vui::Widget::recalculateClipping() {
    computeClipRect();
}

void vui::Widget::recalculateDocking() {
    f32 surplusWidth = m_dimensions.x;
    f32 surplusHeight = m_dimensions.y;
    f32v2 usedSpace = f32v2(0.0f);
    for (auto& w : m_widgets.get_container()) {
        DockingOptions options = w->getDockingOptions();
        f32 size = w->getProcessedDockingSize();
        switch (options.style) {
        case DockingStyle::NONE:
            break;
        case DockingStyle::LEFT:
            w->setHeight(surplusHeight);
            surplusWidth -= size;
            if (surplusWidth > -FLT_EPSILON) {
                w->setWidth(size);
                w->setPosition(f32v2(usedSpace.x, usedSpace.y));
            } else {
                w->setWidth(0.0f);
            }
            usedSpace.x += size;
            break;
        case DockingStyle::TOP:
            w->setWidth(surplusWidth);
            surplusHeight -= size;
            if (surplusHeight > -FLT_EPSILON) {
                w->setHeight(size);
                w->setPosition(f32v2(usedSpace.x, usedSpace.y));
            } else {
                w->setHeight(0.0f);
            }
            usedSpace.y += size;
            break;
        case DockingStyle::RIGHT:
            w->setHeight(surplusHeight);
            surplusWidth -= size;
            if (surplusWidth > -FLT_EPSILON) {
                w->setWidth(size);
                w->setPosition(f32v2(usedSpace.x + surplusWidth, usedSpace.y));
            } else {
                w->setWidth(0.0f);
            }
            break;
        case DockingStyle::BOTTOM:
            w->setWidth(surplusWidth);
            surplusHeight -= size;
            if (surplusHeight > -FLT_EPSILON) {
                w->setHeight(size);
                w->setPosition(f32v2(usedSpace.x, usedSpace.y + surplusHeight));
            } else {
                w->setHeight(0.0f);
            }
            break;
        case DockingStyle::FILL:
            // Not sure what to do with this one yet.
            // TODO(Matthew): Find a way to implement this.
            break;
        default:
            // Shouldn't get here.
            break;
        }
    }
}

void vui::Widget::recalculateDrawableOrder() {
    // TODO(Matthew): Shouldn't have to call sort here (call it on separate event).
    m_widgets.sort();

    for (auto& w : m_widgets.get_container()) {
        w->removeDrawables();
        w->setNeedsDrawableReload(false);
        w->addDrawables(m_renderer);
        w->applyUpdateFlag(UpdateFlag::DRAWABLE_ORDER); // TODO(Matthew): Change method for UI rendering to 
                                                        //                better accommodate z-indexing.
                                                        //                (I.e. so this is unnecessary.)
    }
}

inline void vui::Widget::onSpatialUpdate() {
    m_pendingUpdates |= UpdateFlag::CLIPPING;
}

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
    f32v2 x = processRawValue(f32v2(rawValues.x, 0.0f), rawValues.units.x);
    f32v2 y = processRawValue(f32v2(0.0f, rawValues.y), rawValues.units.y);

    return x + y;
}

f32 vui::Widget::processRawValues(const Length& rawValues) {
    return processRawValue(f32v2(rawValues.x, 0.0f), rawValues.units.x).x;
}

f32v2 vui::Widget::processRawValue(const f32v2& rawValue, const UnitType& units) {
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

void vui::Widget::applyUpdateFlagToParent(ui32 flag) {
    if (m_parentWidget) {
        m_parentWidget->applyUpdateFlag(flag);
    } else if (m_parentForm) {
        m_parentForm->applyUpdateFlag(flag);
    }
}
