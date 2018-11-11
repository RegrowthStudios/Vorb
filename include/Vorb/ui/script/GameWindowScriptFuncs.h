//
// GameWindowScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file GameWindowScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
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

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace ui {
        // Forward Declarations
        class GameWindow;

        namespace GameWindowScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env, const GameWindow* window);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                size_t getNumSupportedResolutions(const GameWindow* window);
                ui32v2 getSupportedResolution(const GameWindow* window, size_t resIndex);
                ui32v2 getCurrentResolution(const GameWindow* window);
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_GameWindowScriptFuncs_h__

