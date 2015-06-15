//
// MouseInputDispatcher.h
// Vorb Engine
//
// Created by Cristian Zaloj on 24 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file MouseInputDispatcher.h
 * @brief Dispatches mouse events.
 * 
 * 
 */

#pragma once

#ifndef Vorb_MouseInputDispatcher_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MouseInputDispatcher_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../decorators.h"
#include "../types.h"
#endif // !VORB_USING_PCH

#include <atomic>

#include "../Events.hpp"

namespace vorb {
    namespace ui {
        namespace impl {
            VORB_INTERNAL class InputDispatcherEventCatcher;
        }

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
            friend class vorb::ui::impl::InputDispatcherEventCatcher;
        public:
            void getPosition(i32* x, i32* y) const;
            i32v2 getPosition() const {
                i32v2 v;
                getPosition(&v.x, &v.y);
                return v;
            }
            bool hasFocus() const;
            bool isRelative() const;
            bool isHidden() const;

            Event<const MouseEvent&> onEvent; ///< Signaled when any mouse event happens
            Event<const MouseEvent&> onFocusLost; ///< Signaled when mouse no longer provides input to application
            Event<const MouseEvent&> onFocusGained; ///< Signaled when mouse begins to provide input to application
            Event<const MouseButtonEvent&> onButtonDown; ///< Signaled when a mouse button is pressed down
            Event<const MouseButtonEvent&> onButtonUp; ///< Signaled when a mouse button is released
            Event<const MouseMotionEvent&> onMotion; ///< Signaled when the mouse moves
            Event<const MouseWheelEvent&> onWheel; ///< Signaled when the mouse wheel scrolls
        private:
            void setPos(i32 x, i32 y);
            void setFocus(bool v);
            void setRelative(bool v);
            void setHidden(bool v);

            i64v2 m_lastPos; ///< The last tracked position of the mouse
            i64v2 m_fullScroll; ///< The accumulated values of the scroll wheel

            std::atomic<i32> m_x = ATOMIC_VAR_INIT(0); ///< The last known x-position of the mouse
            std::atomic<i32> m_y = ATOMIC_VAR_INIT(0); ///< The last known y-position of the mouse
            std::atomic<i32> m_focus = ATOMIC_VAR_INIT(0); ///< 1 if this mouse if focused over the window
            std::atomic<i32> m_relative = ATOMIC_VAR_INIT(0); ///< 1 if this mouse is set in a "relative" mode
            std::atomic<i32> m_hidden = ATOMIC_VAR_INIT(0); ///< 1 if this mouse is hidden over the window
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_MouseInputDispatcher_h__
