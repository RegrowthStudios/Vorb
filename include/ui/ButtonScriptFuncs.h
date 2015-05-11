//
// ButtonScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ButtonScriptFuncs.h
* @brief 
* Scripting functions for buttons
*
*/

#pragma once

#ifndef Vorb_ButtonScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ButtonScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "WidgetScriptFuncs.h"
#include "../graphics/gtypes.h"
#include "../graphics/SpriteFont.h"

namespace vorb {
    namespace ui {
        class ButtonScriptFuncs : public WidgetScriptFuncs {
        public:
            template <class T = ButtonScriptFuncs>
            void registerFuncs(const cString nSpace, vscript::Environment& env);
        protected:
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getTexture(Widget* w) const;
            virtual color4 getBackColor(Widget* w) const;
            virtual color4 getBackHoverColor(Widget* w) const;
            virtual color4 getTextColor(Widget* w) const;
            virtual color4 getTextHoverColor(Widget* w) const;
            virtual nString getText(Widget* w) const;
            virtual vg::TextAlign getTextAlign(Widget* w) const;
            virtual f32v2 getTextScale(Widget* w) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setBackColor(Widget* w, color4 color);
            virtual void setBackHoverColor(Widget* w, color4 color);
            virtual void setText(Widget* w, nString text);
            virtual void setTextColor(Widget* w, color4 color);
            virtual void setTextHoverColor(Widget* w, color4 color);
            virtual void setTextAlign(Widget* w, int textAlign);
            virtual void setTextScale(Widget* w, f32v2 textScale);
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ButtonScriptFuncs_h__
