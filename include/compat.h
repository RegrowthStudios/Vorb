//
// compat.h
// Vorb Engine
//
// Created by Cristian Zaloj on 16 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file compat.h
 * @brief Macros and headers designed to aid in cross-platform and cross-compiler compatibility.
 */

#pragma once

#ifndef Vorb_compat_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_compat_h__
//! @endcond

// Correctly define all debug variables
#if defined(DEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif
// Define the non-debug macro
#if !defined(DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif

// Define some bracket macros
#define MACRO_PARAN_L {
#define MACRO_PARAN_R }

// Windows
#if !defined(WIN32) && defined(_WIN32)
#define WIN32 _WIN32
#define ARCH_32
#endif

#if !defined(WIN64) && defined(_WIN64)
#define WIN64 _WIN64
#define ARCH_64
#endif

#if defined(WIN32) || defined(WIN64)
#define OS_WINDOWS
#define _WINSOCKAPI_
#include "Windows.h"
#endif

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// Detect architecture
// http://sourceforge.net/p/predef/wiki/Architectures/

// TODO(Ben): The rest of them.
#if defined(__clang__)
/* Clang/LLVM. ---------------------------------------------- */

#elif defined(__ICC) || defined(__INTEL_COMPILER)
/* Intel ICC/ICPC. ------------------------------------------ */

#elif defined(__GNUC__) || defined(__GNUG__)
/* GNU GCC/G++. --------------------------------------------- */

#elif defined(__HP_cc) || defined(__HP_aCC)
/* Hewlett-Packard C/aC++. ---------------------------------- */

#elif defined(__IBMC__) || defined(__IBMCPP__)
/* IBM XL C/C++. -------------------------------------------- */

#elif defined(_MSC_VER)
/* Microsoft Visual Studio. --------------------------------- */
#ifndef _M_ARM
#ifdef _M_IX86
#ifdef ARCH_32
#define ARCH_X86_32
#else
#define ARCH_X86_64
#endif
#endif
#endif
#elif defined(__PGI)
/* Portland Group PGCC/PGCPP. ------------------------------- */

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
/* Oracle Solaris Studio. ----------------------------------- */
#endif

// register keyword creates a warning and is deprecated
#if !defined(REGISTER)
#if defined(OS_WINDOWS)
#define REGISTER register
#else
#define REGISTER
#endif
#endif

#endif // !Vorb_compat_h__
