//
// ComboBoxScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ComboBoxScriptFuncs.h
* @brief 
* Script functions for ComboBox widget.
*
*/

#pragma once

#ifndef Vorb_ComboBoxScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ComboBoxScriptFuncs_h__
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
        class ComboBox;

        class ComboBoxScriptFuncs : public WidgetScriptFuncs {
        public:
            void init(const cString nSpace, vscript::Environment* env) override;
            virtual void registerWidget(Widget* w) override;
            virtual void unregisterWidget(Widget* w) override;
        protected:
            virtual bool addCallback(Widget* w, EventType eventType, nString funcName) override;
            /*! @brief Deregisters a LUA callback with a widget* */
            virtual bool removeCallback(Widget* w, EventType eventType, nString funcName) override;

            virtual void addItem(ComboBox* c, nString item) const;
            virtual bool addItemAtIndex(ComboBox* c, int index, nString item) const;
            virtual bool removeItem(ComboBox* c, nString item) const;
            virtual bool removeItemAtIndex(ComboBox* c, int index) const;
            virtual bool selectItemAtIndex(ComboBox* c, int index) const;

            virtual bool isInDropBounds(ComboBox* c, f32v2 point) const;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getTexture(ComboBox* c) const;
            virtual color4 getBackColor(ComboBox* c) const;
            virtual color4 getBackHoverColor(ComboBox* c) const;
            virtual color4 getTextColor(ComboBox* c) const;
            virtual color4 getTextHoverColor(ComboBox* c) const;
            virtual f32v2 getTextScale(ComboBox* c) const;
            virtual nString getItem(ComboBox* c, int index) const;
            virtual size_t getNumItems(ComboBox* c) const;
            virtual vg::TextAlign getTextAlign(ComboBox* c) const;
            virtual nString getText(ComboBox* c) const;
            virtual f32 getMaxDropHeight(ComboBox* c) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTexture(ComboBox* c, VGTexture texture) const;
            virtual void setDropBoxTexture(ComboBox* c, VGTexture texture) const;
            virtual void setDropButtonTexture(ComboBox* c, VGTexture texture) const;
            virtual void setBackColor(ComboBox* c, color4 color) const;
            virtual void setBackHoverColor(ComboBox* c, color4 color) const;
            virtual void setTextColor(ComboBox* c, color4 color) const;
            virtual void setTextHoverColor(ComboBox* c, color4 color) const;
            virtual void setTextScale(ComboBox* c, f32v2 textScale) const;
            virtual void setTextAlign(ComboBox* c, vg::TextAlign align) const;
            virtual void setText(ComboBox* c, nString text) const;
            virtual void setMaxDropHeight(ComboBox* c, f32 maxDropHeight) const;

            void onValueChange(Sender s, const nString& v) const;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ComboBoxScriptFuncs_h__
