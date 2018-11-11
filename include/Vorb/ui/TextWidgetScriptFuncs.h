//
// TextWidgetScriptFuncs.h
// Vorb Engine
//
// Created by Matthew Marshall 11 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file TextWidgetScriptFuncs.h
* \brief Provides methods to register functions and constants of TextWidget to a script environment.
*/

#pragma once

#ifndef Vorb_WidgetListScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetListScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"

DECL_VG(class SpriteFont; enum class TextAlign)
DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace ui {
        // Forward Declarations
        class TextWidget;

        namespace TextWidgetScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                const vg::SpriteFont* getFont      (TextWidget* textWidget);
                              nString getText      (TextWidget* textWidget);
                        vg::TextAlign getTextAlign (TextWidget* textWidget);
                               color4 getTextColor (TextWidget* textWidget);
                                f32v2 getTextScale (TextWidget* textWidget);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setFont      (TextWidget* textWidget, vg::SpriteFont* font);
                void setText      (TextWidget* textWidget, nString text);
                void setTextAlign (TextWidget* textWidget, vg::TextAlign textAlign);
                void setTextColor (TextWidget* textWidget, color4 color);
                void setTextScale (TextWidget* textWidget, f32v2 scale);
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetListScriptFuncs_h__
