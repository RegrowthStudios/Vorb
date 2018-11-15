#include "Vorb/stdafx.h"
#include "Vorb/graphics/script/TextureCacheScriptFuncs.h"

VGTexture vg::TextureCacheScriptFuncs::impl::loadTexture(           TextureCache* cache,
                                                                          nString filepath,
                                                                vg::TextureTarget textureTarget,
                                                                vg::SamplerState* samplingParameters,
                                                        vg::TextureInternalFormat internalFormat,
                                                                vg::TextureFormat textureFormat,
                                                                              i32 mipmapLevels              ) {
    return cache->addTexture(filepath, textureTarget, samplingParameters,
                      internalFormat, textureFormat, mipmapLevels).id;
}

VGTexture vg::TextureCacheScriptFuncs::impl::loadTextureDefault(TextureCache* cache, nString filepath) {
    return cache->addTexture(filepath).id;
}

bool vg::TextureCacheScriptFuncs::impl::freeTexture(TextureCache* cache, VGTexture texture) {
    return cache->freeTexture(texture);
}
