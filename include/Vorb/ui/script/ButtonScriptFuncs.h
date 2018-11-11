//
// ButtonScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ButtonScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_ButtonScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ButtonScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/gtypes.h"

DECL_VG(enum class GradientType)
DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace ui {
        // Forward declarations
        class Button;

        namespace ButtonScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*);

            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                VGTexture getTexture(Button* button);
                VGTexture getHoverTexture(Button* button);
                color4 getBackColor(Button* button);
                color4 getBackHoverColor(Button* button);
                color4 getTextHoverColor(Button* button);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setTexture(Button* button, VGTexture texture);
                void setHoverTexture(Button* button, VGTexture texture);
                void setBackColor(Button* button, color4 color);
                void setBackColorGrad(Button* button, color4 color1, color4 color2, vg::GradientType grad);
                void setBackHoverColor(Button* button, color4 color);
                void setBackHoverColorGrad(Button* button, color4 color1, color4 color2, vg::GradientType grad);
                void setTextHoverColor(Button* button, color4 color);
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ButtonScriptFuncs_h__
