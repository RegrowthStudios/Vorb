//
// LabelScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 15 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file LabelScriptFuncs.h
* @brief 
*
*
*/

#pragma once

#ifndef Vorb_LabelScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_LabelScriptFuncs_h__
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
        class Label;

        class LabelScriptFuncs : public WidgetScriptFuncs {
        public:
            virtual void init(const cString nSpace, vscript::Environment* env) override;
        protected:
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual color4 getTextColor(Label* l) const;
            virtual nString getText(Label* l) const;
            virtual vg::TextAlign getTextAlign(Label* l) const;
            virtual f32v2 getTextScale(Label* l) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setText(Label* l, nString text) const;
            virtual void setTextColor(Label* l, color4 color) const;
            virtual void setTextAlign(Label* l, vg::TextAlign textAlign) const;
            virtual void setTextScale(Label* l, f32v2 textScale) const;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_LabelScriptFuncs_h__
