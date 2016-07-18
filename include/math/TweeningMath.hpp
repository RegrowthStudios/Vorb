//
// TweeningMath.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 02 Nov 2015
// Copyright 2015 Regrowth Studios
// All Rights Reserved
//

/*! \file TweeningMath.hpp
* @brief Defines common tweening math operations. Based on the easing equations provided in jQuery: https://github.com/danro/jquery-easing/blob/master/jquery.easing.js
*       Examples of each tween found here: http://hosted.zeh.com.br/mctween/animationtypes.html
*/

#pragma once

#include "VorbMath.hpp"

#ifndef Vorb_TweeningMath_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TweeningMath_hpp__
//! @endcond

namespace vorb {
    namespace math {
        /*! @brief Computes a linear tweening.
        */
        template <typename T>
         inline T linear(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "linear only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return startVal + range * alpha;
        }
        template <typename T>
        inline T linear(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return linear(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 linear(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(linear(startVal.x, finalVal.x, stageCount, stage),
                linear(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 linear(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(linear(startVal.x, finalVal.x, stageCount, stage),
                linear(startVal.y, finalVal.y, stageCount, stage),
                linear(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 linear(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(linear(startVal.x, finalVal.x, stageCount, stage),
                linear(startVal.y, finalVal.y, stageCount, stage),
                linear(startVal.z, finalVal.z, stageCount, stage),
                linear(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 linear(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(linear(startVal.x, finalVal.x, stageCount, stage),
                linear(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 linear(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(linear(startVal.x, finalVal.x, stageCount, stage),
                linear(startVal.y, finalVal.y, stageCount, stage),
                linear(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 linear(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(linear(startVal.x, finalVal.x, stageCount, stage),
                linear(startVal.y, finalVal.y, stageCount, stage),
                linear(startVal.z, finalVal.z, stageCount, stage),
                linear(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 linear(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(linear(startVal.x, finalVal.x, alpha),
                linear(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 linear(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(linear(startVal.x, finalVal.x, alpha),
                linear(startVal.y, finalVal.y, alpha),
                linear(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 linear(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(linear(startVal.x, finalVal.x, alpha),
                linear(startVal.y, finalVal.y, alpha),
                linear(startVal.z, finalVal.z, alpha),
                linear(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 linear(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(linear(startVal.x, finalVal.x, alpha),
                linear(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 linear(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(linear(startVal.x, finalVal.x, alpha),
                linear(startVal.y, finalVal.y, alpha),
                linear(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 linear(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(linear(startVal.x, finalVal.x, alpha),
                linear(startVal.y, finalVal.y, alpha),
                linear(startVal.z, finalVal.z, alpha),
                linear(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInQuad tweening.
        */
        template <typename T>
        inline T easeInQuad(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuad only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range * alpha * alpha + startVal;
        }
        template <typename T>
        inline T easeInQuad(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInQuad(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInQuad(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInQuad(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInQuad(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInQuad(startVal.y, finalVal.y, stageCount, stage),
                easeInQuad(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInQuad(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInQuad(startVal.y, finalVal.y, stageCount, stage),
                easeInQuad(startVal.z, finalVal.z, stageCount, stage),
                easeInQuad(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInQuad(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInQuad(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInQuad(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInQuad(startVal.y, finalVal.y, stageCount, stage),
                easeInQuad(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInQuad(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInQuad(startVal.y, finalVal.y, stageCount, stage),
                easeInQuad(startVal.z, finalVal.z, stageCount, stage),
                easeInQuad(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInQuad(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInQuad(startVal.x, finalVal.x, alpha),
                easeInQuad(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInQuad(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInQuad(startVal.x, finalVal.x, alpha),
                easeInQuad(startVal.y, finalVal.y, alpha),
                easeInQuad(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInQuad(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInQuad(startVal.x, finalVal.x, alpha),
                easeInQuad(startVal.y, finalVal.y, alpha),
                easeInQuad(startVal.z, finalVal.z, alpha),
                easeInQuad(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInQuad(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInQuad(startVal.x, finalVal.x, alpha),
                easeInQuad(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInQuad(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInQuad(startVal.x, finalVal.x, alpha),
                easeInQuad(startVal.y, finalVal.y, alpha),
                easeInQuad(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInQuad(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInQuad(startVal.x, finalVal.x, alpha),
                easeInQuad(startVal.y, finalVal.y, alpha),
                easeInQuad(startVal.z, finalVal.z, alpha),
                easeInQuad(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeOutQuad tweening.
        */
        template <typename T>
        inline T easeOutQuad(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuad only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return (T)-1.0f * range * alpha * (alpha - (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeOutQuad(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutQuad(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutQuad(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuad(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutQuad(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuad(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuad(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutQuad(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuad(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuad(startVal.z, finalVal.z, stageCount, stage),
                easeOutQuad(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutQuad(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuad(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutQuad(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuad(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuad(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutQuad(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuad(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuad(startVal.z, finalVal.z, stageCount, stage),
                easeOutQuad(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutQuad(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutQuad(startVal.x, finalVal.x, alpha),
                easeOutQuad(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutQuad(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutQuad(startVal.x, finalVal.x, alpha),
                easeOutQuad(startVal.y, finalVal.y, alpha),
                easeOutQuad(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutQuad(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutQuad(startVal.x, finalVal.x, alpha),
                easeOutQuad(startVal.y, finalVal.y, alpha),
                easeOutQuad(startVal.z, finalVal.z, alpha),
                easeOutQuad(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutQuad(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutQuad(startVal.x, finalVal.x, alpha),
                easeOutQuad(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutQuad(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutQuad(startVal.x, finalVal.x, alpha),
                easeOutQuad(startVal.y, finalVal.y, alpha),
                easeOutQuad(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeOutQuad(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutQuad(startVal.x, finalVal.x, alpha),
                easeOutQuad(startVal.y, finalVal.y, alpha),
                easeOutQuad(startVal.z, finalVal.z, alpha),
                easeOutQuad(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInOutQuad tweening.
        */
        template <typename T>
        inline T easeInOutQuad(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuad only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus1 = alpha2 - (T)1.0f;
            return (T)-1.0f * range / (T)2.0f * (alpha2Minus1 * (alpha2Minus1 - (T)2.0f) - (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutQuad(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutQuad(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutQuad(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuad(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutQuad(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuad(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuad(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutQuad(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuad(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuad(startVal.z, finalVal.z, stageCount, stage),
                easeInOutQuad(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutQuad(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuad(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutQuad(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuad(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuad(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutQuad(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutQuad(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuad(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuad(startVal.z, finalVal.z, stageCount, stage),
                easeInOutQuad(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutQuad(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutQuad(startVal.x, finalVal.x, alpha),
                easeInOutQuad(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutQuad(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutQuad(startVal.x, finalVal.x, alpha),
                easeInOutQuad(startVal.y, finalVal.y, alpha),
                easeInOutQuad(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutQuad(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutQuad(startVal.x, finalVal.x, alpha),
                easeInOutQuad(startVal.y, finalVal.y, alpha),
                easeInOutQuad(startVal.z, finalVal.z, alpha),
                easeInOutQuad(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutQuad(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutQuad(startVal.x, finalVal.x, alpha),
                easeInOutQuad(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutQuad(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutQuad(startVal.x, finalVal.x, alpha),
                easeInOutQuad(startVal.y, finalVal.y, alpha),
                easeInOutQuad(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutQuad(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutQuad(startVal.x, finalVal.x, alpha),
                easeInOutQuad(startVal.y, finalVal.y, alpha),
                easeInOutQuad(startVal.z, finalVal.z, alpha),
                easeInOutQuad(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInCubic tweening.
        */
        template <typename T>
        inline T easeInCubic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInCubic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range * alpha * alpha * alpha + startVal;
        }
        template <typename T>
        inline T easeInCubic(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInCubic(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInCubic(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInCubic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInCubic(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInCubic(startVal.y, finalVal.y, stageCount, stage),
                easeInCubic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInCubic(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInCubic(startVal.y, finalVal.y, stageCount, stage),
                easeInCubic(startVal.z, finalVal.z, stageCount, stage),
                easeInCubic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInCubic(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInCubic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInCubic(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInCubic(startVal.y, finalVal.y, stageCount, stage),
                easeInCubic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInCubic(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInCubic(startVal.y, finalVal.y, stageCount, stage),
                easeInCubic(startVal.z, finalVal.z, stageCount, stage),
                easeInCubic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInCubic(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInCubic(startVal.x, finalVal.x, alpha),
                easeInCubic(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInCubic(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInCubic(startVal.x, finalVal.x, alpha),
                easeInCubic(startVal.y, finalVal.y, alpha),
                easeInCubic(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInCubic(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInCubic(startVal.x, finalVal.x, alpha),
                easeInCubic(startVal.y, finalVal.y, alpha),
                easeInCubic(startVal.z, finalVal.z, alpha),
                easeInCubic(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInCubic(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInCubic(startVal.x, finalVal.x, alpha),
                easeInCubic(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInCubic(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInCubic(startVal.x, finalVal.x, alpha),
                easeInCubic(startVal.y, finalVal.y, alpha),
                easeInCubic(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInCubic(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInCubic(startVal.x, finalVal.x, alpha),
                easeInCubic(startVal.y, finalVal.y, alpha),
                easeInCubic(startVal.z, finalVal.z, alpha),
                easeInCubic(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeOutCubic tweening.
        */
        template <typename T>
        inline T easeOutCubic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutCubic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alphaMinus1 = alpha -(T)1.0f;
            return range * (alphaMinus1 * alphaMinus1 * alphaMinus1 + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutCubic(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutCubic(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutCubic(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeOutCubic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutCubic(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeOutCubic(startVal.y, finalVal.y, stageCount, stage),
                easeOutCubic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutCubic(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeOutCubic(startVal.y, finalVal.y, stageCount, stage),
                easeOutCubic(startVal.z, finalVal.z, stageCount, stage),
                easeOutCubic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutCubic(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeOutCubic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutCubic(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeOutCubic(startVal.y, finalVal.y, stageCount, stage),
                easeOutCubic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutCubic(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeOutCubic(startVal.y, finalVal.y, stageCount, stage),
                easeOutCubic(startVal.z, finalVal.z, stageCount, stage),
                easeOutCubic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutCubic(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutCubic(startVal.x, finalVal.x, alpha),
                easeOutCubic(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutCubic(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutCubic(startVal.x, finalVal.x, alpha),
                easeOutCubic(startVal.y, finalVal.y, alpha),
                easeOutCubic(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutCubic(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutCubic(startVal.x, finalVal.x, alpha),
                easeOutCubic(startVal.y, finalVal.y, alpha),
                easeOutCubic(startVal.z, finalVal.z, alpha),
                easeOutCubic(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutCubic(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutCubic(startVal.x, finalVal.x, alpha),
                easeOutCubic(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutCubic(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutCubic(startVal.x, finalVal.x, alpha),
                easeOutCubic(startVal.y, finalVal.y, alpha),
                easeOutCubic(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeOutCubic(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutCubic(startVal.x, finalVal.x, alpha),
                easeOutCubic(startVal.y, finalVal.y, alpha),
                easeOutCubic(startVal.z, finalVal.z, alpha),
                easeOutCubic(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInOutCubic tweening.
        */
        template <typename T>
        inline T easeInOutCubic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutCubic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 + (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutCubic(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutCubic(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutCubic(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCubic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutCubic(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCubic(startVal.y, finalVal.y, stageCount, stage),
                easeInOutCubic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutCubic(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCubic(startVal.y, finalVal.y, stageCount, stage),
                easeInOutCubic(startVal.z, finalVal.z, stageCount, stage),
                easeInOutCubic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutCubic(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCubic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutCubic(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCubic(startVal.y, finalVal.y, stageCount, stage),
                easeInOutCubic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutCubic(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutCubic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCubic(startVal.y, finalVal.y, stageCount, stage),
                easeInOutCubic(startVal.z, finalVal.z, stageCount, stage),
                easeInOutCubic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutCubic(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutCubic(startVal.x, finalVal.x, alpha),
                easeInOutCubic(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutCubic(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutCubic(startVal.x, finalVal.x, alpha),
                easeInOutCubic(startVal.y, finalVal.y, alpha),
                easeInOutCubic(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutCubic(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutCubic(startVal.x, finalVal.x, alpha),
                easeInOutCubic(startVal.y, finalVal.y, alpha),
                easeInOutCubic(startVal.z, finalVal.z, alpha),
                easeInOutCubic(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutCubic(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutCubic(startVal.x, finalVal.x, alpha),
                easeInOutCubic(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutCubic(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutCubic(startVal.x, finalVal.x, alpha),
                easeInOutCubic(startVal.y, finalVal.y, alpha),
                easeInOutCubic(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutCubic(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutCubic(startVal.x, finalVal.x, alpha),
                easeInOutCubic(startVal.y, finalVal.y, alpha),
                easeInOutCubic(startVal.z, finalVal.z, alpha),
                easeInOutCubic(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInQuart tweening.
        */
        template <typename T>
        inline T easeInQuart(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuart only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range * alpha * alpha * alpha * alpha + startVal;
        }
        template <typename T>
        inline T easeInQuart(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInQuart(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInQuart(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInQuart(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInQuart(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInQuart(startVal.y, finalVal.y, stageCount, stage),
                easeInQuart(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInQuart(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInQuart(startVal.y, finalVal.y, stageCount, stage),
                easeInQuart(startVal.z, finalVal.z, stageCount, stage),
                easeInQuart(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInQuart(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInQuart(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInQuart(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInQuart(startVal.y, finalVal.y, stageCount, stage),
                easeInQuart(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInQuart(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInQuart(startVal.y, finalVal.y, stageCount, stage),
                easeInQuart(startVal.z, finalVal.z, stageCount, stage),
                easeInQuart(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInQuart(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInQuart(startVal.x, finalVal.x, alpha),
                easeInQuart(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInQuart(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInQuart(startVal.x, finalVal.x, alpha),
                easeInQuart(startVal.y, finalVal.y, alpha),
                easeInQuart(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInQuart(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInQuart(startVal.x, finalVal.x, alpha),
                easeInQuart(startVal.y, finalVal.y, alpha),
                easeInQuart(startVal.z, finalVal.z, alpha),
                easeInQuart(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInQuart(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInQuart(startVal.x, finalVal.x, alpha),
                easeInQuart(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInQuart(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInQuart(startVal.x, finalVal.x, alpha),
                easeInQuart(startVal.y, finalVal.y, alpha),
                easeInQuart(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInQuart(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInQuart(startVal.x, finalVal.x, alpha),
                easeInQuart(startVal.y, finalVal.y, alpha),
                easeInQuart(startVal.z, finalVal.z, alpha),
                easeInQuart(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeOutQuart tweening.
        */
        template <typename T>
        inline T easeOutQuart(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuart only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alphaMinus1 = alpha - (T)1.0f;
            return (T)-1.0f * range * (alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 - (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutQuart(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutQuart(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutQuart(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuart(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutQuart(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuart(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuart(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutQuart(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuart(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuart(startVal.z, finalVal.z, stageCount, stage),
                easeOutQuart(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutQuart(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuart(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutQuart(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuart(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuart(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutQuart(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuart(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuart(startVal.z, finalVal.z, stageCount, stage),
                easeOutQuart(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutQuart(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutQuart(startVal.x, finalVal.x, alpha),
                easeOutQuart(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutQuart(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutQuart(startVal.x, finalVal.x, alpha),
                easeOutQuart(startVal.y, finalVal.y, alpha),
                easeOutQuart(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutQuart(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutQuart(startVal.x, finalVal.x, alpha),
                easeOutQuart(startVal.y, finalVal.y, alpha),
                easeOutQuart(startVal.z, finalVal.z, alpha),
                easeOutQuart(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutQuart(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutQuart(startVal.x, finalVal.x, alpha),
                easeOutQuart(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutQuart(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutQuart(startVal.x, finalVal.x, alpha),
                easeOutQuart(startVal.y, finalVal.y, alpha),
                easeOutQuart(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeOutQuart(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutQuart(startVal.x, finalVal.x, alpha),
                easeOutQuart(startVal.y, finalVal.y, alpha),
                easeOutQuart(startVal.z, finalVal.z, alpha),
                easeOutQuart(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInOutQuart tweening.
        */
        template <typename T>
        inline T easeInOutQuart(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuart only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return (T)-1.0f * range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 - (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutQuart(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutQuart(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutQuart(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuart(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutQuart(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuart(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuart(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutQuart(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuart(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuart(startVal.z, finalVal.z, stageCount, stage),
                easeInOutQuart(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutQuart(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuart(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutQuart(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuart(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuart(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutQuart(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutQuart(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuart(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuart(startVal.z, finalVal.z, stageCount, stage),
                easeInOutQuart(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutQuart(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutQuart(startVal.x, finalVal.x, alpha),
                easeInOutQuart(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutQuart(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutQuart(startVal.x, finalVal.x, alpha),
                easeInOutQuart(startVal.y, finalVal.y, alpha),
                easeInOutQuart(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutQuart(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutQuart(startVal.x, finalVal.x, alpha),
                easeInOutQuart(startVal.y, finalVal.y, alpha),
                easeInOutQuart(startVal.z, finalVal.z, alpha),
                easeInOutQuart(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutQuart(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutQuart(startVal.x, finalVal.x, alpha),
                easeInOutQuart(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutQuart(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutQuart(startVal.x, finalVal.x, alpha),
                easeInOutQuart(startVal.y, finalVal.y, alpha),
                easeInOutQuart(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutQuart(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutQuart(startVal.x, finalVal.x, alpha),
                easeInOutQuart(startVal.y, finalVal.y, alpha),
                easeInOutQuart(startVal.z, finalVal.z, alpha),
                easeInOutQuart(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInQuint tweening.
        */
        template <typename T>
        inline T easeInQuint(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuint only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range * alpha * alpha * alpha * alpha * alpha + startVal;
        }
        template <typename T>
        inline T easeInQuint(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInQuint(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInQuint(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInQuint(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInQuint(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInQuint(startVal.y, finalVal.y, stageCount, stage),
                easeInQuint(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInQuint(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInQuint(startVal.y, finalVal.y, stageCount, stage),
                easeInQuint(startVal.z, finalVal.z, stageCount, stage),
                easeInQuint(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInQuint(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInQuint(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInQuint(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInQuint(startVal.y, finalVal.y, stageCount, stage),
                easeInQuint(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInQuint(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInQuint(startVal.y, finalVal.y, stageCount, stage),
                easeInQuint(startVal.z, finalVal.z, stageCount, stage),
                easeInQuint(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInQuint(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInQuint(startVal.x, finalVal.x, alpha),
                easeInQuint(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInQuint(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInQuint(startVal.x, finalVal.x, alpha),
                easeInQuint(startVal.y, finalVal.y, alpha),
                easeInQuint(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInQuint(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInQuint(startVal.x, finalVal.x, alpha),
                easeInQuint(startVal.y, finalVal.y, alpha),
                easeInQuint(startVal.z, finalVal.z, alpha),
                easeInQuint(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInQuint(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInQuint(startVal.x, finalVal.x, alpha),
                easeInQuint(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInQuint(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInQuint(startVal.x, finalVal.x, alpha),
                easeInQuint(startVal.y, finalVal.y, alpha),
                easeInQuint(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInQuint(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInQuint(startVal.x, finalVal.x, alpha),
                easeInQuint(startVal.y, finalVal.y, alpha),
                easeInQuint(startVal.z, finalVal.z, alpha),
                easeInQuint(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeOutQuint tweening.
        */
        template <typename T>
        inline T easeOutQuint(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuint only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alphaMinus1 = alpha - (T)1.0f;
            return range * (alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutQuint(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutQuint(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutQuint(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuint(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutQuint(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuint(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuint(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutQuint(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuint(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuint(startVal.z, finalVal.z, stageCount, stage),
                easeOutQuint(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutQuint(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuint(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutQuint(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuint(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuint(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutQuint(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeOutQuint(startVal.y, finalVal.y, stageCount, stage),
                easeOutQuint(startVal.z, finalVal.z, stageCount, stage),
                easeOutQuint(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutQuint(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutQuint(startVal.x, finalVal.x, alpha),
                easeOutQuint(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutQuint(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutQuint(startVal.x, finalVal.x, alpha),
                easeOutQuint(startVal.y, finalVal.y, alpha),
                easeOutQuint(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutQuint(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutQuint(startVal.x, finalVal.x, alpha),
                easeOutQuint(startVal.y, finalVal.y, alpha),
                easeOutQuint(startVal.z, finalVal.z, alpha),
                easeOutQuint(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutQuint(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutQuint(startVal.x, finalVal.x, alpha),
                easeOutQuint(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutQuint(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutQuint(startVal.x, finalVal.x, alpha),
                easeOutQuint(startVal.y, finalVal.y, alpha),
                easeOutQuint(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeOutQuint(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutQuint(startVal.x, finalVal.x, alpha),
                easeOutQuint(startVal.y, finalVal.y, alpha),
                easeOutQuint(startVal.z, finalVal.z, alpha),
                easeOutQuint(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInOutQuint tweening.
        */
        template <typename T>
        inline T easeInOutQuint(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuint only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 * alpha2 * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 + (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutQuint(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutQuint(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutQuint(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuint(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutQuint(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuint(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuint(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutQuint(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuint(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuint(startVal.z, finalVal.z, stageCount, stage),
                easeInOutQuint(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutQuint(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuint(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutQuint(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuint(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuint(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutQuint(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutQuint(startVal.x, finalVal.x, stageCount, stage),
                easeInOutQuint(startVal.y, finalVal.y, stageCount, stage),
                easeInOutQuint(startVal.z, finalVal.z, stageCount, stage),
                easeInOutQuint(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutQuint(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutQuint(startVal.x, finalVal.x, alpha),
                easeInOutQuint(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutQuint(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutQuint(startVal.x, finalVal.x, alpha),
                easeInOutQuint(startVal.y, finalVal.y, alpha),
                easeInOutQuint(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutQuint(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutQuint(startVal.x, finalVal.x, alpha),
                easeInOutQuint(startVal.y, finalVal.y, alpha),
                easeInOutQuint(startVal.z, finalVal.z, alpha),
                easeInOutQuint(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutQuint(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutQuint(startVal.x, finalVal.x, alpha),
                easeInOutQuint(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutQuint(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutQuint(startVal.x, finalVal.x, alpha),
                easeInOutQuint(startVal.y, finalVal.y, alpha),
                easeInOutQuint(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutQuint(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutQuint(startVal.x, finalVal.x, alpha),
                easeInOutQuint(startVal.y, finalVal.y, alpha),
                easeInOutQuint(startVal.z, finalVal.z, alpha),
                easeInOutQuint(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInSine tweening.
        */
        template <typename T>
        inline T easeInSine(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInSine only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return (T)-1.0f * range * vmath::cos(alpha * (T)M_PI_2F) + range + startVal;
        }
        template <typename T>
        inline T easeInSine(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInSine(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInSine(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInSine(startVal.x, finalVal.x, stageCount, stage),
                easeInSine(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInSine(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInSine(startVal.x, finalVal.x, stageCount, stage),
                easeInSine(startVal.y, finalVal.y, stageCount, stage),
                easeInSine(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInSine(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInSine(startVal.x, finalVal.x, stageCount, stage),
                easeInSine(startVal.y, finalVal.y, stageCount, stage),
                easeInSine(startVal.z, finalVal.z, stageCount, stage),
                easeInSine(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInSine(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInSine(startVal.x, finalVal.x, stageCount, stage),
                easeInSine(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInSine(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInSine(startVal.x, finalVal.x, stageCount, stage),
                easeInSine(startVal.y, finalVal.y, stageCount, stage),
                easeInSine(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInSine(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInSine(startVal.x, finalVal.x, stageCount, stage),
                easeInSine(startVal.y, finalVal.y, stageCount, stage),
                easeInSine(startVal.z, finalVal.z, stageCount, stage),
                easeInSine(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInSine(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInSine(startVal.x, finalVal.x, alpha),
                easeInSine(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInSine(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInSine(startVal.x, finalVal.x, alpha),
                easeInSine(startVal.y, finalVal.y, alpha),
                easeInSine(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInSine(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInSine(startVal.x, finalVal.x, alpha),
                easeInSine(startVal.y, finalVal.y, alpha),
                easeInSine(startVal.z, finalVal.z, alpha),
                easeInSine(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInSine(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInSine(startVal.x, finalVal.x, alpha),
                easeInSine(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInSine(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInSine(startVal.x, finalVal.x, alpha),
                easeInSine(startVal.y, finalVal.y, alpha),
                easeInSine(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInSine(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInSine(startVal.x, finalVal.x, alpha),
                easeInSine(startVal.y, finalVal.y, alpha),
                easeInSine(startVal.z, finalVal.z, alpha),
                easeInSine(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeOutSine tweening.
        */
        template <typename T>
        inline T easeOutSine(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutSine only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range * vmath::sin(alpha * (T)M_PI_2F) + startVal;
        }
        template <typename T>
        inline T easeOutSine(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutSine(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutSine(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeOutSine(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutSine(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeOutSine(startVal.y, finalVal.y, stageCount, stage),
                easeOutSine(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutSine(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeOutSine(startVal.y, finalVal.y, stageCount, stage),
                easeOutSine(startVal.z, finalVal.z, stageCount, stage),
                easeOutSine(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutSine(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeOutSine(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutSine(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeOutSine(startVal.y, finalVal.y, stageCount, stage),
                easeOutSine(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutSine(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeOutSine(startVal.y, finalVal.y, stageCount, stage),
                easeOutSine(startVal.z, finalVal.z, stageCount, stage),
                easeOutSine(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutSine(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutSine(startVal.x, finalVal.x, alpha),
                easeOutSine(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutSine(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutSine(startVal.x, finalVal.x, alpha),
                easeOutSine(startVal.y, finalVal.y, alpha),
                easeOutSine(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutSine(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutSine(startVal.x, finalVal.x, alpha),
                easeOutSine(startVal.y, finalVal.y, alpha),
                easeOutSine(startVal.z, finalVal.z, alpha),
                easeOutSine(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutSine(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutSine(startVal.x, finalVal.x, alpha),
                easeOutSine(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutSine(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutSine(startVal.x, finalVal.x, alpha),
                easeOutSine(startVal.y, finalVal.y, alpha),
                easeOutSine(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeOutSine(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutSine(startVal.x, finalVal.x, alpha),
                easeOutSine(startVal.y, finalVal.y, alpha),
                easeOutSine(startVal.z, finalVal.z, alpha),
                easeOutSine(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInOutSine tweening.
        */
        template <typename T>
        inline T easeInOutSine(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutSine only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return (T)-1.0f * range / (T)2.0f * (vmath::cos(alpha * M_PIF) - (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutSine(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutSine(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutSine(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeInOutSine(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutSine(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeInOutSine(startVal.y, finalVal.y, stageCount, stage),
                easeInOutSine(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutSine(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeInOutSine(startVal.y, finalVal.y, stageCount, stage),
                easeInOutSine(startVal.z, finalVal.z, stageCount, stage),
                easeInOutSine(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutSine(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeInOutSine(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutSine(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeInOutSine(startVal.y, finalVal.y, stageCount, stage),
                easeInOutSine(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutSine(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutSine(startVal.x, finalVal.x, stageCount, stage),
                easeInOutSine(startVal.y, finalVal.y, stageCount, stage),
                easeInOutSine(startVal.z, finalVal.z, stageCount, stage),
                easeInOutSine(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutSine(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutSine(startVal.x, finalVal.x, alpha),
                easeInOutSine(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutSine(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutSine(startVal.x, finalVal.x, alpha),
                easeInOutSine(startVal.y, finalVal.y, alpha),
                easeInOutSine(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutSine(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutSine(startVal.x, finalVal.x, alpha),
                easeInOutSine(startVal.y, finalVal.y, alpha),
                easeInOutSine(startVal.z, finalVal.z, alpha),
                easeInOutSine(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutSine(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutSine(startVal.x, finalVal.x, alpha),
                easeInOutSine(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutSine(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutSine(startVal.x, finalVal.x, alpha),
                easeInOutSine(startVal.y, finalVal.y, alpha),
                easeInOutSine(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutSine(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutSine(startVal.x, finalVal.x, alpha),
                easeInOutSine(startVal.y, finalVal.y, alpha),
                easeInOutSine(startVal.z, finalVal.z, alpha),
                easeInOutSine(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInExpo tweening.
        */
        template <typename T>
        inline T easeInExpo(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInExpo only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return (alpha == (T)0.0f) ? startVal : range * vmath::pow((T)2.0f, (T)10.0f * (alpha - (T)1.0f)) + startVal;
        }
        template <typename T>
        inline T easeInExpo(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInExpo(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInExpo(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInExpo(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInExpo(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInExpo(startVal.y, finalVal.y, stageCount, stage),
                easeInExpo(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInExpo(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInExpo(startVal.y, finalVal.y, stageCount, stage),
                easeInExpo(startVal.z, finalVal.z, stageCount, stage),
                easeInExpo(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInExpo(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInExpo(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInExpo(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInExpo(startVal.y, finalVal.y, stageCount, stage),
                easeInExpo(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInExpo(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInExpo(startVal.y, finalVal.y, stageCount, stage),
                easeInExpo(startVal.z, finalVal.z, stageCount, stage),
                easeInExpo(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInExpo(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInExpo(startVal.x, finalVal.x, alpha),
                easeInExpo(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInExpo(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInExpo(startVal.x, finalVal.x, alpha),
                easeInExpo(startVal.y, finalVal.y, alpha),
                easeInExpo(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInExpo(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInExpo(startVal.x, finalVal.x, alpha),
                easeInExpo(startVal.y, finalVal.y, alpha),
                easeInExpo(startVal.z, finalVal.z, alpha),
                easeInExpo(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInExpo(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInExpo(startVal.x, finalVal.x, alpha),
                easeInExpo(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInExpo(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInExpo(startVal.x, finalVal.x, alpha),
                easeInExpo(startVal.y, finalVal.y, alpha),
                easeInExpo(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInExpo(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInExpo(startVal.x, finalVal.x, alpha),
                easeInExpo(startVal.y, finalVal.y, alpha),
                easeInExpo(startVal.z, finalVal.z, alpha),
                easeInExpo(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeOutExpo tweening.
        */
        template <typename T>
        inline T easeOutExpo(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutExpo only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return (alpha == (T)1.0f) ? finalVal : range * ((T)-1.0f * vmath::pow((T)2.0f, (T)-10.0f * alpha) + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutExpo(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutExpo(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutExpo(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeOutExpo(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutExpo(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeOutExpo(startVal.y, finalVal.y, stageCount, stage),
                easeOutExpo(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutExpo(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeOutExpo(startVal.y, finalVal.y, stageCount, stage),
                easeOutExpo(startVal.z, finalVal.z, stageCount, stage),
                easeOutExpo(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutExpo(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeOutExpo(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutExpo(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeOutExpo(startVal.y, finalVal.y, stageCount, stage),
                easeOutExpo(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutExpo(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeOutExpo(startVal.y, finalVal.y, stageCount, stage),
                easeOutExpo(startVal.z, finalVal.z, stageCount, stage),
                easeOutExpo(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutExpo(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutExpo(startVal.x, finalVal.x, alpha),
                easeOutExpo(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutExpo(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutExpo(startVal.x, finalVal.x, alpha),
                easeOutExpo(startVal.y, finalVal.y, alpha),
                easeOutExpo(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutExpo(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutExpo(startVal.x, finalVal.x, alpha),
                easeOutExpo(startVal.y, finalVal.y, alpha),
                easeOutExpo(startVal.z, finalVal.z, alpha),
                easeOutExpo(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutExpo(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutExpo(startVal.x, finalVal.x, alpha),
                easeOutExpo(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutExpo(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutExpo(startVal.x, finalVal.x, alpha),
                easeOutExpo(startVal.y, finalVal.y, alpha),
                easeOutExpo(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeOutExpo(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutExpo(startVal.x, finalVal.x, alpha),
                easeOutExpo(startVal.y, finalVal.y, alpha),
                easeOutExpo(startVal.z, finalVal.z, alpha),
                easeOutExpo(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInOutExpo tweening.
        */
        template <typename T>
        inline T easeInOutExpo(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutExpo only accepts floating-point inputs.");
            T range = finalVal - startVal;
            //if (alpha == (T)0.0f) return startVal;
            //if (alpha == (T)1.0f) return finalVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * vmath::pow((T)2.0f, (T)10.0f * (alpha2 - (T)1.0f)) + startVal;
            }
            return range / (T)2.0f * (T)-1.0f * vmath::pow((T)2.0f, (T)-10.0f * (alpha2 - (T)1.0f) + (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutExpo(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutExpo(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutExpo(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInOutExpo(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutExpo(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInOutExpo(startVal.y, finalVal.y, stageCount, stage),
                easeInOutExpo(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutExpo(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInOutExpo(startVal.y, finalVal.y, stageCount, stage),
                easeInOutExpo(startVal.z, finalVal.z, stageCount, stage),
                easeInOutExpo(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutExpo(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInOutExpo(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutExpo(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInOutExpo(startVal.y, finalVal.y, stageCount, stage),
                easeInOutExpo(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutExpo(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutExpo(startVal.x, finalVal.x, stageCount, stage),
                easeInOutExpo(startVal.y, finalVal.y, stageCount, stage),
                easeInOutExpo(startVal.z, finalVal.z, stageCount, stage),
                easeInOutExpo(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutExpo(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutExpo(startVal.x, finalVal.x, alpha),
                easeInOutExpo(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutExpo(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutExpo(startVal.x, finalVal.x, alpha),
                easeInOutExpo(startVal.y, finalVal.y, alpha),
                easeInOutExpo(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutExpo(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutExpo(startVal.x, finalVal.x, alpha),
                easeInOutExpo(startVal.y, finalVal.y, alpha),
                easeInOutExpo(startVal.z, finalVal.z, alpha),
                easeInOutExpo(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutExpo(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutExpo(startVal.x, finalVal.x, alpha),
                easeInOutExpo(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutExpo(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutExpo(startVal.x, finalVal.x, alpha),
                easeInOutExpo(startVal.y, finalVal.y, alpha),
                easeInOutExpo(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutExpo(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutExpo(startVal.x, finalVal.x, alpha),
                easeInOutExpo(startVal.y, finalVal.y, alpha),
                easeInOutExpo(startVal.z, finalVal.z, alpha),
                easeInOutExpo(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInCirc tweening.
        */
        template <typename T>
        inline T easeInCirc(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInCirc only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return (T)-1.0f * range * (vmath::sqrt((T)1.0f - alpha * alpha) - (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeInCirc(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInCirc(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInCirc(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInCirc(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInCirc(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInCirc(startVal.y, finalVal.y, stageCount, stage),
                easeInCirc(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInCirc(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInCirc(startVal.y, finalVal.y, stageCount, stage),
                easeInCirc(startVal.z, finalVal.z, stageCount, stage),
                easeInCirc(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInCirc(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInCirc(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInCirc(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInCirc(startVal.y, finalVal.y, stageCount, stage),
                easeInCirc(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInCirc(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInCirc(startVal.y, finalVal.y, stageCount, stage),
                easeInCirc(startVal.z, finalVal.z, stageCount, stage),
                easeInCirc(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInCirc(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInCirc(startVal.x, finalVal.x, alpha),
                easeInCirc(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInCirc(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInCirc(startVal.x, finalVal.x, alpha),
                easeInCirc(startVal.y, finalVal.y, alpha),
                easeInCirc(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInCirc(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInCirc(startVal.x, finalVal.x, alpha),
                easeInCirc(startVal.y, finalVal.y, alpha),
                easeInCirc(startVal.z, finalVal.z, alpha),
                easeInCirc(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInCirc(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInCirc(startVal.x, finalVal.x, alpha),
                easeInCirc(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInCirc(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInCirc(startVal.x, finalVal.x, alpha),
                easeInCirc(startVal.y, finalVal.y, alpha),
                easeInCirc(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInCirc(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInCirc(startVal.x, finalVal.x, alpha),
                easeInCirc(startVal.y, finalVal.y, alpha),
                easeInCirc(startVal.z, finalVal.z, alpha),
                easeInCirc(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeOutCirc tweening.
        */
        template <typename T>
        inline T easeOutCirc(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutCirc only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alphaMinus1 = alpha - (T)1.0f;
            return range * vmath::sqrt((T)1.0f - alphaMinus1 * alphaMinus1) + startVal;
        }
        template <typename T>
        inline T easeOutCirc(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutCirc(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutCirc(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeOutCirc(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutCirc(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeOutCirc(startVal.y, finalVal.y, stageCount, stage),
                easeOutCirc(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutCirc(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeOutCirc(startVal.y, finalVal.y, stageCount, stage),
                easeOutCirc(startVal.z, finalVal.z, stageCount, stage),
                easeOutCirc(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutCirc(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeOutCirc(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutCirc(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeOutCirc(startVal.y, finalVal.y, stageCount, stage),
                easeOutCirc(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutCirc(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeOutCirc(startVal.y, finalVal.y, stageCount, stage),
                easeOutCirc(startVal.z, finalVal.z, stageCount, stage),
                easeOutCirc(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutCirc(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutCirc(startVal.x, finalVal.x, alpha),
                easeOutCirc(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutCirc(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutCirc(startVal.x, finalVal.x, alpha),
                easeOutCirc(startVal.y, finalVal.y, alpha),
                easeOutCirc(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutCirc(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutCirc(startVal.x, finalVal.x, alpha),
                easeOutCirc(startVal.y, finalVal.y, alpha),
                easeOutCirc(startVal.z, finalVal.z, alpha),
                easeOutCirc(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutCirc(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutCirc(startVal.x, finalVal.x, alpha),
                easeOutCirc(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutCirc(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutCirc(startVal.x, finalVal.x, alpha),
                easeOutCirc(startVal.y, finalVal.y, alpha),
                easeOutCirc(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeOutCirc(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutCirc(startVal.x, finalVal.x, alpha),
                easeOutCirc(startVal.y, finalVal.y, alpha),
                easeOutCirc(startVal.z, finalVal.z, alpha),
                easeOutCirc(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInOutCirc tweening.
        */
        template <typename T>
        inline T easeInOutCirc(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutCirc only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return (T)-1.0f * range / (T)2.0f * (vmath::sqrt((T)1.0f - alpha2 * alpha2) - (T)1.0f) + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (vmath::sqrt((T)1.0f - alpha2Minus2 * alpha2Minus2) + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutCirc(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutCirc(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutCirc(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCirc(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutCirc(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCirc(startVal.y, finalVal.y, stageCount, stage),
                easeInOutCirc(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutCirc(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCirc(startVal.y, finalVal.y, stageCount, stage),
                easeInOutCirc(startVal.z, finalVal.z, stageCount, stage),
                easeInOutCirc(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutCirc(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCirc(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutCirc(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCirc(startVal.y, finalVal.y, stageCount, stage),
                easeInOutCirc(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutCirc(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutCirc(startVal.x, finalVal.x, stageCount, stage),
                easeInOutCirc(startVal.y, finalVal.y, stageCount, stage),
                easeInOutCirc(startVal.z, finalVal.z, stageCount, stage),
                easeInOutCirc(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutCirc(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutCirc(startVal.x, finalVal.x, alpha),
                easeInOutCirc(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutCirc(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutCirc(startVal.x, finalVal.x, alpha),
                easeInOutCirc(startVal.y, finalVal.y, alpha),
                easeInOutCirc(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutCirc(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutCirc(startVal.x, finalVal.x, alpha),
                easeInOutCirc(startVal.y, finalVal.y, alpha),
                easeInOutCirc(startVal.z, finalVal.z, alpha),
                easeInOutCirc(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutCirc(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutCirc(startVal.x, finalVal.x, alpha),
                easeInOutCirc(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutCirc(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutCirc(startVal.x, finalVal.x, alpha),
                easeInOutCirc(startVal.y, finalVal.y, alpha),
                easeInOutCirc(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutCirc(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutCirc(startVal.x, finalVal.x, alpha),
                easeInOutCirc(startVal.y, finalVal.y, alpha),
                easeInOutCirc(startVal.z, finalVal.z, alpha),
                easeInOutCirc(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInElastic tweening.
        */
        template <typename T>
        inline T easeInElastic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInElastic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            //if (alpha == (T)0.0f) return startVal;
            //if (alpha == (T)1.0f) return finalVal;
            T alphaMinus1 = alpha - 1;
            return (T)-1.0f * range * vmath::pow((T)2.0f, (T)10.0f * alphaMinus1) * vmath::sin(M_PIF * ((T)20.0f / (T)3.0f * alphaMinus1 - (T)0.5f)) + startVal;
        }
        template <typename T>
        inline T easeInElastic(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInElastic(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInElastic(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInElastic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInElastic(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInElastic(startVal.y, finalVal.y, stageCount, stage),
                easeInElastic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInElastic(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInElastic(startVal.y, finalVal.y, stageCount, stage),
                easeInElastic(startVal.z, finalVal.z, stageCount, stage),
                easeInElastic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInElastic(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInElastic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInElastic(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInElastic(startVal.y, finalVal.y, stageCount, stage),
                easeInElastic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInElastic(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInElastic(startVal.y, finalVal.y, stageCount, stage),
                easeInElastic(startVal.z, finalVal.z, stageCount, stage),
                easeInElastic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInElastic(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInElastic(startVal.x, finalVal.x, alpha),
                easeInElastic(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInElastic(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInElastic(startVal.x, finalVal.x, alpha),
                easeInElastic(startVal.y, finalVal.y, alpha),
                easeInElastic(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInElastic(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInElastic(startVal.x, finalVal.x, alpha),
                easeInElastic(startVal.y, finalVal.y, alpha),
                easeInElastic(startVal.z, finalVal.z, alpha),
                easeInElastic(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInElastic(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInElastic(startVal.x, finalVal.x, alpha),
                easeInElastic(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInElastic(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInElastic(startVal.x, finalVal.x, alpha),
                easeInElastic(startVal.y, finalVal.y, alpha),
                easeInElastic(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInElastic(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInElastic(startVal.x, finalVal.x, alpha),
                easeInElastic(startVal.y, finalVal.y, alpha),
                easeInElastic(startVal.z, finalVal.z, alpha),
                easeInElastic(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeOutElastic tweening.
        */
        template <typename T>
        inline T easeOutElastic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutElastic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            //if (alpha == (T)0.0f) return startVal;
            //if (alpha == (T)1.0f) return finalVal;
            return range * vmath::pow((T)2.0f, (T)-10.0f * alpha) * vmath::sin(M_PIF * ((T)20.0f / (T)3.0f * alpha - (T)0.5f)) * (T)0.5f + finalVal;
        }
        template <typename T>
        inline T easeOutElastic(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutElastic(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutElastic(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeOutElastic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutElastic(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeOutElastic(startVal.y, finalVal.y, stageCount, stage),
                easeOutElastic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutElastic(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeOutElastic(startVal.y, finalVal.y, stageCount, stage),
                easeOutElastic(startVal.z, finalVal.z, stageCount, stage),
                easeOutElastic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutElastic(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeOutElastic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutElastic(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeOutElastic(startVal.y, finalVal.y, stageCount, stage),
                easeOutElastic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutElastic(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeOutElastic(startVal.y, finalVal.y, stageCount, stage),
                easeOutElastic(startVal.z, finalVal.z, stageCount, stage),
                easeOutElastic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutElastic(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutElastic(startVal.x, finalVal.x, alpha),
                easeOutElastic(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutElastic(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutElastic(startVal.x, finalVal.x, alpha),
                easeOutElastic(startVal.y, finalVal.y, alpha),
                easeOutElastic(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutElastic(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutElastic(startVal.x, finalVal.x, alpha),
                easeOutElastic(startVal.y, finalVal.y, alpha),
                easeOutElastic(startVal.z, finalVal.z, alpha),
                easeOutElastic(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutElastic(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutElastic(startVal.x, finalVal.x, alpha),
                easeOutElastic(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutElastic(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutElastic(startVal.x, finalVal.x, alpha),
                easeOutElastic(startVal.y, finalVal.y, alpha),
                easeOutElastic(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeIneaseOutElasticElastic(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutElastic(startVal.x, finalVal.x, alpha),
                easeOutElastic(startVal.y, finalVal.y, alpha),
                easeOutElastic(startVal.z, finalVal.z, alpha),
                easeOutElastic(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInOutElastic tweening.
        */
        template <typename T>
        inline T easeInOutElastic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutElastic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            //if (alpha == (T)0.0f) return startVal;
            //if (alpha == (T)1.0f) return finalVal;
            T alpha2 = (T)2.0f * alpha;
            T alpha2Minus1 = alpha2 - (T)1.0f;
            if (alpha2 < (T)1.0f) {
                return (T)-0.5f * range * vmath::pow((T)2.0f, (T)10.0f * alpha2Minus1) * vmath::sin(M_PIF * ((T)40.0f / (T)9.0f * alpha2Minus1 - (T)0.5f)) + startVal;
            }
            return range * vmath::pow((T)2.0f, (T)-10.0f * alpha2Minus1) * vmath::sin(M_PIF * ((T)40.0f / (T)9.0f * alpha2Minus1 - (T)0.5f)) * (T)0.5f + finalVal;
        }
        template <typename T>
        inline T easeInOutElastic(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutElastic(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutElastic(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutElastic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutElastic(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutElastic(startVal.y, finalVal.y, stageCount, stage),
                easeInOutElastic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutElastic(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutElastic(startVal.y, finalVal.y, stageCount, stage),
                easeInOutElastic(startVal.z, finalVal.z, stageCount, stage),
                easeInOutElastic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutElastic(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutElastic(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutElastic(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutElastic(startVal.y, finalVal.y, stageCount, stage),
                easeInOutElastic(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutElastic(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutElastic(startVal.x, finalVal.x, stageCount, stage),
                easeInOutElastic(startVal.y, finalVal.y, stageCount, stage),
                easeInOutElastic(startVal.z, finalVal.z, stageCount, stage),
                easeInOutElastic(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutElastic(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutElastic(startVal.x, finalVal.x, alpha),
                easeInOutElastic(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutElastic(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutElastic(startVal.x, finalVal.x, alpha),
                easeInOutElastic(startVal.y, finalVal.y, alpha),
                easeInOutElastic(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutElastic(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutElastic(startVal.x, finalVal.x, alpha),
                easeInOutElastic(startVal.y, finalVal.y, alpha),
                easeInOutElastic(startVal.z, finalVal.z, alpha),
                easeInOutElastic(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutElastic(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutElastic(startVal.x, finalVal.x, alpha),
                easeInOutElastic(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutElastic(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutElastic(startVal.x, finalVal.x, alpha),
                easeInOutElastic(startVal.y, finalVal.y, alpha),
                easeInOutElastic(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutElastic(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutElastic(startVal.x, finalVal.x, alpha),
                easeInOutElastic(startVal.y, finalVal.y, alpha),
                easeInOutElastic(startVal.z, finalVal.z, alpha),
                easeInOutElastic(startVal.w, finalVal.w, alpha));
        }

        /*! @brief Computes an easeInBack tweening.
        */
        template <typename T>
        inline T easeInBack(T startVal, T finalVal, T alpha, T s = (T)1.70158) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInBack only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range * alpha * alpha * ((s + (T)1.0f) * alpha - s) + startVal;
        }
        template <typename T>
        inline T easeInBack(T startVal, T finalVal, ui16 stageCount, ui16 stage, T s = (T)1.70158) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInBack(startVal, finalVal, (T)stage / (T)stageCount, s);
        }
        inline f32v2 easeInBack(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v2(easeInBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInBack(startVal.y, finalVal.y, stageCount, stage, s));
        }
        inline f32v3 easeInBack(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v3(easeInBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeInBack(startVal.z, finalVal.z, stageCount, stage, s));
        }
        inline f32v4 easeInBack(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v4(easeInBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeInBack(startVal.z, finalVal.z, stageCount, stage, s),
                easeInBack(startVal.w, finalVal.w, stageCount, stage, s));
        }
        inline f64v2 easeInBack(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v2(easeInBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInBack(startVal.y, finalVal.y, stageCount, stage, s));
        }
        inline f64v3 easeInBack(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v3(easeInBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeInBack(startVal.z, finalVal.z, stageCount, stage, s));
        }
        inline f64v4 easeInBack(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v4(easeInBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeInBack(startVal.z, finalVal.z, stageCount, stage, s),
                easeInBack(startVal.w, finalVal.w, stageCount, stage, s));
        }
        inline f32v2 easeInBack(f32v2 startVal, f32v2 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v2(easeInBack(startVal.x, finalVal.x, alpha, s),
                easeInBack(startVal.y, finalVal.y, alpha, s));
        }
        inline f32v3 easeInBack(f32v3 startVal, f32v3 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v3(easeInBack(startVal.x, finalVal.x, alpha, s),
                easeInBack(startVal.y, finalVal.y, alpha, s),
                easeInBack(startVal.z, finalVal.z, alpha, s));
        }
        inline f32v4 easeInBack(f32v4 startVal, f32v4 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v4(easeInBack(startVal.x, finalVal.x, alpha, s),
                easeInBack(startVal.y, finalVal.y, alpha, s),
                easeInBack(startVal.z, finalVal.z, alpha, s),
                easeInBack(startVal.w, finalVal.w, alpha, s));
        }
        inline f64v2 easeInBack(f64v2 startVal, f64v2 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v2(easeInBack(startVal.x, finalVal.x, alpha, s),
                easeInBack(startVal.y, finalVal.y, alpha, s));
        }
        inline f64v3 easeInBack(f64v3 startVal, f64v3 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v3(easeInBack(startVal.x, finalVal.x, alpha, s),
                easeInBack(startVal.y, finalVal.y, alpha, s),
                easeInBack(startVal.z, finalVal.z, alpha, s));
        }
        inline f64v4 easeInBack(f64v4 startVal, f64v4 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v4(easeInBack(startVal.x, finalVal.x, alpha, s),
                easeInBack(startVal.y, finalVal.y, alpha, s),
                easeInBack(startVal.z, finalVal.z, alpha, s),
                easeInBack(startVal.w, finalVal.w, alpha, s));
        }

        /*! @brief Computes an easeOutBack tweening.
        */
        template <typename T>
        inline T easeOutBack(T startVal, T finalVal, T alpha, T s = (T)1.70158) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutBack only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alphaMinus1 = alpha - 1;
            return range * (alphaMinus1 * alphaMinus1 * ((s + (T)1.0f) * alphaMinus1 + s) + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutBack(T startVal, T finalVal, ui16 stageCount, ui16 stage, T s = (T)1.70158) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutBack(startVal, finalVal, (T)stage / (T)stageCount, s);
        }
        inline f32v2 easeOutBack(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v2(easeOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeOutBack(startVal.y, finalVal.y, stageCount, stage, s));
        }
        inline f32v3 easeOutBack(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v3(easeOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeOutBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeOutBack(startVal.z, finalVal.z, stageCount, stage, s));
        }
        inline f32v4 easeOutBack(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v4(easeOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeOutBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeOutBack(startVal.z, finalVal.z, stageCount, stage, s),
                easeOutBack(startVal.w, finalVal.w, stageCount, stage, s));
        }
        inline f64v2 easeOutBack(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v2(easeOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeOutBack(startVal.y, finalVal.y, stageCount, stage, s));
        }
        inline f64v3 easeOutBack(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v3(easeOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeOutBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeOutBack(startVal.z, finalVal.z, stageCount, stage, s));
        }
        inline f64v4 easeOutBack(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v4(easeOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeOutBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeOutBack(startVal.z, finalVal.z, stageCount, stage, s),
                easeOutBack(startVal.w, finalVal.w, stageCount, stage, s));
        }
        inline f32v2 easeOutBack(f32v2 startVal, f32v2 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v2(easeOutBack(startVal.x, finalVal.x, alpha, s),
                easeOutBack(startVal.y, finalVal.y, alpha, s));
        }
        inline f32v3 easeOutBack(f32v3 startVal, f32v3 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v3(easeOutBack(startVal.x, finalVal.x, alpha, s),
                easeOutBack(startVal.y, finalVal.y, alpha, s),
                easeOutBack(startVal.z, finalVal.z, alpha, s));
        }
        inline f32v4 easeOutBack(f32v4 startVal, f32v4 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v4(easeOutBack(startVal.x, finalVal.x, alpha, s),
                easeOutBack(startVal.y, finalVal.y, alpha, s),
                easeOutBack(startVal.z, finalVal.z, alpha, s),
                easeOutBack(startVal.w, finalVal.w, alpha, s));
        }
        inline f64v2 easeOutBack(f64v2 startVal, f64v2 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v2(easeOutBack(startVal.x, finalVal.x, alpha, s),
                easeOutBack(startVal.y, finalVal.y, alpha, s));
        }
        inline f64v3 easeOutBack(f64v3 startVal, f64v3 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v3(easeOutBack(startVal.x, finalVal.x, alpha, s),
                easeOutBack(startVal.y, finalVal.y, alpha, s),
                easeOutBack(startVal.z, finalVal.z, alpha, s));
        }
        inline f64v4 easeOutBack(f64v4 startVal, f64v4 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v4(easeOutBack(startVal.x, finalVal.x, alpha, s),
                easeOutBack(startVal.y, finalVal.y, alpha, s),
                easeOutBack(startVal.z, finalVal.z, alpha, s),
                easeOutBack(startVal.w, finalVal.w, alpha, s));
        }

        /*! @brief Computes an easeInOutBack tweening.
        */
        template <typename T>
        inline T easeInOutBack(T startVal, T finalVal, T alpha, T s = (T)1.70158) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutBack only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alpha2 = alpha * (T)2.0f;
            T sPrime = s * (T)1.525f;
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * (alpha2 * alpha2 * ((sPrime + (T)1.0f) * alpha2 - sPrime)) + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * ((sPrime + (T)1.0f) * alpha2Minus2 + sPrime) + (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutBack(T startVal, T finalVal, ui16 stageCount, ui16 stage, T s = (T)1.70158) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutBack(startVal, finalVal, (T)stage / (T)stageCount, s);
        }
        inline f32v2 easeInOutBack(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v2(easeInOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInOutBack(startVal.y, finalVal.y, stageCount, stage, s));
        }
        inline f32v3 easeInOutBack(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v3(easeInOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInOutBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeInOutBack(startVal.z, finalVal.z, stageCount, stage, s));
        }
        inline f32v4 easeInOutBack(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage, f32 s = 1.70158f) {
            return f32v4(easeInOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInOutBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeInOutBack(startVal.z, finalVal.z, stageCount, stage, s),
                easeInOutBack(startVal.w, finalVal.w, stageCount, stage, s));
        }
        inline f64v2 easeInOutBack(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v2(easeInOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInOutBack(startVal.y, finalVal.y, stageCount, stage, s));
        }
        inline f64v3 easeInOutBack(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v3(easeInOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInOutBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeInOutBack(startVal.z, finalVal.z, stageCount, stage, s));
        }
        inline f64v4 easeInOutBack(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage, f64 s = 1.70158) {
            return f64v4(easeInOutBack(startVal.x, finalVal.x, stageCount, stage, s),
                easeInOutBack(startVal.y, finalVal.y, stageCount, stage, s),
                easeInOutBack(startVal.z, finalVal.z, stageCount, stage, s),
                easeInOutBack(startVal.w, finalVal.w, stageCount, stage, s));
        }
        inline f32v2 easeInOutBack(f32v2 startVal, f32v2 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v2(easeInOutBack(startVal.x, finalVal.x, alpha, s),
                easeInOutBack(startVal.y, finalVal.y, alpha, s));
        }
        inline f32v3 easeInOutBack(f32v3 startVal, f32v3 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v3(easeInOutBack(startVal.x, finalVal.x, alpha, s),
                easeInOutBack(startVal.y, finalVal.y, alpha, s),
                easeInOutBack(startVal.z, finalVal.z, alpha, s));
        }
        inline f32v4 easeInOutBack(f32v4 startVal, f32v4 finalVal, f32 alpha, f32 s = 1.70158f) {
            return f32v4(easeInOutBack(startVal.x, finalVal.x, alpha, s),
                easeInOutBack(startVal.y, finalVal.y, alpha, s),
                easeInOutBack(startVal.z, finalVal.z, alpha, s),
                easeInOutBack(startVal.w, finalVal.w, alpha, s));
        }
        inline f64v2 easeInOutBack(f64v2 startVal, f64v2 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v2(easeInOutBack(startVal.x, finalVal.x, alpha, s),
                easeInOutBack(startVal.y, finalVal.y, alpha, s));
        }
        inline f64v3 easeInOutBack(f64v3 startVal, f64v3 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v3(easeInOutBack(startVal.x, finalVal.x, alpha, s),
                easeInOutBack(startVal.y, finalVal.y, alpha, s),
                easeInOutBack(startVal.z, finalVal.z, alpha, s));
        }
        inline f64v4 easeInOutBack(f64v4 startVal, f64v4 finalVal, f64 alpha, f64 s = 1.70158) {
            return f64v4(easeInOutBack(startVal.x, finalVal.x, alpha, s),
                easeInOutBack(startVal.y, finalVal.y, alpha, s),
                easeInOutBack(startVal.z, finalVal.z, alpha, s),
                easeInOutBack(startVal.w, finalVal.w, alpha, s));
        }

        // TODO(Matthew): This doesn't work. Fix it!
        /*! @brief Computes an easeInBounce tweening.
        */
        template <typename T>
        inline T easeInBounce(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInBounce only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range - easeOutBounce((T)0, finalVal - startVal, (T)1 - alpha) + startVal;
        }
        template <typename T>
        inline T easeInBounce(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInBounce(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInBounce(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInBounce(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInBounce(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInBounce(startVal.y, finalVal.y, stageCount, stage),
                easeInBounce(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInBounce(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInBounce(startVal.y, finalVal.y, stageCount, stage),
                easeInBounce(startVal.z, finalVal.z, stageCount, stage),
                easeInBounce(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInBounce(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInBounce(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInBounce(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInBounce(startVal.y, finalVal.y, stageCount, stage),
                easeInBounce(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInBounce(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInBounce(startVal.y, finalVal.y, stageCount, stage),
                easeInBounce(startVal.z, finalVal.z, stageCount, stage),
                easeInBounce(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInBounce(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInBounce(startVal.x, finalVal.x, alpha),
                easeInBounce(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInBounce(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInBounce(startVal.x, finalVal.x, alpha),
                easeInBounce(startVal.y, finalVal.y, alpha),
                easeInBounce(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInBounce(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInBounce(startVal.x, finalVal.x, alpha),
                easeInBounce(startVal.y, finalVal.y, alpha),
                easeInBounce(startVal.z, finalVal.z, alpha),
                easeInBounce(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInBounce(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInBounce(startVal.x, finalVal.x, alpha),
                easeInBounce(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInBounce(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInBounce(startVal.x, finalVal.x, alpha),
                easeInBounce(startVal.y, finalVal.y, alpha),
                easeInBounce(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInBounce(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInBounce(startVal.x, finalVal.x, alpha),
                easeInBounce(startVal.y, finalVal.y, alpha),
                easeInBounce(startVal.z, finalVal.z, alpha),
                easeInBounce(startVal.w, finalVal.w, alpha));
        }

        // TODO(Matthew): This doesn't work. Fix it!
        /*! @brief Computes an easeOutBounce tweening.
        */
        template <typename T>
        inline T easeOutBounce(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutBounce only accepts floating-point inputs.");
            T range = finalVal - startVal;
            if (alpha < (T)1.0f / (T)2.75f) {
                return range * ((T)7.5625f * alpha * alpha) + startVal;
            } else if (alpha < (T)2.0f / (T)2.75f) {
                T alphaPrime = alpha - ((T)1.5f / (T)2.75f);
                return range * ((T)7.5625f * alphaPrime * alphaPrime * (T)0.75f) + startVal;
            } else if (alpha < (T)2.5f / (T)2.75f) {
                T alphaPrime = alpha - ((T)2.25f / (T)2.75f);
                return range * ((T)7.5625f * alphaPrime * alphaPrime * (T)0.9375f) + startVal;
            } else {
                T alphaPrime = alpha - ((T)2.625f / (T)2.75f);
                return range * ((T)7.5625f * alphaPrime * alphaPrime * (T)0.984375f) + startVal;
            }
        }
        template <typename T>
        inline T easeOutBounce(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeOutBounce(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeOutBounce(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeOutBounce(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeOutBounce(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeOutBounce(startVal.y, finalVal.y, stageCount, stage),
                easeOutBounce(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeOutBounce(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeOutBounce(startVal.y, finalVal.y, stageCount, stage),
                easeOutBounce(startVal.z, finalVal.z, stageCount, stage),
                easeOutBounce(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeOutBounce(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeOutBounce(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeOutBounce(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeOutBounce(startVal.y, finalVal.y, stageCount, stage),
                easeOutBounce(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeOutBounce(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeOutBounce(startVal.y, finalVal.y, stageCount, stage),
                easeOutBounce(startVal.z, finalVal.z, stageCount, stage),
                easeOutBounce(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeOutBounce(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeOutBounce(startVal.x, finalVal.x, alpha),
                easeOutBounce(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeOutBounce(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeOutBounce(startVal.x, finalVal.x, alpha),
                easeOutBounce(startVal.y, finalVal.y, alpha),
                easeOutBounce(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeOutBounce(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeOutBounce(startVal.x, finalVal.x, alpha),
                easeOutBounce(startVal.y, finalVal.y, alpha),
                easeOutBounce(startVal.z, finalVal.z, alpha),
                easeOutBounce(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeOutBounce(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeOutBounce(startVal.x, finalVal.x, alpha),
                easeOutBounce(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeOutBounce(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeOutBounce(startVal.x, finalVal.x, alpha),
                easeOutBounce(startVal.y, finalVal.y, alpha),
                easeOutBounce(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeOutBounce(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeOutBounce(startVal.x, finalVal.x, alpha),
                easeOutBounce(startVal.y, finalVal.y, alpha),
                easeOutBounce(startVal.z, finalVal.z, alpha),
                easeOutBounce(startVal.w, finalVal.w, alpha));
        }

        // TODO(Matthew): This doesn't work. Fix it!
        /*! @brief Computes an easeInOutBounce tweening.
        */
        template <typename T>
        inline T easeInOutBounce(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutBounce only accepts floating-point inputs.");
            T range = finalVal - startVal;
            if (alpha < (T)0.5f) {
                return easeInBounce((T)0, finalVal - startVal, (T)2 * alpha) * (T)0.5f + startVal;
            }
            return easeOutBounce((T)0, finalVal - startVal, (T)2 * alpha - (T)1) * range * (T)0.25f + startVal;
        }
        template <typename T>
        inline T easeInOutBounce(T startVal, T finalVal, ui16 stageCount, ui16 stage) {
            vorb_assert(stageCount != 0, "Stage count must be greater than 0.");
            return easeInOutBounce(startVal, finalVal, (T)stage / (T)stageCount);
        }
        inline f32v2 easeInOutBounce(f32v2 startVal, f32v2 finalVal, ui16 stageCount, ui16 stage) {
            return f32v2(easeInOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInOutBounce(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f32v3 easeInOutBounce(f32v3 startVal, f32v3 finalVal, ui16 stageCount, ui16 stage) {
            return f32v3(easeInOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInOutBounce(startVal.y, finalVal.y, stageCount, stage),
                easeInOutBounce(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f32v4 easeInOutBounce(f32v4 startVal, f32v4 finalVal, ui16 stageCount, ui16 stage) {
            return f32v4(easeInOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInOutBounce(startVal.y, finalVal.y, stageCount, stage),
                easeInOutBounce(startVal.z, finalVal.z, stageCount, stage),
                easeInOutBounce(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f64v2 easeInOutBounce(f64v2 startVal, f64v2 finalVal, ui16 stageCount, ui16 stage) {
            return f64v2(easeInOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInOutBounce(startVal.y, finalVal.y, stageCount, stage));
        }
        inline f64v3 easeInOutBounce(f64v3 startVal, f64v3 finalVal, ui16 stageCount, ui16 stage) {
            return f64v3(easeInOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInOutBounce(startVal.y, finalVal.y, stageCount, stage),
                easeInOutBounce(startVal.z, finalVal.z, stageCount, stage));
        }
        inline f64v4 easeInOutBounce(f64v4 startVal, f64v4 finalVal, ui16 stageCount, ui16 stage) {
            return f64v4(easeInOutBounce(startVal.x, finalVal.x, stageCount, stage),
                easeInOutBounce(startVal.y, finalVal.y, stageCount, stage),
                easeInOutBounce(startVal.z, finalVal.z, stageCount, stage),
                easeInOutBounce(startVal.w, finalVal.w, stageCount, stage));
        }
        inline f32v2 easeInOutBounce(f32v2 startVal, f32v2 finalVal, f32 alpha) {
            return f32v2(easeInOutBounce(startVal.x, finalVal.x, alpha),
                easeInOutBounce(startVal.y, finalVal.y, alpha));
        }
        inline f32v3 easeInOutBounce(f32v3 startVal, f32v3 finalVal, f32 alpha) {
            return f32v3(easeInOutBounce(startVal.x, finalVal.x, alpha),
                easeInOutBounce(startVal.y, finalVal.y, alpha),
                easeInOutBounce(startVal.z, finalVal.z, alpha));
        }
        inline f32v4 easeInOutBounce(f32v4 startVal, f32v4 finalVal, f32 alpha) {
            return f32v4(easeInOutBounce(startVal.x, finalVal.x, alpha),
                easeInOutBounce(startVal.y, finalVal.y, alpha),
                easeInOutBounce(startVal.z, finalVal.z, alpha),
                easeInOutBounce(startVal.w, finalVal.w, alpha));
        }
        inline f64v2 easeInOutBounce(f64v2 startVal, f64v2 finalVal, f64 alpha) {
            return f64v2(easeInOutBounce(startVal.x, finalVal.x, alpha),
                easeInOutBounce(startVal.y, finalVal.y, alpha));
        }
        inline f64v3 easeInOutBounce(f64v3 startVal, f64v3 finalVal, f64 alpha) {
            return f64v3(easeInOutBounce(startVal.x, finalVal.x, alpha),
                easeInOutBounce(startVal.y, finalVal.y, alpha),
                easeInOutBounce(startVal.z, finalVal.z, alpha));
        }
        inline f64v4 easeInOutBounce(f64v4 startVal, f64v4 finalVal, f64 alpha) {
            return f64v4(easeInOutBounce(startVal.x, finalVal.x, alpha),
                easeInOutBounce(startVal.y, finalVal.y, alpha),
                easeInOutBounce(startVal.z, finalVal.z, alpha),
                easeInOutBounce(startVal.w, finalVal.w, alpha));
        }

        struct Transition32_UI16 {
            f32 initialLength, finalLength;
            ui16 currentTime, finalTime;
            f32(*tweeningFunc)(f32, f32, ui16, ui16);
            f32 calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, finalTime, currentTime);
            }
        };
        struct Transition32v2_UI16 {
            f32v2 initialLength, finalLength;
            ui16 currentTime, finalTime;
            f32v2(*tweeningFunc)(f32v2, f32v2, ui16, ui16);
            const f32v2& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, finalTime, currentTime);
            }
        };
        struct Transition32v3_UI16 {
            f32v3 initialLength, finalLength;
            ui16 currentTime, finalTime;
            f32v3(*tweeningFunc)(f32v3, f32v3, ui16, ui16);
            const f32v3& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, finalTime, currentTime);
            }
        };
        struct Transition32v4_UI16 {
            f32v4 initialLength, finalLength;
            ui16 currentTime, finalTime;
            f32v4(*tweeningFunc)(f32v4, f32v4, ui16, ui16);
            const f32v4& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, finalTime, currentTime);
            }
        };
        struct Transition64_UI16 {
            f64 initialLength, finalLength;
            ui16 currentTime, finalTime;
            f64(*tweeningFunc)(f64, f64, ui16, ui16);
            f64 calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, finalTime, currentTime);
            }
        };
        struct Transition64v2_UI16 {
            f64v2 initialLength, finalLength;
            ui16 currentTime, finalTime;
            f64v2(*tweeningFunc)(f64v2, f64v2, ui16, ui16);
            const f64v2& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, finalTime, currentTime);
            }
        };
        struct Transition64v3_UI16 {
            f64v3 initialLength, finalLength;
            ui16 currentTime, finalTime;
            f64v3(*tweeningFunc)(f64v3, f64v3, ui16, ui16);
            const f64v3& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, finalTime, currentTime);
            }
        };
        struct Transition64v4_UI16 {
            f64v4 initialLength, finalLength;
            ui16 currentTime, finalTime;
            f64v4(*tweeningFunc)(f64v4, f64v4, ui16, ui16);
            const f64v4& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, finalTime, currentTime);
            }
        };

        struct Transition32_F32 {
            f32 initialLength, finalLength;
            f32 currentTime, finalTime;
            f32(*tweeningFunc)(f32, f32, f32);
            f32 calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, currentTime / finalTime);
            }
        };
        struct Transition32v2_F32 {
            f32v2 initialLength, finalLength;
            f32 currentTime, finalTime;
            f32v2(*tweeningFunc)(f32v2, f32v2, f32);
            const f32v2& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, currentTime / finalTime);
            }
        };
        struct Transition32v3_F32 {
            f32v3 initialLength, finalLength;
            f32 currentTime, finalTime;
            f32v3(*tweeningFunc)(f32v3, f32v3, f32);
            const f32v3& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, currentTime / finalTime);
            }
        };
        struct Transition32v4_F32 {
            f32v4 initialLength, finalLength;
            f32 currentTime, finalTime;
            f32v4(*tweeningFunc)(f32v4, f32v4, f32);
            const f32v4& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, currentTime / finalTime);
            }
        };
        struct Transition64_F32 {
            f64 initialLength, finalLength;
            f64 currentTime, finalTime;
            f64(*tweeningFunc)(f64, f64, f64);
            f64 calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, currentTime / finalTime);
            }
        };
        struct Transition64v2_F32 {
            f64v2 initialLength, finalLength;
            f64 currentTime, finalTime;
            f64v2(*tweeningFunc)(f64v2, f64v2, f64);
            const f64v2& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, currentTime / finalTime);
            }
        };
        struct Transition64v3_F32 {
            f64v3 initialLength, finalLength;
            f64 currentTime, finalTime;
            f64v3(*tweeningFunc)(f64v3, f64v3, f64);
            const f64v3& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, currentTime / finalTime);
            }
        };
        struct Transition64v4_F32 {
            f64v4 initialLength, finalLength;
            f64 currentTime, finalTime;
            f64v4(*tweeningFunc)(f64v4, f64v4, f64);
            const f64v4& calculateCurrent() {
                return tweeningFunc(initialLength, finalLength, currentTime / finalTime);
            }
        };
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_TweeningMath_hpp__
