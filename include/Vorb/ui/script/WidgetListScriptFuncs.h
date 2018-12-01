//
// WidgetListScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file WidgetListScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_WidgetListScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetListScriptFuncs_h__
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
        class IWidget;
        class WidgetList;

        namespace WidgetListScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                void addItem           (WidgetList* widgetList, IWidget* item);
                bool addItemAtIndex    (WidgetList* widgetList, size_t index, IWidget* item);
                bool removeItem        (WidgetList* widgetList, IWidget* item);
                bool removeItemAtIndex (WidgetList* widgetList, size_t index);

                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                VGTexture getTexture        (WidgetList* widgetList);
                     bool getAutoScroll     (WidgetList* widgetList);
                   color4 getBackColor      (WidgetList* widgetList);
                   color4 getBackHoverColor (WidgetList* widgetList);
                   size_t getItemCount      (WidgetList* widgetList);
                      f32 getSpacing        (WidgetList* widgetList);
                      f32 getMaxHeight      (WidgetList* widgetList);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setTexture        (WidgetList* widgetList, VGTexture texture);
                void setAutoScroll     (WidgetList* widgetList, bool autoScroll);
                void setBackColor      (WidgetList* widgetList, color4 color);
                void setBackHoverColor (WidgetList* widgetList, color4 color);
                void setSpacing        (WidgetList* widgetList, f32 spacing);
                void setMaxHeight      (WidgetList* widgetList, f32 maxHeight);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::WidgetListScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getTexture",        makeDelegate(&impl::getTexture));
    env->addCDelegate("setTexture",        makeDelegate(&impl::setTexture));
    env->addCDelegate("getAutoScroll",     makeDelegate(&impl::getAutoScroll));
    env->addCDelegate("setAutoScroll",     makeDelegate(&impl::setAutoScroll));
    env->addCDelegate("getBackColor",      makeDelegate(&impl::getBackColor));
    env->addCDelegate("setBackColor",      makeDelegate(&impl::setBackColor));
    env->addCDelegate("getBackHoverColor", makeDelegate(&impl::getBackHoverColor));
    env->addCDelegate("setBackHoverColor", makeDelegate(&impl::setBackHoverColor));
    env->addCDelegate("getItemCount",      makeDelegate(&impl::getItemCount));
    env->addCDelegate("getSpacing",        makeDelegate(&impl::getSpacing));
    env->addCDelegate("setSpacing",        makeDelegate(&impl::setSpacing));
    env->addCDelegate("getMaxHeight",      makeDelegate(&impl::getMaxHeight));
    env->addCDelegate("setMaxHeight",      makeDelegate(&impl::setMaxHeight));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::WidgetListScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_WidgetListScriptFuncs_h__
