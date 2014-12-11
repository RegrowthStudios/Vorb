///
/// MouseInputDispatcher.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 10 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Dispatches mouse events
///

#pragma once

#ifndef MouseInputDispatcher_h__
#define MouseInputDispatcher_h__

#include "Events.hpp"

namespace vorb {
    namespace ui {
        /// The known mouse buttons
        enum class MouseButton {
            UNKNOWN, ///< An unknown mouse button
            LEFT, ///< The left mouse button
            MIDDLE, ///< The middle mouse button
            RIGHT, ///< The right mouse button
            X1, ///< The first X button
            X2 ///< The second X button
        };

        /// Common mouse event data
        struct MouseEvent {
        public:
            i32 x; ///< Mouse location on X axis (in pixels)
            i32 y; ///< Mouse location on Y axis (in pixels)
        };

        /// Mouse button event data
        struct MouseButtonEvent : public MouseEvent {
        public:
            MouseButton button; ///< The mouse button
            ui8 clicks; ///< Number of clicks done with this button (1 for single, 2 for double)
        };

        /// Mouse motion event data
        struct MouseMotionEvent : public MouseEvent {
        public:
            i32 dx; ///< Mouse displacement in X direction (in pixels)
            i32 dy; ///< Mouse displacement in Y direction (in pixels)
        };

        /// Mouse wheel scroll event data
        struct MouseWheelEvent : public MouseEvent {
        public:
            i32 dx; ///< Scroll value change in X direction
            i32 dy; ///< Scroll value change in Y direction
            i32 sx; ///< Total scroll value in X direction
            i32 sy; ///< Total scroll value in Y direction
        };

        /// Dispatches mouse events
        class MouseEventDispatcher {
            friend class InputDispatcher;
        public:
            Event<const MouseEvent&> onEvent; ///< Signaled when any mouse event happens
            Event<const MouseEvent&> onFocusLost; ///< Signaled when mouse no longer provides input to application
            Event<const MouseEvent&> onFocusGained; ///< Signaled when mouse begins to provide input to application
            Event<const MouseButtonEvent&> onButtonDown; ///< Signaled when a mouse button is pressed down
            Event<const MouseButtonEvent&> onButtonUp; ///< Signaled when a mouse button is released
            Event<const MouseMotionEvent&> onMotion; ///< Signaled when the mouse moves
            Event<const MouseWheelEvent&> onWheel; ///< Signaled when the mouse wheel scrolls
        private:
            i32v2 m_lastPos; ///< The last tracked position of the mouse
            i32v2 m_fullScroll; ///< The accumulated values of the scroll wheel
        };
    }
}
namespace vui = vorb::ui;

#endif // MouseInputDispatcher_h__