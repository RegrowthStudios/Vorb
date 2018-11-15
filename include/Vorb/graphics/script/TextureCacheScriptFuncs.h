//
// TextureCacheScriptFuncs.h
// Vorb Engine
//
// Created by Matthew Marshall 12 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file TextureCacheScriptFuncs.h
* \brief Registers functions and consts for texture cache to script environment.
*/

#pragma once

#ifndef Vorb_TextureCacheScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TextureCacheScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/GLEnums.h"
#include "Vorb/graphics/TextureCache.h"
#include "Vorb/graphics/SamplerState.h"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace graphics {
        namespace TextureCacheScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env, TextureCache* cache);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                VGTexture loadTexture(        TextureCache* cache,
                                                    nString filePath,
                                              TextureTarget textureTarget,
                                              SamplerState* samplingParameters,
                                      TextureInternalFormat internalFormat,
                                              TextureFormat textureFormat,
                                                        i32 mipmapLevels        );

                VGTexture loadTextureDefault(TextureCache* cache, nString filepath);

                bool freeTexture(TextureCache* cache, VGTexture texture);
            }
        }
    }
}
namespace vg = vorb::graphics;

template <typename ScriptEnvironmentImpl>
void vg::TextureCacheScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env, TextureCache* cache) {
    env->setNamespace("Graphics");
    env->addCDelegate("loadTexture", [cache] (      nString filepath,
                                              TextureTarget textureTarget,
                                              SamplerState* samplingParameters,
                                      TextureInternalFormat internalFormat,
                                              TextureFormat textureFormat,
                                                        i32 mipmapLevels    ){
        return impl::loadTexture(cache, filepath, textureTarget, samplingParameters,
                                    internalFormat, textureFormat, mipmapLevels);
    });
    env->addCDelegate("loadTextureDefault", [cache] (nString filepath){
        return impl::loadTextureDefault(cache, filepath);
    });
    env->addCDelegate("freeTexture", [cache] (VGTexture texture) {
        return impl::freeTexture(cache, texture);
    });
    env->setNamespace();
}

template <typename ScriptEnvironmentImpl>
void vg::TextureCacheScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_TextureCacheScriptFuncs_h__
