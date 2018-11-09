#include "Vorb/stdafx.h"
#include "Vorb/graphics/SamplerState.h"

#ifdef VORB_USING_SCRIPT
#include "Vorb/script/lua/Environment.h"
#endif

vg::SamplerState::SamplerState(TextureMinFilter texMinFilter, TextureMagFilter texMagFilter,
    TextureWrapMode texWrapS, TextureWrapMode texWrapT, TextureWrapMode texWrapR) :
    _minFilter(texMinFilter),
    _magFilter(texMagFilter),
    _wrapS(texWrapS),
    _wrapT(texWrapT),
    _wrapR(texWrapR) {}

void vg::SamplerState::initObject() {
    glGenSamplers(1, &_id);
    glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_minFilter));
    glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_magFilter));
    glSamplerParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_wrapS));
    glSamplerParameteri(_id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(_wrapT));
    glSamplerParameteri(_id, GL_TEXTURE_WRAP_R, static_cast<GLenum>(_wrapR));
}
void vg::SamplerState::initPredefined() {
    SamplerState::POINT_WRAP.initObject();
    SamplerState::POINT_CLAMP.initObject();
    SamplerState::LINEAR_WRAP.initObject();
    SamplerState::LINEAR_CLAMP.initObject();
    SamplerState::POINT_WRAP_MIPMAP.initObject();
    SamplerState::POINT_CLAMP_MIPMAP.initObject();
    SamplerState::LINEAR_WRAP_MIPMAP.initObject();
    SamplerState::LINEAR_CLAMP_MIPMAP.initObject();
}

void vg::SamplerState::set(ui32 textureTarget) const {
    glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_magFilter));
    glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_minFilter));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_wrapS));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, static_cast<GLenum>(_wrapT));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, static_cast<GLenum>(_wrapR));
}
void vg::SamplerState::setObject(ui32 textureUnit) const {
    glBindSampler(textureUnit, _id);
}

#ifdef VORB_USING_SCRIPT
void vg::SamplerState::registerStates(vscript::Environment& env) {
    env.setNamespaces("SamplerState");
    env.addValue("POINT_WRAP", &vg::SamplerState::POINT_WRAP);
    env.addValue("POINT_CLAMP", &vg::SamplerState::POINT_CLAMP);
    env.addValue("LINEAR_WRAP", &vg::SamplerState::LINEAR_WRAP);
    env.addValue("LINEAR_CLAMP", &vg::SamplerState::LINEAR_CLAMP);
    env.addValue("POINT_WRAP_MIPMAP", &vg::SamplerState::POINT_WRAP_MIPMAP);
    env.addValue("POINT_CLAMP_MIPMAP", &vg::SamplerState::POINT_CLAMP_MIPMAP);
    env.addValue("LINEAR_WRAP_MIPMAP", &vg::SamplerState::LINEAR_WRAP_MIPMAP);
    env.addValue("LINEAR_CLAMP_MIPMAP", &vg::SamplerState::LINEAR_CLAMP_MIPMAP);
    env.setNamespaces();
}
#endif

vg::SamplerState vg::SamplerState::POINT_WRAP(TextureMinFilter::NEAREST, TextureMagFilter::NEAREST,
    TextureWrapMode::REPEAT, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT);
vg::SamplerState vg::SamplerState::POINT_CLAMP(TextureMinFilter::NEAREST, TextureMagFilter::NEAREST,
    TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE);
vg::SamplerState vg::SamplerState::LINEAR_WRAP(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR,
    TextureWrapMode::REPEAT, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT);
vg::SamplerState vg::SamplerState::LINEAR_CLAMP(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR,
    TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE);
vg::SamplerState vg::SamplerState::POINT_WRAP_MIPMAP(TextureMinFilter::NEAREST_MIPMAP_NEAREST, TextureMagFilter::NEAREST,
    TextureWrapMode::REPEAT, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT);
vg::SamplerState vg::SamplerState::POINT_CLAMP_MIPMAP(TextureMinFilter::NEAREST_MIPMAP_NEAREST, TextureMagFilter::NEAREST,
    TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE);
vg::SamplerState vg::SamplerState::LINEAR_WRAP_MIPMAP(TextureMinFilter::LINEAR_MIPMAP_LINEAR, TextureMagFilter::LINEAR,
    TextureWrapMode::REPEAT, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT);
vg::SamplerState vg::SamplerState::LINEAR_CLAMP_MIPMAP(TextureMinFilter::LINEAR_MIPMAP_LINEAR, TextureMagFilter::LINEAR,
                                                       TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE);
