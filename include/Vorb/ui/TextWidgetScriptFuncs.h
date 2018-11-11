//
// TextWidgetScriptFuncs.h
// Vorb Engine
//
// Created by Matthew Marshall 11 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file TextWidgetScriptFuncs.h
* \brief Provides methods to register functions and constants of TextWidget to a script environment.
*/

#pragma once

#ifndef Vorb_WidgetListScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetListScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/script/IEnvironment.hpp"

namespace vorb {
    namespace ui {
        namespace TextWidgetScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                // Here go functions to be registered.
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetListScriptFuncs_h__
