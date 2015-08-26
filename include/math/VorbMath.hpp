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

#include "VectorMath.hpp"
#include "MatrixMath.hpp"

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

        template <typename T>
        inline T sin(T a) {
            return std::sin(a);
        }
        template <typename T>
        inline T cos(T a) {
            return std::cos(a);
        }
        template <typename T>
        inline T tan(T a) {
            return std::tan(a);
        }
        template <typename T>
        inline T acos(T a) {
            return std::acos(a);
        }
        template <typename T>
        inline T asin(T a) {
            return std::asin(a);
        }
        template <typename T>
        inline T atan(T a) {
            return std::atan(a);
        }
        template <typename T>
        inline T atan2(T y, T x) {
            return std::atan2(y, x);
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_VorbMath_h__

