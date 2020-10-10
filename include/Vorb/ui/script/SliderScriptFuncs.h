//
// SliderScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file SliderScriptFuncs.h
* \brief Registers functions ands for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_SliderScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SliderScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/graphics/gtypes.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/script/WidgetScriptFuncs.h"
#include "Vorb/ui/widgets/Slider.h"

namespace vorb {
    namespace ui {
        // Forward Declarations
        class Slider;

        namespace SliderScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                VGTexture getSlideTexture         (Slider* slider);
                VGTexture getBarTexture           (Slider* slider);
                   color4 getSlideColor           (Slider* slider);
                   color4 getSlideHoverColor      (Slider* slider);
                   color4 getBarColor             (Slider* slider);
                      i32 getValue                (Slider* slider);
                      i32 getMin                  (Slider* slider);
                      i32 getMax                  (Slider* slider);
                    f32v2 getSlideSize            (Slider* slider);
                  Length2 getRawSlideSize         (Slider* slider);
                     bool isVertical              (Slider* slider);
                     bool isHorizontal            (Slider* slider);
                      f32 getValueScaled          (Slider* slider);
                      f32 getSlideWeight          (Slider* slider);
                      f32 getSlideKineticFriction (Slider* slider);
                      f32 getSlideStaticFriction  (Slider* slider);
                      f32 getSlideMaxSpeed        (Slider* slider);
                      f32 getScrollSensitivity    (Slider* slider);
                      bool scrollOnParent         (Slider* slider);

                bool isInSlideBounds(Slider* slider, f32 x, f32 y);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setSlideTexture         (Slider* slider, VGTexture texture);
                void setBarTexture           (Slider* slider, VGTexture texture);
                void setSlideColor           (Slider* slider, color4 color);
                void setSlideHoverColor      (Slider* slider, color4 color);
                void setBarColor             (Slider* slider, color4 color);
                void setValue                (Slider* slider, i32 value);
                void setMin                  (Slider* slider, i32 min);
                void setMax                  (Slider* slider, i32 max);
                void setSlideSize            (Slider* slider, f32v2 dimensions);
                void setRawSlideSize         (Slider* slider, Length2 dimensions);
                void setIsVertical           (Slider* slider, bool isVertical);
                void setRange                (Slider* slider, i32 min, i32 max);
                void setSlideWeight          (Slider* slider, f32 slideWeight);
                void setSlideKineticFriction (Slider* slider, f32 slideKineticFriction);
                void setSlideStaticFriction  (Slider* slider, f32 slideStaticFriction);
                void setSlideMaxSpeed        (Slider* slider, f32 slideMaxSpeed);
                void setScrollSensitivity    (Slider* slider, f32 scrollSensitivity);
                void setScrollOnParent       (Slider* slider, bool scrollOnParent);

                // void onValueChange(Sender s, i32 v);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::SliderScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getSlideTexture",         makeDelegate(&impl::getSlideTexture));
    env->addCDelegate("setSlideTexture",         makeDelegate(&impl::setSlideTexture));
    env->addCDelegate("getBarTexture",           makeDelegate(&impl::getBarTexture));
    env->addCDelegate("setBarTexture",           makeDelegate(&impl::setBarTexture));
    env->addCDelegate("getSlideColor",           makeDelegate(&impl::getSlideColor));
    env->addCDelegate("setSlideColor",           makeDelegate(&impl::setSlideColor));
    env->addCDelegate("getSlideHoverColor",      makeDelegate(&impl::getSlideHoverColor));
    env->addCDelegate("setSlideHoverColor",      makeDelegate(&impl::setSlideHoverColor));
    env->addCDelegate("getBarColor",             makeDelegate(&impl::getBarColor));
    env->addCDelegate("setBarColor",             makeDelegate(&impl::setBarColor));
    env->addCDelegate("getValue",                makeDelegate(&impl::getValue));
    env->addCDelegate("setValue",                makeDelegate(&impl::setValue));
    env->addCDelegate("getMin",                  makeDelegate(&impl::getMin));
    env->addCDelegate("setMin",                  makeDelegate(&impl::setMin));
    env->addCDelegate("getMax",                  makeDelegate(&impl::getMax));
    env->addCDelegate("setMax",                  makeDelegate(&impl::setMax));
    env->addCDelegate("getSlideSize",            makeDelegate(&impl::getSlideSize));
    env->addCDelegate("setSlideSize",            makeDelegate(&impl::setSlideSize));
    // env->addCDelegate("getRawSlideSize",    makeDelegate(&impl::getRawSlideSize));
    // env->addCDelegate("setRawSlideSize",    makeDelegate(&impl::setRawSlideSize));
    env->addCDelegate("isVertical",              makeDelegate(&impl::isVertical));
    env->addCDelegate("setIsVertical",           makeDelegate(&impl::setIsVertical));
    env->addCDelegate("isHorizontal",            makeDelegate(&impl::isHorizontal));
    env->addCDelegate("getValueScaled",          makeDelegate(&impl::getValueScaled));
    env->addCDelegate("setRange",                makeDelegate(&impl::setRange));
    env->addCDelegate("getSlideWeight",          makeDelegate(&impl::getSlideWeight));
    env->addCDelegate("setSlideWeight",          makeDelegate(&impl::setSlideWeight));
    env->addCDelegate("getSlideKineticFriction", makeDelegate(&impl::getSlideKineticFriction));
    env->addCDelegate("setSlideKineticFriction", makeDelegate(&impl::setSlideKineticFriction));
    env->addCDelegate("getSlideStaticFriction",  makeDelegate(&impl::getSlideStaticFriction));
    env->addCDelegate("setSlideStaticFriction",  makeDelegate(&impl::setSlideStaticFriction));
    env->addCDelegate("getSlideMaxSpeed",        makeDelegate(&impl::getSlideMaxSpeed));
    env->addCDelegate("setSlideMaxSpeed",        makeDelegate(&impl::setSlideMaxSpeed));
    env->addCDelegate("getScrollSensitivity",    makeDelegate(&impl::getScrollSensitivity));
    env->addCDelegate("setScrollSensitivity",    makeDelegate(&impl::setScrollSensitivity));
    env->addCDelegate("scrollOnParent",          makeDelegate(&impl::scrollOnParent));
    env->addCDelegate("setScrollOnParent",       makeDelegate(&impl::setScrollOnParent));

    env->setNamespaces("UI", namespace_, "onValueChange");
    env->addCDelegate("subscribe", makeFunctor([=](Slider* slider, nString name) {
        slider->ValueChange.add(env->template getScriptDelegate<void, Sender, i32>(name), true);
    }));
    env->addCDelegate("unsubscribe",  makeFunctor([=](Slider* slider, nString name) {
        slider->ValueChange.remove(env->template getScriptDelegate<void, Sender, i32>(name, false));
    }));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::SliderScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_SliderScriptFuncs_h__
