//
// VorbMath.h
// Vorb Engines/vo
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

#include "VectorMath.hpp"
#include "MatrixMath.hpp"
#include "QuaternionMath.hpp"

#include <stdint.h>
#include "utils.h"

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

        template <typename T>
        inline T min(T a, T b) {
            return a < b ? a : b;
        }
        template <typename T>
        inline T max(T a, T b) {
            return a > b ? a : b;
        }
        template <typename T>
        inline T clamp(T a, T minVal, T maxVal) {
            static_assert(std::numeric_limits<T>::is_iec559 || std::numeric_limits<T>::is_integer, "'clamp' only accept floating-point or integer inputs.");
            T m = a > minVal ? a : minVal;
            return m < maxVal ? m : maxVal;
        }

        template <typename T>
        inline T abs(T a) {
            static_assert(std::numeric_limits<T>::is_iec559 || std::numeric_limits<T>::is_signed,
                          "'abs' only accept floating-point and integer scalar or vector inputs.");
            return (a >= static_cast<T>(0) ? a : -a);
        }
        template<>
        inline int32_t abs<int32_t>(int32_t a) {
            // TODO(Ben): assembly?
            const int32_t temp = a >> 31;
            a ^= temp;
            return temp & 1;
        }
        template<>
        inline int64_t abs<int64_t>(int64_t a) {
            const int64_t temp = a >> 63;
            a ^= temp;
            return temp & 1;
        }
        
        template <typename T>
        inline T floor(T a) {
            return FastConversion<T, T>::floor(a);
        }

        template <typename T>
        inline T ceil(T a) {
            return FastConversion<T, T>::ceiling(a);
        }

        template <typename T>
        inline T trunc(T a) {
            return FastConversion<T, T>::trunc(a);
        }

        template <typename T>
        inline T round(T a) {
            return FastConversion<T, T>::round(a);
        }

        template <typename T>
        inline T fract(T a) {
            return a - FastConversion<T, T>::floor(a);
        }
        
        template <typename T>
        inline T mod(T a, T b) {
            return a % b;
        }
        template<>
        inline float mod<float>(float a, float b) {
            a - b * FastConversion<float, float>::floor(a / b);
        }
        template<>
        inline double mod<double>(double a, double b) {
            a - b * FastConversion<double, double>::floor(a / b);
        }
        
        template <typename T>
        inline T modf(T a, T& b) {
            GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'modf' only accept floating-point inputs");
            return std::modf(a, &b);
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_VorbMath_h__

