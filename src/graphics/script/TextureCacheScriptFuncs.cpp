#include "stdafx.h"
#include "Vorb/graphics/script/TextureCacheScriptFuncs.h"

#include "Vorb/graphics/TextureCache.h"

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
        return impl::loadTexture(cache, filepath);
    });
    env->addCDelegate("freeTexture", [cache] (VGTexture texture) {
        return impl::freeTexture(cache, texture);
    });
    env->setNamespace();
}

template <typename ScriptEnvironmentImpl>
void vg::TextureCacheScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    // Empty
}

VGTexture vg::TextureCacheScriptFuncs::impl::loadTexture(
                    TextureCache* cache,
                          nString filepath,
                vg::TextureTarget textureTarget,
                vg::SamplerState* samplingParameters,
        vg::TextureInternalFormat internalFormat,
                vg::TextureFormat textureFormat,
                              i32 mipmapLevels) {
    return cache->addTexture(filepath, textureTarget, samplingParameters,
                      internalFormat, textureFormat, mipmapLevels).id;
}

VGTexture vg::TextureCacheScriptFuncs::impl::loadTextureDefault(TextureCache* cache, nString filepath) {
    return cache->addTexture(filepath).id;
}

bool vg::TextureCacheScriptFuncs::impl::freeTexture(TextureCache* cache, VGTexture texture) {
    return cache->freeTexture(texture);
}
