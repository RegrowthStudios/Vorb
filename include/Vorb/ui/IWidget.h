//
// IWidget.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IWidget.h
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
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#ifdef VORB_USING_SCRIPT
#include "Vorb/script/Function.h"
#endif

#include <vector>

#include "Vorb/Events.hpp"
#include "Vorb/VorbPreDecl.inl"

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        // Forward Declarations
        struct MouseButtonEvent;
        struct MouseMotionEvent;
        struct MouseEvent;
        class Widget;

        // //! Bitfield of container styling flags
        // struct ContainerStyle {
        //     bool fixedHeight : 1; ///< If true, the control has fixed height when auto-scaled.
        //     bool fixedWidth : 1; ///< If true, the control has fixed width when auto-scaled.
        //     bool selectable : 1; ///< If true, the control can receive focus.
        // };
        //! Bitfield of anchor flags
        struct AnchorStyle {
            bool left : 1; ///< If true, anchored to the left of parent
            bool right : 1; ///< If true, anchored to the right of parent
            bool bottom : 1; ///< If true, anchored to the bottom of parent
            bool top : 1; ///< If true, anchored to the top of parent
        };
        //! Bitfield of dock flags
        enum class DockStyle {
            NONE = 0, LEFT, RIGHT, BOTTOM, TOP, FILL
        };

        // Forward Declarations
        class UIRenderer;

        class IWidget {
            using IWidgets = std::vector<IWidget*>;
            using Font = vorb::graphics::SpriteFont;
        public:
            /*! @brief Default constructor. */
            IWidget();
            /*! @brief Constructor that sets name, position, and dimensions.
             *
             * @param name: Name of the widget.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            IWidget(const nString& name, const f32v4& destRect = f32v4(0));     
            /*! @brief Destructor that unhooks events */
            virtual ~IWidget();
            /*! @brief Releases all resources used by the Widget.
             *
             * Gets called in the destructor.
             */
            virtual void dispose();
            /*! @brief Childs another widget to this widget.
             *
             * @param child: The Widget to add
             * @return true on success.
             */
            virtual bool addWidget(IWidget* child);
            /*! @brief Unchilds a child widget of this widget.
            *
            * @param child: The Widget to remove
            * @return true on success.
            */
            virtual bool removeWidget(IWidget* child);

            /*! \brief Updates the dimensions of the new IWidget according to specific widget rules.
             *
             *  The simplest form could be m_position = m_relativePosition;
             */
            virtual void updateDimensions() = 0;

            /*! @brief Enables events* */
            virtual void enable();
            /*! @brief Disables events* */
            virtual void disable();
            /*! @brief Checks if a point is inside the container
             *
             * @param point: The point to check
             * @return true if point is in the bounds of the container
             */
            virtual bool isInBounds(const f32v2& point) const { return isInBounds(point.x, point.y); }
            virtual bool isInBounds(f32 x, f32 y) const;

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to.
            */
            virtual void addDrawables(UIRenderer* renderer);

            /*! @brief Removes all drawables from the UIRenderer */
            virtual void removeDrawables();

            // virtual void setChildDock(Widget* widget, DockStyle dockStyle);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            // virtual bool getFixedHeight() const { return m_style.fixedHeight; }
            // virtual bool getFixedWidth() const { return m_style.fixedWidth; }
            // virtual bool getSelectable() const { return m_style.selectable; }
            // virtual const ContainerStyle& getStyle() const { return m_style; }
            virtual       const Font* getFont()             const { return m_font; }
            virtual const UIRenderer* getRenderer()         const { return m_renderer; }
            virtual          IWidget* getCanvas()           const { return m_canvas; }
            virtual          IWidget* getParent()           const { return m_parent; }
            virtual   const IWidgets& getWidgets()          const { return m_widgets; }
            virtual             f32v4 getClipRect()         const { return m_clipRect; }
            virtual             f32v4 getDestRect()         const { return f32v4(m_position.x, m_position.y, m_size.x, m_size.y); }
            virtual        const f32& getX()                const { return m_position.x; }
            virtual        const f32& getY()                const { return m_position.y; }
            virtual      const f32v2& getPosition()         const { return m_position; }
            virtual        const f32& getWidth()            const { return m_size.x; }
            virtual        const f32& getHeight()           const { return m_size.y; }
            virtual      const f32v2& getSize()             const { return m_size; }
            virtual    const nString& getName()             const { return m_name; }
            // virtual const bool& getClippingEnabled() const { return m_isClippingEnabled; }
            virtual       const bool& isEnabled()           const { return m_isEnabled; }
            virtual              bool isMouseIn()           const { return m_isMouseIn; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/         
            // virtual void setDestRect(const f32v4& destRect);
            // virtual void setSize(const f32v2& size) { m_size = size; updateChildPositions(); }
            // virtual void setFixedHeight(bool fixedHeight) { m_style.fixedHeight = fixedHeight; }
            // virtual void setFixedWidth(bool fixedWidth) { m_style.fixedWidth = fixedWidth; }
            // virtual void setWidth(f32 width) { m_size.x = width;  updateChildPositions(); }
            // virtual void setHeight(f32 height) { m_size.y = height;  updateChildPositions(); }
            // virtual void setPosition(const f32v2& position) { m_relativePosition = position; updatePosition(); }
            // virtual void setSelectable(bool selectable) { m_style.selectable = selectable; }
            // virtual void setStyle(const ContainerStyle& style) { m_style = style; }
            // virtual void setX(f32 x) { m_relativePosition.x = x; updatePosition(); }
            // virtual void setY(f32 y) { m_relativePosition.y = y; updatePosition(); }
            // virtual void setClippingEnabled(bool isClippingEnabled) { m_isClippingEnabled = isClippingEnabled; updatePosition(); }
            /*!
             * \brief Sets the parent widget of this widget.
             * 
             * \warning This function could end up being costly if called too often - has to traverse all descendant widgets and sometimes all ancestor widgets.
             */
            virtual void setFont(const Font* font)    { m_font = font; }
            virtual void setParent(IWidget* parent);
            virtual void setName(const nString& name) { m_name = name; }

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
            /*! Removes a widget from a dock and returns true on success. */
            // bool removeChildFromDock(Widget* widget);
            /*! Refreshes all docked widget positions and sizes. */
            // void recalculateDockedWidgets();
            /*! Computes clipping for rendering and propagates through children. */
            // virtual void computeClipRect(const f32v4& parentClipRect = f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX));
            // virtual void computeChildClipRects();
            /*!
             * \brief Updates all child widgets' dimensions.
             */
            virtual void updateChildDimensions();
            /*!
             * \brief Updates all child widgets' canvas fields.
             */
            virtual void updateChildCanvases();
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
            const Font* m_font;                                                              ///< Font for rendering.
            UIRenderer* m_renderer;                                                              ///< Renderer to use for drawing widget.    
            // ContainerStyle m_style;            ///< The current style.
            IWidget*    m_canvas;           ///< Canvas widget - i.e. the oldest ancestor of this widget. If this widget is the canvas of its children, m_canvas is set to the this pointer (which is the case by default). NOTE: We should never update this parameter via a setter, it should only change via private logic on parent changes.
            IWidget*    m_parent;           ///< Parent widget.
            IWidgets    m_widgets;          ///< Collection of child widgets.
            // std::vector<Widget*> m_dockedWidgets[5]; ///< Widgets that are docked. TODO(Ben): Linked list instead?
            f32v4       m_clipRect;         ///< Clipping rectangle for rendering.
            // f32v4    m_dockSizes;        ///< Total size of each dock other than fill.
            f32v2       m_position;         ///< Position of widget relative to window in pixels.
            f32v2       m_size;             ///< Size of the widget in pixels.
            nString     m_name;             ///< Display name of the container.

            // bool m_isClippingEnabled = true;

            // TODO(Ben): Bitfield for memory reduction?.
            bool m_isClicking; ///< Used for click event tracking.
            bool m_isEnabled; ///< True when events are enabled.
            bool m_isMouseIn; ///< Used for motion event tracking.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_IContainer_h__
