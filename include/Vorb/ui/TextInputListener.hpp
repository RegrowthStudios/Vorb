///
/// TextInputListener.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 14 Dec 2014
/// Copyright 2014 Regrowth Studios
/// MIT License
///
/// Summary:
/// Controls text input events within a string
///

#pragma once

#ifndef TextInputListener_h__
#define TextInputListener_h__

#include "../Events.hpp"
#include "InputDispatcher.h"

namespace vorb {
    namespace ui {
        template<typename T> class TextInputConverter; ///< Handles character conversions from cString

        /// Provides string manipulation functionality
        /// @tparam T: Internal character type (char, wchar_t, i32, enums, ...)
        template<typename T>
        class TextInputController {
        public:
            /// Constructor that inserts the null character
            TextInputController() {
                m_input.push_back(0);
            }

            /// Insert a string at the caret location
            /// @param s: String value
            void insert(const cString s) {
                T v = TextInputConverter<T>::convert(s);
                m_input.insert(m_input.begin() + m_caret, v);
                m_caret++;
            }
            /// Deletes the character behind the caret
            void back() {
                if (m_caret > 0 && (m_input.size() > 1)) {
                    m_caret--;
                    m_input.erase(m_input.begin() + m_caret);
                }
            }
            /// Deletes the character in front of the caret
            void del() {
                if (m_caret < (m_input.size() - 1)) {
                    m_input.erase(m_input.begin() + m_caret);
                }
            }
            /// Moves the caret
            /// @param amount: Direction and magnitude of movement
            void move(const i32& amount) {
                i64 v = (i64)m_caret + amount;
                m_caret = (v < 0) ? 0 : (size_t)v;
                if (m_caret > m_input.size() - 1) m_caret = m_input.size() - 1;
            }
            /// Empties the string contents
            void clear() {
                std::vector<T>().swap(m_input);
                m_input.push_back(0);
                m_caret = 0;
            }

            /// @return The current length of the string
            size_t length() const {
                return m_input.size() - 1;
            }

            /// @return Conversion into string of target character type
            explicit operator const T*() const {
                return (T*)m_input.data();
            }
        private:
            std::vector<T> m_input; ///< String storage
            size_t m_caret = 0; ///< Caret position
        };

        /// ASCII converter
        template<>
        class TextInputConverter<char> {
        public:
            static char convert(const cString s) {
                return *s;
            }
        };

        /// Unicode converter
        template<>
        class TextInputConverter<wchar_t> {
        public:
            static wchar_t convert(const cString s) {
                wchar_t v[2];
                mbstowcs(v, s, 1);
                return v[0];
            }
        };

        /// Listens to OS events for text manipulation
        /// @tparam T: Internal character type (char, wchar_t, i32, enums, ...)
        template<typename T>
        class TextInputListener {
        public:
            /// Begin receiving events
            void start() {
                m_delegatePool.addAutoHook(vui::InputDispatcher::key.onKeyDown, [&] (Sender sender VORB_UNUSED, const vui::KeyEvent& e) {
                    switch (e.keyCode) {
                    case VKEY_BACKSPACE:
                        m_text.back();
                        onTextChange((const T*)m_text);
                        break;
                    case VKEY_DELETE:
                        m_text.del();
                        onTextChange((const T*)m_text);
                        break;
                    case VKEY_RETURN:
                        if (m_text.length() > 0) {
                            onTextEntry((const T*)m_text);
                            m_text.clear();
                        }
                        break;
                    case VKEY_LEFT:
                        m_text.move(-1);
                        break;
                    case VKEY_RIGHT:
                        m_text.move(1);
                        break;
                    default:
                        break;
                    }
                });
                m_delegatePool.addAutoHook(vui::InputDispatcher::key.onText, [&] (Sender sender VORB_UNUSED, const vui::TextEvent& e) {
                    m_text.insert(e.text);
                    onTextChange((const T*)m_text);
                });
            }
            /// Stop receiving events
            void stop() {
                m_delegatePool.dispose();
            }

            Event<const T*> onTextEntry; ///< Signaled when user completes text input
            Event<const T*> onTextChange; ///< Signaled when the text changes
        private:
            TextInputController<T> m_text; ///< Text value and manipulator
            AutoDelegatePool m_delegatePool; ///< Input hooks reservoir
        };
    }
}
namespace vui = vorb::ui;

#endif // TextInputListener_h__