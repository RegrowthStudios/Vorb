//
// GraphicsScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file GraphicsScriptFuncs.h
* \brief Registers functions and consts for graphics (that are useful for UI) to a script environment.
*/

#pragma once

#ifndef Vorb_GraphicsScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_GraphicsScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/graphics/gtypes.h"
#include "Vorb/graphics/TextureCache.h"
#include "Vorb/graphics/script/impl/GLEnumsScriptFuncs.hpp"
#include "Vorb/graphics/script/impl/SamplerStateScriptFuncs.hpp"
#include "Vorb/graphics/script/impl/SpriteBatchScriptFuncs.hpp"
#include "Vorb/graphics/script/impl/SpriteFontScriptFuncs.hpp"
#include "Vorb/graphics/script/impl/TextureCacheScriptFuncs.h"
#include "Vorb/script/IEnvironment.hpp"

namespace vorb {
    namespace ui {
        namespace GraphicsScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env, vg::TextureCache* cache);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*);

            namespace impl {
                // Empty
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::GraphicsScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env, vg::TextureCache* cache) {
    using namespace vg::impl;

    GLEnumsScriptFuncs::registerFuncs(env);

    SamplerStateScriptFuncs::registerFuncs(env);

    SpriteBatchScriptFuncs::registerFuncs(env);

    SpriteFontScriptFuncs::registerFuncs(env);

    TextureCacheScriptFuncs::registerFuncs(env, cache);
}

template <typename ScriptEnvironmentImpl>
void vui::GraphicsScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    using namespace vg::impl;

    GLEnumsScriptFuncs::registerConsts(env);

    SamplerStateScriptFuncs::registerConsts(env);

    SpriteBatchScriptFuncs::registerConsts(env);

    SpriteFontScriptFuncs::registerConsts(env);

    TextureCacheScriptFuncs::registerConsts(env);
}

#endif // !Vorb_GraphicsScriptFuncs_h__
