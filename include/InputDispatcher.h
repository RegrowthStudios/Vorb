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

union SDL_Event;
class GameWindow;

#include "KeyboardEventDispatcher.h"
#include "MouseInputDispatcher.h"
#include "WindowEventDispatcher.h"

namespace vorb {
    namespace ui {
        /// Handles receiving and dispatching important events
        class InputDispatcher {
        public:
            /// Adds an event listening hook
            /// @param w: The window where events will be generated
            /// @pre: SDL is initialized
            /// @throws std::exception: When this is already initialized
            static void init(GameWindow* w);
            /// Removes the event listener from SDL
            static void dispose();

            static MouseEventDispatcher mouse; ///< Dispatches mouse events
            static KeyboardEventDispatcher key; ///< Dispatches keyboard events
            static WindowEventDispatcher window; ///< Dispatches window events
            static Event<> onQuit; ///< Signaled when application should quit
        private:
            /// Listener which is used to dispatch events and execute listener code
            /// @param userData: Ignored
            /// @param e: Event pointer
            /// @return 1 if event is not dispatched
            static i32 onSDLEvent(void* userData, SDL_Event* e);

            static GameWindow* m_window; ///< Active window
            volatile static bool m_isInit; ///< Keeps track of initialization status
        };
    }
}
namespace vui = vorb::ui;

#endif // InputDispatcher_h__