//
// ViewportScriptFuncs.h
// Vorb Engine
//
// Created by Matthew Marshall 11 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file ViewportScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_ViewportScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ViewportScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/gtypes.h"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace ui {
        // Forward Declarations
        class Viewport;

        namespace ViewportScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);
            
            namespace impl {
                // Empty
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ViewportScriptFuncs_h__
