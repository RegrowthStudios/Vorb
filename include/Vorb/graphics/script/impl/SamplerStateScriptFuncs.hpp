
//
// SamplerStateScriptFuncs.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 12 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file SamplerStateScriptFuncs.hpp
* \brief Registers functions and consts for sampler state to a script environment.
*/

#pragma once

#ifndef Vorb_SamplerStateScriptFuncs_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SamplerStateScriptFuncs_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace graphics {
        namespace impl {
            namespace SamplerStateScriptFuncs {
                template <typename ScriptEnvironmentImpl>
                void registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

                template <typename ScriptEnvironmentImpl>
                void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);
            }
        }
    }
}

namespace vg = vorb::graphics;
template <typename ScriptEnvironmentImpl>
void vg::impl::SamplerStateScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

template <typename ScriptEnvironmentImpl>
void vg::impl::SamplerStateScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("Graphics", "SamplerState");
    env->addValue("POINT_WRAP",          &vg::SamplerState::POINT_WRAP);
    env->addValue("POINT_CLAMP",         &vg::SamplerState::POINT_CLAMP);
    env->addValue("LINEAR_WRAP",         &vg::SamplerState::LINEAR_WRAP);
    env->addValue("LINEAR_CLAMP",        &vg::SamplerState::LINEAR_CLAMP);
    env->addValue("POINT_WRAP_MIPMAP",   &vg::SamplerState::POINT_WRAP_MIPMAP);
    env->addValue("POINT_CLAMP_MIPMAP",  &vg::SamplerState::POINT_CLAMP_MIPMAP);
    env->addValue("LINEAR_WRAP_MIPMAP",  &vg::SamplerState::LINEAR_WRAP_MIPMAP);
    env->addValue("LINEAR_CLAMP_MIPMAP", &vg::SamplerState::LINEAR_CLAMP_MIPMAP);
    env->setNamespaces();
}

#endif // !Vorb_SamplerStateScriptFuncs_hpp__
