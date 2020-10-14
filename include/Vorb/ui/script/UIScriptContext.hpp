//
// UIScriptContext.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 13 Oct 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file UIScriptContext.hpp
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_UIScriptContext_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UIScriptContext_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/widgets/IWidget.h"
#include "Vorb/ui/script/ViewScriptContext.hpp"

namespace vorb {
    namespace ui {
        using IWidgets  = std::vector<IWidget*>;

        namespace UIScriptContext {
            template <typename ScriptEnvironment>
            using ScriptEnv = vscript::IEnvironment<ScriptEnvironment>;

            template <typename ScriptEnvironment>
            void injectInto(ScriptEnvironment* scriptEnv, const GameWindow* window, vg::TextureCache* textureCache);

            namespace impl {
                template <typename ScriptEnvironmentImpl>
                void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

                template <typename ScriptEnvironmentImpl>
                void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironment>
void vui::UIScriptContext::injectInto(ScriptEnvironment* env, const GameWindow* window, vg::TextureCache* textureCache) {
    ViewScriptContext::injectInto(env, window, textureCache);

    impl::registerFuncs(env);

    impl::registerConsts(env);
}

template <typename ScriptEnvironment>
void vui::UIScriptContext::impl::registerFuncs(ScriptEnvironment* env) {
    env->setNamespaces("UI");
    env->addCDelegate("makeView", makeFunctor([](UIBase* ui, nString name, ZIndex zIndex) {
        return ui->makeView(name, zIndex);
    }));
    env->addCDelegate("makeViewFromYAML", makeFunctor([](UIBase* ui, nString name, ZIndex zIndex, nString filepath) {
        return ui->makeViewFromYAML(name, zIndex, filepath);
    }));
    env->addCDelegate("getView", makeFunctor([](UIBase* ui, nString name) {
        return ui->getView(name);
    }));
    env->addCDelegate("enableView", makeFunctor([](UIBase* ui, nString name) {
        Viewport* view = ui->getView(name);
        if (view != nullptr) {
            view->enable();
            return true;
        }
        return false;
    }));
    env->addCDelegate("disableView", makeFunctor([](UIBase* ui, nString name) {
        Viewport* view = ui->getView(name);
        if (view != nullptr) {
            view->disable()
            return true;
        }
        return false;
    }));
    env->addCDelegate("destroyView", makeFunctor([](UIBase* ui, Viewport* view) {
        return ui->destroyView(view);
    }));
    env->addCDelegate("destroyViewWithName", makeFunctor([](UIBase* ui, nString name) {
        return ui->destroyViewWithName(name);
    }));
    env->addCDelegate("setViewZIndex", makeFunctor([](UIBase* ui, Viewport* view, ZIndex zIndex) {
        return ui->setViewZIndex(view, zIndex);
    }));
    env->addCDelegate("setViewZIndexWithName", makeFunctor([](UIBase* ui, nString name, ZIndex zIndex) {
        return ui->setViewWithNameZIndex(name, zIndex);
    }));
    env->setNamespaces();
}

template <typename ScriptEnvironment>
void vui::UIScriptContext::impl::registerConsts(ScriptEnvironment* env) {
    // Empty.
}

#endif // !Vorb_UIScriptContext_hpp__
