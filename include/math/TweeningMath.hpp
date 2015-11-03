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
        // TODO(Matthew): Ensure the checks for alpha being 0.0f or 1.0f in various locations actually fix possible precision issues - and if the possible performance hit is worthwhile.

        /*! @brief Computes a linear tweening.
        */
        template <typename T>
        inline T linear(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "linear only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return startVal + range * alpha;
        }
        template <typename T>
        inline T linear(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return linear(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInQuad(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInQuad(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeOutQuad(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeOutQuad(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInOutQuad(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutQuad(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInCubic(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeOutCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeOutCubic(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutCubic tweening.
        */
        template <typename T>
        inline T easeInOutCubic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutCubic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alpha2 = alpha * (T)2.0f);
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 + (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInOutCubic(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInQuart(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeOutQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeOutQuart(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutQuart tweening.
        */
        template <typename T>
        inline T easeInOutQuart(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuart only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alpha2 = alpha * (T)2.0f);
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return (T)-1.0f * range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 - (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInOutQuart(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInQuint(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeOutQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeOutQuint(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInOutQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInOutQuint(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInSine(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInSine(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeOutSine(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeOutSine(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInOutSine(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInOutSine(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInExpo(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInExpo(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeOutExpo(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeOutExpo(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutExpo tweening.
        */
        template <typename T>
        inline T easeInOutExpo(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutExpo only accepts floating-point inputs.");
            T range = finalVal - startVal;
            if (alpha == (T)0.0f) return startVal;
            if (alpha == (T)1.0f) return finalVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * vmath::pow((T)2.0f, (T)10.0f * (alpha2 - (T)1.0f)) + startVal;
            }
            return range / (T)2.0f * ((T)-1.0f * vmath::pow((T)2.0f, (T)-10.0f * (alpha2 - (T)1.0f) + (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutExpo(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInOutExpo(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInCirc(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInCirc(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutCirc tweening.
        */
        template <typename T>
        inline T easeOutCirc(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutCirc only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alphaMinus1 = alpha - (T)1.0f;
            return range * (vmath::sqrt((T)1.0f - alphaMinus1 * alphaMinus1) + startVal;
        }
        template <typename T>
        inline T easeOutCirc(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeOutCirc(startVal, finalVal, (T)stage / (T)stageCount);
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
        inline T easeInOutCirc(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInOutCirc(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInElastic tweening.
        */
        template <typename T>
        inline T easeInElastic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInElastic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            if (alpha == (T)0.0f) return startVal;
            if (alpha == (T)1.0f) return finalVal;
            alphaMinus1 = alpha - 1;
            return (T)-1.0f * range * vmath::pow((T)2.0f, (T)10.0f * alphaMinus1) * vmath::sin(M_PIF * ((T)20.0f / (T)3.0f * alphaMinus1 - (T)0.5f)) + startVal;
        }
        template <typename T>
        inline T easeInElastic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInElastic(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutElastic tweening.
        */
        template <typename T>
        inline T easeOutElastic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutElastic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            if (alpha == (T)0.0f) return startVal;
            if (alpha == (T)1.0f) return finalVal;
            return range * vmath::pow((T)2.0f, (T)-10.0f * alpha) * vmath::sin(M_PIF * ((T)20.0f / (T)3.0f * alpha - (T)0.5f)) * (T)0.5f + finalVal;
        }
        template <typename T>
        inline T easeOutElastic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutElastic(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutElastic tweening.
        */
        template <typename T>
        inline T easeInOutElastic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutElastic only accepts floating-point inputs.");
            T range = finalVal - startVal;
            if (alpha == (T)0.0f) return startVal;
            if (alpha == (T)1.0f) return finalVal;
            alpha2 = (T)2.0f * alpha;
            alpha2Minus1 = alpha2 - (T)1.0f;
            if (alpha2 < (T)1.0f) {
                return (T)-0.5f * range * vmath::pow((T)2.0f, (T)10.0f * alpha2Minus1) * vmath::sin(M_PIF * ((T)40.0f / (T)9.0f * alpha2Minus1 - (T)0.5f)) + startVal;
            }
            return range * vmath::pow((T)2.0f, (T)-10.0f * alpha2Minus1) * vmath::sin(M_PIF * ((T)40.0f / (T)9.0f * alpha2Minus1 - (T)0.5f)) * (T)0.5f + finalVal;
        }
        template <typename T>
        inline T easeInOutElastic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutElastic(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInBack tweening.
        */
        template <typename T>
        inline T easeInBack(T startVal, T finalVal, T alpha, T s = 1.70158) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInBack only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range * alpha * alpha * ((s + (T)1.0f) * alpha - s) + startVal;
        }
        template <typename T>
        inline T easeInBack(T startVal, T finalVal, unsigned int stageCount, unsigned int stage, T s = 1.70158) {
            if (stageCount == 0) return finalVal;
            return easeInBack(startVal, finalVal, (T)stage / (T)stageCount, s);
        }

        /*! @brief Computes an easeOutBack tweening.
        */
        template <typename T>
        inline T easeOutBack(T startVal, T finalVal, T alpha, T s = 1.70158) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutBack only accepts floating-point inputs.");
            T range = finalVal - startVal;
            T alphaMinus1 = alpha - 1;
            return range * (alphaMinus1 * alphaMinus1 * ((s + (T)1.0f) * alphaMinus1 + s) + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutBack(T startVal, T finalVal, unsigned int stageCount, unsigned int stage, T s = 1.70158) {
            return easeOutBack(startVal, finalVal, (T)stage / (T)stageCount, s);
        }

        /*! @brief Computes an easeInOutBack tweening.
        */
        template <typename T>
        inline T easeInOutBack(T startVal, T finalVal, T alpha, T s = 1.70158) {
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
        inline T easeInOutBack(T startVal, T finalVal, unsigned int stageCount, unsigned int stage, T s = 1.70158) {
            return easeInOutBack(startVal, finalVal, (T)stage / (T)stageCount, s);
        }

        /*! @brief Computes an easeInBounce tweening.
        */
        template <typename T>
        inline T easeInBounce(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInBounce only accepts floating-point inputs.");
            T range = finalVal - startVal;
            return range - easeOutBounce(0, finalVal - startVal, 1 - alpha) + startVal;
        }
        template <typename T>
        inline T easeInBounce(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            if (stageCount == 0) return finalVal;
            return easeInBounce(startVal, finalVal, (T)stage / (T)stageCount);
        }

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
        inline T easeOutBounce(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutBounce(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutBounce tweening.
        */
        template <typename T>
        inline T easeInOutBounce(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutBounce only accepts floating-point inputs.");
            T range = finalVal - startVal;
            if (alpha < (T)0.5f) {
                return easeInBounce(0, finalVal - startVal, (T)2.0f * alpha) * (T)0.5f + startVal;
            }
            return easeOutBounce(0, finalVal - startVal, (T)2.0f * alpha - (T)1.0f) * range * (T)0.25f + startVal;
        }
        template <typename T>
        inline T easeInOutBounce(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutBounce(startVal, finalVal, (T)stage / (T)stageCount);
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_TweeningMath_hpp__