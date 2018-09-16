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
        struct WindowResizeEvent;
        class Widget;

        //! Enum of clipping states.
        enum class ClippingState {
            VISIBLE = 0,
            HIDDEN,
            INHERIT
        };
        //! Bitfield of clipping flags.
        struct Clipping {
            ClippingState left   : 2;
            ClippingState top    : 2;
            ClippingState right  : 2;
            ClippingState bottom : 2;
        };
        const Clipping DEFAULT_CLIPPING = {
            ClippingState::VISIBLE,
            ClippingState::VISIBLE,
            ClippingState::VISIBLE,
            ClippingState::VISIBLE
        };
        const f32v4 DEFAULT_CLIP_RECT = f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX);

        //! Bitfield of docking states.
        enum class DockState {
            NONE = 0,
            LEFT,
            RIGHT,
            BOTTOM,
            TOP,
            FILL
        };
        //! Struct of docking state and size.
        struct Dock {
            DockState state;
            f32       size;
        };

        //! Bitfield of widget flags
        struct WidgetFlags {
            bool isClicking : 1; ///< Used for click event tracking.
            bool isEnabled  : 1; ///< True when events are enabled.
            bool isMouseIn  : 1; ///< Used for motion event tracking.

            volatile bool needsDimensionUpdate       : 1; ///< Whether we need to recalculate widget dimensions.
            volatile bool needsZIndexReorder         : 1; ///< Whether we need to recalculate the order of widgets based on their z-index values.
            volatile bool needsDockRecalculation     : 1; ///< Whether we need to recalculate docking of child widgets.
            volatile bool needsClipRectRecalculation : 1; ///< Whether we need to recalculate the clip rectangle.
            volatile bool needsDrawableRefresh       : 1; ///< Whether we need to refresh our drawables.
            volatile bool needsDrawableReregister;        ///< Whether we need to reregister our drawables.
        };

        // Forward Declarations
        class UIRenderer;
        class Viewport;

        class IWidget {
            using IWidgets = std::vector<IWidget*>;
            using Font = vorb::graphics::SpriteFont;
        public:
            /*! @brief Constructor default and parametised to set renderer and window.
             * 
             * @param renderer: The renderer to use for drawing widget.
             * @param window: The window in which to set the UI.
             */
            IWidget();
            // /*! @brief Constructor that sets name, position, and dimensions.
            //  *
            //  * @param name: Name of the widget.
            //  * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            //  */
            // IWidget(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~IWidget();
            /*! @brief Releases all resources used by the Widget.
             *
             * Gets called in the destructor.
             */
            virtual void dispose();

            /*! @brief Updates the widget. Can be used for animation.
            *
            * @param dt: The TimeStep
            */
            virtual void update(f32 dt VORB_MAYBE_UNUSED = 1.0f);

            /*! @brief Enables events that all widgets share in common. */
            virtual void enable();
            /*! @brief Disables events that all widgets share in common. */
            virtual void disable();

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

            /*! @brief Checks if a point is inside the container
             *
             * @param point: The point to check
             * @return true if point is in the bounds of the container
             */
            virtual bool isInBounds(const f32v2& point) const { return isInBounds(point.x, point.y); }
            virtual bool isInBounds(f32 x, f32 y) const;
            
            /*! @brief Adds all drawables to the UIRenderer. */
            virtual void addDrawables() = 0;
            /*! @brief Removes all drawables from the UIRenderer. */
            virtual void removeDrawables();
            /*! @brief Refreshes all drawables. */
            virtual void refreshDrawables() = 0;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual          IWidget* getViewport()         const { return m_viewport; }
            virtual          IWidget* getParent()           const { return m_parent; }
            virtual   const IWidgets& getWidgets()          const { return m_widgets; }
            virtual       const Font* getFont()             const { return m_font; }
            virtual             f32v4 getDestRect()         const { return f32v4(m_position.x, m_position.y, m_size.x, m_size.y); }
            virtual        const f32& getX()                const { return m_position.x; }
            virtual        const f32& getY()                const { return m_position.y; }
            virtual      const f32v2& getPosition()         const { return m_position; }
            virtual        const f32& getWidth()            const { return m_size.x; }
            virtual        const f32& getHeight()           const { return m_size.y; }
            virtual      const f32v2& getSize()             const { return m_size; }
            virtual          Clipping getClipping()         const { return m_clipping; }
            virtual     ClippingState getClippingLeft()     const;
            virtual     ClippingState getClippingTop()      const;
            virtual     ClippingState getClippingRight()    const;
            virtual     ClippingState getClippingBottom()   const;
            virtual             f32v4 getClipRect()         const { return m_clipRect; }
            virtual              ui16 getZIndex()           const { return m_zIndex; }
            virtual              Dock getDock()             const { return m_dock; }
            virtual         DockState getDockState()        const { return m_dock.state; }
            virtual               f32 getDockSize()         const { return m_dock.size; }
            virtual    const nString& getName()             const { return m_name; }
            virtual              bool isEnabled()           const { return m_flags.isEnabled; }
            virtual              bool isMouseIn()           const { return m_flags.isMouseIn; }

            virtual bool needsDimensionUpdate()       const { return m_flags.needsDimensionUpdate; }
            virtual bool needsClipRectRecalculation() const { return m_flags.needsClipRectRecalculation; }
            virtual bool needsDrawableReload()        const { return m_flags.needsDrawableRefresh; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setFont(const Font* font) { m_font = font; }
            virtual void setPosition(f32v2 position);
            virtual void setX(f32 x);
            virtual void setY(f32 y);
            virtual void setRelativePosition(f32v2 relativePosition);
            virtual void setRelativeX(f32 relX);
            virtual void setRelativeY(f32 relY);
            virtual void setSize(f32v2 size);
            virtual void setWidth(f32 width);
            virtual void setHeight(f32 height);
            virtual void setClipping(Clipping clipping);
            virtual void setClippingLeft(ClippingState state);  
            virtual void setClippingTop(ClippingState state);
            virtual void setClippingRight(ClippingState state);
            virtual void setClippingBottom(ClippingState state);
            virtual void setZIndex(ui16 zIndex);
            virtual void setDock(Dock dock);
            virtual void setDockState(DockState state);
            virtual void setDockSize(f32 size);
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
            /*! \brief Updates descendant widgets. */
            virtual void updateDescendants(f32 dt);

            /*!
             * \brief Updates all descendant widgets' viewport fields.
             */
            virtual void updateDescendantViewports();

            // TODO(Matthew): Ensure we don't go out of the update loop to go through all descendants and update individual bits. Let's set flags on children only, let those children do their updates and set any necessary flags on their children, and so on.

            /*!
             * \brief Reregisters drawables of the widget.
             */
            virtual void reregisterDrawables();
            /*!
             * \brief Marks children to reregister their drawables.
             */
            virtual void markChildrenToReregisterDrawables();

            /*! \brief Updates the dimensions of the new IWidget according to specific widget rules.
             */
            virtual bool updateDimensions() = 0;
            /*!
             * \brief Marks all children to update their dimensions.
             */
            virtual void markChildrenToUpdateDimensions();

            /*! Removes a widget from a dock and returns true on success. */
            // bool removeChildFromDock(Widget* widget);
            /*! Calculates positions and sizes of docked child widgets. */
            void calculateDockedWidgets();
            
            /*! Computes clipping for rendering and propagates through children. */
            virtual void calculateClipRect();
            /*! Computes the clipping of child widgets. */
            virtual void calculateChildClipRects();

            /*!
             * \brief Reorders widgets relative to their z-index value.
             */
            virtual void reorderWidgets();

            /*!
             * \brief Sorts, then removes and re-adds all child widgets to renderer.
             */
            virtual void reorderChildWidgets();

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
            Viewport*         m_viewport;         ///< Viewport this widget resides in.
            IWidget*          m_parent;           ///< Parent widget.
            IWidgets          m_widgets;          ///< Collection of child widgets.
            const Font*       m_font;             ///< Font for rendering.
            f32v2             m_position;         ///< Position of widget relative to window in pixels.
            f32v2             m_size;             ///< Size of the widget in pixels.
            Clipping          m_clipping;         ///< Clipping rules to use for generating the clip rectangle.
            f32v4             m_clipRect;         ///< Clipping rectangle for rendering.
            ui16              m_zIndex;           ///< Z-index of widget for depth.
            Dock              m_dock;             ///< Information for docking of widget.
            nString           m_name;             ///< Display name of the container.

            WidgetFlags m_flags;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_IContainer_h__
