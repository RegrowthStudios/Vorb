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
*/

#pragma once

#ifndef Vorb_TweeningMath_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TweeningMath_hpp__
//! @endcond

namespace vorb {
    namespace math {
        /*! @brief Computes a linear tweening.
        */
        template <typename T>
        inline T linear(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "linear only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T rangeStep = range / (T)stageCount;
            return minVal + rangeStep * (T)stage;
        }

        /*! @brief Computes an easeInQuad tweening.
        */
        template <typename T>
        inline T easeInQuad(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuad only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction = (T)stage / (T)stageCount;
            return range * stageFraction * stageFraction + startVal;
        }

        /*! @brief Computes an easeOutQuad tweening.
        */
        template <typename T>
        inline T easeOutQuad(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuad only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction = (T)stage / (T)stageCount;
            return (T)-1.0f * range * stageFraction * (stageFraction - (T)2.0f) + startVal;
        }

        /*! @brief Computes an easeInOutQuad tweening.
        */
        template <typename T>
        inline T easeInOutQuad(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuad only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction2 = (T)stage / ((T)stageCount / (T)2.0f);
            if (stageFraction2 < (T)1.0f) {
                return range / (T)2.0f * stageFraction2 * stageFraction2 + startVal;
            }
            T stageFraction2Minus1 = stageFraction2 - (T)1.0f;
            return (T)-1.0f * range / (T)2.0f * (stageFraction2Minus1 * (stageFraction2Minus1 - (T)2.0f) - (T)1.0f) + startVal;
        }

        /*! @brief Computes an easeInCubic tweening.
        */
        template <typename T>
        inline T easeInCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInCubic only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction = (T)stage / (T)stageCount;
            return range * stageFraction * stageFraction * stageFraction + startVal;
        }

        /*! @brief Computes an easeOutCubic tweening.
        */
        template <typename T>
        inline T easeOutCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutCubic only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFractionMinus1 = ((T)stage / (T)stageCount) - (T)1.0f;
            return range * (stageFractionMinus1 * stageFractionMinus1 * stageFractionMinus1 + (T)1.0f) + startVal;
        }

        /*! @brief Computes an easeInOutCubic tweening.
        */
        template <typename T>
        inline T easeInOutCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutCubic only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction2 = (T)stage / ((T)stageCount / (T)2.0f);
            if (stageFraction2 < (T)1.0f) {
                return range / (T)2.0f * stageFraction2 * stageFraction2 * stageFraction2 + startVal;
            }
            T stageFraction2Minus2 = stageFraction2 - (T)2.0f;
            return range / (T)2.0f * (stageFraction2Minus2 * stageFraction2Minus2 * stageFraction2Minus2 + (T)2.0f) + startVal;
        }

        /*! @brief Computes an easeInQuart tweening.
        */
        template <typename T>
        inline T easeInQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuart only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction = (T)stage / (T)stageCount;
            return range * stageFraction * stageFraction * stageFraction * stageFraction + startVal;
        }

        /*! @brief Computes an easeOutQuart tweening.
        */
        template <typename T>
        inline T easeOutQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuart only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFractionMinus1 = ((T)stage / (T)stageCount) - (T)1.0f;
            return (T)-1.0f * range * (stageFractionMinus1 * stageFractionMinus1 * stageFractionMinus1 * stageFractionMinus1 - (T)1.0f) + startVal;
        }

        /*! @brief Computes an easeInOutQuart tweening.
        */
        template <typename T>
        inline T easeInOutQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuart only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction2 = (T)stage / ((T)stageCount / (T)2.0f);
            if (stageFraction2 < (T)1.0f) {
                return range / (T)2.0f * stageFraction2 * stageFraction2 * stageFraction2 * stageFraction2 + startVal;
            }
            T stageFraction2Minus2 = stageFraction2 - (T)2.0f;
            return (T)-1.0f * range / (T)2.0f * (stageFraction2Minus2 * stageFraction2Minus2 * stageFraction2Minus2 * stageFraction2Minus2 - (T)2.0f) + startVal;
        }

        /*! @brief Computes an easeInQuint tweening.
        */
        template <typename T>
        inline T easeInQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuint only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction = (T)stage / (T)stageCount;
            return range * stageFraction * stageFraction * stageFraction * stageFraction * stageFraction + startVal;
        }

        /*! @brief Computes an easeOutQuint tweening.
        */
        template <typename T>
        inline T easeOutQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuint only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFractionMinus1 = ((T)stage / (T)stageCount) - (T)1.0f;
            return (T)-1.0f * range * (stageFractionMinus1 * stageFractionMinus1 * stageFractionMinus1 * stageFractionMinus1 - (T)1.0f) + startVal;
        }

        /*! @brief Computes an easeInOutQuint tweening.
        */
        template <typename T>
        inline T easeInOutQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuint only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T stageFraction2 = (T)stage / ((T)stageCount / (T)2.0f);
            if (stageFraction2 < (T)1.0f) {
                return range / (T)2.0f * stageFraction2 * stageFraction2 * stageFraction2 * stageFraction2 + startVal;
            }
            T stageFraction2Minus2 = stageFraction2 - (T)2.0f;
            return (T)-1.0f * range / (T)2.0f * (stageFraction2Minus2 * stageFraction2Minus2 * stageFraction2Minus2 * stageFraction2Minus2 - (T)2.0f) + startVal;
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_TweeningMath_hpp__