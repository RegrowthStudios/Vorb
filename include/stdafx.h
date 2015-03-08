//
// stdafx.h
// Vorb Engine
//
// Created by Cristian Zaloj on 16 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file stdafx.h
 * @brief Vorb's precompiled header, should you wish to use it.
 */

#pragma once

// Perform a sanity check when not using precompiled headers
//! @cond DOXY_SHOW_HEADER_GUARDS
#ifndef VORB_USING_PCH
#define Vorb_stdafx_h__
#endif // !VORB_USING_PCH
//! @endcond

#ifndef Vorb_stdafx_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_stdafx_h__
//! @endcond

// Create cross-compatibility information
#include "compat.h"

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

#endif // !Vorb_stdafx_h__
