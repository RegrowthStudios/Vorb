#include "stdafx.h"
#include "GBuffer.h"

#include "SamplerState.h"

vg::GBuffer::GBuffer(ui32 w /*= 0*/, ui32 h /*= 0*/) :
m_size(w, h) {
    // Empty
}

void initTarget(const ui32v2& _size, const ui32& texID, const vg::TextureInternalFormat& format, const ui32& attachment) {
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, (VGEnum)format, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    SamplerState::POINT_CLAMP.set(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, texID, 0);
}
vg::GBuffer& vg::GBuffer::init() {
    // Make the framebuffer
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create texture targets
    glGenTextures(3, m_textures);
    initTarget(m_size, m_tex.color, GBUFFER_INTERNAL_FORMAT_COLOR, 0);
    initTarget(m_size, m_tex.normal, GBUFFER_INTERNAL_FORMAT_NORMAL, 1);
    initTarget(m_size, m_tex.depth, GBUFFER_INTERNAL_FORMAT_DEPTH, 2);

    // Set the output location for pixels
    VGEnum bufs[3] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    glDrawBuffers(3, bufs);

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO: Change The Memory Usage Of The GPU

    return *this;
}
vg::GBuffer& vg::GBuffer::initDepth(TextureInternalFormat depthFormat /*= TextureInternalFormat::DEPTH_COMPONENT32*/) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glGenTextures(1, &m_texDepth);
    glBindTexture(GL_TEXTURE_2D, m_texDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, (VGEnum)depthFormat, m_size.x, m_size.y, 0, (VGEnum)vg::TextureFormat::DEPTH_COMPONENT, (VGEnum)vg::TexturePixelType::FLOAT, nullptr);
    SamplerState::POINT_CLAMP.set(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texDepth, 0);

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO: Change The Memory Usage Of The GPU

    return *this;
}
void vg::GBuffer::dispose() {
    // TODO: Change The Memory Usage Of The GPU

    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
    if (m_tex.color != 0) {
        glDeleteTextures(3, m_textures);
        m_tex = { 0, 0, 0 };
    }
    if (m_texDepth != 0) {
        glDeleteTextures(1, &m_texDepth);
        m_texDepth = 0;
    }
}

void vg::GBuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_size.x, m_size.y);
}
