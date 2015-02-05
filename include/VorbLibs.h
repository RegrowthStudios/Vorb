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

// Add FreeImage libraries
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

#endif // VorbLibs_h__
