
//
// Graphics.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 13 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file Graphics.hpp
* \brief Provides an entry-point to register various graphics callbacks and constants to a script environment.
*/

#pragma once

#ifndef Vorb_Graphics_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Graphics_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "impl/GLEnumsScriptFuncs.hpp"
#include "impl/SamplerStateScriptFuncs.hpp"
#include "impl/SpriteBatchScriptFuncs.hpp"
#include "impl/SpriteFontScriptFuncs.hpp"
#include "impl/TextureCacheScriptFuncs.h"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace graphics {
        template <typename ScriptEnvironmentImpl>
        void registerToScriptEnv(vscript::IEnvironment<ScriptEnvironmentImpl>* env);
    }
}
namespace vg = vorb::graphics;

template <typename ScriptEnvironmentImpl>
void vg::registerToScriptEnv(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    using namespace vg::impl;

    GLEnumsScriptFuncs::registerFuncs(env);
    GLEnumsScriptFuncs::registerConsts(env);

    SamplerStateScriptFuncs::registerFuncs(env);
    SamplerStateScriptFuncs::registerConsts(env);

    SpriteBatchScriptFuncs::registerFuncs(env);
    SpriteBatchScriptFuncs::registerConsts(env);

    SpriteFontScriptFuncs::registerFuncs(env);
    SpriteFontScriptFuncs::registerConsts(env);

    TextureCacheScriptFuncs::registerFuncs(env);
    TextureCacheScriptFuncs::registerConsts(env);
}

#endif // !Vorb_Graphicss_h__

