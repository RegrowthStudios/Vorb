//
// CheckBoxScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file CheckBoxScriptFuncs.h
* @brief 
* Script functions for CheckBox widget.
*
*/

#pragma once

#ifndef Vorb_CheckBoxScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_CheckBoxScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/WidgetScriptFuncs.h"
#include "Vorb/graphics/gtypes.h"
#include "Vorb/graphics/SpriteFont.h"

namespace vorb {
    namespace ui {

        // Forward declarations
        class CheckBox;

        class CheckBoxScriptFuncs : public WidgetScriptFuncs {
        public:
            void init(const cString nSpace, vscript::Environment* env) override;

            virtual void registerWidget(Widget* w) override;
            virtual void unregisterWidget(Widget* w) override;
        protected:
            virtual bool addCallback(Widget* w, EventType eventType, nString funcName) override;
            /*! @brief Deregisters a LUA callback with a widget* */
            virtual bool removeCallback(Widget* w, EventType eventType, nString funcName) override;

            virtual void check(CheckBox* c) const;
            virtual void unCheck(CheckBox* c) const;
            virtual void toggleChecked(CheckBox* c) const;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getUncheckedTexture(CheckBox* c) const;
            virtual VGTexture getCheckedTexture(CheckBox* c) const;
            virtual color4 getBoxColor(CheckBox* c) const;
            virtual color4 getBoxHoverColor(CheckBox* c) const;
            virtual color4 getBoxCheckedColor(CheckBox* c) const;
            virtual color4 getBoxCheckedHoverColor(CheckBox* c) const;
            virtual color4 getTextColor(CheckBox* c) const;
            virtual nString getText(CheckBox* c) const;
            virtual vg::TextAlign getTextAlign(CheckBox* c) const;
            virtual f32v2 getTextScale(CheckBox* c) const;
            virtual bool isChecked(CheckBox* c) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setCheckedTexture(CheckBox* c, VGTexture texture) const;
            virtual void setUncheckedTexture(CheckBox* c, VGTexture texture) const;
            virtual void setBoxColor(CheckBox* c, color4 color) const;
            virtual void setBoxHoverColor(CheckBox* c, color4 color) const;
            virtual void setBoxCheckedColor(CheckBox* c, color4 color) const;
            virtual void setBoxCheckedHoverColor(CheckBox* c, color4 color) const;
            virtual void setText(CheckBox* c, nString text) const;
            virtual void setTextColor(CheckBox* c, color4 color) const;
            virtual void setTextAlign(CheckBox* c, vg::TextAlign textAlign) const;
            virtual void setTextScale(CheckBox* c, f32v2 textScale) const;
            virtual void setChecked(CheckBox* c, bool checked) const;

            virtual void onValueChange(Sender s, bool b) const;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_CheckBoxScriptFuncs_h__
