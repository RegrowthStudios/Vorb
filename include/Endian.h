//
// Endian.h
// Vorb Engine
//
// Created by Cristian Zaloj on 26 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file Endian.h
 * 
 * \brief 
 *
 *
 */

#pragma once

#ifndef Vorb_Endian_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Endian_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include <mmintrin.h>
#include <intrin.h>

namespace vorb {
    // Check sizes of types to make sure no function conflicts arise
    static_assert(sizeof(unsigned short) != sizeof(unsigned long), "Size match conflict for endian swap (short - long)");
    static_assert(sizeof(unsigned short) != sizeof(unsigned long long), "Size match conflict for endian swap (short - long long)");
    static_assert(sizeof(unsigned long) != sizeof(unsigned long long), "Size match conflict for endian swap (long - long long)");

    template<typename T, typename = void> struct EndianSwapImpl;

    template<typename T>
    struct EndianSwapImpl<T, typename std::enable_if<sizeof(T) == sizeof(unsigned short)>::type> {
        static T swap(T v) {
            unsigned short* ptr = (unsigned short*)&v;
            *ptr = _byteswap_ushort(*ptr);
            return v;
        }
    };
    template<typename T>
    struct EndianSwapImpl<T, typename std::enable_if<sizeof(T) == sizeof(unsigned long)>::type> {
        static T swap(T v) {
            unsigned long* ptr = (unsigned long*)&v;
            *ptr = _byteswap_ulong(*ptr);
            return v;
        }
    };
    template<typename T>
    struct EndianSwapImpl<T, typename std::enable_if<sizeof(T) == sizeof(unsigned long long)>::type> {
        static T swap(T v) {
            unsigned long long* ptr = (unsigned long long*)&v;
            *ptr = _byteswap_uint64(*ptr);
            return v;
        }
    };

    template<typename T>
    T endianSwap(T v) {
        return EndianSwapImpl<T>::swap(v);
    }
}


#endif // !Vorb_Endian_h__
