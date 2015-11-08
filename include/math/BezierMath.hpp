//
// BezierMath.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 02 Nov 2015
// Copyright 2015 Regrowth Studios
// All Rights Reserved
//

/*! \file BezierMath.hpp
* @brief Defines the maths needed for constructing bezier curves of nth order.
*/

#pragma once

#include "VorbMath.hpp"

#ifndef Vorb_TweeningMath_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TweeningMath_hpp__
//! @endcond

namespace vorb {
    namespace math {
        /*! @brief Computes a bezier point for a set of 3D control points.
        */
        template <typename T>
        inline Vector3<T> bezier3d(T alpha, std::vector<Vector3<T>> controlPoints) {
            ui16 n = controlPoints.size();
            T u = (T)1.0 - alpha;

            Vector3<T> b = Vector3<T>(0, 0, 0);
            for (ui16 i = 0; i < n; ++i) {
                T bc = vmath::binomial<T>(n, i);
                T up = vmath::pow(u, n - i);
                T ap = vmath::pow(alpha, i);
                b += (controlPoints[i] * bc * up * ap);
            }
        }
        /*! @brief Computes a bezier point for a set of 2D control points.
        */
        template <typename T>
        inline Vector2<T> bezier2d(T alpha, std::vector<Vector2<T>> controlPoints) {
            ui16 n = controlPoints.size();
            T u = (T)1.0 - alpha;

            Vector2<T> b = Vector2<T>(0, 0);
            for (ui16 i = 0; i < n; ++i) {
                T bc = vmath::binomial<T>(n, i);
                T up = vmath::pow(u, n - i);
                T ap = vmath::pow(alpha, i);
                b += (controlPoints[i] * bc * up * ap);
            }
        }
        /*! @brief Computes a bezier point for a set of 1D control points.
        */
        template <typename T>
        inline T bezier1d(T alpha, std::vector<T> controlPoints) {
            ui16 n = controlPoints.size();
            T u = (T)1.0 - alpha;

            T b = (T)0.0;
            for (ui16 i = 0; i < n; ++i) {
                T bc = vmath::binomial<T>(n, i);
                T up = vmath::pow(u, n - i);
                T ap = vmath::pow(alpha, i);
                b += (controlPoints[i] * bc * up * ap);
            }
        }
    }
}