//
// VorbMath.h
// Vorb Engine
//
// Created by Benjamin Arnold on 20 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file VorbMath.h
* @brief Defines all vorb math.
*/

#pragma once

#ifndef Vorb_VorbMath_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VorbMath_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace math {

        /*! @brief Computes the dot product of two values.
         */
        template <typename T>
        inline T dot(T x, T y) {
            static_assert(std::numeric_limits<T>::is_iec559, "'dot' only accept floating-point inputs.");
            return x * y;
        }

        /*! @brief Computes the square root of a number.
        */
        template <typename T>
        inline T sqrt(T a) {
            // TODO(Ben): Optional fast implementation with #define.
            return std::sqrt(a);
        }
        /*! @brief John Carmack's fast inverse sqrt from Quake3.
        */
        inline f32 fastInverseSqrt(f32 number) {
            i32 i;
            f32 x2, y;
            x2 = number * 0.5f;
            y = number;
            i = *(i32*)&y;
            i = 0x5f3759df - (i >> 1);
            y = *(f32*)&i;
            y = y * (1.5f - (x2 * y * y));
            return y;
        }
        inline f64 fastInverseSqrt(f64 number) {
            i64 i;
            f64 x2, y;
            x2 = number * 0.5;
            y = number;
            i = *(i64*)&y;
            i = 0x5fe6eb50c7b537a9 - (i >> 1);
            y = *(f64*)&i;
            y = y * (1.5 - (x2 * y * y));
            return y;
        }
    }
}

#include "VectorMath.inl"

#endif // !Vorb_VorbMath_h__

