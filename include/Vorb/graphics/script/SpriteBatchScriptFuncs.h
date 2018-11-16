
//
// SpriteBatchScriptFuncs.h
// Vorb Engine
//
// Created by Matthew Marshall on 12 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file SpriteBatchScriptFuncs.h
* \brief Registers functions and consts for sprite batch to a script environment.
*/

#pragma once

#ifndef Vorb_SpriteBatchScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SpriteBatchScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace graphics {
        namespace SpriteBatchScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                // Empty
            }
        }
    }
}

namespace vg = vorb::graphics;
template <typename ScriptEnvironmentImpl>
void vg::SpriteBatchScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

template <typename ScriptEnvironmentImpl>
void vg::SpriteBatchScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("Graphics", "GradientType");
    env->addValue("NONE",           vg::GradientType::NONE);
    env->addValue("HORIZONTAL",     vg::GradientType::HORIZONTAL);
    env->addValue("VERTICAL",       vg::GradientType::VERTICAL);
    env->addValue("LEFT_DIAGONAL",  vg::GradientType::LEFT_DIAGONAL);
    env->addValue("RIGHT_DIAGONAL", vg::GradientType::RIGHT_DIAGONAL);
    env->setNamespaces();
}

#endif // !Vorb_SpriteBatchScriptFuncs_h__
