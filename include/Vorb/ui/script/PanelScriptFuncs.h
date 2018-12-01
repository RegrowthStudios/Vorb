//
// PanelScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file PanelScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_PanelScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_PanelScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/script/WidgetScriptFuncs.h"
#include "Vorb/graphics/gtypes.h"
#include "Vorb/graphics/SpriteFont.h"
#include "Vorb/script/IEnvironment.hpp"

namespace vorb {
    namespace ui {
        // Forward Declarations
        class Panel;

        namespace PanelScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                VGTexture getTexture      (Panel* panel);
                VGTexture getHoverTexture (Panel* panel);
                     bool getAutoScroll   (Panel* panel);
                      f32 getSliderWidth  (Panel* panel);
                   color4 getColor        (Panel* panel);
                   color4 getHoverColor   (Panel* panel);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setTexture      (Panel* panel, VGTexture texture);
                void setHoverTexture (Panel* panel, VGTexture texture);
                void setAutoScroll   (Panel* panel, bool autoScroll);
                void setSliderWidth  (Panel* panel, f32 width);
                void setColor        (Panel* panel, color4 color);
                void setHoverColor   (Panel* panel, color4 color);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::PanelScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getTexture",      makeDelegate(&impl::getTexture));
    env->addCDelegate("setTexture",      makeDelegate(&impl::setTexture));
    env->addCDelegate("getHoverTexture", makeDelegate(&impl::getHoverTexture));
    env->addCDelegate("setHoverTexture", makeDelegate(&impl::setHoverTexture));
    env->addCDelegate("getAutoScroll",   makeDelegate(&impl::getAutoScroll));
    env->addCDelegate("setAutoScroll",   makeDelegate(&impl::setAutoScroll));
    env->addCDelegate("getSliderWidth",  makeDelegate(&impl::getSliderWidth));
    env->addCDelegate("setSliderWidth",  makeDelegate(&impl::setSliderWidth));
    env->addCDelegate("getColor",        makeDelegate(&impl::getColor));
    env->addCDelegate("setColor",        makeDelegate(&impl::setColor));
    env->addCDelegate("getHoverColor",   makeDelegate(&impl::getHoverColor));
    env->addCDelegate("setHoverColor",   makeDelegate(&impl::setHoverColor));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::PanelScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_PanelScriptFuncs_h__
