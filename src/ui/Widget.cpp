#include "Vorb/stdafx.h"
#include "Vorb/ui/Widget.h"

#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/InputDispatcher.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/Viewport.h"

// TODO(Matthew): Some zero-initialisation in constructor?
vui::Widget::Widget() :
    IWidget(),
    m_positionType(PositionType::STATIC_TO_PARENT),
    m_minRawSize({ 0.0f, 0.0f, { DimensionType::PIXEL, DimensionType::PIXEL } }),
    m_maxRawSize({ FLT_MAX, FLT_MAX, { DimensionType::PIXEL, DimensionType::PIXEL } }),
    m_rawDockSize({ 0.0f, { DimensionType::PIXEL } }),
    m_rawPadding({ 0.0f, 0.0f, 0.0f, 0.0f, { DimensionType::PIXEL, DimensionType::PIXEL, DimensionType::PIXEL, DimensionType::PIXEL } }),
    m_rawDimensions({ { 0.0f, 0.0f, { DimensionType::PIXEL, DimensionType::PIXEL } }, { 0.0, 0.0, { DimensionType::PIXEL, DimensionType::PIXEL } } }) {
    // Empty
}

vui::Widget::~Widget() {
    // Empty
}

void vui::Widget::init(const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/, ui16 zIndex /*= 0*/) {
    IWidget::init(name, dimensions, zIndex);

    m_rawDimensions.position.x = dimensions.x;
    m_rawDimensions.position.y = dimensions.y;
    m_rawDimensions.position.dimension.x = DimensionType::PIXEL;
    m_rawDimensions.position.dimension.y = DimensionType::PIXEL;

    m_rawDimensions.size.x     = dimensions.z;
    m_rawDimensions.size.y     = dimensions.w;
    m_rawDimensions.size.dimension.x = DimensionType::PIXEL;
    m_rawDimensions.size.dimension.y = DimensionType::PIXEL;
}

void vui::Widget::init(const nString& name, const Length2& position, const Length2& size, ui16 zIndex /*= 0*/) {
    m_name = name;

    m_rawDimensions.position = position;
    m_rawDimensions.size     = size;

    m_zIndex = zIndex;

    m_flags.needsDimensionUpdate = true;

    initBase();
}

void vui::Widget::init(IWidget* parent, const nString& name, const f32v4& dimensions /*= f32v4(0.0f)*/, ui16 zIndex /*= 0*/) {
    Widget::init(name, dimensions, zIndex);

    parent->addWidget(this);
}

void vui::Widget::init(IWidget* parent, const nString& name, const Length2& position, const Length2& size, ui16 zIndex /*= 0*/) {
    Widget::init(name, position, size, zIndex);

    parent->addWidget(this);
}

void vui::Widget::setMaxRawSize(const f32v2& maxRawSize) {
    m_maxRawSize = { maxRawSize.x, maxRawSize.y, { DimensionType::PIXEL, DimensionType::PIXEL } };

    m_flags.needsDimensionUpdate = true;
}

void vui::Widget::setMinRawSize(const f32v2& minRawSize) {
    m_minRawSize = { minRawSize.x, minRawSize.y, { DimensionType::PIXEL, DimensionType::PIXEL } };

    m_flags.needsDimensionUpdate = true;
}

void vui::Widget::setRawDockSize(const Length& rawDockSize) {
    m_rawDockSize = rawDockSize;

    if (m_parent) {
        m_parent->setNeedsDockRecalculation(true);
    } else if (m_viewport && m_viewport != this) {
        m_viewport->setNeedsDockRecalculation(true);
    }
}

void vui::Widget::setRawDockSize(f32 rawDockSize) {
    m_rawDockSize = { rawDockSize, { DimensionType::PIXEL } };

    if (m_parent) {
        m_parent->setNeedsDockRecalculation(true);
    } else if (m_viewport && m_viewport != this) {
        m_viewport->setNeedsDockRecalculation(true);
    }
}

void vui::Widget::setRawPosition(const f32v2& rawPosition) {
    m_rawDimensions.position = { rawPosition.x, rawPosition.y, { DimensionType::PIXEL, DimensionType::PIXEL } };

    m_flags.needsDimensionUpdate = true;
}

void vui::Widget::setRawSize(const f32v2& rawSize) {
    m_rawDimensions.size = { rawSize.x, rawSize.y, { DimensionType::PIXEL, DimensionType::PIXEL } };

    m_flags.needsDimensionUpdate = true;
}

