///
/// InputDispatcher.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 10 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Global class that dispatches OS and device inputs via events
///

#pragma once

#ifndef InputDispatcher_h__
#define InputDispatcher_h__


#include "KeyboardEventDispatcher.h"
#include "MouseInputDispatcher.h"
#include "WindowEventDispatcher.h"

namespace vorb {
    namespace ui {
        class GameWindow;
     
        namespace impl {
            VORB_INTERNAL class InputDispatcherEventCatcher;
        }

        struct DragDropEvent {
        public:
            const cString file;
        };

        /// Handles receiving and dispatching important events
        class InputDispatcher {
            friend class impl::InputDispatcherEventCatcher;
            friend class OSWindow;
        public:
            /// Adds an event listening hook
            /// @param w: The window where events will be generated
            /// @pre: SDL is initialized
            /// @throws std::runtime_error: When this is already initialized
            static void init();
            /// Removes the event listener from SDL
            static void dispose();

            static Event<const DragDropEvent&> onDragDrop; ///< Signaled when application should quit
            static Event<> onQuit; ///< Signaled when application should quit

            MouseEventDispatcher mouse; ///< Dispatches mouse events
            KeyboardEventDispatcher key; ///< Dispatches keyboard events
            WindowEventDispatcher window; ///< Dispatches window events
        private:
            volatile static bool m_isInit; ///< Keeps track of initialization status

            OSWindow* m_window; ///< Parent window
        };
    }
}
namespace vui = vorb::ui;

#endif // InputDispatcher_h__