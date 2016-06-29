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
#include "Form.h"

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
            FIXED,
            RELATIVE
        };

        enum class UnitType {
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
            virtual const Widget* getParentWidget() const { return m_parentWidget; }
            virtual const IWidgetContainer* getFirstPositionedParent() const;
            virtual const AnchorStyle& getAnchor() const { return m_anchor; }
            virtual const PositionType& getPositionType() const { return m_positionType; }
            virtual const volatile bool& needsDrawableReload() const { return m_needsDrawableReload; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            virtual const UIRenderer* getRenderer() const { return m_renderer; }
            virtual const std::pair<f32v2, UnitType>& getRawPosition() const { return m_rawPosition; }
            virtual const std::pair<f32v2, UnitType>& getRawDimensions() const { return m_rawDimensions; }
            virtual const std::pair<f32v2, UnitType>& getMinSize() const { return m_rawMinSize; }
            virtual const std::pair<f32v2, UnitType>& getMaxSize() const { return m_rawMaxSize; }
            virtual const WidgetAlign& getWidgetAlign() const { return m_align; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setParentWidget(Widget* parent);
            virtual void setAnchor(const AnchorStyle& anchor) { m_anchor = anchor; }
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setNeedsDrawableReload(bool needsDrawableReload) { m_needsDrawableReload = needsDrawableReload; }
            virtual void setPositionType(PositionType& positionType) { m_positionType = positionType; updatePosition(); }
            virtual void setPosition(const f32v2& position);
            virtual void setRawPosition(const std::pair<f32v2, UnitType>& rawPosition) { m_rawPosition = rawPosition; updatePosition(); }
            virtual void setRawPosition(const f32v2& rawPosition, UnitType& UnitType) { m_rawPosition.first = rawPosition; m_rawPosition.second = UnitType; updatePosition(); }
            virtual void setDimensions(const f32v2& dimensions);
            virtual void setRawDimensions(const std::pair<f32v2, UnitType>& rawDimensions) { m_rawDimensions = rawDimensions; updateDimensions(); }
            virtual void setRawDimensions(const f32v2& rawDimensions, UnitType& UnitType) { m_rawDimensions.first = rawDimensions; m_rawDimensions.second = UnitType; updateDimensions(); }
            virtual void setRawMaxSize(const std::pair<f32v2, UnitType>& maxSize) { m_rawMaxSize = maxSize; updateMaxSize(); }
            virtual void setRawMaxSize(const f32v2& maxSize, UnitType& UnitType) { m_rawMaxSize.first = maxSize; m_rawMaxSize.second = UnitType; updateMaxSize(); }
            virtual void setRawMaxWidth(f32 maxWidth) { m_rawMaxSize.first.x = maxWidth; updateMaxSize(); }
            virtual void setRawMaxHeight(f32 maxHeight) { m_rawMaxSize.first.y = maxHeight; updateMaxSize(); }
            virtual void setMaxSize(const f32v2& maxSize);
            virtual void setRawMinSize(const std::pair<f32v2, UnitType>& minSize) { m_rawMinSize = minSize; updateMinSize(); }
            virtual void setRawMinSize(const f32v2& minSize) { m_rawMinSize.first = minSize; updateMinSize(); }
            virtual void setRawMinWidth(f32 minWidth) { m_rawMinSize.first.x = minWidth; updateMinSize(); }
            virtual void setRawMinHeight(f32 minHeight) { m_rawMinSize.first.y = minHeight; updateMinSize(); }
            virtual void setMinSize(const f32v2& minSize);
            virtual void setWidgetAlign(WidgetAlign align) { m_align = align; updatePosition(); }
            
        protected:
            virtual f32v2 getWidgetAlignOffset();
            virtual void updateDimensions();
            virtual void updateMaxSize() { m_maxSize = processRawValue(m_rawMaxSize); updateDimensions(); }
            virtual void updateMinSize() { m_minSize = processRawValue(m_rawMinSize); updateDimensions(); }

            f32v2 processRawValue(std::pair<f32v2, UnitType> rawValue) { processRawValue(rawValue.first, rawValue.second); }
            f32v2 processRawValue(f32v2 rawValue, UnitType units);
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            Widget* m_parentWidget = nullptr; ///< Parent widget. Nullptr implies widget is directly under its parent form.
            WidgetAlign m_align = WidgetAlign::TOP_LEFT;
            AnchorStyle m_anchor; ///< The anchor data.
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            UIRenderer* m_renderer = nullptr;
            PositionType m_positionType = PositionType::STATIC;
            std::pair<f32v2, UnitType> m_rawPosition = std::pair<f32v2, UnitType>(f32v2(0.0f), UnitType::PIXEL);
            std::pair<f32v2, UnitType> m_rawDimensions = std::pair<f32v2, UnitType>(f32v2(0.0f), UnitType::PIXEL);
            std::pair<f32v2, UnitType> m_rawMinSize = std::pair<f32v2, UnitType>(f32v2(0.0f), UnitType::PIXEL);
            f32v2 m_minSize = f32v2(0.0f);
            std::pair<f32v2, UnitType> m_rawMaxSize = std::pair<f32v2, UnitType>(f32v2(FLT_MAX), UnitType::PIXEL);
            f32v2 m_maxSize = f32v2(FLT_MAX);
            volatile bool m_needsDrawableReload = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
