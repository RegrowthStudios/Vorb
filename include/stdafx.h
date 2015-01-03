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

#include "compat.h"

// Make Sure We Use Correct Debug Variables
#if defined(DEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#define MACRO_PARAN_L {
#define MACRO_PARAN_R }

#include <fstream>
#include <iostream>
#include <map>
#include <cmath>
#include <queue>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "decorators.h"
#include "types.h"

#endif // stdafx_h__Vorb
