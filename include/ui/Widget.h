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
            FORM_MAX_PERC
        };

        /*! Stores two lengths and their units. */
        struct Length2 {
            f32 x, y;
            struct {
                UnitType x, y;
            } units;
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
            //Widget(IWidgetContainer* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~Widget();
            /*! @brief Releases all resources used by the Widget.
            *
            * Gets called in the destructor.
            */
            virtual void dispose() override;

            /*! @brief Adds a widget to the Form and initializes it for rendering.
            *
            * @param widget: The Widget to add.
            * @return true on success.
            */
            virtual bool addWidget(Widget* widget);

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
            virtual Widget* getParentWidget() const { return m_parentWidget; }
            virtual const IWidgetContainer* getFirstPositionedParent() const;
            virtual const AnchorStyle& getAnchor() const { return m_anchor; }
            virtual const PositionType& getPositionType() const { return m_positionType; }
            virtual const volatile bool& needsDrawableReload() const { return m_needsDrawableReload; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            virtual const UIRenderer* getRenderer() const { return m_renderer; }
            virtual const Length2& getRawPosition() const { return m_rawPosition; }
            virtual const f32v2& getRelativePosition() const { return m_relativePosition; }
            virtual const Length2& getRawDimensions() const { return m_rawDimensions; }
            virtual const Length2& getRawMinSize() const { return m_rawMinSize; }
            virtual const f32v2& getMinSize() const { return m_minSize; }
            virtual const Length2& getRawMaxSize() const { return m_rawMaxSize; }
            virtual const f32v2& getMaxSize() const { return m_maxSize; }
            virtual const WidgetAlign& getWidgetAlign() const { return m_align; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setParentWidget(Widget* parent);
            virtual void setAnchor(const AnchorStyle& anchor) { m_anchor = anchor; }
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setNeedsDrawableReload(bool needsDrawableReload) { m_needsDrawableReload = needsDrawableReload; }
            virtual void setPositionType(const PositionType& positionType) { m_positionType = positionType; updatePosition(); }
            virtual void setPosition(const f32v2& position);
            virtual void setRawPosition(const Length2& rawPosition) { m_rawPosition = rawPosition; updatePosition(); }
            virtual void setRawPosition(const f32v2& rawPosition, UnitType& units);
            virtual void setRawPositionX(f32 value, UnitType& units) { m_rawPosition.x = value; m_rawPosition.units.x = units; updatePosition(); }
            virtual void setRawPositionY(f32 value, UnitType& units) { m_rawPosition.y = value; m_rawPosition.units.y = units; updatePosition(); }
            virtual void setDimensions(const f32v2& dimensions);
            virtual void setRawDimensions(const Length2& rawDimensions) { m_rawDimensions = rawDimensions; updateDimensions(); }
            virtual void setRawDimensions(const f32v2& rawDimensions, UnitType& units);
            virtual void setRawWidth(f32 value, UnitType& units) { m_rawDimensions.x = value; m_rawDimensions.units.x = units; updateDimensions(); }
            virtual void setRawHeight(f32 value, UnitType& units) { m_rawDimensions.y = value; m_rawDimensions.units.y = units; updateDimensions(); }
            virtual void setRawMaxSize(const Length2& maxSize) { m_rawMaxSize = maxSize; updateMaxSize(); }
            virtual void setRawMaxSize(const f32v2& maxSize, UnitType& units);
            virtual void setRawMaxWidth(f32 maxWidth, UnitType& units) { m_rawMaxSize.x = maxWidth; m_rawMaxSize.units.x = units; updateMaxSize(); }
            virtual void setRawMaxHeight(f32 maxHeight, UnitType& units) { m_rawMaxSize.y = maxHeight; m_rawMaxSize.units.y = units; updateMaxSize(); }
            virtual void setMaxSize(const f32v2& maxSize);
            virtual void setRawMinSize(const Length2& minSize) { m_rawMinSize = minSize; updateMinSize(); }
            virtual void setRawMinSize(const f32v2& minSize, UnitType& units);
            virtual void setRawMinWidth(f32 minWidth, UnitType& units) { m_rawMinSize.x = minWidth; m_rawMinSize.units.x = units; updateMinSize(); }
            virtual void setRawMinHeight(f32 minHeight, UnitType& units) { m_rawMinSize.y = minHeight; m_rawMinSize.units.y = units; updateMinSize(); }
            virtual void setMinSize(const f32v2& minSize);
            virtual void setWidgetAlign(WidgetAlign align) { m_align = align; updatePosition(); }
            
        protected:
            virtual f32v2 getWidgetAlignOffset();
            /*! @brief Updates the dimensions of the widget based on processed positioning and size boundaries. */
            virtual void updateDimensions();
            /*! @brief Processes the raw maximum size then updates the dimensions appropriately. */
            virtual void updateMaxSize() { m_maxSize = processRawValues(m_rawMaxSize); updateDimensions(); }
            /*! @brief Processes the raw minimum size then updates the dimensions appropriately. */
            virtual void updateMinSize() { m_minSize = processRawValues(m_rawMinSize); updateDimensions(); }

            /*! @brief Processes a set of raw values and converts them to processed values that can be used for basic calculations. */
            f32v2 processRawValues(const Length2& rawValues);
            f32v2 processRawValue(const f32v2& rawValue, const UnitType& unit);
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            Widget* m_parentWidget = nullptr; ///< Parent widget. Nullptr implies widget is directly under its parent form.
            WidgetAlign m_align = WidgetAlign::TOP_LEFT; ///< Direction of alignment of the widget.
            AnchorStyle m_anchor; ///< The anchor data.
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            UIRenderer* m_renderer = nullptr; ///< The renderer to be used by the widget.
            PositionType m_positionType = PositionType::STATIC; ///< The type of positioning of the widget.
            Length2 m_rawPosition; ///< The raw position of the widget.
            // TODO(Matthew): Do we need this?
            f32v2 m_relativePosition = f32v2(0.0f); ///< The relative position of the widget.
            Length2 m_rawDimensions; ///< The raw dimensions of the widget.
            Length2 m_rawMinSize; ///< The raw minimum dimensions of the widget.
            f32v2 m_minSize = f32v2(0.0f); ///< The processed minimum dimensions of the widget.
            Length2 m_rawMaxSize; ///< The raw maximum dimensions of the widget.
            f32v2 m_maxSize = f32v2(FLT_MAX); ///< The processed maximum dimensions of the widget.
            volatile bool m_needsDrawableReload = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
