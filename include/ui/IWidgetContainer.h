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

        //! Enum of updatable properties that have changed since last update.
        enum UpdateFlag {
            CLIPPING                    = 0x0001,
            POSITION                    = 0x0002 | CLIPPING,
            RAW_POSITION                = 0x0004 | POSITION | CLIPPING,
            DIMENSIONS                  = 0x0008 | CLIPPING,
            RAW_DIMENSIONS              = 0x0010 | DIMENSIONS | CLIPPING,
            MIN_SIZE                    = 0x0020 | DIMENSIONS | CLIPPING,
            RAW_MIN_SIZE                = 0x0040 | MIN_SIZE | DIMENSIONS | CLIPPING,
            MAX_SIZE                    = 0x0080 | DIMENSIONS | CLIPPING,
            RAW_MAX_SIZE                = 0x0100 | MAX_SIZE | DIMENSIONS | CLIPPING,
            DOCKING                     = 0x0200 | DIMENSIONS | POSITION | CLIPPING,
            TRANSITION_POSITION         = 0x0400,
            TRANSITION_DIMENSIONS       = 0x0800,
            TRANSITION_MIN_SIZE         = 0x1000,
            TRANSITION_MAX_SIZE         = 0x2000,
            TRANSITION_DOCKING          = 0x4000,
            DRAWABLE_ORDER              = 0x8000,
            ALL                         = CLIPPING | POSITION | RAW_POSITION | DIMENSIONS | RAW_DIMENSIONS | MIN_SIZE | MAX_SIZE | DOCKING 
                                            | TRANSITION_POSITION | TRANSITION_DIMENSIONS | TRANSITION_MIN_SIZE | TRANSITION_MAX_SIZE | TRANSITION_DOCKING
                                            | DRAWABLE_ORDER,
            SPATIAL                     = CLIPPING | POSITION | DIMENSIONS | MIN_SIZE | MAX_SIZE
        };

        //! Bitfield of container styling flags
        struct ContainerStyle { // TODO(Matthew): Implement auto sizing and fixed height/width.
            //bool fixedHeight : 1; ///< If true, the control has fixed height when auto-scaled.
            //bool fixedWidth : 1; ///< If true, the control has fixed width when auto-scaled.
            bool selectable : 1; ///< If true, the control can receive focus.
        };

        //! Bitfield of anchor flags
        struct AnchorStyle {
            bool left : 1; ///< If true, anchored to the left of parent
            bool right : 1; ///< If true, anchored to the right of parent
            bool bottom : 1; ///< If true, anchored to the bottom of parent
            bool top : 1; ///< If true, anchored to the top of parent
        };

        //! Enum of unit types.
        enum class UnitType {
            PIXEL,
            PERCENTAGE,
            FORM_WIDTH_PERC,
            FORM_HEIGHT_PERC,
            FORM_MIN_PERC,
            FORM_MAX_PERC
        };

        //! Stores a single length and its units.
        struct Length {
            f32 x;
            struct {
                UnitType x;
            } units;
        };
        //! Stores two lengths and their units.
        struct Length2 {
            f32 x, y;
            struct {
                UnitType x, y;
            } units;
        };

        //! Bitfield of clipping options.
        struct ClippingOptions {
            ClippingOptions() : left{ true },
                top{ true },
                right{ true },
                bottom{ true } {
            }
            ClippingOptions(bool left, bool top, bool right, bool bottom) : 
                left{ left },
                top{ top },
                right{ right },
                bottom{ bottom } {
            }

            bool left : 1;
            bool top : 1;
            bool right : 1;
            bool bottom : 1;
        };

        // TODO(Matthew): Template/bool flag for IWidgetContainer and down to deactivate use of Z index?
        //                Coule be useful for cases where feature is less useful than performance penalty hurts.
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
            /*! @brief Adds a Widget to the container. */
            virtual bool addWidget(Widget* child) = 0;

            /*! @brief Enables events* */
            virtual void enable();
            /*! @brief Disables events* */
            virtual void disable();

            ///*! @brief Updates all spatial state. I.e. position, dimensions, clipping and the same for children. */
            //virtual void updateSpatialState();
            ///*! @brief Calls updateSpatialState on all children. */
            //void updateChildSpatialStates();
            ///*! @brief Updates position spatial state. I.e. position, clipping and same for children. */
            //void updatePositionState();
            ///*! @brief Updates dimension spatial state. I.e. dimensions, clipping and same for children. */
            //void updateDimensionState();
            ///*! @brief Updates clipping for both this widget container and its children. */
            //void updateClippingState();
            ///*! @brief Calls updateClippingState on all children. */
            //void updateChildClippingStates();
            // TODO(Matthew): Implement z-indexing with new update system. (In some cases a full sort is not needed and may be quicker to just remove and reinsert specific widget.)
            ///*! @brief Updates the ordered child widget collection.
            //*
            //* @param changedChild: If provided, function assumes only that child changed and only replaces that child.
            //*/
            //void updateZIndexState(Widget* changedChild = nullptr);
            ///*! @brief Updates children based on their docking options. */
            //void updateDockingState();
            ///*! @brief Updates all transitionary states. */
            //virtual void updateTransitionState();


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
            //virtual bool getFixedHeight() const { return m_style.fixedHeight; }
            //virtual bool getFixedWidth() const { return m_style.fixedWidth; }
            bool getSelectable() const { return m_style.selectable; }
            bool isMouseIn() const { return m_isMouseIn; }      
            const ContainerStyle& getStyle() const { return m_style; }
            const bool& isEnabled() const { return m_isEnabled; }
            const f32& getHeight() const { return m_dimensions.y; }
            const f32& getWidth() const { return m_dimensions.x; }
            const f32& getX() const { return m_position.x; }
            const f32& getY() const { return m_position.y; }
            const f32v2& getDimensions() const { return m_dimensions; }
            const f32v2& getPosition() const { return m_position; }
            const SortedVector<Widget*, true, bool(*)(Widget*, Widget*)>& getWidgets() const { return m_widgets; }
            const nString& getName() const { return m_name; }
            f32v4 getDestRect() const { return f32v4(m_position.x, m_position.y, m_dimensions.x, m_dimensions.y); }
            const ClippingOptions& getOverflowOptions() const { return m_clippingOptions; }
            f32v4 getClipRect() const { return m_clipRect; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setDestRect(const f32v4& destRect);
            virtual void setDimensions(const f32v2& dimensions) { m_dimensions = dimensions; m_pendingUpdates |= UpdateFlag::DIMENSIONS; } // TODO(Matthew): Cache pending sets for application on next update() call?
                                                                                                                                           //                (I.e. update not just processing side effects of set calls.)
                                                                                                                                           //                Would increase memory footprint but would make current and 
                                                                                                                                           //                iminent state accessible.
            //virtual void setFixedHeight(bool fixedHeight) { m_style.fixedHeight = fixedHeight; }
            //virtual void setFixedWidth(bool fixedWidth) { m_style.fixedWidth = fixedWidth; }
            virtual void setHeight(f32 height) { m_dimensions.y = height; m_pendingUpdates |= UpdateFlag::DIMENSIONS; }
            virtual void setWidth(f32 width) { m_dimensions.x = width; m_pendingUpdates |= UpdateFlag::DIMENSIONS; }
            virtual void setPosition(const f32v2& position) { m_position = position; m_pendingUpdates |= UpdateFlag::POSITION; }
            virtual void setX(f32 x) { m_position.x = x; m_pendingUpdates |= UpdateFlag::POSITION; }
            virtual void setY(f32 y) { m_position.y = y; m_pendingUpdates |= UpdateFlag::POSITION; }
            virtual void setSelectable(bool selectable) { m_style.selectable = selectable; }
            virtual void setStyle(const ContainerStyle& style) { m_style = style; }
            virtual void setName(const nString& name) { m_name = name; }
            virtual void setClipping(const ClippingOptions& clippingOptions) { m_clippingOptions = clippingOptions; m_pendingUpdates |= UpdateFlag::CLIPPING; }
            virtual void setClipping(bool clipping) { m_clippingOptions = ClippingOptions(clipping, clipping, clipping, clipping); m_pendingUpdates |= UpdateFlag::CLIPPING; }
            virtual void setClippingTop(bool clipping) { m_clippingOptions.top = clipping; m_pendingUpdates |= UpdateFlag::CLIPPING; }
            virtual void setClippingRight(bool clipping) { m_clippingOptions.right = clipping; m_pendingUpdates |= UpdateFlag::CLIPPING; }
            virtual void setClippingBottom(bool clipping) { m_clippingOptions.bottom = clipping; m_pendingUpdates |= UpdateFlag::CLIPPING; }
            virtual void setClippingLeft(bool clipping) { m_clippingOptions.left = clipping; m_pendingUpdates |= UpdateFlag::CLIPPING; }

            /*! @brief Adds an update flag to the pending updates for this element. */
            void applyUpdateFlag(ui32 flag) { m_pendingUpdates |= flag; }

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
            * @param self: The Widget that is the new parent.
            *
            * @return bool: True on successful addition, false otherwise.
            */
            bool addWidget(Widget* child, Widget* self);
            /*! @brief Adds a child Widget to the container
            *
            * @param child: The Widget to add
            * @param self: The Form that is the new parent.
            *
            * @return bool: True on successful addition, false otherwise.
            */
            bool addWidget(Widget* child, Form* self);

            ///*! @brief Updates data affected by a change in clipping. */
            //virtual void updateClipping();
            ///*! @brief Updates data affected by a change in position. */
            //virtual void updatePosition() = 0;
            ///*! @brief Updates data affected by a change in target position. */
            //virtual void updateTargetPosition() = 0;
            ///*! @brief Updates data affected by a change in dimensions. */
            //virtual void updateDimensions() = 0;
            ///*! @brief Updates data affected by a change in target dimensions. */
            //virtual void updateTargetDimensions() = 0;

            // TODO(Matthew): Should be procedures?
            /*! @brief Computes clipping for this widget container. */
            virtual void computeClipRect() = 0;
            /*! @brief Processes a set of raw values and converts them to processed values that can be used for basic calculations.
             *         Uses the widget provided for parent data etc.
             */
            virtual f32v2 processRawValues(Widget* widget, const Length2& rawValues);
            virtual f32v2 processRawValue(Widget* widget, const f32v2& rawValue, const UnitType& unit);
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
            
            bool(*m_zIndexCompare)(Widget*, Widget*);
            SortedVector<Widget*, true, bool(*)(Widget*, Widget*)> m_widgets; ///< Child widgets in order of Z-index.

            f32v4 m_clipRect = f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX); ///< The clipping rectangle of the container.
            f32v2 m_position = f32v2(0.0f); ///< The position of the container.
            f32v2 m_dimensions = f32v2(0.0f); ///< The dimensions of the container.
            nString m_name = ""; ///< Display name of the container.

            ClippingOptions m_clippingOptions; ///< Determines directions in which clipping may or may not occur.

            ui32 m_pendingUpdates = 0; ///< Pending updates for the UI.

            // TODO(Ben): Bitfield for memory reduction?.
            bool m_isClicking = false; ///< Used for click event tracking.
            bool m_isEnabled = false; ///< True when events are enabled.
            bool m_isMouseIn = false; ///< Used for motion event tracking.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_IContainer_h__
