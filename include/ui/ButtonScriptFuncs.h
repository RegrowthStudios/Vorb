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
* Scripting functions for buttons.
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

        // Forward declarations
        class Button;

        class ButtonScriptFuncs : public WidgetScriptFuncs {
        public:
            template <class T = ButtonScriptFuncs>
            void registerFuncs(const cString nSpace, vscript::Environment& env);
        protected:
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getTexture(Button* b) const;
            virtual color4 getBackColor(Button* b) const;
            virtual color4 getBackHoverColor(Button* b) const;
            virtual color4 getTextColor(Button* b) const;
            virtual color4 getTextHoverColor(Button* b) const;
            virtual nString getText(Button* b) const;
            virtual vg::TextAlign getTextAlign(Button* b) const;
            virtual f32v2 getTextScale(Button* b) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setBackColor(Button* b, color4 color) const;
            virtual void setBackHoverColor(Button* b, color4 color) const;
            virtual void setText(Button* b, nString text) const;
            virtual void setTextColor(Button* b, color4 color) const;
            virtual void setTextHoverColor(Button* b, color4 color) const;
            virtual void setTextAlign(Button* b, vg::TextAlign textAlign) const;
            virtual void setTextScale(Button* b, f32v2 textScale) const;
            virtual void setTexture(Button* b, VGTexture texture) const;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ButtonScriptFuncs_h__
