//
// ButtonScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ButtonScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_ButtonScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ButtonScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/gtypes.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/script/TextWidgetScriptFuncs.h"

DECL_VG(enum class GradientType)

namespace vorb {
    namespace ui {
        // Forward declarations
        class Button;

        namespace ButtonScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*);

            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                VGTexture getTexture(Button* button);
                VGTexture getHoverTexture(Button* button);
                color4 getBackColor(Button* button);
                color4 getBackHoverColor(Button* button);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setTexture(Button* button, VGTexture texture);
                void setHoverTexture(Button* button, VGTexture texture);
                void setBackColor(Button* button, color4 color);
                void setBackColorGrad(Button* button, color4 color1, color4 color2, vg::GradientType grad);
                void setBackHoverColor(Button* button, color4 color);
                void setBackHoverColorGrad(Button* button, color4 color1, color4 color2, vg::GradientType grad);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::ButtonScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getTexture",            makeDelegate(&impl::getTexture));
    env->addCDelegate("setTexture",            makeDelegate(&impl::setTexture));
    env->addCDelegate("getHoverTexture",       makeDelegate(&impl::getHoverTexture));
    env->addCDelegate("setHoverTexture",       makeDelegate(&impl::setHoverTexture));
    env->addCDelegate("getBackColor",          makeDelegate(&impl::getBackColor));
    env->addCDelegate("setBackColor",          makeDelegate(&impl::setBackColor));
    env->addCDelegate("getBackHoverColor",     makeDelegate(&impl::getBackHoverColor));
    env->addCDelegate("setBackHoverColor",     makeDelegate(&impl::setBackHoverColor));
    env->addCDelegate("setBackColorGrad",      makeDelegate(&impl::setBackColorGrad));
    env->addCDelegate("setBackHoverColorGrad", makeDelegate(&impl::setBackHoverColorGrad));
    env->setNamespaces();

    TextWidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::ButtonScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_ButtonScriptFuncs_h__
