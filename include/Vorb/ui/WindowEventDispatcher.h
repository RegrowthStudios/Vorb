///
/// WindowEventDispatcher.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 10 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Dispatches events related to the application window handle
///

#pragma once

#ifndef WindowEventDispatcher_h__
#define WindowEventDispatcher_h__

#include "../Events.hpp"

namespace vorb {
    namespace ui {
        /// Window resize event data
        struct WindowResizeEvent {
        public:
            ui32 w; ///< New window width in pixels
            ui32 h; ///< New window height in pixels
        };

        /// Window path drop event data
        struct WindowFileEvent {
        public:
            const cString file; ///< Absolute path dropped into the window
        };

        /// Dispatches window events
        class WindowEventDispatcher {
            friend class InputDispatcher;
        public:
            Event<> onEvent; ///< Signaled when any window event happens
            Event<> onClose; ///< Signaled when user requests to close the window
            Event<const WindowResizeEvent&> onResize; ///< Signaled when window is resized
            Event<const WindowFileEvent&> onFile; ///< Signaled when a path is dropped into the window
        };
    }
}
namespace vui = vorb::ui;

#endif // WindowEventDispatcher_h__