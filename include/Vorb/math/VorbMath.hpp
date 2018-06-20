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

#include "Vorb/types.h"
#include "Vorb/math/VectorMath.hpp"
#include "Vorb/math/MatrixMath.hpp"
#include "Vorb/math/QuaternionMath.hpp"

#include <stdint.h>
#include "Vorb/utils.h"

#ifndef M_PI
#define M_PI   3.14159265358979323846264338327950288   /* pi */
#endif
#ifndef M_PIF
#define M_PIF   3.14159265358979323846264338327950288f   /* pi */
#endif
#ifndef M_2_PI
#define M_2_PI 6.283185307179586476925286766559 /* 2 * pi */
#endif
#ifndef M_2_PIF
#define M_2_PIF 6.283185307179586476925286766559F /* 2 * pi */
#endif
#ifndef M_4_PI
#define M_4_PI 12.566370614359172953850573533118 /* 4 * pi */
#endif
#ifndef M_4_PIF
#define M_4_PIF 12.566370614359172953850573533118f /* 4 * pi */
#endif
#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398   /* pi / 2 */
#endif
#ifndef M_PI_2F
#define M_PI_2F 1.5707963267948966192313216916398f   /* pi / 2 */
#endif
#ifndef M_PI_4
#define M_PI_4 0.78539816339744830961566084581988   /* pi / 4 */
#endif
#ifndef M_PI_4F
#define M_PI_4F 0.78539816339744830961566084581988f   /* pi / 4 */
#endif
#ifndef M_G
#define M_G 0.0000000000667384
#endif

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
        
#define FAST_EPSILON static_cast<T>(0.99999999999999)

        template <typename T>
        inline T floor(T a) {
            return FastConversion<T, T>::floor(a);
        }
        template <typename T>
        inline T floorApprox(T a) {
            return static_cast<T>(a < 0 ? (int)(a - FAST_EPSILON) : (int)a);
        }

        template <typename T>
        inline T ceil(T a) {
            return FastConversion<T, T>::ceiling(a);
        }
        template <typename T>
        inline T ceilApprox(T a) {
            return static_cast<T>(a <= 0 ? (int)a : (int)(a + FAST_EPSILON));
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
        inline T roundApprox(T a) {
            return static_cast<T>(a <= 0 ? (int)(a - static_cast<T>(0.5)) : (int)(a + static_cast<T>(0.5)));
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
            return a - b * FastConversion<float, float>::floor(a / b);
        }
        template<>
        inline double mod<double>(double a, double b) {
            return a - b * FastConversion<double, double>::floor(a / b);
        }
        
        template <typename T>
        inline T modf(T a, T& b) {
            static_assert(std::numeric_limits<T>::is_iec559, "'modf' only accept floating-point inputs");
            return std::modf(a, &b);
        }

        template <typename T>
        inline T sign(T a) {
            // TODO(Ben): Optimize
            return static_cast<T>(a < 0 ? -1 : 1);
        }

        template <typename T>
        inline T radians(T a) {
            return a * static_cast<T>(M_PI / 180.0);
        }

        template <typename T>
        inline T degrees(T a) {
            return a * static_cast<T>(180.0 / M_PI);
        }

        template <typename T>
        inline T exp(T a) {
            return std::exp(a);
        }

        template <typename T>
        inline T exp2(T a) {
            return std::exp2(a);
        }

        template <typename T>
        inline T log(T a) {
            return std::log(a);
        }

        template <typename T>
        inline T log2(T a) {
            return std::log2(a);
        }

        template<typename T>
        inline T pow(T a, T p) {
            return std::pow(a, p);
        }
        template<typename T>
        inline T pow2(T a) {
            return a * a;
        }
        template<typename T>
        inline T pow3(T a) {
            return a * a * a;
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_VorbMath_h__

