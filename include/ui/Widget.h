//
// Widget.h
// Vorb Engine
//
// Created by Benjamin Arnold on 27 Apr 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Widget.h
* @brief 
* Base class for all Widgets.
*
*/

#pragma once

#ifndef Vorb_Widget_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Widget_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <vector>
#include "../Events.hpp"
#include "../VorbPreDecl.inl"
#include "IWidgetContainer.h"

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        enum class WidgetAlign {
            LEFT,
            TOP_LEFT,
            TOP,
            TOP_RIGHT,
            RIGHT,
            BOTTOM_RIGHT,
            BOTTOM,
            BOTTOM_LEFT,
            CENTER
        };

        enum class PositionType {
            STATIC,
            ABSOLUTE,
            FIXED_TO_WINDOW,
            FIXED_TO_FORM,
            RELATIVE
        };

        enum class DimensionType {
            PIXEL,
            PERCENTAGE,
            FORM_WIDTH_PERC,
            FORM_HEIGHT_PERC,
            FORM_MIN_PERC,
            FORM_MAX_PERC,
            WINDOW_WIDTH_PERC,
            WINDOW_HEIGHT_PERC,
            WINDOW_MIN_PERC,
            WINDOW_MAX_PERC
        };

        // Forward Declarations
        class UIRenderer;

        class Widget : public IWidgetContainer {
            friend class WidgetScriptFuncs;
            friend class IWidgetContainer;
        public:
            /*! @brief Default constructor. */
            Widget();
            /*! @brief Constructor that sets name, position, and dimensions.
             *
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
             *
             * The widget will be made a child of parent.
             *
             * @param parent: Parent container object.
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(IWidgetContainer* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~Widget();
            /*! @brief Releases all resources used by the Widget.
            *
            * Gets called in the destructor.
            */
            virtual void dispose() override;

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to.
            */
            virtual void addDrawables(UIRenderer* renderer);

            /*! @brief Removes all drawables from the UIRenderer */
            virtual void removeDrawables();

            /*! @brief Updates the widget. Can be used for animation.
            *
            * @param dt: The TimeStep
            */
            virtual void update(f32 dt = 1.0f) { }

            /*! @brief Updates the position relative to parent */
            virtual void updatePosition() override;    

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/ 
            virtual const AnchorStyle& getAnchor() const { return m_anchor; }
            virtual const IWidgetContainer* getParent() const { return m_parent; }
            virtual const PositionType& getPositionType() const { return m_positionType; }
            virtual const volatile bool& needsDrawableReload() const { return m_needsDrawableReload; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            virtual const UIRenderer* getRenderer() const { return m_renderer; }
            virtual const std::pair<f32v2, DimensionType>& getRawPosition() const { return m_rawPosition; }
            virtual const std::pair<f32v2, DimensionType>& getRawDimensions() const { return m_rawDimensions; }
            // TODO(Matthew): Add functionality to return in pixels specifically?
            virtual const std::pair<f32v2, DimensionType>& getMinSize() const { return m_minSize; }
            virtual const std::pair<f32v2, DimensionType>& getMaxSize() const { return m_maxSize; }
            virtual const WidgetAlign& getWidgetAlign() const { return m_align; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setAnchor(const AnchorStyle& anchor);
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setNeedsDrawableReload(bool needsDrawableReload) { m_needsDrawableReload = needsDrawableReload; }
            virtual void setParent(IWidgetContainer* parent);
            virtual void setPositionType(PositionType& positionType) { m_positionType = positionType; updatePosition(); }
            virtual void setPosition(const f32v2& position);
            virtual void setRawPosition(const std::pair<f32v2, DimensionType>& rawPosition) { m_rawPosition = rawPosition; updatePosition(); }
            virtual void setRawPosition(const f32v2& rawPosition, DimensionType& dimensionType) { m_rawPosition.first = rawPosition; m_rawPosition.second = dimensionType; updatePosition(); }
            virtual void setDimensions(const f32v2& dimensions);
            virtual void setRawDimensions(const std::pair<f32v2, DimensionType>& rawDimensions) { m_rawDimensions = rawDimensions; updatePosition(); }
            virtual void setRawDimensions(const f32v2& rawDimensions, DimensionType& dimensionType) { m_rawDimensions.first = rawDimensions; m_rawDimensions.second = dimensionType; updatePosition(); }
            virtual void setMaxSize(const std::pair<f32v2, DimensionType>& maxSize) { m_maxSize = maxSize; updatePosition(); }
            virtual void setMaxSize(const f32v2& maxSize, DimensionType& dimensionType) { m_maxSize.first = maxSize; m_maxSize.second = dimensionType; updatePosition(); }
            virtual void setMaxWidth(f32 maxWidth) { m_maxSize.first.x = maxWidth; updatePosition(); }
            virtual void setMaxHeight(f32 maxHeight) { m_maxSize.first.y = maxHeight; updatePosition(); }
            virtual void setMaxSize(const std::pair<f32v2, DimensionType>& minSize) { m_minSize = minSize; updatePosition(); }
            virtual void setMinSize(const f32v2& minSize) { m_minSize.first = minSize; updatePosition(); }
            virtual void setMinWidth(f32 minWidth) { m_minSize.first.x = minWidth; updatePosition(); }
            virtual void setMinHeight(f32 minHeight) { m_minSize.first.y = minHeight; updatePosition(); }
            virtual void setWidgetAlign(WidgetAlign align) { m_align = align; updatePosition(); }
            
        protected:
            virtual f32v2 getWidgetAlignOffset();
            virtual void updateDimensions();
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            WidgetAlign m_align = WidgetAlign::TOP_LEFT;
            AnchorStyle m_anchor; ///< The anchor data.
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            UIRenderer* m_renderer = nullptr;
            IWidgetContainer* m_parent = nullptr; ///< Parent container
            PositionType m_positionType = PositionType::STATIC;
            std::pair<f32v2, DimensionType> m_minSize = std::pair<f32v2, DimensionType>(f32v2(0.0f), DimensionType::PIXEL);
            std::pair<f32v2, DimensionType> m_maxSize = std::pair<f32v2, DimensionType>(f32v2(FLT_MAX), DimensionType::PIXEL);
            std::pair<f32v2, DimensionType> m_rawPosition = std::pair<f32v2, DimensionType>(f32v2(0.0f), DimensionType::PIXEL);
            std::pair<f32v2, DimensionType> m_rawDimensions = std::pair<f32v2, DimensionType>(f32v2(0.0f), DimensionType::PIXEL);
            volatile bool m_needsDrawableReload = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
