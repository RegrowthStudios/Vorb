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

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace graphics {
        // Forward Declarations
        class TextureCache;
        class SamplerState;

        namespace TextureCacheScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env, TextureCache* cache);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                VGTexture loadTexture(        TextureCache* cache,
                                                    nString filePath,
                                              TextureTarget textureTarget,
                                              SamplerState* samplingParameters,
                                      TextureInternalFormat internalFormat,
                                              TextureFormat textureFormat,
                                                        i32 mipmapLevels);

                VGTexture loadTextureDefault(TextureCache* cache, nString filepath);

                bool freeTexture(TextureCache* cache, VGTexture texture);
            }
        }
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_TextureCacheScriptFuncs_h__
