// 
//  Texture.h
//  Vorb Engine
//
//  Created by Ben Arnold on 20 Oct 2014
//  Copyright 2014 Regrowth Studios
//  All Rights Reserved
//  
//  This file provides a wrapper for a texture
//
#pragma once

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "gtypes.h"
#include "GLEnums.h"

namespace vorb {
    namespace graphics {
        /// Wrapper struct for a texture
        class Texture {
        public:
            Texture(VGTexture id = 0u,
                    ui32 w = 0u,
                    ui32 h = 0u,
                    vg::TextureTarget target = vg::TextureTarget::TEXTURE_2D) :
                id(id),
                width(w),
                height(h) {
                // Empty
            }
            void bind();
            void unbind();

            vg::TextureTarget textureTarget = vg::TextureTarget::TEXTURE_2D;
            VGTexture id = 0; ///< OpenGL texture ID
            ui32 width = 0; ///< Texture width in pixels
            ui32 height = 0; ///< Texture height in pixels
        };
    }
}
namespace vg = vorb::graphics;

#endif // TEXTURE_H_
