//
// MainGame.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file MainGame.h
 * @brief Application and screen management system.
 */

#pragma once

#ifndef Vorb_MainGame_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MainGame_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#include "../decorators.h"
#endif // !VORB_USING_PCH

#include "../Events.hpp"
#include "GameWindow.h"
#include "ScreenList.h"

namespace vorb {
    namespace ui {
        class IGameScreen;
        class ScreenList;
        
        /*! @brief Keeps track of time for an application.
         */
        struct GameTime {
        public:
            UNIT_SPACE(SECONDS) f64 total; ///< Total time since the update/draw loop started.
            UNIT_SPACE(SECONDS) f64 elapsed; ///< Elapsed time of the previous frame.
        };
        
        /*! @brief An entry point for an graphical application.
         * 
         * This class handles running and switching between multiple screens.
         * It also performs frame-limiting tasks.
         */
        class MainGame {
        public:
            /*! @brief Basic constructor that initializes a few default values.
             */
            MainGame();
            /*! @brief Virtual destructor for subclasses.
             */
            virtual ~MainGame();
        
            /*! @brief Retrieve this application's window.
            *
            * @warning Do not attempt to set the window to a value other than its own.
            *
            * @return This game's window reference.
            */
            GameWindow& getWindow() {
                return m_window;
            }
            /*! @brief Retrieve this application's window.
            *
            * @return This game's window reference.
            */
            const GameWindow& getWindow() const {
                return m_window;
            }
        
            /*! @brief Initialize UI libraries and begin running the main loop.
             * 
             * The run method blocks the thread on which it is called and it makes it the
             * active rendering thread.
             */
            void run();
        
            /*! @brief Simple initialization logic for the application should reside here.
             * 
             * This is called only once on startup and access to graphics is available.
             */
            virtual void onInit() = 0;
            /*! @brief Screens should be added to the member m_screenList and the active screen should be set.
             * 
             * This method is called after onInit. No presence of an active screen will cause the application to
             * exit its main loop.
             */
            virtual void addScreens() = 0;
            /*! @brief This method is called when the application exits.
             * 
             * Access to the graphics context is still available. All screens have been destroyed at this point.
             */
            virtual void onExit() = 0;
        
            /*! @brief Provides access to the most recently calculated FPS value.
             * 
             * @return The FPS of the main loop.
             */
            const f32& getFps() const {
                return m_fps;
            }
        protected:
            VORB_NON_COPYABLE(MainGame);
            VORB_NON_MOVABLE(MainGame);

            // Initializes Necessary Children Systems (UI, OpenGL, etc.)
            bool init();
            bool initSystems();
            void exitGame();

            // Logic used for game loop screen management
            bool shouldTerminate() const;
            bool checkScreenChange();

            // Steps Performed During Game Loop
            void refreshElapsedTime();
            void onUpdateFrame();
            void onRenderFrame();

            GameWindow m_window; ///< The application's window
        
            f32 m_fps = 0.0f; ///< Cached FPS value of the main loop
            UNIT_SPACE(MILLISECONDS) ui32 m_lastMS = 0; ///< Value of the last obtained clock time.
            GameTime m_curTime; ///< The current known application time.
            GameTime m_lastTime; ///< The last time value, used for delta purposes.
        
            bool m_isRunning = false; ///< Flag if the application thinks it's running.
            ScreenList m_screenList; ///< List of the managed screens.
            IGameScreen* m_screen = nullptr; ///< The active screen.
        
            volatile bool m_signalQuit = false; ///< Tracks application's request to quit.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_MainGame_h__
