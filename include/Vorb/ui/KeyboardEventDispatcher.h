//
// KeyboardEventDispatcher.h
// Vorb Engine
//
// Created by Cristian Zaloj on 10 Dec 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file KeyboardEventDispatcher.h
 * @brief Dispatches keyboard events.
 */

#pragma once

#ifndef Vorb_KeyboardEventDispatcher_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KeyboardEventDispatcher_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <array>
#include <atomic>

#include "../Event.hpp"
#include "Keys.inl"

namespace vorb {
    namespace ui {
        namespace impl {
            VORB_INTERNAL class InputDispatcherEventCatcher;
        }

#define NUM_KEY_CODES VKEY_HIGHEST_VALUE

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
            ui16 keyCode; ///< Virtually mapped key code
            ui16 scanCode; ///< Physical key code
            KeyModifiers mod; ///< Current modifiers
            ui32 repeatCount; ///< Number of times this event was repeated
        };
        
        /// Text event data
        struct TextEvent {
        public:
            char text[32]; ///< Provided input text
            wchar_t wtext[16]; ///< Text in wide format
        };

        /// Dispatches keyboard events
        class KeyboardEventDispatcher {
            friend class InputDispatcher;
            friend class vorb::ui::impl::InputDispatcherEventCatcher;
        public:
            KeyboardEventDispatcher();

            i32 getNumPresses(VirtualKey k) const;
            bool hasFocus() const;

            Event<> onEvent; ///< Signaled when any keyboard event happens
            Event<> onFocusLost; ///< Signaled when keyboard no longer provides input to application
            Event<> onFocusGained; ///< Signaled when keyboard begins to provide input to application
            Event<const KeyEvent&> onKeyDown; ///< Signaled when a key is pressed
            Event<const KeyEvent&> onKeyUp; ///< Signaled when a key is released
            Event<const TextEvent&> onText; ///< Signaled when text is provided
        private:
            void addPress(VirtualKey k);
            void release(VirtualKey k);

            bool m_state[NUM_KEY_CODES]; ///< The pressed state each virtual key

            std::array<std::atomic<i32>, NUM_KEY_CODES> m_presses;
            std::atomic<i32> m_focus = ATOMIC_VAR_INIT(0);
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_KeyboardEventDispatcher_h__
