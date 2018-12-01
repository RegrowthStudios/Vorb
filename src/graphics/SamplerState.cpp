#include "Vorb/stdafx.h"
#include "Vorb/graphics/SamplerState.h"

vg::SamplerState::SamplerState(TextureMinFilter texMinFilter, TextureMagFilter texMagFilter, TextureWrapMode texWrapS, 
                                    TextureWrapMode texWrapT, TextureWrapMode texWrapR) :
    m_minFilter(texMinFilter),
    m_magFilter(texMagFilter),
    m_wrapS(texWrapS),
    m_wrapT(texWrapT),
    m_wrapR(texWrapR) {
    // Empty
}
vg::SamplerState::SamplerState(ui32 texMinFilter, ui32 texMagFilter, ui32 texWrapS, ui32 texWrapT, ui32 texWrapR) :
	SamplerState(static_cast<TextureMinFilter>(texMinFilter), static_cast<TextureMagFilter>(texMagFilter), static_cast<TextureWrapMode>(texWrapS),
                    static_cast<TextureWrapMode>(texWrapT), static_cast<TextureWrapMode>(texWrapR)) {
    // Empty
}

void vg::SamplerState::initObject() {
    glGenSamplers(1, &m_id);
    glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(m_minFilter));
    glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(m_magFilter));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S,     static_cast<GLenum>(m_wrapS));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T,     static_cast<GLenum>(m_wrapT));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R,     static_cast<GLenum>(m_wrapR));
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
    glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(m_magFilter));
    glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(m_minFilter));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S,     static_cast<GLenum>(m_wrapS));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T,     static_cast<GLenum>(m_wrapT));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R,     static_cast<GLenum>(m_wrapR));
}

void vg::SamplerState::setObject(ui32 textureUnit) const {
    glBindSampler(textureUnit, m_id);
}

vg::SamplerState          vg::SamplerState::POINT_WRAP(TextureMinFilter::NEAREST,                TextureMagFilter::NEAREST, 
                                                       TextureWrapMode::REPEAT,                  TextureWrapMode::REPEAT,    TextureWrapMode::REPEAT);

vg::SamplerState         vg::SamplerState::POINT_CLAMP(TextureMinFilter::NEAREST,                TextureMagFilter::NEAREST,
                                                       TextureWrapMode::CLAMP_EDGE,              TextureWrapMode::CLAMP_EDGE, TextureWrapMode::CLAMP_EDGE);

vg::SamplerState         vg::SamplerState::LINEAR_WRAP(TextureMinFilter::LINEAR,                 TextureMagFilter::LINEAR,
                                                       TextureWrapMode::REPEAT,                  TextureWrapMode::REPEAT,     TextureWrapMode::REPEAT);

vg::SamplerState        vg::SamplerState::LINEAR_CLAMP(TextureMinFilter::LINEAR,                 TextureMagFilter::LINEAR,
                                                       TextureWrapMode::CLAMP_EDGE,              TextureWrapMode::CLAMP_EDGE,  TextureWrapMode::CLAMP_EDGE);

vg::SamplerState   vg::SamplerState::POINT_WRAP_MIPMAP(TextureMinFilter::NEAREST_MIPMAP_NEAREST, TextureMagFilter::NEAREST,
                                                       TextureWrapMode::REPEAT,                  TextureWrapMode::REPEAT,      TextureWrapMode::REPEAT);

vg::SamplerState  vg::SamplerState::POINT_CLAMP_MIPMAP(TextureMinFilter::NEAREST_MIPMAP_NEAREST, TextureMagFilter::NEAREST,
                                                       TextureWrapMode::CLAMP_EDGE,              TextureWrapMode::CLAMP_EDGE,  TextureWrapMode::CLAMP_EDGE);

vg::SamplerState  vg::SamplerState::LINEAR_WRAP_MIPMAP(TextureMinFilter::LINEAR_MIPMAP_LINEAR,   TextureMagFilter::LINEAR,
                                                       TextureWrapMode::REPEAT,                  TextureWrapMode::REPEAT,      TextureWrapMode::REPEAT);

vg::SamplerState vg::SamplerState::LINEAR_CLAMP_MIPMAP(TextureMinFilter::LINEAR_MIPMAP_LINEAR,   TextureMagFilter::LINEAR,
                                                       TextureWrapMode::CLAMP_EDGE,              TextureWrapMode::CLAMP_EDGE,  TextureWrapMode::CLAMP_EDGE);

