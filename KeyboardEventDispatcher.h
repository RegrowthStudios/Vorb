///
/// KeyboardEventDispatcher.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 10 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Dispatches keyboard events
///

#pragma once

#ifndef KeyboardEventDispatcher_h__
#define KeyboardEventDispatcher_h__

#include "Events.hpp"

namespace vorb {
    namespace ui {
#define NUM_KEY_CODES 256

        /// Bit array common key modifiers
        struct KeyModifiers {
        public:
            bool lShift : 1;
            bool rShift : 1;
            bool lCtrl : 1;
            bool rCtrl : 1;
            bool lAlt : 1;
            bool rAlt : 1;
            bool lGUI : 1;
            bool rGUI : 1;
            bool num : 1;
            bool caps : 1;
        };

        /// Keyboard event data
        struct KeyEvent {
        public:
            ui8 keyCode; ///< Virtually mapped key code
            ui8 scanCode; ///< Physical key code
            KeyModifiers mod; ///< Current modifiers
            ui32 repeatCount; ///< Number of times this event was repeated including the first key press
        };

        /// Dispatches keyboard events
        class KeyboardEventDispatcher {
            friend class InputDispatcher;
        public:
            Event<> onEvent; ///< Signaled when any keyboard event happens
            Event<> onFocusLost; ///< Signaled when keyboard no longer provides input to application
            Event<> onFocusGained; ///< Signaled when keyboard begins to provide input to application
            Event<const KeyEvent&> onKeyDown; ///< Signaled when a key is pressed
            Event<const KeyEvent&> onKeyUp; ///< Signaled when a key is released
        private:
            bool m_state[NUM_KEY_CODES]; ///< The pressed state each virtual key
        };
    }
}
namespace vui = vorb::ui;


#endif // KeyboardEventDispatcher_h__