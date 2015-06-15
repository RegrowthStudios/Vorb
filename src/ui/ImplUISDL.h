//
// ImplUISDL.h
// Vorb Engine
//
// Created by Cristian Zaloj on 9 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ImplUISDL.h
* @brief 
*
*
*/

#pragma once

#ifndef Vorb_ImplUISDL_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ImplUISDL_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include <SDL/SDL_video.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_events.h>

namespace vorb {
    namespace ui {
        class InputDispatcher;

        extern std::unordered_map<ui32, InputDispatcher*> dispatchers;
    }
}

#endif // !Vorb_ImplUISDL_h__
