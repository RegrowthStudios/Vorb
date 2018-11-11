//
// LabelScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 15 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file LabelScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_LabelScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_LabelScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/gtypes.h"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace ui {
        // Forward Declarations
        class Label;

        namespace LabelScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);
            
            namespace impl {
                /******************************************************************/
                /* Getters                                                        */
                /******************************************************************/
                   color4 getLabelColor        (Label* label);
                   color4 getLabelHoverColor   (Label* label);
                VGTexture getLabelTexture      (Label* label);
                VGTexture getLabelHoverTexture (Label* label);
                   color4 getTextHoverColor    (Label* label);

                /******************************************************************/
                /* Setters                                                        */
                /******************************************************************/
                void setLabelColor        (Label* label, color4 color);
                void setLabelHoverColor   (Label* label, color4 color);
                void setLabelTexture      (Label* label, VGTexture texture);
                void setLabelHoverTexture (Label* label, VGTexture texture);
                void setTextHoverColor    (Label* label, color4 color);
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_LabelScriptFuncs_h__
