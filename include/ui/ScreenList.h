///
/// ScreenList.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 22 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Indexed screen container
///

#pragma once

#ifndef ScreenList_h__
#define ScreenList_h__

#include "MainGame.h"

namespace vorb {
    namespace ui {
        class IGameScreen;
        
        class ScreenList {
        public:
            ScreenList(MainGame* g);
        
            IGameScreen* getCurrent() const {
                const i32 screen_size = static_cast<const i32>(_screens.size());
                if (_current < 0 || _current >= screen_size) {
                    return nullptr;
                }
                else return _screens[_current];
            }
            IGameScreen* moveNext();
            IGameScreen* movePrevious();
        
            void setScreen(i32 s);
            ScreenList* addScreens(IGameScreen** s, i32 c = 0);
            ScreenList* addScreen(IGameScreen* s);
        
            void destroy(GameTime gameTime);
        
        protected:
            MainGame* _game;
        
            std::vector<IGameScreen*> _screens;
            i32 _current;
        };
    }
}
namespace vui = vorb::ui;

#endif // ScreenList_h__