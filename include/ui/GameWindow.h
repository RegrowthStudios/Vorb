//
// GameWindow.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Jan 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file GameWindow.h
 * @brief A window wrapper object for simple applications.
 */

#pragma once

#ifndef Vorb_GameWindow_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_GameWindow_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#include "../decorators.h"
#endif // !VORB_USING_PCH

#include "../io/Keg.h"

#if defined(VORB_DX_11)
struct ID3D11RenderTargetView;
#endif

namespace vorb {
    namespace ui {

        // Forward declarations
        struct WindowResizeEvent;

        /*! @brief Typeless window handle.
         * <br/>
         * GLFW - GLFWwindow* <br/>
         * SDL - SDL_Window* <br/>
         * SFML - sf::RenderWindow* <br/>
         */
        typedef void* WindowHandle;
        /*! @brief Typeless graphics handle
         * <br/>
         * GLFW - GLFWwindow* <br/>
         * SDL/D3D - D3DContext* <br/>
         * SDL/GL - SDL_GLContext <br/>
         * SFML - sf::RenderWindow* <br/>
         */
        typedef void* GraphicsContext;

        #define DEFAULT_TITLE "Vorb Application Management Peer Interface Redist Environment (V.A.M.P.I.R.E.)"
        #define DEFAULT_WINDOW_WIDTH 800
        #define DEFAULT_WINDOW_HEIGHT 600
        #define DEFAULT_SWAP_INTERVAL GameSwapInterval::V_SYNC
        #define DEFAULT_MAX_FPS 60.0f
        #define DEFAULT_APP_CONFIG_FILE "app.config"
        
        /*! @brief Swapping intervals used for monitor synchronization and graphics stability.
         */
        enum class GameSwapInterval : i32 {
            UNLIMITED_FPS = 0, ///< No synchronization or FPS limiting is performed.
            V_SYNC = 1, ///< Synchronize with monitor frame by frame, full refresh rate.
            LOW_SYNC = 2, ///< Skip synchronization every other frame, half the refresh rate of the system.
            POWER_SAVER = 3, ///< Large pause between window and graphics updates, saves computations and power.
            USE_VALUE_CAP = 4 ///< Do not use monitor synchronization, but FPS cap and high resolution timer.
        };
        
        /* 
         */
        struct GraphicsContextProperties {
        public:
            ui32 major; ///< Major version for the graphics context (ex. GL:4, DX:11).
            ui32 minor; ///< Minor version for the graphics context (ex. GL:4, DX:0).
            bool core; ///< True if the graphics context should disable compatibility with lower versions.
            bool debugging; ///< True if the graphics context should enable debugging features.
            ui32 swapChainSize; ///< Number of framebuffers in the swap chain.
        };

        /*! @brief Window properties that additionally define graphics usage.
         */
        struct GameDisplayMode {
        public:
            UNIT_SPACE(PIXEL) i32 screenWidth; ///< Width of the backbuffer (and borderless window width).
            UNIT_SPACE(PIXEL) i32 screenHeight; ///< Height of the backbuffer (and borderless window height).
        
            bool isFullscreen; ///< True if the window is fullscreen.
            bool isBorderless; ///< True if the window does not have a border.
            bool isResizable; ///< True if the window can be resized.
        
            GameSwapInterval swapInterval; ///< Synchronization value for the system's refresh rate.
            f32 maxFPS; ///< Maximum desired FPS with if the FPS limiter is used.

            GraphicsContextProperties context; ///< Properties related to the creation of the graphics context.
        };
        
        class GameWindow {
        public:
            /*! @brief Initializes window with default application settings.
             * 
             * Does not create the physical window or modify any OS settings.
             */
            GameWindow();
        
            /*! @brief Create the window and graphics context.
             * 
             * The window will attempt to load application settings from DEFAULT_APP_CONFIG_FILE
             * for user configuration purposes, otherwise it will use the default settings. The
             * graphics context will be created during window initialization and it will be configured
             * to the thread that this method is called upon. The InputDispatcher will be created as well.
             * 
             * @return True if no error occurred.
             */
            bool init(bool isResizable = true);
            /*! @brief Destroys the window and associated graphics context.
             * 
             * @pre: This disposal method should be called on the thread where the window was created and the
             * graphics context has ownership.
             * 
             * The window will save its current settings to DEFAULT_APP_CONFIG_FILE. The InputDispatcher will
             * be destroyed.
             */
            void dispose();
        
