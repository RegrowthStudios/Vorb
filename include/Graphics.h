//
// Graphics.h
//
// Created by Cristian Zaloj on 13 Mar 2015
//

#pragma once

#ifndef Graphics_h__
#define Graphics_h__

#if defined(VORB_IMPL_GRAPHICS_OPENGL)
#include <GL/gl.h>
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

#endif // !Graphics_h__
