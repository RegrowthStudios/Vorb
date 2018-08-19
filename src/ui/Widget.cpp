#include "Vorb/stdafx.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"

vui::Widget::Widget() : IWidget() {
    enable();
    // m_anchor = {};
}

vui::Widget::Widget(const nString& name, const f32v4& destRect /*= f32v4(0)*/) : IWidget(name, destRect) {
    enable();
}

vui::Widget::Widget(IWidget* parent, const nString& name, const f32v4& destRect /*= f32v4(0)*/) : Widget(name, destRect) {
    setParent(parent);
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::dispose() {
    removeDrawables();
    IWidget::dispose();
}

void vui::Widget::updateDimensions() {
    updatePosition();

    updateDimensions();

    updateChildDimensions();
}

// void vui::Widget::setAnchor(const AnchorStyle& anchor) {
//     m_anchor = anchor;
// }

// void vui::Widget::setDock(const DockStyle& dock) {
//     if (m_parent) {
//         m_parent->setChildDock(this, dock);
//     } else {
//         m_dock = dock;
//     }
// }


// f32v2 vui::Widget::getWidgetAlignOffset() {
//     switch (m_align) {
//         case WidgetAlign::LEFT:
//             return f32v2(0, -m_dimensions.y * 0.5f);
//         case WidgetAlign::TOP_LEFT:
//             return f32v2(0.0f);
//         case WidgetAlign::TOP:
//             return f32v2(-m_dimensions.x * 0.5f, 0.0f);
//         case WidgetAlign::TOP_RIGHT:
//             return f32v2(-m_dimensions.x, 0.0f);
//         case WidgetAlign::RIGHT:
//             return f32v2(-m_dimensions.x, -m_dimensions.y * 0.5f);
//         case WidgetAlign::BOTTOM_RIGHT:
//             return f32v2(-m_dimensions.x, -m_dimensions.y);
//         case WidgetAlign::BOTTOM:
//             return f32v2(-m_dimensions.x * 0.5f, -m_dimensions.y);
//         case WidgetAlign::BOTTOM_LEFT:
//             return f32v2(0.0f, -m_dimensions.y);
//         case WidgetAlign::CENTER:
//             return f32v2(-m_dimensions.x * 0.5f, -m_dimensions.y * 0.5f);
//     }
//     return f32v2(0.0f); // Should never happen
// }

f32 vui::Widget::processLength(Length length) {
    switch (length.dimension.x) {
        case DimensionType::PIXEL:
            return length.x;
        case DimensionType::WIDTH_PERCENTAGE:
            switch(m_positionType) {
                case PositionType::STATIC_TO_PARENT:
                case PositionType::RELATIVE_TO_PARENT:
                    return length.x * m_parent->getWidth();
                case PositionType::STATIC_TO_CANVAS:
                case PositionType::RELATIVE_TO_CANVAS:
                    return length.x * m_canvas->getWidth();
                case PositionType::STATIC_TO_WINDOW:
                case PositionType::RELATIVE_TO_WINDOW:
                    // TODO(Matthew): Get window dimensions...
                    assert (false);
            }
        case DimensionType::HEIGHT_PERCENTAGE:
            switch(m_positionType) {
                case PositionType::STATIC_TO_PARENT:
                case PositionType::RELATIVE_TO_PARENT:
                    return length.x * m_parent->getHeight();
                case PositionType::STATIC_TO_CANVAS:
                case PositionType::RELATIVE_TO_CANVAS:
                    return length.x * m_canvas->getHeight();
                case PositionType::STATIC_TO_WINDOW:
                case PositionType::RELATIVE_TO_WINDOW:
                    // TODO(Matthew): Get window dimensions...
                    assert (false);
            }
        case DimensionType::MIN_PERCENTAGE:
            switch(m_positionType) {
                case PositionType::STATIC_TO_PARENT:
                case PositionType::RELATIVE_TO_PARENT:
                    return length.x * glm::min(m_parent->getWidth(), m_parent->getHeight());
                case PositionType::STATIC_TO_CANVAS:
                case PositionType::RELATIVE_TO_CANVAS:
                    return length.x * glm::min(m_canvas->getWidth(), m_canvas->getHeight());
                case PositionType::STATIC_TO_WINDOW:
                case PositionType::RELATIVE_TO_WINDOW:
                    // TODO(Matthew): Get window dimensions...
                    assert (false);
            }
        case DimensionType::MAX_PERCENTAGE:
            switch(m_positionType) {
                case PositionType::STATIC_TO_PARENT:
                case PositionType::RELATIVE_TO_PARENT:
                    return length.x * glm::max(m_parent->getWidth(), m_parent->getHeight());
                case PositionType::STATIC_TO_CANVAS:
                case PositionType::RELATIVE_TO_CANVAS:
                    return length.x * glm::max(m_canvas->getWidth(), m_canvas->getHeight());
                case PositionType::STATIC_TO_WINDOW:
                case PositionType::RELATIVE_TO_WINDOW:
                    // TODO(Matthew): Get window dimensions...
                    assert (false);
            }
        case DimensionType::PARENT_WIDTH_PERCENTAGE:
            return length.x * m_parent->getWidth();
        case DimensionType::PARENT_HEIGHT_PERCENTAGE: 
            return length.x * m_parent->getHeight();
        case DimensionType::PARENT_MIN_PERCENTAGE:
            return length.x * glm::min(m_parent->getWidth(), m_parent->getHeight());
        case DimensionType::PARENT_MAX_PERCENTAGE:
            return length.x * glm::max(m_parent->getWidth(), m_parent->getHeight());
        case DimensionType::CANVAS_WIDTH_PERCENTAGE:
            return length.x * m_canvas->getWidth();
        case DimensionType::CANVAS_HEIGHT_PERCENTAGE:
            return length.x * m_canvas->getHeight();
        case DimensionType::CANVAS_MIN_PERCENTAGE:
            return length.x * glm::min(m_canvas->getWidth(), m_canvas->getHeight());
        case DimensionType::CANVAS_MAX_PERCENTAGE:
            return length.x * glm::max(m_canvas->getWidth(), m_canvas->getHeight());
        case DimensionType::WINDOW_WIDTH_PERCENTAGE:
            // TODO(Matthew): Get window dimensions...
        case DimensionType::WINDOW_HEIGHT_PERCENTAGE:
            // TODO(Matthew): Get window dimensions...
        case DimensionType::WINDOW_MIN_PERCENTAGE:
            // TODO(Matthew): Get window dimensions...
        case DimensionType::WINDOW_MAX_PERCENTAGE:
            // TODO(Matthew): Get window dimensions...
        default:
            // Shouldn't get here.
            assert(false);
    }
}

f32v2 vui::Widget::processLength(Length2 length) {
    return f32v2(
        processLength({ length.x, length.dimension.x }),
        processLength({ length.y, length.dimension.y })
    );
}

void vui::Widget::applyMinMaxSizes() {
    f32v2 processedMinSize = processLength(m_minRawSize);
    f32v2 processedMaxSize = processLength(m_maxRawSize);

    if (m_position.x < processedMinSize.x) {
        m_position.x = processedMinSize.x;
    } else if (m_position.x > processedMaxSize.x) {
        m_position.x = processedMaxSize.x;
    }
    if (m_position.y < processedMinSize.y) {
        m_position.y = processedMinSize.y;
    } else if (m_position.y > processedMaxSize.y) {
        m_position.y = processedMaxSize.y;
    }
}

// TODO(Matthew): Clean up these update functions once all features are implemented.
void vui::Widget::updatePosition() {
    // TODO(Matthew): Implement margins...
    switch(m_positionType) {
        case PositionType::STATIC_TO_WINDOW:
            f32v2 processedPosition = processLength(m_rawPosition);
            m_position = processedPosition;
            break;
        case PositionType::STATIC_TO_CANVAS:
            f32v2 processedPosition = processLength(m_rawPosition);
            m_position = processedPosition + m_canvas->getPosition();
            break;
        case PositionType::STATIC_TO_PARENT:
            f32v2 processedPosition = processLength(m_rawPosition);
            m_position = processedPosition + m_parent->getPosition();
            break;
        case PositionType::RELATIVE_TO_WINDOW:
            // TODO(Matthew): Implement {left, top, right, bottom} directives...
        case PositionType::RELATIVE_TO_CANVAS:
            // TODO(Matthew): Implement {left, top, right, bottom} directives...
        case PositionType::RELATIVE_TO_PARENT:
            // TODO(Matthew): Implement {left, top, right, bottom} directives...
        default:
            // Shouldn't get here.
            assert(false);
    }

    // TODO(Matthew): Implement this once we've implemented overflow.
    // if (m_parent) computeClipRect(m_parent->getClipRect());
}

void vui::Widget::updateSize() {
    // TODO(Matthew): Implement margins...
    switch(m_positionType) {
        case PositionType::STATIC_TO_WINDOW:
        case PositionType::STATIC_TO_CANVAS:
        case PositionType::STATIC_TO_PARENT:
            f32v2 processedSize    = processLength(m_rawSize);
            m_position = processedSize;

            applyMinMaxSizes();

            break;
        case PositionType::RELATIVE_TO_WINDOW:
        case PositionType::RELATIVE_TO_CANVAS:
        case PositionType::RELATIVE_TO_PARENT:
            // TODO(Matthew): Implement {left, top, right, bottom} directives...
        default:
            // Shouldn't get here.
            assert(false);
    }

    // TODO(Matthew): Check what setDimensions did, it may have had some important side-effects.
}