            /*! @brief A check for whether the window has been created or not.
             * 
             * @return True if the window is created and if it is still alive.
             */
            bool isInitialized() const {
                return m_window != nullptr;
            }

            /*! @brief Detection of input to terminate this window.
             * 
             * @return True if a quit signal was received by this window.
             */
            const bool& shouldQuit() const {
                return m_quitSignal;
            }

            // Access Display Settings
            i32 getX() const;
            i32 getY() const;
            i32v2 getPosition() const;
            const i32& getWidth() const {
                return m_displayMode.screenWidth;
            }
            const i32& getHeight() const {
                return m_displayMode.screenHeight;
            }
            ui32v2 getViewportDims() const {
                return ui32v2(m_displayMode.screenWidth, m_displayMode.screenHeight);
            }
            f32 getAspectRatio() const {
                return (float)m_displayMode.screenWidth / (float)m_displayMode.screenHeight;
            }
            const bool& isFullscreen() const {
                return m_displayMode.isFullscreen;
            }
            const bool& isBorderless() const {
                return m_displayMode.isBorderless;
            }
            const bool& isResizable() const {
                return m_displayMode.isResizable;
            }
            const GameSwapInterval& getSwapInterval() const {
                return m_displayMode.swapInterval;
            }
            const f32& getMaxFPS() const {
                return m_displayMode.maxFPS;
            }
            WindowHandle getHandle() const {
                return m_window;
            }
            const std::vector<ui32v2>& getSupportedResolutions() const {
                return m_supportedResolutions;
            }

            /*! @brief Obtain the handle to the graphics context.
             * 
             * @warning When utilizing D3D, the D3D device pointer will be returned instead of the full struct.
             * 
             * @return This window's graphics context.
             */
            GraphicsContext getContext() const; // TODO(Cristian): Does returning the D3D Device cause convenience or confusion?
#if defined(VORB_DX_11)
            ID3D11RenderTargetView* getMainRenderTargetView() const;
#endif

            // Change Display Settings
            void setScreenSize(i32 w, i32 h, bool overrideCheck = false);
            void setFullscreen(bool useFullscreen, bool overrideCheck = false);
            void setBorderless(bool useBorderless, bool overrideCheck = false);
            void setSwapInterval(GameSwapInterval mode, bool overrideCheck = false);
            /*! @brief Sets the position of the window
             * 
             * @param x: X position. 0 will center it.
             * @param Y: Y position. 0 will center it.
             */
            void setPosition(int x, int y);
            void setMaxFPS(f32 fpsLimit);
            void setTitle(const cString title) const;
        
            void sync(ui32 frameTime);

            Event<> onQuit;

            void saveSettings() const;
        private:
            VORB_NON_COPYABLE(GameWindow);
            VORB_MOVABLE_DECL(GameWindow);

            void onResize(Sender s, const WindowResizeEvent& e);
            void onQuitSignal(Sender);
            void pollInput();

            // Application Setting Management
            static void setDefaultSettings(GameDisplayMode* mode);
            void readSettings();
            
        
            std::vector<ui32v2> m_supportedResolutions; ///< All resolutions supported by the window
            WindowHandle m_window = nullptr; ///< Window's OS handle.
            GraphicsContext m_glc = nullptr; ///< Window's graphics context.
            GameDisplayMode m_displayMode; ///< The current display settings of the window.
            bool m_quitSignal = false; ///< Flag for the window's termination request status.
        };
    }
}
namespace vui = vorb::ui;

KEG_ENUM_DECL(GameSwapInterval);
KEG_TYPE_DECL(GraphicsContextProperties);
KEG_TYPE_DECL(GameDisplayMode);


#endif // !Vorb_GameWindow_h__
