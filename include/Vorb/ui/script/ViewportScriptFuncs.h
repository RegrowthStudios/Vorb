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

#include "Vorb/graphics/gtypes.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/script/WidgetScriptFuncs.h"

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

template <typename ScriptEnvironmentImpl>
void vui::ViewportScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", "Viewport");
    env->addCDelegate("enable", makeFunctor([](Viewport* view) {
        view->enable();
    }));
    env->addCDelegate("disable", makeFunctor([](Viewport* view) {
        view->disable();
    }));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::ViewportScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_ViewportScriptFuncs_h__
