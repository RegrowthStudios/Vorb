//
// GameWindowScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file GameWindowScriptFuncs.h
* @brief 
* Scripting functions for a vui::GameWindow
*
*/

#pragma once

#ifndef Vorb_GameWindowScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_GameWindowScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"

DECL_VSCRIPT(class Environment)

namespace vorb {
    namespace ui {

        class GameWindow;

        class GameWindowScriptFuncs {
        public:
            virtual void init(const cString nSpace, const GameWindow* gameWindow, vscript::Environment* env);

        protected:
            virtual i32 getNumSupportedResolutions() const;
            virtual ui32v2 getSupportedResolution(int resIndex) const;
            virtual ui32v2 getCurrentResolution() const;

            const GameWindow* m_window = nullptr;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_GameWindowScriptFuncs_h__

