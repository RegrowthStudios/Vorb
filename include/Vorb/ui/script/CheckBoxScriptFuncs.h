//
// CheckBoxScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file CheckBoxScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_CheckBoxScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_CheckBoxScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/graphics/gtypes.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/script/TextWidgetScriptFuncs.h"

namespace vorb {
    namespace ui {
        // Forward Declarations
        class CheckBox;

        namespace CheckBoxScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);
            
            namespace impl {
                void check(CheckBox* checkBox);
                void uncheck(CheckBox* checkBox);
                void toggleChecked(CheckBox* checkBox);

                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                VGTexture getUncheckedTexture(CheckBox* checkBox);
                VGTexture getCheckedTexture(CheckBox* checkBox);
                VGTexture getUncheckedHoverTexture(CheckBox* checkBox);
                VGTexture getCheckedHoverTexture(CheckBox* checkBox);
                color4 getBoxColor(CheckBox* checkBox);
                color4 getBoxHoverColor(CheckBox* checkBox);
                color4 getBoxCheckedColor(CheckBox* checkBox);
                color4 getBoxCheckedHoverColor(CheckBox* checkBox);
                color4 getTextHoverColor(CheckBox* checkBox);
                bool isChecked(CheckBox* checkBox);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setCheckedTexture(CheckBox* checkBox, VGTexture texture);
                void setUncheckedTexture(CheckBox* checkBox, VGTexture texture);
                void setCheckedHoverTexture(CheckBox* checkBox, VGTexture texture);
                void setUncheckedHoverTexture(CheckBox* checkBox, VGTexture texture);
                void setBoxColor(CheckBox* checkBox, color4 color);
                void setBoxHoverColor(CheckBox* checkBox, color4 color);
                void setBoxCheckedColor(CheckBox* checkBox, color4 color);
                void setBoxCheckedHoverColor(CheckBox* checkBox, color4 color);
                void setTextHoverColor(CheckBox* checkBox, color4 color);
                void setChecked(CheckBox* checkBox, bool checked);

                // void onValueChange(Sender s, bool b);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::CheckBoxScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getUncheckedTexture",      makeDelegate(&impl::getUncheckedTexture));
    env->addCDelegate("setCheckedTexture",        makeDelegate(&impl::setCheckedTexture));
    env->addCDelegate("getCheckedTexture",        makeDelegate(&impl::getCheckedTexture));
    env->addCDelegate("setUncheckedTexture",      makeDelegate(&impl::setUncheckedTexture));
    env->addCDelegate("getUncheckedHoverTexture", makeDelegate(&impl::getUncheckedHoverTexture));
    env->addCDelegate("setCheckedHoverTexture",   makeDelegate(&impl::setCheckedHoverTexture));
    env->addCDelegate("getCheckedHoverTexture",   makeDelegate(&impl::getCheckedHoverTexture));
    env->addCDelegate("setUncheckedHoverTexture", makeDelegate(&impl::setUncheckedHoverTexture));
    env->addCDelegate("getBoxColor",              makeDelegate(&impl::getBoxColor));
    env->addCDelegate("setBoxColor",              makeDelegate(&impl::setBoxColor));
    env->addCDelegate("getBoxHoverColor",         makeDelegate(&impl::getBoxHoverColor));
    env->addCDelegate("setBoxHoverColor",         makeDelegate(&impl::setBoxHoverColor));
    env->addCDelegate("getBoxCheckedColor",       makeDelegate(&impl::getBoxCheckedColor));
    env->addCDelegate("setBoxCheckedColor",       makeDelegate(&impl::setBoxCheckedColor));
    env->addCDelegate("getBoxCheckedHoverColor",  makeDelegate(&impl::getBoxCheckedHoverColor));
    env->addCDelegate("setBoxCheckedHoverColor",  makeDelegate(&impl::setBoxCheckedHoverColor));
    env->addCDelegate("getTextHoverColor",        makeDelegate(&impl::getTextHoverColor));
    env->addCDelegate("setTextHoverColor",        makeDelegate(&impl::setTextHoverColor));
    env->addCDelegate("isChecked",                makeDelegate(&impl::isChecked));
    env->addCDelegate("setChecked",               makeDelegate(&impl::setChecked));
    env->setNamespaces();

    TextWidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::CheckBoxScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_CheckBoxScriptFuncs_h__
