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
        enum class MouseButton {
            UNKNOWN,
            LEFT,
            MIDDLE,
            RIGHT,
            X1,
            X2
        };

        struct MouseEvent {
        public:
            i32 x;
            i32 y;
        };
        struct MouseButtonEvent : public MouseEvent {
        public:
            MouseButton button;
            ui8 clicks;
        };
        struct MouseMotionEvent : public MouseEvent {
        public:
            i32 dx;
            i32 dy;
        };
        struct MouseWheelEvent : public MouseEvent {
        public:
            i32 dx;
            i32 dy;
            i32 sx;
            i32 sy;
        };

        class MouseEventDispatcher {
            friend class InputDispatcher;
        public:
            Event<const MouseEvent&> onEvent;
            Event<const MouseButtonEvent&> onButtonDown;
            Event<const MouseButtonEvent&> onButtonUp;
            Event<const MouseMotionEvent&> onMotion;
            Event<const MouseWheelEvent&> onWheel;
        private:
            i32v2 m_lastPos;
            i32v2 m_fullScroll;
        };
    }
}
namespace vui = vorb::ui;

#endif // MouseInputDispatcher_h__