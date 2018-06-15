///
/// InputDispatcherEventCatcher.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 7 Feb 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef InputDispatcherEventCatcher_h__
#define InputDispatcherEventCatcher_h__

#if defined(VORB_IMPL_UI_SDL)
//#if defined(VORB_OS_WINDOWS)
//#include <SDL/SDL.h>
//#else
#include <SDL2/SDL.h>
//#endif
#elif defined(VORB_IMPL_UI_GLFW)
#include <GLFW/glfw3.h>
#elif defined(VORB_IMPL_UI_SFML)
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#endif

#include "ui/KeyboardEventDispatcher.h"

namespace vorb {
    namespace ui {
        namespace impl {
            class InputDispatcherEventCatcher {
            public:
#if defined(VORB_IMPL_UI_SDL)
                static i32 onSDLEvent(void* userData, SDL_Event* e);
#elif defined(VORB_IMPL_UI_GLFW)
                static void onMonitorEvent(GLFWmonitor*, int);
                static void onFileDropEvent(GLFWwindow*, int, const cString*);
                static void onKeyFocusEvent(GLFWwindow*, int);
                static void onKeyEvent(GLFWwindow*, int, int, int, int);
                static void onCharEvent(GLFWwindow*, unsigned int);
                static void onMouseFocusEvent(GLFWwindow*, int);
                static void onMousePosEvent(GLFWwindow*, f64, f64);
                static void onMouseButtonEvent(GLFWwindow*, int, int, int);
                static void onMouseScrollEvent(GLFWwindow*, f64, f64);
                static void onWindowCloseEvent(GLFWwindow*);
                static void onWindowPosEvent(GLFWwindow*, int, int);
                static void onWindowSizeEvent(GLFWwindow*, int, int);
                static void onFramebufferSizeEvent(GLFWwindow*, int, int);
                static void onIconifyEvent(GLFWwindow*, int);
                static vui::KeyModifiers mods;
#elif defined(VORB_IMPL_UI_SFML)
                static void onSFMLEvent(sf::RenderWindow* userData, sf::Event& e);
                static vui::KeyModifiers mods;
#endif
            };
        }
    }
}
namespace vui = vorb::ui;

#endif // InputDispatcherEventCatcher_h__
