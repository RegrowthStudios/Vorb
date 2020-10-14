//
// WidgetBuilderScriptFuncs.h
// Vorb Engine
//
// Created by Matthew Marshall on 27 Sept 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file WidgetBuilderScriptFuncs.h
* \brief Provides functions for building widgets.
*/

#pragma once

#ifndef Vorb_WidgetBuilderScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetBuilderScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/UI.h"
#include "Vorb/ui/widgets/Button.h"
#include "Vorb/ui/widgets/CheckBox.h"
#include "Vorb/ui/widgets/ComboBox.h"
#include "Vorb/ui/widgets/Label.h"
#include "Vorb/ui/widgets/Panel.h"
#include "Vorb/ui/widgets/Slider.h"
#include "Vorb/ui/widgets/Viewport.h"
#include "Vorb/ui/widgets/WidgetList.h"

namespace vorb {
    namespace ui {
        namespace WidgetBuilderScriptFuncs {
            template <typename ScriptEnvironment>
            void registerFuncs(vscript::IEnvironment<ScriptEnvironment>* env);

            template <typename ScriptEnvironment>
            void registerConsts(vscript::IEnvironment<ScriptEnvironment>* env);

            namespace impl {
                    vui::Button* makeButton     (IWidget* parent, nString name, f32v4 dimensions);
                  vui::CheckBox* makeCheckBox   (IWidget* parent, nString name, f32v4 dimensions);
                  vui::ComboBox* makeComboBox   (IWidget* parent, nString name, f32v4 dimensions);
                     vui::Label* makeLabel      (IWidget* parent, nString name, f32v4 dimensions);
                     vui::Panel* makePanel      (IWidget* parent, nString name, f32v4 dimensions);
                    vui::Slider* makeSlider     (IWidget* parent, nString name, f32v4 dimensions);
                vui::WidgetList* makeWidgetList (IWidget* parent, nString name, f32v4 dimensions);

                void destroyWidgetTree(IWidget* topLevelWidget, IWidgets& widgets);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::WidgetBuilderScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    // Set up creation functions for extending the view.
    env->setNamespaces("UI", "View");
    env->addCDelegate("makeButton", makeFunctor([&](IWidget* parent, nString name, f32v4 dimensions) {
        return impl::makeButton(parent, name, dimensions);
    }));
    env->addCDelegate("makeCheckBox", makeFunctor([&](IWidget* parent, nString name, f32v4 dimensions) {
        return impl::makeCheckBox(parent, name, dimensions);
    }));
    env->addCDelegate("makeComboBox", makeFunctor([&](IWidget* parent, nString name, f32v4 dimensions) {
        return impl::makeComboBox(parent, name, dimensions);
    }));
    env->addCDelegate("makeLabel", makeFunctor([&](IWidget* parent, nString name, f32v4 dimensions) {
        return impl::makeLabel(parent, name, dimensions);
    }));
    env->addCDelegate("makePanel", makeFunctor([&](IWidget* parent, nString name, f32v4 dimensions) {
        return impl::makePanel(parent, name, dimensions);
    }));
    env->addCDelegate("makeSlider", makeFunctor([&](IWidget* parent, nString name, f32v4 dimensions) {
        return impl::makeSlider(parent, name, dimensions);
    }));
    env->addCDelegate("makeWidgetList", makeFunctor([&](IWidget* parent, nString name, f32v4 dimensions) {
        return impl::makeWidgetList(parent, name, dimensions);
    }));
    env->addCDelegate("destroyWidgetTree", makeFunctor([&](IWidget* topLevelWidget) {
        impl::destroyWidgetTree(topLevelWidget, widgets);
    }));
    env->setNamespaces();
}

template <typename ScriptEnvironmentImpl>
void vui::WidgetBuilderScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", "ClippingState");
    env->addValue("VISIBLE", ClippingState::VISIBLE);
    env->addValue("HIDDEN",  ClippingState::HIDDEN);
    env->addValue("INHERIT", ClippingState::INHERIT);
    env->setNamespaces();

    env->setNamespaces("UI", "DockState");
    env->addValue("NONE",   DockState::NONE);
    env->addValue("LEFT",   DockState::LEFT);
    env->addValue("RIGHT",  DockState::RIGHT);
    env->addValue("BOTTOM", DockState::BOTTOM);
    env->addValue("TOP",    DockState::TOP);
    env->addValue("FILL",   DockState::FILL);
    env->setNamespaces();

    env->setNamespaces("UI", "PositionType");
    env->addValue("STATIC_TO_WINDOW",     PositionType::STATIC_TO_WINDOW);
    env->addValue("STATIC_TO_VIEWPORT",   PositionType::STATIC_TO_VIEWPORT);
    env->addValue("STATIC_TO_PARENT",     PositionType::STATIC_TO_PARENT);
    env->addValue("RELATIVE_TO_WINDOW",   PositionType::RELATIVE_TO_WINDOW);
    env->addValue("RELATIVE_TO_VIEWPORT", PositionType::RELATIVE_TO_VIEWPORT);
    env->addValue("RELATIVE_TO_PARENT",   PositionType::RELATIVE_TO_PARENT);
    env->setNamespaces();

    env->setNamespaces("UI", "DimensionType");
    env->addValue("PIXEL",                      DimensionType::PIXEL);
    env->addValue("WIDTH_PERCENTAGE",           DimensionType::WIDTH_PERCENTAGE);
    env->addValue("HEIGHT_PERCENTAGE",          DimensionType::HEIGHT_PERCENTAGE);
    env->addValue("MIN_PERCENTAGE",             DimensionType::MIN_PERCENTAGE);
    env->addValue("MAX_PERCENTAGE",             DimensionType::MAX_PERCENTAGE);
    env->addValue("PARENT_WIDTH_PERCENTAGE",    DimensionType::PARENT_WIDTH_PERCENTAGE);
    env->addValue("PARENT_HEIGHT_PERCENTAGE",   DimensionType::PARENT_HEIGHT_PERCENTAGE);
    env->addValue("PARENT_MIN_PERCENTAGE",      DimensionType::PARENT_MIN_PERCENTAGE);
    env->addValue("PARENT_MAX_PERCENTAGE",      DimensionType::PARENT_MAX_PERCENTAGE);
    env->addValue("VIEWPORT_WIDTH_PERCENTAGE",  DimensionType::VIEWPORT_WIDTH_PERCENTAGE);
    env->addValue("VIEWPORT_HEIGHT_PERCENTAGE", DimensionType::VIEWPORT_HEIGHT_PERCENTAGE);
    env->addValue("VIEWPORT_MIN_PERCENTAGE",    DimensionType::VIEWPORT_MIN_PERCENTAGE);
    env->addValue("VIEWPORT_MAX_PERCENTAGE",    DimensionType::VIEWPORT_MAX_PERCENTAGE);
    env->addValue("WINDOW_WIDTH_PERCENTAGE",    DimensionType::WINDOW_WIDTH_PERCENTAGE);
    env->addValue("WINDOW_HEIGHT_PERCENTAGE",   DimensionType::WINDOW_HEIGHT_PERCENTAGE);
    env->addValue("WINDOW_MIN_PERCENTAGE",      DimensionType::WINDOW_MIN_PERCENTAGE);
    env->addValue("WINDOW_MAX_PERCENTAGE",      DimensionType::WINDOW_MAX_PERCENTAGE);
    env->setNamespaces();
}

#endif // !Vorb_WidgetBuilderScriptFuncs_h__
