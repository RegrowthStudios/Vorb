///
/// stdafx.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 29 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// PCH for Vorb
///

#pragma once

#ifndef stdafx_h__Vorb
#define stdafx_h__Vorb

// Create cross-compatibility information
#include "compat.h"

// Correctly define all debug variables
#if defined(DEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#define MACRO_PARAN_L {
#define MACRO_PARAN_R }

/************************************************************************/
/* C Libraries                                                          */
/************************************************************************/
#include <cmath>
#include <cstdio>
#include <cstdlib>
/************************************************************************/
/* Stream libraries                                                     */
/************************************************************************/
#include <fstream>
#include <iostream>
#include <sstream>
/************************************************************************/
/* STL Containers                                                       */
/************************************************************************/
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
/************************************************************************/
/* Other                                                                */
/************************************************************************/
#include <mutex>
#include <string>
#include <thread>

// TODO: Distribute OpenGL from this location
#include <GL/glew.h>

/************************************************************************/
/* Vorb common files                                                    */
/************************************************************************/
#include "decorators.h"
#include "types.h"

#endif // stdafx_h__Vorb
