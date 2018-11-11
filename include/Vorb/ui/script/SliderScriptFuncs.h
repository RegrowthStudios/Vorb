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

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/gtypes.h"
#include "Vorb/ui/Widget.h"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

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
                VGTexture getSlideTexture    (Slider* slider);
                VGTexture getBarTexture      (Slider* slider);
                   color4 getSlideColor      (Slider* slider);
                   color4 getSlideHoverColor (Slider* slider);
                   color4 getBarColor        (Slider* slider);
                      i32 getValue           (Slider* slider);
                      i32 getMin             (Slider* slider);
                      i32 getMax             (Slider* slider);
                    f32v2 getSlideSize       (Slider* slider);
                  Length2 getRawSlideSize    (Slider* slider);
                     bool isVertical         (Slider* slider);
                     bool isHorizontal       (Slider* slider);
                      f32 getValueScaled     (Slider* slider);

                bool isInSlideBounds(Slider* slider, f32 x, f32 y);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setSlideTexture    (Slider* slider, VGTexture texture);
                void setBarTexture      (Slider* slider, VGTexture texture);
                void setSlideColor      (Slider* slider, color4 color);
                void setSlideHoverColor (Slider* slider, color4 color);
                void setBarColor        (Slider* slider, color4 color);
                void setValue           (Slider* slider, i32 value);
                void setMin             (Slider* slider, i32 min);
                void setMax             (Slider* slider, i32 max);
                void setSlideSize       (Slider* slider, f32v2 dimensions);
                void setRawSlideSize    (Slider* slider, Length2 dimensions);
                void setIsVertical      (Slider* slider, bool isVertical);
                void setRange           (Slider* slider, i32 min, i32 max);

                // void onValueChange(Sender s, i32 v);
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_SliderScriptFuncs_h__
