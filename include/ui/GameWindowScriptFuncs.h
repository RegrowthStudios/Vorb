//
// GameWindowScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
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
#include "types.h"
#endif // !VORB_USING_PCH

#include "../VorbPreDecl.inl"

DECL_VSCRIPT(class Environment)

namespace vorb {
    namespace ui {

        class GameWindow;

        class GameWindowScriptFuncs {
        public:
            virtual void init(const cString nSpace, const GameWindow* gameWindow, vscript::Environment* env);

        protected:
            virtual int getNumSupportedResolutions() const;
            virtual ui32v2 getSupportedResolution(int resIndex) const;

            const GameWindow* m_window = nullptr;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_GameWindowScriptFuncs_h__

