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
#include "graphics/gtypes.h"
#include "graphics/SpriteFont.h"

namespace vorb {
    namespace ui {
        class ButtonScriptFuncs : public WidgetScriptFuncs {
        public:
            template <class T = ButtonScriptFuncs>
            virtual void registerFuncs(const cString nSpace, vscript::Environment& env) override;
        protected:
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getTexture(WidgetID id) const;
            virtual color4 getBackColor(WidgetID id) const;
            virtual color4 getBackHoverColor(WidgetID id) const;
            virtual color4 getTextColor(WidgetID id) const;
            virtual color4 getTextHoverColor(WidgetID id) const;
            virtual nString getText(WidgetID id) const;
            virtual vg::TextAlign getTextAlign(WidgetID id) const;
            virtual f32v2 getTextScale(WidgetID id) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setBackColor(WidgetID id, const color4& color);
            virtual void setBackHoverColor(WidgetID id, const color4& color);
            virtual void setText(WidgetID id, const nString& text);
            virtual void setTextColor(WidgetID id, const color4& color);
            virtual void setTextHoverColor(WidgetID id, const color4& color);
            virtual void setTextAlign(WidgetID id, int textAlign);
            virtual void setTextScale(WidgetID id, const f32v2& textScale);
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ButtonScriptFuncs_h__
