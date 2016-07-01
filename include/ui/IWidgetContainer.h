//
// IWidgetContainer.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IWidgetContainer.h
* @brief 
* Abstract Container for widgets
*
*/

#pragma once

#ifndef Vorb_IContainer_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IContainer_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#ifdef VORB_USING_SCRIPT
#include "../script/Function.h"
#endif

namespace vorb {
    namespace ui {

        // Forward Declarations
        struct MouseButtonEvent;
        struct MouseMotionEvent;
        struct MouseEvent;
        class Widget;
        class Form;

        //! Bitfield of container styling flags
        struct ContainerStyle {
            bool fixedHeight : 1; ///< If true, the control has fixed height when auto-scaled.
            bool fixedWidth : 1; ///< If true, the control has fixed width when auto-scaled.
            bool selectable : 1; ///< If true, the control can receive focus.
        };

        //! Bitfield of anchor flags
        struct AnchorStyle {
            bool left : 1; ///< If true, anchored to the left of parent
            bool right : 1; ///< If true, anchored to the right of parent
            bool bottom : 1; ///< If true, anchored to the bottom of parent
            bool top : 1; ///< If true, anchored to the top of parent
        };

        //! Bitfield of clipping options.
        struct ClippingOptions {
            ClippingOptions() : left{ true },
                top{ true },
                right{ true },
                bottom{ true }
            {
            }
            ClippingOptions(bool left, bool top, bool right, bool bottom) : 
                left{ left },
                top{ top },
                right{ right },
                bottom{ bottom }
            {
            }

            bool left : 1;
            bool top : 1;
            bool right : 1;
            bool bottom : 1;
        };

        class IWidgetContainer {
        public:
            /*! @brief Default constructor. */
            IWidgetContainer();
            /*! @brief Constructor that sets name, position, and dimensions.
             *
             * @param name: Name of the container.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            IWidgetContainer(const nString& name, const f32v4& destRect = f32v4(0));     
            /*! @brief Destructor that unhooks events */
            virtual ~IWidgetContainer();
            /*! @brief Releases all resources used by the Widget.
             *
             * Gets called in the destructor.
             */
            virtual void dispose();
            /*! @brief Removes a Widget from the container
            *
            * @param child: The Widget to remove
            * @return true on success.
            */
            virtual bool removeWidget(Widget* child);

            /*! @brief Enables events* */
            virtual void enable();
            /*! @brief Disables events* */
            virtual void disable();

            /*! @brief Updates all spatial state. I.e. position, dimensions, clipping and the same for children. */
            void updateSpatialState();
            /*! @brief Updates position spatial state. I.e. position, clipping and same for children. */
            void updatePositionState();
            /*! @brief Updates dimension spatial state. I.e. dimensions, clipping and same for children. */
            void updateDimensionState();
            /*! @brief Updates clipping for both this widget container and its children. */
            void updateClippingState();

