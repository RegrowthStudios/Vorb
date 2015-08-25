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
        template<typename T>
        inline Matrix4<T> computeInverse(const Matrix4<T>& m) {
            // Coefficients
            T c00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            T c02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
            T c03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

            T c04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            T c06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
            T c07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

            T c08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            T c10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
            T c11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

            T c12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            T c14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
            T c15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

            T c16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            T c18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
            T c19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

            T c20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            T c22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
            T c23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

            Vector4<T> fac0(c00, c00, c02, c03);
            Vector4<T> fac1(c04, c04, c06, c07);
            Vector4<T> fac2(c08, c08, c10, c11);
            Vector4<T> fac3(c12, c12, c14, c15);
            Vector4<T> fac4(c16, c16, c18, c19);
            Vector4<T> fac5(c20, c20, c22, c23);

            Vector4<T> v0(m[1][0], m[0][0], m[0][0], m[0][0]);
            Vector4<T> v1(m[1][1], m[0][1], m[0][1], m[0][1]);
            Vector4<T> v2(m[1][2], m[0][2], m[0][2], m[0][2]);
            Vector4<T> v3(m[1][3], m[0][3], m[0][3], m[0][3]);

            Vector4<T> inv0(v1 * fac0 - v2 * fac1 + v3 * fac2);
            Vector4<T> inv1(v0 * fac0 - v2 * fac3 + v3 * fac4);
            Vector4<T> inv2(v0 * fac1 - v1 * fac3 + v3 * fac5);
            Vector4<T> inv3(v0 * fac2 - v1 * fac4 + v2 * fac5);

            const Vector4<T> signA( 1, -1,  1, -1);
            const Vector4<T> signB(-1, 1, -1, 1);
            Matrix4<T> inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

            Vector4<T> r0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

            Vector4<T> dot0(m[0] * r0);
            T dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

            T invDet = static_cast<T>(1) / dot1;

            return inverse * invDet;
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_MatrixMath_h__
