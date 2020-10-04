//
// FontCacheScriptFuncs.hpp
// Vorb Engine
//
// Created by Matthew Marshall 04 Oct 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file FontCacheScriptFuncs.hpp
* \brief Registers functions and consts for font cache to script environment.
*/

#pragma once

#ifndef Vorb_FontCacheScriptFuncs_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FontCacheScriptFuncs_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/FontCache.h"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace graphics {
        namespace impl {
            namespace FontCacheScriptFuncs {
                template <typename ScriptEnvironmentImpl>
                void registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env, FontCache* cache);

                template <typename ScriptEnvironmentImpl>
                void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);
            }
        }
    }
}
namespace vg = vorb::graphics;

template <typename ScriptEnvironmentImpl>
void vg::impl::FontCacheScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env, FontCache* cache) {
    env->setNamespaces("Graphics");
    env->addCDelegate("getFont", makeFunctor([cache] (nString filepath, ui32 size, char cs, char ce) {
        return cache->getFont(vio::Path(filepath), size, cs, ce);
    }));
    env->addCDelegate("getCompleteFont", makeFunctor([cache] (nString filepath, ui32 size) {
        return cache->getFont(vio::Path(filepath), size);
    }));
    env->addCDelegate("tryGetFont", makeFunctor([cache] (nString filepath, ui32 size, char cs, char ce) {
        return cache->tryGetFont(vio::Path(filepath), size, cs, ce);
    }));
    env->addCDelegate("tryGetCompleteFont", makeFunctor([cache] (nString filepath, ui32 size) {
        return cache->tryGetFont(vio::Path(filepath), size);
    }));
    env->addCDelegate("freeFont", makeFunctor([cache] (nString filepath, ui32 size, char cs, char ce) {
        return cache->freeFont(vio::Path(filepath), size, cs, ce);
    }));
    env->addCDelegate("freeCompleteFont", makeFunctor([cache] (nString filepath, ui32 size) {
        return cache->freeFont(vio::Path(filepath), size);
    }));
    env->setNamespaces();
}

template <typename ScriptEnvironmentImpl>
void vg::impl::FontCacheScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_FontCacheScriptFuncs_hpp__
