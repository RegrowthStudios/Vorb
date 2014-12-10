///
/// KeyboardEventDispatcher.h
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

#ifndef KeyboardEventDispatcher_h__
#define KeyboardEventDispatcher_h__

#include "Events.hpp"

namespace vorb {
    namespace ui {
#define NUM_KEY_CODES 256

        struct KeyModifiers {
        public:
            ui16 lShift : 1;
            ui16 rShift : 1;
            ui16 lCtrl : 1;
            ui16 rCtrl : 1;
            ui16 lAlt : 1;
            ui16 rAlt : 1;
            ui16 lGUI : 1;
            ui16 rGUI : 1;
            ui16 num : 1;
            ui16 caps : 1;
        };

        struct KeyEvent {
        public:
            ui8 keyCode;
            ui8 scanCode;
            KeyModifiers mod;
            ui32 repeatCount;
        };

        class KeyboardEventDispatcher {
            friend class InputDispatcher;
        public:
            Event<const KeyEvent&> onEvent;
            Event<const KeyEvent&> onKeyDown;
            Event<const KeyEvent&> onKeyUp;
        private:
            bool m_state[NUM_KEY_CODES];
        };
    }
}
namespace vui = vorb::ui;


#endif // KeyboardEventDispatcher_h__