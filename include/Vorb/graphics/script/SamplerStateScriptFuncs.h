
//
// SamplerStateScriptFuncs.h
// Vorb Engine
//
// Created by Matthew Marshall on 12 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file SamplerStateScriptFuncs.h
* \brief Registers functions and consts for sampler state to a script environment.
*/

#pragma once

#ifndef Vorb_SamplerStateScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SamplerStateScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace graphics {
        namespace SamplerStateScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                // Empty
            }
        }
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_SamplerStateScriptFuncs_h__

