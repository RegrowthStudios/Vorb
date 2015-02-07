///
/// VorbLibs.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Feb 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Control library linkage
///

#pragma once

#ifndef VorbLibs_h__
#define VorbLibs_h__

/************************************************************************/
/* Common                                                               */
/************************************************************************/
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#ifdef DEBUG
#pragma comment(lib, "libboost_system-vc120-mt-gd-1_55.lib")
#pragma comment(lib, "libboost_filesystem-vc120-mt-gd-1_55.lib")
#pragma comment(lib, "enet-d.lib")
#pragma comment(lib, "LinearMath-d.lib")
#pragma comment(lib, "BulletCollision-d.lib")
#pragma comment(lib, "BulletDynamics-d.lib")
#pragma comment(lib, "OOLua-d.lib")
#pragma comment(lib, "libyaml-cppmdd.lib")
#pragma comment(lib, "ZLIB-d.lib")
#else
#pragma comment(lib, "libboost_system-vc120-mt-1_55.lib")
#pragma comment(lib, "libboost_filesystem-vc120-mt-1_55.lib")
#pragma comment(lib, "enet.lib")
#pragma comment(lib, "LinearMath.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "OOLua.lib")
#pragma comment(lib, "libyaml-cppmd.lib")
#pragma comment(lib, "ZLIB.lib")
#endif // DEBUG
#if defined(OS_WINDOWS)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#else
#error TODO(Brian): What is used here?
#endif

/************************************************************************/
/* FreeImage                                                            */
/************************************************************************/
#ifdef DEBUG
#pragma comment(lib, "FreeImage-d.lib")
#pragma comment(lib, "libJPEG-d.lib")
#pragma comment(lib, "libJXR-d.lib")
#pragma comment(lib, "libPNG-d.lib")
#pragma comment(lib, "libRAW-d.lib")
#pragma comment(lib, "libTIFF-d.lib")
#pragma comment(lib, "libWebP-d.lib")
#pragma comment(lib, "OpenEXR-d.lib")
#pragma comment(lib, "OpenJPEG-d.lib")
#else
#pragma comment(lib, "FreeImage.lib")
#pragma comment(lib, "libJPEG.lib")
#pragma comment(lib, "libJXR.lib")
#pragma comment(lib, "libPNG.lib")
#pragma comment(lib, "libRAW.lib")
#pragma comment(lib, "libTIFF.lib")
#pragma comment(lib, "libWebP.lib")
#pragma comment(lib, "OpenEXR.lib")
#pragma comment(lib, "OpenJPEG.lib")
#endif // DEBUG

/************************************************************************/
/* UI                                                                   */
/************************************************************************/
#if defined(VORB_IMPL_UI_GLFW)
#ifdef DEBUG
#pragma comment(lib, "glfw3-d.lib")
#else
#pragma comment(lib, "glfw3.lib")
#endif // DEBUG
#elif defined(VORB_IMPL_UI_SDL)
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_ttf.lib")
#elif defined(VORB_IMPL_UI_SFML)
#ifdef DEBUG
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-graphics.lib")
#endif // DEBUG
#pragma comment(lib, "freetype.lib")
#else
#error A UI implementation must be specified
#endif

/************************************************************************/
/* Sound                                                                */
/************************************************************************/
#if defined(VORB_IMPL_SOUND_FMOD)
#ifdef DEBUG
#pragma comment(lib, "fmodexL_vc.lib")
#else
#pragma comment(lib, "fmodex_vc.lib")
#endif // DEBUG
#elif defined(VORB_IMPL_SOUND_SFML)
#ifdef DEBUG
#pragma comment(lib, "sfml-audio-d.lib")
#else
#pragma comment(lib, "sfml-audio.lib")
#endif // DEBUG
#pragma comment(lib, "openal32.lib")
#pragma comment(lib, "sndfile.lib")
#else
#error A sound implementation must be specified
#endif

#endif // VorbLibs_h__
