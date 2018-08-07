///
/// IGameScreen.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 22 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Screen interface
///

#pragma once

#ifndef IGameScreen_h__
#define IGameScreen_h__

#include "MainGame.h"
#include "FocusController.h"

namespace vorb {
    namespace ui {
#define SCREEN_INDEX_NO_SCREEN -1
#define SCREEN_INDEX_NO_START_SELECTED -2
        
        // A Screen Must Be In One Of These States
        enum class ScreenState {
            // The Screen Is Doing Nothing At The Moment
            NONE,
            // The Screen Is Running (Current Active)
            RUNNING,
            // Exit Request To The Main Game
            EXIT_APPLICATION,
            // Request To Move To The Next Screen
            CHANGE_NEXT,
            // Request To Move To The Previous Screen
            CHANGE_PREVIOUS
        };
        
        // Common Interface For A Game Screen
        class IGameScreen {
        public:
            IGameScreen()
            : m_state(ScreenState::NONE) {
                // empty
            }
        
            virtual ~IGameScreen() {
                // empty
            }
        
            // All Screens Should Have A Parent
            void setParentGame(MainGame* game, i32 index) {
                m_game = game;
                m_index = index;
            }
        
            // The Screen's Location In The List
            i32 getIndex() const {
                return m_index;
            }
        
            // Returns Screen Index When Called To Change Screens
            virtual i32 getNextScreen() const = 0;
            virtual i32 getPreviousScreen() const = 0;
        
            // Screen State Functions
            ScreenState getState() const {
                return m_state;
            }
            void setRunning() {
                m_state = ScreenState::RUNNING;
            }
        
            // Called At The Beginning And End Of The Application
            virtual void build() = 0;
            virtual void destroy(const vui::GameTime& gameTime) = 0;
        
            // Called When A Screen Enters And Exits Focus
            virtual void onEntry(const vui::GameTime& gameTime) = 0;
            virtual void onExit(const vui::GameTime& gameTime) = 0;
        
            // Called In The Main Update Loop
            virtual void update(const vui::GameTime& gameTime) = 0;
            virtual void draw(const vui::GameTime& gameTime) = 0;
        protected:
            ScreenState m_state;
            MainGame* m_game = nullptr;
            FocusController m_focusController;
        private:
            // Location In The ScreenList
            i32 m_index = -1;
        };
        
        template<typename T>
        class IAppScreen : public IGameScreen {
        public:
            IAppScreen(const T* app)
                : m_app(app) {
            }
        protected:
            const T* const m_app;
        };
        
// Shorten Super-Constructors
#define CTOR_APP_SCREEN_INL(SCR_CLASS, APP_CLASS) SCR_CLASS(const APP_CLASS* app) : vui::IAppScreen<APP_CLASS>(app)
#define CTOR_APP_SCREEN_DECL(SCR_CLASS, APP_CLASS) SCR_CLASS(const APP_CLASS* app)
#define CTOR_APP_SCREEN_DEF(SCR_CLASS, APP_CLASS) SCR_CLASS::SCR_CLASS(const APP_CLASS* app) : vui::IAppScreen<APP_CLASS>(app)
    }
}
namespace vui = vorb::ui;

#endif // IGameScreen_h__