void vui::Widget::setRawLeft(f32 rawLeft) {
    m_rawRelativePositions.left = { rawLeft, { DimensionType::PIXEL } };

    m_flags.needsDimensionUpdate = true;
}

void vui::Widget::setRawTop(f32 rawTop) {
    m_rawRelativePositions.top = { rawTop, { DimensionType::PIXEL } };

    m_flags.needsDimensionUpdate = true;
}

void vui::Widget::setRawRight(f32 rawRight) {
    m_rawRelativePositions.right = { rawRight, { DimensionType::PIXEL } };

    m_flags.needsDimensionUpdate = true;
}

void vui::Widget::setRawBottom(f32 rawBottom) {
    m_rawRelativePositions.bottom = { rawBottom, { DimensionType::PIXEL } };

    m_flags.needsDimensionUpdate = true;
}

void vui::Widget::setRawPadding(const f32v4& rawPadding) {
    m_rawPadding = { rawPadding.x, rawPadding.y, rawPadding.z, rawPadding.w, { DimensionType::PIXEL, DimensionType::PIXEL, DimensionType::PIXEL, DimensionType::PIXEL } };

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::setRawPaddingLeft(const Length& rawLeft) {
    m_rawPadding.x = rawLeft.x;
    m_rawPadding.dimension.x = rawLeft.dimension.x;

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::setRawPaddingLeft(f32 rawLeft) {
    m_rawPadding.x = rawLeft;
    m_rawPadding.dimension.x = DimensionType::PIXEL;

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::setRawPaddingTop(const Length& rawTop) {
    m_rawPadding.y = rawTop.x;
    m_rawPadding.dimension.y = rawTop.dimension.x;

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::setRawPaddingTop(f32 rawTop) {
    m_rawPadding.y = rawTop;
    m_rawPadding.dimension.y = DimensionType::PIXEL;

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::setRawPaddingRight(const Length& rawRight) {
    m_rawPadding.z = rawRight.x;
    m_rawPadding.dimension.z = rawRight.dimension.x;

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::setRawPaddingRight(f32 rawRight) {
    m_rawPadding.z = rawRight;
    m_rawPadding.dimension.z = DimensionType::PIXEL;

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::setRawPaddingBottom(const Length& rawBottom) {
    m_rawPadding.w = rawBottom.x;
    m_rawPadding.dimension.w = rawBottom.dimension.x;

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::setRawPaddingBottom(f32 rawBottom) {
    m_rawPadding.w = rawBottom;
    m_rawPadding.dimension.w = DimensionType::PIXEL;

    m_flags.needsClipRectRecalculation = true;
}

void vui::Widget::updateDimensions(f32) {
    if (m_dock.state != DockState::NONE) return;

    auto applyRawPosition = [&](f32v2 modifier = { 0.0f, 0.0f }) {
            f32v2 processedPosition = processLength(m_rawDimensions.position);
            setPosition(processedPosition + modifier);
    };

    auto applyRawSize = [&]() {
            f32v2 processedSize = processLength(m_rawDimensions.size);
            setSize(processedSize);
    };

    auto applyRelativeDirectives = [&](f32v2 position, f32v2 size) {
        f32 left   = processLength(m_rawRelativePositions.left);
        f32 top    = processLength(m_rawRelativePositions.top);
        f32 right  = processLength(m_rawRelativePositions.right);
        f32 bottom = processLength(m_rawRelativePositions.bottom);
        
        f32 sizeX = size.x - left - right;
        if (sizeX < 0.0f) sizeX = 0.0f;
        f32 sizeY = size.y - top - bottom;
        if (sizeY < 0.0f) sizeY = 0.0f;

        setPosition(f32v2(position.x + left, position.y + top));
        setSize(f32v2(sizeX, sizeY));
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

    m_padding = {
        processLength({ m_rawPadding.x, { m_rawPadding.dimension.x } }),
        processLength({ m_rawPadding.y, { m_rawPadding.dimension.y } }),
        processLength({ m_rawPadding.z, { m_rawPadding.dimension.z } }),
        processLength({ m_rawPadding.w, { m_rawPadding.dimension.w } })
    };
    // TODO(Matthew): Check what setDimensions did, it may have had some important side-effects.
}

f32 vui::Widget::processLength(Length length) const {
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

    return 0;
}

f32v2 vui::Widget::processLength(Length2 length) const {
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
