#include "stdafx.h"
#include "graphics/GBuffer.h"

#include "graphics/SamplerState.h"

vg::GBuffer::GBuffer(ui32 w /*= 0*/, ui32 h /*= 0*/) :
m_size(w, h) {
    // Empty
}

void vg::GBuffer::initTarget(const ui32v2& _size, const ui32& texID, const vg::GBufferAttachment& attachment) {
    glBindTexture(GL_TEXTURE_2D, texID);
    if (glTexStorage2D) {
        glTexStorage2D(GL_TEXTURE_2D, 1, (VGEnum)attachment.format, _size.x, _size.y);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, (VGEnum)attachment.format, _size.x, _size.y, 0, (VGEnum)attachment.pixelFormat, (VGEnum)attachment.pixelType, nullptr);
    }
    SamplerState::POINT_CLAMP.set(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment.number, GL_TEXTURE_2D, texID, 0);
}
vg::GBuffer& vg::GBuffer::init(const Array<GBufferAttachment>& attachments, vg::TextureInternalFormat lightFormat) {
    // Create texture targets
    m_textures.setData(attachments.size() + 1);
    glGenTextures((GLsizei)m_textures.size(), &m_textures[0]);

    // Make the framebuffer
    glGenFramebuffers(1, &m_fboGeom);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboGeom);

    // Add the attachments
    VGEnum* bufs = (VGEnum*)alloca(attachments.size() * sizeof(VGEnum));
    for (ui32 i = 0; i < attachments.size(); i++) {
        bufs[i] = GL_COLOR_ATTACHMENT0 + attachments[i].number;
        initTarget(m_size, m_textures[i], attachments[i]);
    }

    // Set the output location for pixels
    glDrawBuffers((GLsizei)attachments.size(), bufs);

    // Make the framebuffer for lighting
    glGenFramebuffers(1, &m_fboLight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboLight);
    initTarget(m_size, m_textures[m_textures.size() - 1], { lightFormat, vg::TextureFormat::RGBA, vg::TexturePixelType::UNSIGNED_BYTE, 0 });

    // Set the output location for pixels
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO(Cristian): Change The Memory Usage Of The GPU

    return *this;
}
vg::GBuffer& vg::GBuffer::initDepth(TextureInternalFormat depthFormat /*= TextureInternalFormat::DEPTH_COMPONENT32*/) {
    glGenTextures(1, &m_texDepth);
    glBindTexture(GL_TEXTURE_2D, m_texDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, (VGEnum)depthFormat, m_size.x, m_size.y, 0, (VGEnum)vg::TextureFormat::DEPTH_COMPONENT, (VGEnum)vg::TexturePixelType::FLOAT, nullptr);
    SamplerState::POINT_CLAMP.set(GL_TEXTURE_2D);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboGeom);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texDepth, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboLight);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texDepth, 0);

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO: Change The Memory Usage Of The GPU

    return *this;
}
vg::GBuffer& vg::GBuffer::initDepthStencil(TextureInternalFormat depthFormat /*= TextureInternalFormat::DEPTH24_STENCIL8*/) {
    glGenTextures(1, &m_texDepth);
    glBindTexture(GL_TEXTURE_2D, m_texDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, (VGEnum)depthFormat, m_size.x, m_size.y, 0, (VGEnum)vg::TextureFormat::DEPTH_STENCIL, (VGEnum)vg::TexturePixelType::UNSIGNED_INT_24_8, nullptr);
    SamplerState::POINT_CLAMP.set(GL_TEXTURE_2D);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboGeom);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_texDepth, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboLight);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_texDepth, 0);

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO(Cristian): Change The Memory Usage Of The GPU

    return *this;
}
void vg::GBuffer::dispose() {
    // TODO(Cristian): Change The Memory Usage Of The GPU

    if (m_fboGeom != 0) {
        glDeleteFramebuffers(1, &m_fboGeom);
        m_fboGeom = 0;
    }
    if (m_fboLight != 0) {
        glDeleteFramebuffers(1, &m_fboLight);
        m_fboLight = 0;
    }
    if (m_textures.size() != 0) {
        glDeleteTextures((GLsizei)m_textures.size(), &m_textures[0]);
        m_textures.setData(0);
    }
    if (m_texDepth != 0) {
        glDeleteTextures(1, &m_texDepth);
        m_texDepth = 0;
    }
}

void vg::GBuffer::useGeometry() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboGeom);
    glViewport(0, 0, m_size.x, m_size.y);
}
void vg::GBuffer::useLight() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboLight);
    glViewport(0, 0, m_size.x, m_size.y);
}
