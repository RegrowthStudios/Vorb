//
// Graphics.h
// Vorb Engine
//
// Created by Cristian Zaloj on 13 Mar 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Graphics.h
 * @brief Graphics headers from API detection.
 */

#pragma once

#ifndef Vorb_Graphics_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Graphics_h__
//! @endcond

#if defined(VORB_IMPL_GRAPHICS_OPENGL)
#include <GL/gl.h>

// GL Error checking
// TODO(Ben): This could go somewhere else.
namespace vorb {
    namespace graphics {
        inline bool checkGlError(const nString& descriptor, OUT nString& errorMsg) {
            GLenum error = glGetError();
            if (error != GL_NO_ERROR) {
                switch (error) {
                    case GL_INVALID_ENUM:
                        errorMsg = ("GL error at " + descriptor + ". Error code 1280: GL_INVALID_ENUM");
                        break;
                    case GL_INVALID_VALUE:
                        errorMsg = ("GL error at " + descriptor + ". Error code 1281: GL_INVALID_VALUE");
                        break;
                    case GL_INVALID_OPERATION:
                        errorMsg = ("GL error at " + descriptor + ". Error code 1282: GL_INVALID_OPERATION");
                        break;
                    case GL_STACK_OVERFLOW:
                        errorMsg = ("GL error at " + descriptor + ". Error code 1283: GL_STACK_OVERFLOW");
                        break;
                    case GL_STACK_UNDERFLOW:
                        errorMsg = ("GL error at " + descriptor + ". Error code 1284: GL_STACK_UNDERFLOW");
                        break;
                    case GL_OUT_OF_MEMORY:
                        errorMsg = ("GL error at " + descriptor + ". Error code 1285: GL_OUT_OF_MEMORY");
                        break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        errorMsg = ("GL error at " + descriptor + ". Error code 1285: GL_INVALID_FRAMEBUFFER_OPERATION");
                        break;
                    default:
                        errorMsg = ("GL error at " + descriptor + ". Error code " + std::to_string(error) + ": UNKNOWN");
                        break;
                }
                return true;
            }
            return false;
        }
    }
}
namespace vg = vorb::graphics;


#elif defined(VORB_IMPL_GRAPHICS_D3D)
// Make sure we get correct DirectX version
#if defined(VORB_DX_11)
#include <d3d11.h>
#define VORB_DX_COM void
#define VORB_DX_DEVICE ID3D10Device
#elif defined(VORB_DX_9)
#include <d3d9.h>
#define VORB_DX_COM IDirect3D9
#define VORB_DX_DEVICE IDirect3DDevice9
#endif
#define VG_DX_DEVICE(V) ((VORB_DX_DEVICE*)V)
#endif

#endif // !Vorb_Graphics_h__
