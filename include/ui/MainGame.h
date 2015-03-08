///
/// MainGame.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 22 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Application and screen manager
///

#pragma once

#ifndef MainGame_h__
#define MainGame_h__

#include "../Events.hpp"
#include "GameWindow.h"
#include "../graphics/GraphicsDevice.h"

namespace vorb {
    namespace ui {
        class IGameScreen;
        class ScreenList;
        
        // Provides Temporal Information Since The Start Of The Application
        struct GameTime {
        public:
            // Total Time Since The Main Loop Started
            f64 total;
            // Elapsed Time For The Current Frame
            f64 elapsed;
        };
        
        // Encapsulates An Entry Point Into A Rendering Program
        class MainGame {
        public:
            MainGame();
            virtual ~MainGame();
        
            const GameWindow& getWindow() const {
                return _window;
            }
        
            void run();
            void exitGame();
        
            // Initialization Logic When Application Starts Up
            virtual void onInit() = 0;
            // The Method Where IGameScreens Must Be Added To _screenList
            virtual void addScreens() = 0;
            // Called When The Application Is Going To Close
            virtual void onExit() = 0;
        
            // Returns the FPS of the run() loop
            const f32& getFps() const {
                return _fps;
            }
        protected:
            // Initializes Necessary Children Systems (OpenGL, TTF, etc.)
            bool init();
            bool initSystems();
        
            // Steps Performed During Game Loop
            void refreshElapsedTime();
            void checkInput();
            void onUpdateFrame();
            void onRenderFrame();

            void onQuit(Sender);
        
            vg::GraphicsDevice* _gDevice;
            GameWindow _window;
        
            f32 _fps;
            ui32 _lastMS;
            GameTime _curTime, _lastTime;
        
            bool _isRunning;
            ScreenList* _screenList;
            IGameScreen* _screen;
        
            volatile bool m_signalQuit = false; ///< Tracks application's request to quit
        };
    }
}
namespace vui = vorb::ui;

#endif // MainGame_h__