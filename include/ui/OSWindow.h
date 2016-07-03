//
// OSWindow.h
// Vorb Engine
//
// Created by Cristian Zaloj on 9 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file OSWindow.h
 * 
 * \brief
 *
 *
 */

#pragma once

#ifndef Vorb_OSWindow_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_OSWindow_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#if defined(VORB_OS_WINDOWS)
#define _WINSOCKAPI_
#include <Windows.h>
#endif

#include "InputDispatcher.h"

#define DEFAULT_TITLE "Vorb Application Management Peer Interface Redist Environment (V.A.M.P.I.R.E.)"

namespace vorb {
    namespace ui {

        /*! \brief Typeless window handle.
         * <br/>
         * Windows - HWND <br/>
         * Mac OSX - ? <br/>
         * Linux   - ? <br/>
         */
#if defined(VORB_OS_WINDOWS)
        typedef HWND OSWindowHandle;
#else 
        // TODO(Cristian): Add other typedefs
        typedef void* OSWindowHandle;
#endif
        struct OSWindowSettings {
        public:
            UNIT_SPACE(PIXEL) ui64 width; ///< Width of the window excluding borders.
            UNIT_SPACE(PIXEL) ui64 height; ///< Height of the window excluding borders.
            bool isBorderless; ///< Width of the border on the sides of the window (0 if no border).
            bool isFullscreen; ///< True if the window is fullscreen.
            bool isResizable; ///< True if the window can be resized.
        };

        class OSWindow {
        public:
            static bool create(OSWindow& outWindow, OSWindowSettings& settings);
            static void moveValues(OSWindow& dest, OSWindow&& src);

            OSWindow() = default;
            OSWindow(OSWindow&& other) {
                moveValues(*this, std::move(other));
            }
            ~OSWindow();

            OSWindow& operator=(OSWindow&& other) {
                moveValues(*this, std::move(other));
                return *this;
            }

            void update();

            void* getAPIHandle() const {
                return m_apiHandle;
            }
            OSWindowHandle getWindowHandle() const {
                return m_hWnd;
            }

            InputDispatcher& getDispatcher() {
                return *m_dispatcher;
            }
            const InputDispatcher& getDispatcher() const {
                return *m_dispatcher;
            }
            i64v2 getPosition() const;

            bool setFullscreen(bool value);
            bool setBorderless(bool value);
            bool setResizable(bool value);
            bool setPosition(UNIT_SPACE(PIXEL) i64 x, UNIT_SPACE(PIXEL) i64 y);
            bool setSize(UNIT_SPACE(PIXEL) i64 w, UNIT_SPACE(PIXEL) i64 h);
            bool setTitle(const cString value);
        protected:
            /*! \brief Typeless UI API handle.
             * <br/>
             * GLFW - GLFWwindow* <br/>
             * SDL - SDL_Window* <br/>
             * SFML - sf::RenderWindow* <br/>
             */
            void* m_apiHandle;
            OSWindowHandle m_hWnd; ///< A handle to the OS's representation of the window.
            InputDispatcher* m_dispatcher; ///< The input dispatcher that forwards this window's events.
        private:
            VORB_NON_COPYABLE(OSWindow); ///< You shalt not make shallow copies of windows.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_OSWindow_h__
