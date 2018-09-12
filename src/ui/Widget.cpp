#include "Vorb/stdafx.h"
#include "Vorb/ui/Widget.h"

#include "Vorb/ui/GameWindow.h"
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

// TODO(Matthew): See if we can minimise calculations done on update.
void vui::Widget::updateDimensions() {
    static auto applyRawPosition = [&](f32v2 modifier = { 0.0f, 0.0f }) {
            f32v2 processedPosition = processLength(m_rawDimensions.position);
            m_position = processedPosition + modifier;
    };

    static auto applyRawSize = [&]() {
            f32v2 processedSize = processLength(m_rawDimensions.size);
            m_size = processedSize;
    };

    static auto applyRelativeDirectives = [&](f32v2 position, f32v2 size) {
        f32 left   = processLength(m_rawRelativePositions.left);
        f32 top    = processLength(m_rawRelativePositions.top);
        f32 right  = processLength(m_rawRelativePositions.right);
        f32 bottom = processLength(m_rawRelativePositions.bottom);
        
        m_position.x = position.x + left;
        m_position.y = position.y + top;
        m_size.x     = size.x - left + right;
        m_size.y     = size.y - top + bottom;
    };

    switch(m_positionType) {
        case PositionType::STATIC_TO_WINDOW:
            applyRawPosition();

            applyRawSize();

            break;
        case PositionType::STATIC_TO_CANVAS:
            applyRawPosition(m_canvas->getPosition());

            applyRawSize();

            break;
        case PositionType::STATIC_TO_PARENT:
            applyRawPosition(m_parent->getPosition());

            applyRawSize();

            break;
        case PositionType::RELATIVE_TO_WINDOW:
            applyRelativeDirectives(f32v2(0.0f, 0.0f), f32v2((f32)getGameWindow()->getWidth(), (f32)getGameWindow()->getHeight()));

            break;
        case PositionType::RELATIVE_TO_CANVAS:
            applyRelativeDirectives(m_canvas->getPosition(), m_canvas->getSize());
            
            break;
        case PositionType::RELATIVE_TO_PARENT:
            applyRelativeDirectives(m_parent->getPosition(), m_parent->getSize());
            
            break;
        default:
            // Shouldn't get here.
            assert(false);
    }

    applyMinMaxSizes();

    m_needsClipRectRecalculation = true;

    // TODO(Matthew): Check what setDimensions did, it may have had some important side-effects.

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
                    // This may look dangerous as getGameWindow can return nullptr, but we want to cause the game to crash if we've got a bad UI.
                    // TODO(Matthew): Maybe it could be more elegant though (log and crash?).
                    return length.x * getGameWindow()->getWidth();
            }
            break;
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
                    // This may look dangerous as getGameWindow can return nullptr, but we want to cause the game to crash if we've got a bad UI.
                    // TODO(Matthew): Maybe it could be more elegant though (log and crash?).
                    return length.x * getGameWindow()->getHeight();
            }
            break;
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
                    // This may look dangerous as getGameWindow can return nullptr, but we want to cause the game to crash if we've got a bad UI.
                    // TODO(Matthew): Maybe it could be more elegant though (log and crash?).
                    return length.x * glm::min(getGameWindow()->getWidth(), getGameWindow()->getHeight());
            }
            break;
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
                    // This may look dangerous as getGameWindow can return nullptr, but we want to cause the game to crash if we've got a bad UI.
                    // TODO(Matthew): Maybe it could be more elegant though (log and crash?).
                    return length.x * glm::max(getGameWindow()->getWidth(), getGameWindow()->getHeight());
            }
            break;
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
            return length.x * getGameWindow()->getWidth();
        case DimensionType::WINDOW_HEIGHT_PERCENTAGE:
            return length.x * getGameWindow()->getHeight();
        case DimensionType::WINDOW_MIN_PERCENTAGE:
            return length.x * glm::min(getGameWindow()->getWidth(), getGameWindow()->getHeight());
        case DimensionType::WINDOW_MAX_PERCENTAGE:
            return length.x * glm::max(getGameWindow()->getWidth(), getGameWindow()->getHeight());
        default:
            // Shouldn't get here.
            assert(false);
    }
}

f32v2 vui::Widget::processLength(Length2 length) {
    return f32v2(
        processLength({ length.x, { length.dimension.x } }),
        processLength({ length.y, { length.dimension.y } })
    );
}

void vui::Widget::applyMinMaxSizes() {
    f32v2 processedMinSize = processLength(m_minRawSize);
    f32v2 processedMaxSize = processLength(m_maxRawSize);

    if (m_size.x < processedMinSize.x) {
        m_size.x = processedMinSize.x;
    } else if (m_size.x > processedMaxSize.x) {
        m_size.x = processedMaxSize.x;
    }
    if (m_size.y < processedMinSize.y) {
        m_size.y = processedMinSize.y;
    } else if (m_size.y > processedMaxSize.y) {
        m_size.y = processedMaxSize.y;
    }
}
