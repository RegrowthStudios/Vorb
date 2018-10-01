//
// os.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Sep 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file os.h
 * @brief
 *
 *
 */

#pragma once

#ifndef Vorb_os_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_os_h__
//! @endcond

#define NOMINMAX

#if defined(_WIN32) || defined(_WIN64)
    #define _WINSOCKAPI_
    #include <Windows.h>
#endif//VORB_OS_WINDOWS

#endif // !Vorb_os_h__
