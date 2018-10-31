//
// gtypes.h
// Vorb Engine
//
// Created by Cristian Zaloj on 26 Nov 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file gtypes.h
 * @brief Typenames for general graphics types.
 */

#pragma once

#ifndef Vorb_gtypes_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_gtypes_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <GL/glew.h>

#include "../types.h"
#endif // !VORB_USING_PCH

/************************************************************************/
/* Objects                                                              */
/************************************************************************/
typedef GLuint VGObject;
// Mesh
typedef VGObject VGBuffer;
typedef VGBuffer VGVertexBuffer;
typedef VGBuffer VGInstanceBuffer;
typedef VGBuffer VGIndexBuffer;
typedef VGObject VGVertexArray;
// Texture
typedef VGObject VGTexture;
typedef VGObject VGFramebuffer;
typedef VGObject VGRenderbuffer;
// Shading
typedef VGObject VGProgram;
typedef VGObject VGShader;
// Other
typedef VGObject VGSampler;
typedef VGObject VGQuery;

/************************************************************************/
/* Special identifiers                                                  */
/************************************************************************/
typedef GLint VGAttribute;
typedef GLint VGUniform;
typedef GLenum VGEnum;
typedef ui32 VGSemantic;

#endif // !Vorb_gtypes_h__
