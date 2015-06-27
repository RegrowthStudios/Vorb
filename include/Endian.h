//
// Endian.h
// Vorb Engine
//
// Created by Cristian Zaloj on 26 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Endian.h
* @brief 
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
    template<typename T>
    T endianSwap(T v);

    namespace impl {
        template<typename T, typename Proxy>
        T endianSwapProxy(T v) {
            static_assert(sizeof(T) == sizeof(Proxy), "Type sizes of original and proxy do not match");
            Proxy* ptr = (Proxy*)&v;
            *ptr = endianSwap<Proxy>(*ptr);
            return v;
        }
    }

    template<typename T, typename = void>
    T endianSwapS(T v) {
        return T({});
    }

    template<typename T>
    T endianSwapS<T, typename std::enable_if<sizeof(T) == sizeof(unsigned short)>::type>(T v) {
        unsigned short* ptr = (unsigned short*)&v;
        *ptr = _byteswap_ushort(*ptr);
        return v;
    }
    template<typename T>
    T endianSwapS<T, typename std::enable_if<sizeof(T) == sizeof(unsigned long)>::type>(T v) {
        unsigned long* ptr = (unsigned long*)&v;
        *ptr = _byteswap_ulong(*ptr);
        return v;
    }
    template<typename T>
    T endianSwapS<T, typename std::enable_if<sizeof(T) == sizeof(unsigned long long)>::type>(T v) {
        unsigned long long* ptr = (unsigned long long*)&v;
        *ptr = _byteswap_uint64(*ptr);
        return v;
    }

    template<>
    ui16 endianSwap(ui16 v) {
        static_assert(sizeof(unsigned short) == sizeof(ui16), "UI16 swap is of wrong size");
        return _byteswap_ushort(v);
    }
    template<>
    ui32 endianSwap(ui32 v) {
        static_assert(sizeof(unsigned long) == sizeof(ui32), "UI32 swap is of wrong size");
        return _byteswap_ulong(v);
    }
    template<>
    ui64 endianSwap(ui64 v) {
        static_assert(sizeof(unsigned long long) == sizeof(ui64), "UI64 swap is of wrong size");
        return _byteswap_uint64(v);
    }
    template<>
    unsigned long endianSwap(unsigned long v) {
        return _byteswap_ulong(v);
    }
}

template<typename T>
T endianSwap(T v) {
    static_assert(false, "Cannot compile this, no matching intrinsic");
}


#endif // !Vorb_Endian_h__
