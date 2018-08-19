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
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <vector>
#include "Vorb/Events.hpp"
#include "Vorb/VorbPreDecl.inl"
#include "Vorb/ui/IWidget.h"

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

        /*!
         * \brief Enum of ways a widget may be positioned.
         */
        enum class PositionType {
            STATIC,
            ABSOLUTE,
            FIXED_TO_WINDOW,
            FIXED_TO_FORM,
            RELATIVE
        };

        /*!
         * \brief Enum of ways the dimensions of a widget may be measured.
         */
        enum class DimensionType {
            PIXEL,
            PERCENTAGE,
            FORM_WIDTH_PERCENTAGE,
            FORM_HEIGHT_PERCENTAGE,
            FORM_MIN_PERCENTAGE,
            FORM_MAX_PERCENTAGE,
            WINDOW_WIDTH_PERCENTAGE,
            WINDOW_HEIGHT_PERCENTAGE,
            WINDOW_MIN_PERCENTAGE,
            WINDOW_MAX_PERCENTAGE,
        };

        /*!
         * \brief Type that stores a length and the dimensions of that length.
         */
        struct Length {
            f32 x;
            struct {
                DimensionType x;
            } dimension;
        };

        /*!
         * \brief Type that stores two lengths and the dimensions of those lengths.
         */
        struct Length2 {
            f32 x, y;
            struct {
                DimensionType x, y;
            } dimension;
        };

        // Forward Declarations
        class UIRenderer;

        class Widget : public IWidget {
            friend class WidgetScriptFuncs;
            friend class IWidget;
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
            Widget(IWidget* parent, const nString& name, const f32v4& destRect = f32v4(0));
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
            virtual void update(f32 dt VORB_UNUSED = 1.0f) { }

            /*! @brief Updates the position relative to parent */
            virtual void updatePosition() override;    

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/ 
            virtual const AnchorStyle& getAnchor() const { return m_anchor; }
            virtual const DockStyle& getDock() const { return m_dock; }
            virtual const IWidget* getParent() const { return m_parent; }
            virtual const volatile bool& needsDrawableReload() const { return m_needsDrawableReload; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            virtual const UIRenderer* getRenderer() const { return m_renderer; }
            virtual const f32v2& getMinSize() const { return m_minSize; }
            virtual const f32v2& getMaxSize() const { return m_maxSize; }
            virtual PositionType getPositionType() const { return m_positionType; }
            virtual const f32v2& getPositionPercentage() const { return m_positionPercentage; }
            virtual const f32v2& getDimensionsPercentage() const { return m_dimensionsPercentage; }
            virtual const WidgetAlign& getWidgetAlign() const { return m_align; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setAnchor(const AnchorStyle& anchor);
            virtual void setDock(const DockStyle& dock);
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setNeedsDrawableReload(bool needsDrawableReload) { m_needsDrawableReload = needsDrawableReload; }
            virtual void setParent(IWidget* parent);
            virtual void setPositionType(PositionType positionType) { m_positionType = positionType; updatePosition(); }
            virtual void setPositionPercentage(const f32v2& positionPercentage) { m_positionPercentage = positionPercentage; updatePosition(); }
            virtual void setDimensionsPercentage(const f32v2& dimensionsPercentage) { m_dimensionsPercentage = dimensionsPercentage; updateDimensions(); }
            virtual void setXPercentage(f32 xPercentage) { m_positionPercentage.x = xPercentage; updatePosition(); }
            virtual void setYPercentage(f32 yPercentage) { m_positionPercentage.y = yPercentage; updatePosition(); }
            virtual void setMaxSize(const f32v2& maxSize) { m_maxSize = maxSize; updatePosition(); }
            virtual void setMinSize(const f32v2& minSize) { m_minSize = minSize; updatePosition(); }
            virtual void setWidthPercentage(f32 widthPercentage) { m_dimensionsPercentage.x = widthPercentage; updateDimensions(); }
            virtual void setHeightPercentage(f32 heightPercentage) { m_dimensionsPercentage.y = heightPercentage; updateDimensions(); }
            virtual void setWidgetAlign(WidgetAlign align) { m_align = align; updatePosition(); }
            
        protected:
            virtual f32v2 getWidgetAlignOffset();
            virtual void updateDimensions();
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            WidgetAlign m_align = WidgetAlign::TOP_LEFT;
            AnchorStyle m_anchor; ///< The anchor data.
            DockStyle m_dock = DockStyle::NONE; ///< The dock type.
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            UIRenderer* m_renderer = nullptr;
            f32v2 m_minSize = f32v2(0.0f);
            f32v2 m_maxSize = f32v2(FLT_MAX);
            f32v2 m_positionPercentage = f32v2(-1.0f); ///< Position as percentage of parent dims
            f32v2 m_dimensionsPercentage = f32v2(-1.0f); ///< Dims as percentage of parent dims
            IWidget* m_parent = nullptr; ///< Parent container
            PositionType m_positionType;
            volatile bool m_needsDrawableReload = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
