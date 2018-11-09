//
// SliderScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file SliderScriptFuncs.h
* @brief 
* Scripting functions for the Slider widget.
*
*/

#pragma once

#ifndef Vorb_SliderScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SliderScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/WidgetScriptFuncs.h"
#include "Vorb/graphics/gtypes.h"

namespace vorb {
    namespace ui {

        // Forward declarations
        class Slider;

        class SliderScriptFuncs : public WidgetScriptFuncs {
        public:
            virtual void init(const cString nSpace, vscript::Environment* env) override;
            virtual void registerWidget(Widget* w) override;
            virtual void unregisterWidget(Widget* w) override;
        protected:
            virtual bool addCallback(Widget* w, EventType eventType, nString funcName) override;
            /*! @brief Deregisters a LUA callback with a widget* */
            virtual bool removeCallback(Widget* w, EventType eventType, nString funcName) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getSlideTexture(Slider* s) const;
            virtual VGTexture getBarTexture(Slider* s) const;
            virtual color4 getSlideColor(Slider* s) const;
            virtual color4 getSlideHoverColor(Slider* s) const;
            virtual color4 getBarColor(Slider* s) const;
            virtual int getValue(Slider* s) const;
            virtual int getMin(Slider* s) const;
            virtual int getMax(Slider* s) const;
            /// Gets slider value scaled between 0.0f and 1.0f
            virtual f32 getValueScaled(Slider* s) const;
            virtual bool isVertical(Slider* s) const;
           
            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setSlideSize(Slider* s, f32v2 dimensions) const;
            virtual void setSlideTexture(Slider* s, VGTexture texture) const;
            virtual void setBarTexture(Slider* s, VGTexture texture) const;
            virtual void setBarColor(Slider* s, color4 color) const;
            virtual void setSlideColor(Slider* s, color4 color) const;
            virtual void setSlideHoverColor(Slider* s, color4 color) const;
            virtual void setValue(Slider* s, int value) const;
            virtual void setRange(Slider* s, int min, int max) const;
            virtual void setMin(Slider* s, int min) const;
            virtual void setMax(Slider* s, int max) const;
            virtual void setIsVertical(Slider* s, bool isVertical) const;

            virtual bool isInSlideBounds(Slider* s, f32v2 point) const;

            void onValueChange(Sender s, int v) const;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_SliderScriptFuncs_h__
