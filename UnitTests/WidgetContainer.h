#pragma once

//
// IWidgetContainer.h
// Vorb Engine
//
// Created by Dylan Scheltens on 27 August 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file WidgetContainer.h
* @brief
* Abstract Container for widgets
*
*/

#pragma once

#ifndef Vorb_Container_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Container_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <include/types.h>
#endif // !VORB_USING_PCH

#ifdef VORB_USING_SCRIPT
#include <include/script/Function.h>
#endif

namespace vorb {
    namespace ui {
        // Forward Declarations
        struct MouseButtonEvent;
        struct MouseMotionEvent;
        struct MouseEvent;
        class Widget;

        class WidgetContainer {
        public:
            /*! @brief Default constructor. */
            WidgetContainer();

            /*! @brief Constructor that sets name, position, and dimensions.
            *
            * @param name: Name of the container.
            */
            WidgetContainer(const nString& name);

            /*! @brief Destructor that unhooks events */
            virtual ~WidgetContainer();

            /*! @brief Enables events* */
            virtual void enable();

            /*! @brief Disables events* */
            virtual void disable();

            /*! @brief Releases all resources used by the Widget.
            *
            * Gets called in the destructor.
            */
            virtual void dispose();

            /*! @brief Adds a child Widget to the container
            *
            * @param child: The Widget to add
            * @return true on success.
            */
            virtual void addWidget(Widget* child);

            /*! @brief Removes a Widget from the container
            *
            * @param child: The Widget to remove
            * @return true on success.
            */
            virtual void removeWidget(Widget* child);

            /*! @brief Updates the widget.*/
            virtual void update(int flags, f32 dt = 1.0f);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const bool isEnabled();
            virtual const bool isClippingEnabled();
            virtual bool isMouseIn();
            virtual const f32& getHeight() const { return m_dimensions.y; }
            virtual const f32& getWidth() const { return m_dimensions.x; }
            virtual const f32& getX() const { return m_position.x; }
            virtual const f32& getY() const { return m_position.y; }
            virtual const f32v2& getDimensions() const { return m_dimensions; }
            virtual const f32v2& getPosition() const { return m_position; }
            virtual const f32v2& getRelativePosition() const { return m_relativePosition; }
            virtual const std::vector<Widget*>& getWidgets() const { return m_widgets; }
            virtual const nString& getName() const { return m_name; }
            virtual f32v4 getRect() const { return f32v4(m_position.x, m_position.y, m_dimensions.x, m_dimensions.y); }
            virtual f32v4 getClipRect() const { return m_clipRect; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setPosition(const f32v2& position) { m_relativePosition = position; update(0x00 | 0x01, 1.0); }
            virtual void setX(f32 x) { m_relativePosition.x = x; update(0x00 | 0x01, 1.0); }
            virtual void setY(f32 y) { m_relativePosition.y = y; update(0x00 | 0x01, 1.0); }
            virtual void setDimensions(const f32v2& dimensions) { m_dimensions = dimensions; updateChilds(0x00 | 0x01); }
            virtual void setWidth(f32 width) { m_dimensions.x = width; updateChilds(0x00 | 0x01); }
            virtual void setHeight(f32 height) { m_dimensions.y = height; updateChilds(0x00 | 0x01); }
            virtual void setName(const nString& name) { m_name = name; }
            virtual void setClippingEnabled(bool enabled);

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<const MouseButtonEvent&> MouseClick; ///< Occurs when container is clicked by mouse.
            Event<const MouseButtonEvent&> MouseDown; ///< Occurs when mouse button is pressed over container.
            Event<const MouseButtonEvent&> MouseUp; ///< Occurs when mouse button is released over container.
            Event<const MouseMotionEvent&> MouseEnter; ///< Occurs when mouse pointer enters the container.
            Event<const MouseMotionEvent&> MouseLeave; ///< Occurs when mouse pointer leaves the container.
            Event<const MouseMotionEvent&> MouseMove; ///< Occurs when mouse pointer is moved over container.

        protected:
            virtual void computeClipRect(const f32v4& parentClipRect = f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX));
            virtual void computeChildClipRects();
            virtual void updateChilds(int flags);

            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseDown(Sender s, const MouseButtonEvent& e);
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e);
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e);
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            std::vector<Widget*> m_widgets; ///< All child widgets.
            std::vector<Widget*> m_dockedWidgets[5]; ///< Widgets that are docked. TODO(Ben): Linked list instead?

            f32v4 m_clipRect = f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX);
            f32v4 m_dockSizes = f32v4(0.0f); ///< Total size of each dock other than fill.
            f32v2 m_relativePosition = f32v2(0.0f); ///< Position relative to parent.
            f32v2 m_position = f32v2(0.0f); ///< The position and dimensions.
            f32v2 m_dimensions = f32v2(0.0f); ///< The position and dimensions.

            nString m_name = ""; ///< Name of the widget.
            int m_widgetFlags = 0; ///< Bitfield that keeps track of the widget's events. (Hover, Enabled, Clicked, etc.)
        };
    }
}

namespace vui = vorb::ui;

#endif // !Vorb_Container_h__