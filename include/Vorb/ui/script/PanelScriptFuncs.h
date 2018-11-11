//
// PanelScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file PanelScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_PanelScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_PanelScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/script/WidgetScriptFuncs.h"
#include "Vorb/graphics/gtypes.h"
#include "Vorb/graphics/SpriteFont.h"

namespace vorb {
    namespace ui {
        // Forward Declarations
        class Panel;

        namespace PanelScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                VGTexture getTexture     (Panel* panel);
                     bool getAutoScroll  (Panel* panel);
                      f32 getSliderWidth (Panel* panel);
                   color4 getColor       (Panel* panel);
                   color4 getHoverColor  (Panel* panel);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setTexture     (Panel* panel, VGTexture texture);
                void setAutoScroll  (Panel* panel, bool autoScroll);
                void setSliderWidth (Panel* panel, f32 width);
                void setColor       (Panel* panel, color4 color);
                void setHoverColor  (Panel* panel, color4 color);
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_PanelScriptFuncs_h__
