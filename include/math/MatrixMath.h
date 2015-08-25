//
// MatrixMath.h
// Vorb Engine
//
// Created by Benjamin Arnold on 25 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file MatrixMath.h
* @brief Defines common matrix math operations.
*/

#pragma once

#ifndef Vorb_MatrixMath_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MatrixMath_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace math {
        template<typename T>
        inline Matrix2<T> computeInverse(const Matrix2<T>& m) {
            // Inverse of determinant
            const T invDet = static_cast<T>(1) / (
                m[0][0] * m[1][1] - m[1][0] * m[0][1]);

            return Matrix2<T>(m[1][1] * invDet,
                             -m[0][1] * invDet,
                             -m[1][0] * invDet,
                              m[0][0] * invDet);
        }
        template<typename T>
        inline Matrix3<T> computeInverse(const Matrix3<T>& m) {
            // Inverse of determinant
            const T invDet = static_cast<T>(1) / (
                m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
                m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) +
                m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

            return Matrix3<T>((m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invDet,
                                -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * invDet,
                                 (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invDet,
                                -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * invDet,
                                 (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * invDet,
                                -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * invDet,
                                 (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * invDet,
                                -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * invDet,
                                 (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet);
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_MatrixMath_h__
