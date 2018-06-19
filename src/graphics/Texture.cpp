#include "Vorb/stdafx.h"
#include "Vorb/graphics/Texture.h"

void vg::Texture::bind() {
    glBindTexture(static_cast<GLenum>(textureTarget), id);
}

void vg::Texture::unbind() {
    glBindTexture(static_cast<GLenum>(textureTarget), 0);
}