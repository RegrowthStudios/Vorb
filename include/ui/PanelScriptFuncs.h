//
// PanelScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
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
#include "types.h"
#endif // !VORB_USING_PCH

#include "WidgetScriptFuncs.h"
#include "../graphics/gtypes.h"
#include "../graphics/SpriteFont.h"

namespace vorb {
    namespace ui {

        // Forward declarations
        class Panel;

        class PanelScriptFuncs : public WidgetScriptFuncs {
        public:
            template <class T = PanelScriptFuncs>
            void registerFuncs(const cString nSpace, vscript::Environment& env);
        protected:
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getTexture(Panel* p) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTexture(Panel* p, VGTexture texture) const;
            virtual void setColor(Panel* p, color4 color) const;
            virtual void setHoverColor(Panel* p, color4 color) const;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_PanelScriptFuncs_h__
