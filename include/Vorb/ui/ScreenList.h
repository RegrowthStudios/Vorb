//
// ScreenList.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Jan 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ScreenList.h
 * @brief Indexed screen container.
 */

#pragma once

#ifndef Vorb_ScreenList_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ScreenList_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <vector>

#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace ui {
        class MainGame;
        class IGameScreen;
        struct GameTime;
        
        class ScreenList {
        public:
            ScreenList(MainGame* g);
        
            IGameScreen* getCurrent() const {
                const i32 screen_size = static_cast<i32>(_screens.size());
                if (_current < 0 || _current >= screen_size) {
                    return nullptr;
                }
                else return _screens[_current];
            }
            IGameScreen* moveNext();
            IGameScreen* movePrevious();
        
            void setScreen(i32 s);
            ScreenList* addScreens(IGameScreen** s, size_t c = 0);
            ScreenList* addScreen(IGameScreen* s);
        
            void destroy(const GameTime& gameTime);
        
        protected:
            MainGame* _game;
        
            std::vector<IGameScreen*> _screens;
            i32 _current;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ScreenList_h__
