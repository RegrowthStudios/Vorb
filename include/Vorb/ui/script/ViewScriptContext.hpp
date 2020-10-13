//
// ViewScriptContext.h
// Vorb Engine
//
// Created by Matthew Marshall on 11 Nov 2018
// Based on Ben Arnold's FormScriptEnvironment.
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ViewScriptContext.h
* \brief Provides the context in which scripts can interact with views.
*/

#pragma once

#ifndef Vorb_ViewScriptContext_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ViewScriptContext_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/script/lua/ValueMediator.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/io/File.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/widgets/IWidget.h"
#include "Vorb/ui/script/ButtonScriptFuncs.h"
#include "Vorb/ui/script/CheckBoxScriptFuncs.h"
#include "Vorb/ui/script/ComboBoxScriptFuncs.h"
#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/script/GameWindowScriptFuncs.h"
#include "Vorb/ui/script/GraphicsScriptFuncs.h"
#include "Vorb/ui/script/LabelScriptFuncs.h"
#include "Vorb/ui/script/PanelScriptFuncs.h"
#include "Vorb/ui/script/SliderScriptFuncs.h"
#include "Vorb/ui/script/TextWidgetScriptFuncs.h"
#include "Vorb/ui/script/ViewportScriptFuncs.h"
#include "Vorb/ui/script/WidgetScriptFuncs.h"
#include "Vorb/ui/script/WidgetBuilderScriptFuncs.h"
#include "Vorb/ui/script/WidgetListScriptFuncs.h"

namespace vorb {
    namespace ui {
        using IWidgets  = std::vector<IWidget*>;

        namespace ViewScriptContext {
            template <typename ScriptEnvironment>
            using ScriptEnv = vscript::IEnvironment<ScriptEnvironment>;

            template <typename ScriptEnvironment>
            void injectInto(ScriptEnvironment* scriptEnv, const GameWindow* window, vg::TextureCache* textureCache, IWidgets& widgets);

            namespace impl {
                template <typename ScriptEnvironment>
                void registerFuncs(ScriptEnvironment* scriptEnv, const GameWindow* window, vg::TextureCache* textureCache, IWidgets& widgets);
                template <typename ScriptEnvironment>
                void registerConsts(ScriptEnvironment* scriptEnv);
            }

        };
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironment>
void vui::ViewScriptContext::injectInto(ScriptEnvironment* scriptEnv, const GameWindow* window, vg::TextureCache* textureCache, IWidgets& widgets) {
    impl::registerFuncs(scriptEnv, window, textureCache, widgets);

    impl::registerConsts(scriptEnv);
}

template <typename ScriptEnvironment>
void vui::ViewScriptContext::impl::registerFuncs(ScriptEnvironment* scriptEnv, const GameWindow* window, vg::TextureCache* textureCache, IWidgets& widgets) {
    ButtonScriptFuncs::registerFuncs<ScriptEnvironment>("Button", scriptEnv);

    CheckBoxScriptFuncs::registerFuncs<ScriptEnvironment>("CheckBox", scriptEnv);

    ComboBoxScriptFuncs::registerFuncs<ScriptEnvironment>("ComboBox", scriptEnv);

    GameWindowScriptFuncs::registerFuncs<ScriptEnvironment>(scriptEnv, window);

    GraphicsScriptFuncs::registerFuncs<ScriptEnvironment>(scriptEnv, textureCache);

    LabelScriptFuncs::registerFuncs<ScriptEnvironment>("Label", scriptEnv);

    PanelScriptFuncs::registerFuncs<ScriptEnvironment>("Panel", scriptEnv);

    SliderScriptFuncs::registerFuncs<ScriptEnvironment>("Slider", scriptEnv);

    ViewportScriptFuncs::registerFuncs<ScriptEnvironment>(scriptEnv);

    WidgetBuilderScriptFuncs::registerFuncs<ScriptEnvironment>(scriptEnv, widgets);

    WidgetListScriptFuncs::registerFuncs<ScriptEnvironment>("WidgetList", scriptEnv);
}

template <typename ScriptEnvironment>
void vui::ViewScriptContext::impl::registerConsts(ScriptEnvironment* scriptEnv) {
    // TODO(Matthew): Provide way to search for viewports.

    ButtonScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    CheckBoxScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    ComboBoxScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    GameWindowScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    GraphicsScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    LabelScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    PanelScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    SliderScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    ViewportScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    WidgetBuilderScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    WidgetScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);

    WidgetListScriptFuncs::registerConsts<ScriptEnvironment>(scriptEnv);
}


#endif // !Vorb_ViewScriptContext_h__
