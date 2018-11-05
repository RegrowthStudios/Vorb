//
// IWidget.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IWidget.h
* \brief 
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

            bool operator==(Clipping rhs) {
                return this->left   == rhs.left
                    && this->top    == rhs.top
                    && this->right  == rhs.right
                    && this->bottom == rhs.bottom;
            }
            bool operator!=(Clipping rhs) {
                return !(*this == rhs);
            }
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

            bool ignoreOffset : 1; ///< Ignore parent's childOffset factor.

            volatile bool needsDimensionUpdate       : 1; ///< Whether we need to recalculate widget dimensions.
            volatile bool needsZIndexReorder         : 1; ///< Whether we need to recalculate the order of widgets based on their z-index values.
            volatile bool needsDockRecalculation     : 1; ///< Whether we need to recalculate docking of child widgets.
            volatile bool needsClipRectRecalculation : 1; ///< Whether we need to recalculate the clip rectangle.
            volatile bool needsDrawableRecalculation : 2; ///< Whether we need to recalculate the drawables.
            volatile bool needsDrawableRefresh       : 2; ///< Whether we need to refresh the drawables currently drawn to screen.
            volatile bool needsDrawableReregister    : 4; ///< Whether we need to reregister our drawables with the renderer.
        };

        // Forward Declarations
        class UIRenderer;
        class Viewport;

        class IWidget {
            using IWidgets = std::vector<IWidget*>;
            using Font = vorb::graphics::SpriteFont;
        public:
            /*! \brief Default constructor.  */
            IWidget();
            /*! \brief Copy constructor. */
            IWidget(const IWidget&);
            /*! \brief Destructor that unhooks events 
             *
             * Note that we remove all ties to any UI structure the copied-from widget may be a part of. That is,
             * we set the viewport and parent to nullptr and create an empty vector of child widgets.
             */
            virtual ~IWidget();

            /*! \brief Initialiser that sets name, position, size, and Z-index.
            *
            * \param name: Name of the widget.
            * \param dimensions: Position and size of widget.
            * \param zIndex: Z-index of widget.
            */
            virtual void init(const nString& name, const f32v4& dimensions = f32v4(0.0f), ui16 zIndex = 0);
            /*! \brief Initialiser that sets parent, name, position, size, and Z-index.
            *
            * \param parent: Parent of the widget.
            * \param name: Name of the widget.
            * \param dimensions: Position and size of widget.
            * \param zIndex: Z-index of widget.
            */
            virtual void init(IWidget* parent, const nString& name, const f32v4& dimensions = f32v4(0.0f), ui16 zIndex = 0);

            /*! \brief Releases all resources used by the Widget.
             *
             * Gets called in the destructor.
             */
            virtual void dispose();

            /*! \brief Updates the widget. Can be used for animation.
            *
            * \param dt: Change in time since last update.
            */
            virtual void update(f32 dt = 1.0f);

            /*! \brief Enables events that all widgets share in common. */
            virtual void enable();
            /*! \brief Disables events that all widgets share in common. */
            virtual void disable();

            /*! \brief Childs another widget to this widget.
             *
             * \param child: The Widget to add
             * \return true on success.
             */
            virtual bool addWidget(IWidget* child);
            /*! \brief Unchilds a child widget of this widget.
            *
            * \param child: The Widget to remove
            * \return true on success.
            */
            virtual bool removeWidget(IWidget* child);

            /*! \brief Checks if a point is inside the container
             *
             * \param point: The point to check
             * \return true if point is in the bounds of the container
             */
            virtual bool isInBounds(const f32v2& point) const { return isInBounds(point.x, point.y); }
            virtual bool isInBounds(f32 x, f32 y) const;
            
            /*! \brief Adds all drawables to the UIRenderer. */
            virtual void addDrawables() = 0;
            /*! \brief Removes all drawables from the UIRenderer. */
            virtual void removeDrawables();
            /*! \brief Refreshes all drawables. */
            virtual void refreshDrawables() = 0;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual         Viewport* getViewport()         const { return m_viewport; }
            virtual          IWidget* getParent()           const { return m_parent;   }
            virtual   const IWidgets& getWidgets()          const { return m_widgets;  }
            virtual             f32v4 getDestRect()         const { return f32v4(m_position.x, m_position.y, m_size.x, m_size.y); }
            virtual        const f32& getX()                const { return m_position.x; }
            virtual        const f32& getY()                const { return m_position.y; }
            virtual      const f32v2& getPosition()         const { return m_position;   }
            virtual             f32v2 getPaddedPosition()   const { return m_position - f32v2(m_padding.x, m_padding.y); }
            virtual               f32 getRelativeX()        const { return m_parent ? m_position.x - m_parent->getPosition().x : m_position.x; }
            virtual               f32 getRelativeY()        const { return m_parent ? m_position.y - m_parent->getPosition().y : m_position.y; }
            virtual             f32v2 getRelativePosition() const { return m_parent ? m_position - m_parent->getPosition() : m_position; }
            virtual        const f32& getWidth()            const { return m_size.x;     }
            virtual        const f32& getHeight()           const { return m_size.y;     }
            virtual      const f32v2& getSize()             const { return m_size;       }
            virtual             f32v2 getPaddedSize()       const { return m_size + f32v2(m_padding.x + m_padding.z, m_padding.y + m_padding.w); }
            virtual      const f32v4& getPadding()          const { return m_padding;    }
            virtual               f32 getPaddingLeft()      const { return m_padding.x;  }
            virtual               f32 getPaddingTop()       const { return m_padding.y;  }
            virtual               f32 getPaddingRight()     const { return m_padding.z;  }
            virtual               f32 getPaddingBottom()    const { return m_padding.w;  }
            virtual          Clipping getClipping()         const { return m_clipping;   }
            virtual     ClippingState getClippingLeft()     const;
            virtual     ClippingState getClippingTop()      const;
            virtual     ClippingState getClippingRight()    const;
            virtual     ClippingState getClippingBottom()   const;
            virtual             f32v4 getClipRect()         const { return m_clipRect;    }
            virtual              ui16 getZIndex()           const { return m_zIndex;      }
            virtual              Dock getDock()             const { return m_dock;        }
            virtual         DockState getDockState()        const { return m_dock.state;  }
            virtual               f32 getDockSize()         const { return m_dock.size;   }
            virtual    const nString& getName()             const { return m_name;        }
            virtual      const f32v2& getChildOffset()      const { return m_childOffset; }
            virtual              bool isEnabled()           const { return m_flags.isEnabled; }
            virtual              bool isMouseIn()           const { return m_flags.isMouseIn; }
            virtual              bool ignoreOffset()        const { return m_flags.ignoreOffset; }

            virtual bool needsDimensionUpdate()       const { return m_flags.needsDimensionUpdate;       }
            virtual bool needsZIndexReorder()         const { return m_flags.needsZIndexReorder;         }
            virtual bool needsDockRecalculation()     const { return m_flags.needsDockRecalculation;     }
            virtual bool needsClipRectRecalculation() const { return m_flags.needsClipRectRecalculation; }
            virtual bool needsDrawableRecalculation() const { return m_flags.needsDrawableRecalculation; }
            virtual bool needsDrawableRefresh()       const { return m_flags.needsDrawableRefresh;       }
            virtual bool needsDrawableReregister()    const { return m_flags.needsDrawableReregister;    }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setPosition(f32v2 position);
            virtual void setX(f32 x);
            virtual void setY(f32 y);
            virtual void setRelativePosition(f32v2 relativePosition);
            virtual void setRelativeX(f32 relX);
            virtual void setRelativeY(f32 relY);
            virtual void setSize(f32v2 size);
            virtual void setWidth(f32 width);
            virtual void setHeight(f32 height);
            virtual void setPadding(const f32v4& padding);
            virtual void setPaddingLeft(f32 left);
            virtual void setPaddingTop(f32 top);
            virtual void setPaddingRight(f32 right);
            virtual void setPaddingBottom(f32 bottom);
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
            virtual void setChildOffset(const f32v2& offset);
            virtual void setChildOffsetX(f32 offset);
            virtual void setChildOffsetY(f32 offset);
            virtual void setIgnoreOffset(bool ignoreOffset);

            virtual void setNeedsDimensionUpdate(bool flag)       { m_flags.needsDimensionUpdate       = flag; }
            virtual void setNeedsZIndexReorder(bool flag)         { m_flags.needsZIndexReorder         = flag; }
            virtual void setNeedsDockRecalculation(bool flag)     { m_flags.needsDockRecalculation     = flag; }
            virtual void setNeedsClipRectRecalculation(bool flag) { m_flags.needsClipRectRecalculation = flag; }
            virtual void setNeedsDrawableRecalculation(bool flag) { m_flags.needsDrawableRecalculation = flag; }
            virtual void setNeedsDrawableRefresh(bool flag)       { m_flags.needsDrawableRefresh       = flag; }
            virtual void setNeedsDrawableReregister(bool flag)    { m_flags.needsDrawableReregister    = flag; }

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<const MouseButtonEvent&> MouseClick; ///< Occurs when container is clicked by mouse.
            Event<const MouseButtonEvent&> MouseDown;  ///< Occurs when mouse button is pressed over container.
            Event<const MouseButtonEvent&> MouseUp;    ///< Occurs when mouse button is released over container.
            Event<const MouseMotionEvent&> MouseEnter; ///< Occurs when mouse pointer enters the container.
            Event<const MouseMotionEvent&> MouseLeave; ///< Occurs when mouse pointer leaves the container.
            Event<const MouseMotionEvent&> MouseMove;  ///< Occurs when mouse pointer is moved over container.
            // TODO(Ben): Lots more events!

        protected:
            /*! \brief Initialiser for general set-up. */
            virtual void initBase() { /* Empty */ }

            /*!
             * \brief Updates descendant widgets.
             *
             * \param dt: Change in time since last update.
             */
            virtual void updateDescendants(f32 dt);

            /*!
             * \brief Removes drawables from all descendant widgets.
             */
            virtual void removeDescendantDrawables();

            /*!
             * \brief Updates all descendant widgets' viewport fields.
             */
            virtual void updateDescendantViewports();

            /*!
             * \brief Reregisters drawables of the widget.
             */
            virtual void reregisterDrawables();
            /*!
             * \brief Marks children to reregister their drawables.
             */
            virtual void markChildrenToReregisterDrawables();

            /*!
             * \brief Updates the dimensions of the new IWidget according to specific widget rules.
             * 
             * \param dt: Change in time since last update.
             */
            virtual void updateDimensions(f32 dt) = 0;
            /*!
             * \brief Marks all children to update their dimensions.
             */
            virtual void markChildrenToUpdateDimensions();
            /*!
             * \brief Marks all descendants to update their dimensions.
             */
            virtual void markDescendantsToUpdateDimensions();

            /*! \brief Calculates positions and sizes of docked child widgets. */
            void calculateDockedWidgets();
            
            /*! \brief Calculates clipping for rendering and propagates through children. */
            virtual void calculateClipRect();
            /*! \brief Calculates the clipping of child widgets. */
            virtual void markChildrenToCalculateClipRect();

            /*! \brief Calculates the properties of the drawables. */
            virtual void calculateDrawables() = 0;

            /*!
             * \brief Reorders widgets relative to their z-index value.
             */
            virtual void reorderWidgets();

            /*!
             * \brief Sorts, then removes and re-adds all child widgets to renderer.
             */
            virtual void reorderChildWidgets();

            /*! \brief Applies parent's child offset to this widget. */
            virtual void applyOffset();

            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseDown(Sender, const MouseButtonEvent& e);
            virtual void onMouseUp(Sender, const MouseButtonEvent& e);
            virtual void onMouseMove(Sender, const MouseMotionEvent& e);
            virtual void onMouseFocusLost(Sender, const MouseEvent& e);

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
            Viewport*         m_viewport;    ///< Viewport this widget resides in.
            IWidget*          m_parent;      ///< Parent widget.
            IWidgets          m_widgets;     ///< Collection of child widgets.
            f32v2             m_position;    ///< Position of widget relative to window in pixels.
            f32v2             m_size;        ///< Size of the widget in pixels.
            f32v4             m_padding;     ///< Padding of the widget in pixels.
            Clipping          m_clipping;    ///< Clipping rules to use for generating the clip rectangle.
            f32v4             m_clipRect;    ///< Clipping rectangle for rendering.
            ui16              m_zIndex;      ///< Z-index of widget for depth.
            Dock              m_dock;        ///< Information for docking of widget.
            nString           m_name;        ///< Display name of the container.
            f32v2             m_childOffset; ///< Apply this offset to all child widgets' positions.

            WidgetFlags m_flags;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_IContainer_h__
