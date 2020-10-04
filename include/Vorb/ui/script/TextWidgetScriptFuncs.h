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

#ifndef Vorb_TestWidgetScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TestWidgetScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/SpriteFont.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/script/WidgetScriptFuncs.h"

namespace vorb {
    namespace ui {
        // Forward Declarations
        class TextWidget;

        namespace TextWidgetScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                const vg::SpriteFont* getFont           (TextWidget* textWidget);
                              nString getText           (TextWidget* textWidget);
                        vg::TextAlign getTextAlign      (TextWidget* textWidget);
                               color4 getTextColor      (TextWidget* textWidget);
                                f32v2 getTextScale      (TextWidget* textWidget);
                const vg::SpriteFont* getHoverFont      (TextWidget* textWidget);
                              nString getHoverText      (TextWidget* textWidget);
                        vg::TextAlign getHoverTextAlign (TextWidget* textWidget);
                               color4 getHoverTextColor (TextWidget* textWidget);
                                f32v2 getHoverTextScale (TextWidget* textWidget);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setFont           (TextWidget* textWidget, vg::SpriteFont* font);
                void setText           (TextWidget* textWidget, nString text);
                void setTextAlign      (TextWidget* textWidget, vg::TextAlign textAlign);
                void setTextColor      (TextWidget* textWidget, color4 color);
                void setTextScale      (TextWidget* textWidget, f32v2 scale);
                void setHoverFont      (TextWidget* textWidget, vg::SpriteFont* font);
                void setHoverText      (TextWidget* textWidget, nString text);
                void setHoverTextAlign (TextWidget* textWidget, vg::TextAlign textAlign);
                void setHoverTextColor (TextWidget* textWidget, color4 color);
                void setHoverTextScale (TextWidget* textWidget, f32v2 scale);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::TextWidgetScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getFont",           makeDelegate(&impl::getFont));
    env->addCDelegate("setFont",           makeDelegate(&impl::setFont));
    env->addCDelegate("getText",           makeDelegate(&impl::getText));
    env->addCDelegate("setText",           makeDelegate(&impl::setText));
    env->addCDelegate("getTextAlign",      makeDelegate(&impl::getTextAlign));
    env->addCDelegate("setTextAlign",      makeDelegate(&impl::setTextAlign));
    env->addCDelegate("getTextColor",      makeDelegate(&impl::getTextColor));
    env->addCDelegate("setTextColor",      makeDelegate(&impl::setTextColor));
    env->addCDelegate("getTextScale",      makeDelegate(&impl::getTextScale));
    env->addCDelegate("setTextScale",      makeDelegate(&impl::setTextScale));
    env->addCDelegate("getHoverFont",      makeDelegate(&impl::getHoverFont));
    env->addCDelegate("setHoverFont",      makeDelegate(&impl::setHoverFont));
    env->addCDelegate("getHoverText",      makeDelegate(&impl::getHoverText));
    env->addCDelegate("setHoverText",      makeDelegate(&impl::setHoverText));
    env->addCDelegate("getHoverTextAlign", makeDelegate(&impl::getHoverTextAlign));
    env->addCDelegate("setHoverTextAlign", makeDelegate(&impl::setHoverTextAlign));
    env->addCDelegate("getHoverTextColor", makeDelegate(&impl::getHoverTextColor));
    env->addCDelegate("setHoverTextColor", makeDelegate(&impl::setHoverTextColor));
    env->addCDelegate("getHoverTextScale", makeDelegate(&impl::getHoverTextScale));
    env->addCDelegate("setHoverTextScale", makeDelegate(&impl::setHoverTextScale));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::TextWidgetScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", "TextAlign");
    env->addValue("LEFT", vg::TextAlign::LEFT);
    env->addValue("TOP_LEFT", vg::TextAlign::TOP_LEFT);
    env->addValue("TOP", vg::TextAlign::TOP);
    env->addValue("TOP_RIGHT", vg::TextAlign::TOP_RIGHT);
    env->addValue("RIGHT", vg::TextAlign::RIGHT);
    env->addValue("BOTTOM_RIGHT", vg::TextAlign::BOTTOM_RIGHT);
    env->addValue("BOTTOM", vg::TextAlign::BOTTOM);
    env->addValue("BOTTOM_LEFT", vg::TextAlign::BOTTOM_LEFT);
    env->addValue("CENTER", vg::TextAlign::CENTER);
    env->setNamespaces();
}

#endif // !Vorb_TestWidgetScriptFuncs_h__
