//
// PanelScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file PanelScriptFuncs.h
* @brief 
* Scripting functions for Panel Widget.
*
*/

#pragma once

#ifndef Vorb_PanelScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_PanelScriptFuncs_h__
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
        class Panel;

        class PanelScriptFuncs : public WidgetScriptFuncs {
        public:
            virtual void init(const cString nSpace, vscript::Environment* env) override;
        protected:
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getTexture(Panel* p) const;
            virtual color4 getColor(Panel* p) const;
            virtual color4 getHoverColor(Panel* p) const;
            virtual bool getAutoScroll(Panel* p) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTexture(Panel* p, VGTexture texture) const;
            virtual void setColor(Panel* p, color4 color) const;
            virtual void setHoverColor(Panel* p, color4 color) const;
            virtual void setAutoScroll(Panel* p, bool autoScroll) const;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_PanelScriptFuncs_h__
