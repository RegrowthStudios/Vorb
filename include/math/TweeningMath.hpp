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
            return minVal + range * alpha;
        }
        template <typename T>
        inline T linear(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return linear(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInQuad tweening.
        */
        template <typename T>
        inline T easeInQuad(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuad only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return range * alpha * alpha + startVal;
        }
        template <typename T>
        inline T easeInQuad(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInQuad(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutQuad tweening.
        */
        template <typename T>
        inline T easeOutQuad(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuad only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return (T)-1.0f * range * alpha * (alpha - (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeOutQuad(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutQuad(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutQuad tweening.
        */
        template <typename T>
        inline T easeInOutQuad(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuad only accepts floating-point inputs.");
            T range = maxVal - minVal;
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
            T range = maxVal - minVal;
            return range * alpha * alpha * alpha + startVal;
        }
        template <typename T>
        inline T easeInCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInCubic(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutCubic tweening.
        */
        template <typename T>
        inline T easeOutCubic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutCubic only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alphaMinus1 = alpha -(T)1.0f;
            return range * (alphaMinus1 * alphaMinus1 * alphaMinus1 + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutCubic(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutCubic tweening.
        */
        template <typename T>
        inline T easeInOutCubic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutCubic only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alpha2 = alpha * (T)2.0f);
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 + (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutCubic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutCubic(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInQuart tweening.
        */
        template <typename T>
        inline T easeInQuart(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuart only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return range * alpha * alpha * alpha * alpha + startVal;
        }
        template <typename T>
        inline T easeInQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInQuart(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutQuart tweening.
        */
        template <typename T>
        inline T easeOutQuart(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuart only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alphaMinus1 = alpha - (T)1.0f;
            return (T)-1.0f * range * (alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 - (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutQuart(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutQuart tweening.
        */
        template <typename T>
        inline T easeInOutQuart(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuart only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alpha2 = alpha * (T)2.0f);
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return (T)-1.0f * range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 - (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutQuart(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutQuart(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInQuint tweening.
        */
        template <typename T>
        inline T easeInQuint(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInQuint only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return range * alpha * alpha * alpha * alpha * alpha + startVal;
        }
        template <typename T>
        inline T easeInQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInQuint(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutQuint tweening.
        */
        template <typename T>
        inline T easeOutQuint(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutQuint only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alphaMinus1 = alpha - (T)1.0f;
            return range * (alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 * alphaMinus1 + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutQuint(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutQuint tweening.
        */
        template <typename T>
        inline T easeInOutQuint(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutQuint only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return range / (T)2.0f * alpha2 * alpha2 * alpha2 * alpha2 * alpha2 + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 * alpha2Minus2 + (T)2.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutQuint(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutQuint(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInSine tweening.
        */
        template <typename T>
        inline T easeInSine(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInSine only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return (T)-1.0f * range * vmath::cos(alpha * (T)M_PI_2F) + range + startVal;
        }
        template <typename T>
        inline T easeInSine(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInSine(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutSine tweening.
        */
        template <typename T>
        inline T easeOutSine(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutSine only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return range * vmath::sin(alpha * (T)M_PI_2F) + startVal;
        }
        template <typename T>
        inline T easeOutSine(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutSine(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutSine tweening.
        */
        template <typename T>
        inline T easeInOutSine(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutSine only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return (T)-1.0f * range / (T)2.0f * (vmath::cos(alpha * M_PIF) - (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutSine(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutSine(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInExpo tweening.
        */
        template <typename T>
        inline T easeInExpo(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInExpo only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return (alpha == (T)0.0f) ? startVal : range * vmath::pow((T)2.0f, (T)10.0f * (alpha - (T)1.0f)) + startVal;
        }
        template <typename T>
        inline T easeInExpo(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInExpo(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutExpo tweening.
        */
        template <typename T>
        inline T easeOutExpo(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutExpo only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return (alpha == (T)1.0f) ? maxVal : range * ((T)-1.0f * vmath::pow((T)2.0f, (T)-10.0f * alpha) + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeOutExpo(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutExpo(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutExpo tweening.
        */
        template <typename T>
        inline T easeInOutExpo(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutExpo only accepts floating-point inputs.");
            T range = maxVal - minVal;
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
            return easeInOutExpo(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInCirc tweening.
        */
        template <typename T>
        inline T easeInCirc(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInCirc only accepts floating-point inputs.");
            T range = maxVal - minVal;
            return (T)-1.0f * range * (vmath::sqrt((T)1.0f - alpha * alpha) - (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeInCirc(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInCirc(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeOutCirc tweening.
        */
        template <typename T>
        inline T easeOutCirc(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutCirc only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alphaMinus1 = alpha - (T)1.0f;
            return range * (vmath::sqrt((T)1.0f - alphaMinus1 * alphaMinus1) + startVal;
        }
        template <typename T>
        inline T easeOutCirc(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeOutCirc(startVal, finalVal, (T)stage / (T)stageCount);
        }

        /*! @brief Computes an easeInOutCirc tweening.
        */
        template <typename T>
        inline T easeInOutCirc(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInOutCirc only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return (T)-1.0f * range / (T)2.0f * (vmath::sqrt((T)1.0f - alpha2 * alpha2) - (T)1.0f) + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (vmath::sqrt((T)1.0f - alpha2Minus2 * alpha2Minus2) + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutCirc(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutCirc(startVal, finalVal, (T)stage / (T)stageCount);
        }

        //////////////////////////////////////////////////////////////////////////
        //                      UNDERGOING CONSTRUCTION                        //
        /////////////////////////////////////////////////////////////////////////
        // TODO(Matthew): Complete this...

        /*! @brief Computes an easeInElastic tweening.
        */
        template <typename T>
        inline T easeInElastic(T startVal, T finalVal, T alpha, unsigned int stageCount) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeInElastic only accepts floating-point inputs.");
            static T s = (T)1.70158f;
            T p = (T)0.0f;
            T range = maxVal - minVal;
            T a = range;
            if (alpha == (T)0.0f) return startVal;
            if (alpha == (T)1.0f) return finalVal;
            if (!p) p = (T)stageCount * (T)0.3f; //HERE
        }
        template <typename T>
        inline T easeInElastic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInElastic(startVal, finalVal, (T)stage / (T)stageCount, stageCount);
        }

        /*! @brief Computes an easeOutElastic tweening.
        */
        template <typename T>
        inline T easeOutElastic(T startVal, T finalVal, T alpha) {
            static_assert(std::numeric_limits<T>::is_iec559, "easeOutElastic only accepts floating-point inputs.");
            T range = maxVal - minVal;
            T alphaMinus1 = alpha - (T)1.0f;
            return range * (vmath::sqrt((T)1.0f - alphaMinus1 * alphaMinus1) + startVal;
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
            T range = maxVal - minVal;
            T alpha2 = alpha * (T)2.0f;
            if (alpha2 < (T)1.0f) {
                return (T)-1.0f * range / (T)2.0f * (vmath::sqrt((T)1.0f - alpha2 * alpha2) - (T)1.0f) + startVal;
            }
            T alpha2Minus2 = alpha2 - (T)2.0f;
            return range / (T)2.0f * (vmath::sqrt((T)1.0f - alpha2Minus2 * alpha2Minus2) + (T)1.0f) + startVal;
        }
        template <typename T>
        inline T easeInOutElastic(T startVal, T finalVal, unsigned int stageCount, unsigned int stage) {
            return easeInOutElastic(startVal, finalVal, (T)stage / (T)stageCount);
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_TweeningMath_hpp__