            /*! @brief Checks if a point is inside the container
             *
             * @param point: The point to check
             * @return true if point is in the bounds of the container
             */
            virtual bool isInBounds(const f32v2& point) const { return isInBounds(point.x, point.y); }
            virtual bool isInBounds(f32 x, f32 y) const;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual Form* getParentForm() const { return m_parentForm; }
            virtual Widget* getParentWidget() const { return m_parentWidget; }
            virtual bool getFixedHeight() const { return m_style.fixedHeight; }
            virtual bool getFixedWidth() const { return m_style.fixedWidth; }
            virtual bool getSelectable() const { return m_style.selectable; }
            virtual bool isMouseIn() const { return m_isMouseIn; }      
            virtual const ContainerStyle& getStyle() const { return m_style; }
            virtual const bool& isEnabled() const { return m_isEnabled; }
            virtual const f32& getHeight() const { return m_dimensions.y; }
            virtual const f32& getWidth() const { return m_dimensions.x; }
            virtual const f32& getX() const { return m_position.x; }
            virtual const f32& getY() const { return m_position.y; }
            virtual const f32v2& getDimensions() const { return m_dimensions; }
            virtual const f32v2& getPosition() const { return m_position; }
            virtual const std::vector<Widget*>& getWidgets() const { return m_widgets; }
            virtual const nString& getName() const { return m_name; }
            virtual f32v4 getDestRect() const { return f32v4(m_position.x, m_position.y, m_dimensions.x, m_dimensions.y); }
            virtual const ClippingOptions& getOverflowOptions() const { return m_clippingOptions; }
            virtual f32v4 getClipRect() const { return m_clipRect; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setParentForm(Form* form);
            virtual void setParentWidget(Widget* parent, Widget* self);
            virtual void setDestRect(const f32v4& destRect);
            virtual void setDimensions(const f32v2& dimensions) { m_dimensions = dimensions; updateDimensionState(); }
            virtual void setFixedHeight(bool fixedHeight) { m_style.fixedHeight = fixedHeight; }
            virtual void setFixedWidth(bool fixedWidth) { m_style.fixedWidth = fixedWidth; }
            virtual void setHeight(f32 height) { m_dimensions.y = height;  updateDimensionState(); }
            virtual void setWidth(f32 width) { m_dimensions.x = width;  updateDimensionState(); }
            virtual void setPosition(const f32v2& position) { m_position = position; updatePositionState(); }
            virtual void setX(f32 x) { m_position.x = x; updatePositionState(); }
            virtual void setY(f32 y) { m_position.y = y; updatePositionState(); }
            virtual void setSelectable(bool selectable) { m_style.selectable = selectable; }
            virtual void setStyle(const ContainerStyle& style) { m_style = style; }
            virtual void setName(const nString& name) { m_name = name; }
            virtual void setClipping(const ClippingOptions& clippingOptions) { m_clippingOptions = clippingOptions; updateClippingState(); }
            virtual void setClipping(bool clipping) { m_clippingOptions = ClippingOptions(clipping, clipping, clipping, clipping); updateClippingState(); }
            virtual void setClippingTop(bool clipping) { m_clippingOptions.top = clipping; updateClippingState(); }
            virtual void setClippingRight(bool clipping) { m_clippingOptions.right = clipping; updateClippingState(); }
            virtual void setClippingBottom(bool clipping) { m_clippingOptions.bottom = clipping; updateClippingState(); }
            virtual void setClippingLeft(bool clipping) { m_clippingOptions.left = clipping; updateClippingState(); }

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<const MouseButtonEvent&> MouseClick; ///< Occurs when container is clicked by mouse.
            Event<const MouseButtonEvent&> MouseDown; ///< Occurs when mouse button is pressed over container.
            Event<const MouseButtonEvent&> MouseUp; ///< Occurs when mouse button is released over container.
            Event<const MouseMotionEvent&> MouseEnter; ///< Occurs when mouse pointer enters the container.
            Event<const MouseMotionEvent&> MouseLeave; ///< Occurs when mouse pointer leaves the container.
            Event<const MouseMotionEvent&> MouseMove; ///< Occurs when mouse pointer is moved over container.
            // TODO(Ben): Lots more events!

        protected:
            /*! @brief Adds a child Widget to the container
            *
            * @param child: The Widget to add
            * @return true on success.
            */
            virtual bool addWidget(Widget* child, Widget* self);
            virtual bool addWidget(Widget* child, Form* self);
            
            /*! @brief Computes clipping for this widget container. */
            virtual void computeClipRect();


            /*! @brief Calls updateSpatialState on all children. */
            virtual void updateChildSpatialStates();
            /*! @brief Calls updateClippingState on all children. */
            virtual void updateChildClippingStates();

            /*! @brief Defines how raw position is used to process the actual position. */
            virtual void updatePosition() = 0;
            /*! @brief Defines how raw dimensions are used to process the actual dimensions. */
            virtual void updateDimensions() = 0;
            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseDown(Sender s, const MouseButtonEvent& e);
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e);
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e);
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e);

            /************************************************************************/
            /* LUA Callbacks                                                        */
            /************************************************************************/
#ifdef VORB_USING_SCRIPT
            std::vector<script::Function> m_mouseClickFuncs;
            std::vector<script::Function> m_mouseDownFuncs;
            std::vector<script::Function> m_mouseUpFuncs;
            std::vector<script::Function> m_mouseEnterFuncs;
            std::vector<script::Function> m_mouseLeaveFuncs;
            std::vector<script::Function> m_mouseMoveFuncs;
#endif

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            ContainerStyle m_style; ///< The current style.
            Form* m_parentForm = nullptr; ///< Parent form.
            Widget* m_parentWidget = nullptr; ///< Parent widget.
            std::vector<Widget*> m_widgets; ///< All child widgets.
            f32v4 m_clipRect = f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX); ///< The clipping rectangle of the container.
            f32v2 m_position = f32v2(0.0f); ///< The position of the container.
            f32v2 m_dimensions = f32v2(0.0f); ///< The dimensions of the container.
            nString m_name = ""; ///< Display name of the container.

            ClippingOptions m_clippingOptions; ///< Determines directions in which clipping may or may not occur.

            // TODO(Ben): Bitfield for memory reduction?.
            bool m_isClicking = false; ///< Used for click event tracking.
            bool m_isEnabled = false; ///< True when events are enabled.
            bool m_isMouseIn = false; ///< Used for motion event tracking.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_IContainer_h__
