#include "Vorb/stdafx.h"
#include "Vorb/ui/Widget.h"

#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"

vui::Widget::Widget() : IWidget() {
    // Empty
}

vui::Widget::Widget(const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/) : IWidget() {
    m_name = name;

    m_position = f32v2(dimensions.x, dimensions.y);
    m_size     = f32v2(dimensions.z, dimensions.w);

    m_rawDimensions.position.x = dimensions.x;
    m_rawDimensions.position.y = dimensions.y;
    m_rawDimensions.size.x     = dimensions.z;
    m_rawDimensions.size.y     = dimensions.w;
}

vui::Widget::Widget(const nString& name, const Length2& position, const Length2& size) : IWidget() {
    m_name = name;

    m_rawDimensions.position = position;
    m_rawDimensions.size     = size;

    m_flags.needsDimensionUpdate = true;
}

vui::Widget::Widget(IWidget* parent, const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/) : Widget(name, dimensions) {
    parent->addWidget(this);
}

vui::Widget::Widget(IWidget* parent, const nString& name, const Length2& position, const Length2& size) : Widget(name, position, size) {
    parent->addWidget(this);
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::updateDimensions(f32 dt VORB_MAYBE_UNUSED) {
    if (m_dock.state != DockState::NONE) return;

    static auto applyRawPosition = [&](f32v2 modifier = { 0.0f, 0.0f }) {
            f32v2 processedPosition = processLength(m_rawDimensions.position);
            setPosition(processedPosition + modifier);
    };

    static auto applyRawSize = [&]() {
            f32v2 processedSize = processLength(m_rawDimensions.size);
            setSize(processedSize);
    };

    static auto applyRelativeDirectives = [&](f32v2 position, f32v2 size) {
        f32 left   = processLength(m_rawRelativePositions.left);
        f32 top    = processLength(m_rawRelativePositions.top);
        f32 right  = processLength(m_rawRelativePositions.right);
        f32 bottom = processLength(m_rawRelativePositions.bottom);
        
        setPosition(f32v2(position.x + left, position.y + top));
        setSize(f32v2(size.x - left + right, size.y - top + bottom));
    };

    switch(m_positionType) {
        case PositionType::STATIC_TO_WINDOW:
            applyRawPosition();

            applyRawSize();

            break;
        case PositionType::STATIC_TO_VIEWPORT:
            applyRawPosition(m_viewport->getPosition());

            applyRawSize();

            break;
        case PositionType::STATIC_TO_PARENT:
            applyRawPosition(m_parent->getPosition());

            applyRawSize();

            break;
        case PositionType::RELATIVE_TO_WINDOW:
            applyRelativeDirectives(f32v2(0.0f, 0.0f), f32v2((f32)m_viewport->getGameWindow()->getWidth(), (f32)m_viewport->getGameWindow()->getHeight()));

            break;
        case PositionType::RELATIVE_TO_VIEWPORT:
            applyRelativeDirectives(m_viewport->getPosition(), m_viewport->getSize());
            
            break;
        case PositionType::RELATIVE_TO_PARENT:
            applyRelativeDirectives(m_parent->getPosition(), m_parent->getSize());
            
            break;
        default:
            // Shouldn't get here.
            assert(false);
    }

    applyMinMaxSizes();

    // TODO(Matthew): Check what setDimensions did, it may have had some important side-effects.
}

f32 vui::Widget::processLength(Length length) {
    switch (length.dimension.x) {
        case DimensionType::PIXEL:
            return length.x;
        case DimensionType::WIDTH_PERCENTAGE:
            switch(m_positionType) {
                case PositionType::STATIC_TO_PARENT:
                case PositionType::RELATIVE_TO_PARENT:
                    return length.x * m_parent->getWidth();
                case PositionType::STATIC_TO_VIEWPORT:
                case PositionType::RELATIVE_TO_VIEWPORT:
                    return length.x * m_viewport->getWidth();
                case PositionType::STATIC_TO_WINDOW:
                case PositionType::RELATIVE_TO_WINDOW:
                    // This may look dangerous as getGameWindow can return nullptr, but we want to cause the game to crash if we've got a bad UI.
                    // TODO(Matthew): Maybe it could be more elegant though (log and crash?).
                    return length.x * m_viewport->getGameWindow()->getWidth();
            }
            break;
        case DimensionType::HEIGHT_PERCENTAGE:
            switch(m_positionType) {
                case PositionType::STATIC_TO_PARENT:
                case PositionType::RELATIVE_TO_PARENT:
                    return length.x * m_parent->getHeight();
                case PositionType::STATIC_TO_VIEWPORT:
                case PositionType::RELATIVE_TO_VIEWPORT:
                    return length.x * m_viewport->getHeight();
                case PositionType::STATIC_TO_WINDOW:
                case PositionType::RELATIVE_TO_WINDOW:
                    // This may look dangerous as getGameWindow can return nullptr, but we want to cause the game to crash if we've got a bad UI.
                    // TODO(Matthew): Maybe it could be more elegant though (log and crash?).
                    return length.x * m_viewport->getGameWindow()->getHeight();
            }
            break;
        case DimensionType::MIN_PERCENTAGE:
            switch(m_positionType) {
                case PositionType::STATIC_TO_PARENT:
                case PositionType::RELATIVE_TO_PARENT:
                    return length.x * glm::min(m_parent->getWidth(), m_parent->getHeight());
                case PositionType::STATIC_TO_VIEWPORT:
                case PositionType::RELATIVE_TO_VIEWPORT:
                    return length.x * glm::min(m_viewport->getWidth(), m_viewport->getHeight());
                case PositionType::STATIC_TO_WINDOW:
                case PositionType::RELATIVE_TO_WINDOW:
                    // This may look dangerous as getGameWindow can return nullptr, but we want to cause the game to crash if we've got a bad UI.
                    // TODO(Matthew): Maybe it could be more elegant though (log and crash?).
                    return length.x * glm::min(m_viewport->getGameWindow()->getWidth(), m_viewport->getGameWindow()->getHeight());
            }
            break;
        case DimensionType::MAX_PERCENTAGE:
            switch(m_positionType) {
                case PositionType::STATIC_TO_PARENT:
                case PositionType::RELATIVE_TO_PARENT:
                    return length.x * glm::max(m_parent->getWidth(), m_parent->getHeight());
                case PositionType::STATIC_TO_VIEWPORT:
                case PositionType::RELATIVE_TO_VIEWPORT:
                    return length.x * glm::max(m_viewport->getWidth(), m_viewport->getHeight());
                case PositionType::STATIC_TO_WINDOW:
                case PositionType::RELATIVE_TO_WINDOW:
                    // This may look dangerous as getGameWindow can return nullptr, but we want to cause the game to crash if we've got a bad UI.
                    // TODO(Matthew): Maybe it could be more elegant though (log and crash?).
                    return length.x * glm::max(m_viewport->getGameWindow()->getWidth(), m_viewport->getGameWindow()->getHeight());
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
        case DimensionType::VIEWPORT_WIDTH_PERCENTAGE:
            return length.x * m_viewport->getWidth();
        case DimensionType::VIEWPORT_HEIGHT_PERCENTAGE:
            return length.x * m_viewport->getHeight();
        case DimensionType::VIEWPORT_MIN_PERCENTAGE:
            return length.x * glm::min(m_viewport->getWidth(), m_viewport->getHeight());
        case DimensionType::VIEWPORT_MAX_PERCENTAGE:
            return length.x * glm::max(m_viewport->getWidth(), m_viewport->getHeight());
        case DimensionType::WINDOW_WIDTH_PERCENTAGE:
            return length.x * m_viewport->getGameWindow()->getWidth();
        case DimensionType::WINDOW_HEIGHT_PERCENTAGE:
            return length.x * m_viewport->getGameWindow()->getHeight();
        case DimensionType::WINDOW_MIN_PERCENTAGE:
            return length.x * glm::min(m_viewport->getGameWindow()->getWidth(), m_viewport->getGameWindow()->getHeight());
        case DimensionType::WINDOW_MAX_PERCENTAGE:
            return length.x * glm::max(m_viewport->getGameWindow()->getWidth(), m_viewport->getGameWindow()->getHeight());
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
        setWidth(processedMinSize.x);
    } else if (m_size.x > processedMaxSize.x) {
        setWidth(processedMaxSize.x);
    }
    if (m_size.y < processedMinSize.y) {
        setHeight(processedMinSize.y);
    } else if (m_size.y > processedMaxSize.y) {
        setHeight(processedMaxSize.y);
    }
}
