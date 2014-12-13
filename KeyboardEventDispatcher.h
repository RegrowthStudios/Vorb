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
            union {
                struct {
                    bool lShift : 1; ///< Left shift pressed state
                    bool rShift : 1; ///< Right shift pressed state
                    bool lCtrl : 1; ///< Left control pressed state
                    bool rCtrl : 1; ///< Right control pressed state
                    bool lAlt : 1; ///< Left alt pressed state
                    bool rAlt : 1; ///< Right alt pressed state
                    bool lGUI : 1; ///< Left menu pressed state
                    bool rGUI : 1; ///< Right menu pressed state
                };
                struct {
                    ui8 shift : 2; ///< Any of the two shift keys
                    ui8 ctrl : 2; ///< Any of the two control keys
                    ui8 alt : 2; ///< Any of the two alt keys
                    ui8 gui : 2; ///< Any of the two menu keys
                };
                ui8 state; ///< State of all the modifier keys
            };
            bool num : 1;
            bool caps : 1;
        };

        /// Keyboard event data
        struct KeyEvent {
        public:
            ui8 keyCode; ///< Virtually mapped key code
            ui8 scanCode; ///< Physical key code
            KeyModifiers mod; ///< Current modifiers
            ui32 repeatCount; ///< Number of times this event was repeated
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