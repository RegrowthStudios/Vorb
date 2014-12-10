///
/// WindowEventDispatcher.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 10 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef WindowEventDispatcher_h__
#define WindowEventDispatcher_h__

#include "Events.hpp"

namespace vorb {
    namespace ui {
        struct WindowEvent {
            // Empty
        };
        struct WindowResizeEvent : public WindowEvent {
        public:
            ui32 w;
            ui32 h;
        };
        struct WindowFileEvent : public WindowEvent {
        public:
            const cString file;
        };

        class WindowEventDispatcher {
        public:
            Event<const WindowEvent&> onEvent;
            Event<const WindowEvent&> onQuit;
            Event<const WindowResizeEvent&> onResize;
            Event<const WindowFileEvent&> onFile;
        };
    }
}
namespace vui = vorb::ui;

#endif // WindowEventDispatcher_h__