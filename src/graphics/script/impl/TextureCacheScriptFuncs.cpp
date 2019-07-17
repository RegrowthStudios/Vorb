#include "Vorb/stdafx.h"
#include "Vorb/graphics/script/impl/TextureCacheScriptFuncs.h"

VGTexture vg::impl::TextureCacheScriptFuncs::impl::loadTexture(           TextureCache* cache,
                                                                          nString filepath,
                                                                vg::TextureTarget textureTarget,
                                                                vg::SamplerState* samplingParameters,
                                                        vg::TextureInternalFormat internalFormat,
                                                                vg::TextureFormat textureFormat,
                                                                              i32 mipmapLevels              ) {
    return cache->addTexture(filepath, textureTarget, samplingParameters,
                      internalFormat, textureFormat, mipmapLevels).id;
}

VGTexture vg::impl::TextureCacheScriptFuncs::impl::loadTextureDefault(TextureCache* cache, nString filepath) {
    return cache->addTexture(filepath).id;
}

bool vg::impl::TextureCacheScriptFuncs::impl::freeTexture(TextureCache* cache, VGTexture texture) {
    return cache->freeTexture(texture);
}
