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
#include "../VorbPreDecl.inl"

DECL_VG(class Renderer);

namespace vorb {
    namespace ui {
#define SCREEN_INDEX_NO_SCREEN -1
#define SCREEN_INDEX_NO_START_SELECTED -2
        
        enum class ScreenState {
            NONE,
            RUNNING,
            EXIT_APPLICATION,
            CHANGE_NEXT,    ///< Go to next screen.
            CHANGE_PREVIOUS ///< Go to previous screen.
        };
        
        // Common interface for a game screen
        class IGameScreen {
        public:
            IGameScreen() {
                // empty
            }
        
            ~IGameScreen() {
                // empty
            }
        
            /*! @brief All screens should have a parent.
             */
            void setParentGame(MainGame* game, i32 index) {
                m_game = game;
                m_index = index;
            }
        
            /*! @brief The screen's location in the list.
             */
            i32 getIndex() const {
                return m_index;
            }
        
            /*! @brief Returns screen index when called to change to next screen.
             */
            virtual i32 getNextScreen() const = 0;
            /*! @brief Returns screen index when called to change to previous screen.
            */
            virtual i32 getPreviousScreen() const = 0;
        
            /*! @brief Screen state functions
             */
            ScreenState getState() const {
                return m_state;
            }
            void setRunning() {
                m_state = ScreenState::RUNNING;
            }
        
            /*! @brief Called At the beginning and end of the application.
             */
            virtual void build() = 0;

            /*! @brief Destroys all resources
             */
            virtual void destroy(const vui::GameTime& gameTime) = 0;
        
            /*! @brief Called when a screen enters and exits focus.
             */
            virtual void onEntry(const vui::GameTime& gameTime) = 0;
            virtual void onExit(const vui::GameTime& gameTime) = 0;

            /*! @brief Initializes and register any scenes for rendering.
             *  @param renderer: Register scenes and postprocesses with this to enable rendering.
             */
            virtual void registerRendering(vg::Renderer& renderer) = 0;
            
            /*! @brief Called after the screen clear, but before the Renderer begins to render.
             *  It is recommended to do rendering via Scenes and PostProcesses rather than here,
             *  but you can optionally do rendering here.
             */
            virtual void onRenderFrame(const vui::GameTime& gameTime) {};
        
            /*! @brief Called In the main update loop.
             */
            virtual void update(const vui::GameTime& gameTime) = 0;
        protected:
            ScreenState     m_state = ScreenState::NONE;
            MainGame*       m_game  = nullptr;
            FocusController m_focusController;
        private:
            // Location in the screenList
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