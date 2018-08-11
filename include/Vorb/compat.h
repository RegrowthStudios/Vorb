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

/* 
 * The purpose of the following statements is to detect what kind of computer architecture we are targeting as well as
 * what compiler we are dealing with. The following is the list of enums that we search for:
 * 
 * == VORB_ARCH == Enumerates memory architecture of the compilation target
 * 32
 * 64
 * 
 * == VORB_ARCH == Enumerates specific processors
 * X86
 * X86_64
 * ARM
 * 
 * == VORB_COMPILER ==
 * CLANG
 * MSVC
 * GCC
 *
 * == VORB_OS ==
 * WINDOWS
 * MAC
 * LINUX
 */

// Detect 32- or 64-bit architectures
#if defined(_WIN32)
#define VORB_ARCH_32
#elif defined(_WIN64)
#define VORB_ARCH_64
#else
//let the compilier options figure it out
//#error Unknown compilation target to determine size
#endif

// Detect OS
#if defined(_WIN32) || defined(_WIN64)
#define VORB_OS_WINDOWS
#endif

// Detect architecture
// http://sourceforge.net/p/predef/wiki/Architectures/

// TODO(Ben): The rest of them.
#if defined(__clang__)
/* Clang/LLVM. ---------------------------------------------- */
#define VORB_COMPILER_CLANG

#ifdef __i386__
#ifdef __x86_64__
#define VORB_ARCH_64
#define VORB_ARCH_X86_64
#else
#define VORB_ARCH_32
#define VORB_ARCH_X86_32
#endif
#elif defined(__arm__)
#error ARM 32-bit and 64-bit architectures must be defined
#endif

#elif defined(__ICC) || defined(__INTEL_COMPILER)
/* Intel ICC/ICPC. ------------------------------------------ */
#error Intel ICC/ICPC compiler is not supported by Vorb

#elif defined(__GNUC__) || defined(__GNUG__)
/* GNU GCC/G++. --------------------------------------------- */
#define VORB_COMPILER_GCC

#ifdef __i386__
#ifdef __x86_64__
#define VORB_ARCH_64
#define VORB_ARCH_X86_64
#else
#define VORB_ARCH_32
#define VORB_ARCH_X86_32
#endif
#elif defined(__arm__)
#error ARM 32-bit and 64-bit architectures must be defined
#endif

#elif defined(__HP_cc) || defined(__HP_aCC)
/* Hewlett-Packard C/aC++. ---------------------------------- */
#error HP C/aC++ compiler is not supported by Vorb

#elif defined(__IBMC__) || defined(__IBMCPP__)
/* IBM XL C/C++. -------------------------------------------- */
#error IBM XL C/C++ compiler is not supported by Vorb

#elif defined(_MSC_VER)
/* Microsoft Visual Studio. --------------------------------- */
#define VORB_COMPILER_MSVC
#ifdef _M_IX86
#ifdef VORB_ARCH_32
#define VORB_ARCH_X86_32
#else
#define VORB_ARCH_X86_64
#endif
#elif defined(_M_ARM)
#error ARM 32-bit and 64-bit architectures must be defined
#endif

#elif defined(__PGI)
/* Portland Group PGCC/PGCPP. ------------------------------- */
#error Portlan Group PGCC/PGCPP compiler is not supported by Vorb

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
/* Oracle Solaris Studio. ----------------------------------- */
#error Solaris compiler is not supported by Vorb

#endif

// Include UNIX threading and scheduling headers if not compiling on Windows. 
// Currently all OS's supported (and plenty not) that are not Windows are UNIX-based.
#ifndef _WINDOWS
#include <pthread.h>
#include <sched.h>
#endif//_WINDOWS

#endif // !Vorb_compat_h__
