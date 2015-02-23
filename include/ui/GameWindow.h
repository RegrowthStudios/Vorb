///
/// GameWindow.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 22 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Controls system window operations
///

#pragma once

#ifndef GameWindow_h__
#define GameWindow_h__

#include "../io/Keg.h"

namespace vorb {
    namespace ui {
        typedef void* WindowHandle;
        typedef void* GraphicsContext;

        #define DEFAULT_TITLE "Vorb Application Management Peer Interface Redist Environment (V.A.M.P.I.R.E.)"
        #define DEFAULT_WINDOW_WIDTH 800
        #define DEFAULT_WINDOW_HEIGHT 600
        #define DEFAULT_SWAP_INTERVAL GameSwapInterval::V_SYNC
        #define DEFAULT_MAX_FPS 60.0f
        #define DEFAULT_APP_CONFIG_FILE "app.config"
        
        // Different Kinds Of Swap Intervals Available
        enum class GameSwapInterval : i32 {
            UNLIMITED_FPS = 0,
            V_SYNC = 1,
            LOW_SYNC = 2,
            POWER_SAVER = 3,
            USE_VALUE_CAP = 4
        };
        
        // The Current Displaying Mode
        struct GameDisplayMode {
        public:
            // Screen Buffer Parameters
            i32 screenWidth;
            i32 screenHeight;
        
            // Window Settings
            bool isFullscreen;
            bool isBorderless;
        
            // Frame Rate Options
            GameSwapInterval swapInterval;
            f32 maxFPS;

            // Graphics Context
            ui32 major;
            ui32 minor;
            bool core;
        };
        
        class GameWindow {
        public:
            GameWindow();
        
            // Attempts To Load Application Settings Creates The Window And OpenGL Context
            bool init();
            // Saves Application Settings (If Changed) And Destroys The Window And OpenGL Context
            void dispose();
        
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
                return m_displayMode.isFullscreen;
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
        
            // Change Display Settings
            void setScreenSize(const i32& w, const i32& h, const bool& overrideCheck = false);
            void setFullscreen(const bool& useFullscreen, const bool& overrideCheck = false);
            void setBorderless(const bool& useBorderless, const bool& overrideCheck = false);
            void setSwapInterval(const GameSwapInterval& mode, const bool& overrideCheck = false);
            void setMaxFPS(const f32& fpsLimit);
            void setTitle(const cString title) const;
        
            void sync(ui32 frameTime);
        private:
            void pollInput();

            // Application Setting Management
            static void setDefaultSettings(GameDisplayMode* mode);
            void readSettings();
            void saveSettings() const;
        
            // Window Handle
            WindowHandle m_window = nullptr;
            GraphicsContext m_glc = nullptr;
        
            // Display Settings
            GameDisplayMode m_displayMode;
        };
    }
}
namespace vui = vorb::ui;

KEG_ENUM_DECL(GameSwapInterval);
KEG_TYPE_DECL(GameDisplayMode);

#endif // GameWindow_h__
