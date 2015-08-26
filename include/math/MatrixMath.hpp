//
// MatrixMath.hpp
// Vorb Engine
//
// Created by Benjamin Arnold on 25 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file MatrixMath.hpp
* @brief Defines common matrix math operations.
*/

#pragma once

#ifndef Vorb_MatrixMath_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MatrixMath_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

// TODO(Ben): Functions are right handed only.
namespace vorb {
    namespace math {
        template<typename T>
        inline Matrix2<T> inverse(const Matrix2<T>& m) {
            // Inverse of determinant
            const T invDet = static_cast<T>(1) / (
                m[0][0] * m[1][1] - m[1][0] * m[0][1]);

            return Matrix2<T>(m[1][1] * invDet,
                              -m[0][1] * invDet,
                              -m[1][0] * invDet,
                              m[0][0] * invDet);
        }
        template<typename T>
        inline Matrix3<T> inverse(const Matrix3<T>& m) {
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
        inline Matrix4<T> inverse(const Matrix4<T>& m) {
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

            const Vector4<T> signA(1, -1, 1, -1);
            const Vector4<T> signB(-1, 1, -1, 1);
            Matrix4<T> invm(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

            Vector4<T> r0(invm[0][0], invm[1][0], invm[2][0], invm[3][0]);

            Vector4<T> dot0(m[0] * r0);
            T dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

            T invDet = static_cast<T>(1) / dot1;

            return invm * invDet;
        }

        template <typename T>
        inline Matrix4<T> lookAt(const Vector3<T>& eye,
                                 const Vector3<T>& center,
                                 const Vector3<T>& up) {
            const Vector3<T> f(normalize(center - eye));
            const Vector3<T> s(normalize(cross(f, up)));
            const Vector3<T> u(cross(s, f));

            Matrix4<T> rv(1);
            rv[0][0] = s.x;
            rv[1][0] = s.y;
            rv[2][0] = s.z;
            rv[0][1] = u.x;
            rv[1][1] = u.y;
            rv[2][1] = u.z;
            rv[0][2] = -f.x;
            rv[1][2] = -f.y;
            rv[2][2] = -f.z;
            rv[3][0] = -dot(s, eye);
            rv[3][1] = -dot(u, eye);
            rv[3][2] = dot(f, eye);
            return rv;
        }
        template <typename T>
        inline Matrix4<T> perspective(T yFOV, T aspect, T zNear, T zFar) {
            assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));
            T const tanHalfYFOV = tan(yFOV / static_cast<T>(2));
            Matrix4<T> rv(static_cast<T>(0));
            rv[0][0] = static_cast<T>(1) / (aspect * tanHalfYFOV);
            rv[1][1] = static_cast<T>(1) / (tanHalfYFOV);
            rv[2][2] = -(zFar + zNear) / (zFar - zNear);
            rv[2][3] = -static_cast<T>(1);
            rv[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
            return rv;
        }
        template <typename T>
        inline Matrix4<T> perspectiveFov(T fov, T width, T height, T zNear, T zFar) {
            assert(width > static_cast<T>(0));
            assert(height > static_cast<T>(0));
            assert(fov > static_cast<T>(0));

            const T rad = fov;
            const T h = vmath::cos(static_cast<T>(0.5) * rad) / vmath::sin(static_cast<T>(0.5) * rad);
            const T w = h * height / width; // TODO(Ben): max(width , Height) / min(width , Height)?

            Matrix4<T> rv(static_cast<T>(0));
            rv[0][0] = w;
            rv[1][1] = h;
            rv[2][2] = -(zFar + zNear) / (zFar - zNear);
            rv[2][3] = -static_cast<T>(1);
            rv[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
            return rv;
        }
        template <typename T>
        inline Matrix4<T> ortho(T left, T right, T bottom, T top) {
            Matrix4<T> rv(static_cast<T>(1));
            rv[0][0] = static_cast<T>(2) / (right - left);
            rv[1][1] = static_cast<T>(2) / (top - bottom);
            rv[2][2] = -static_cast<T>(1);
            rv[3][0] = -(right + left) / (right - left);
            rv[3][1] = -(top + bottom) / (top - bottom);
            return rv;
        }

        template <typename T>
        inline Matrix4<T> translate(const Vector3<T>& v) {
            return translate(Matrix4<T>(static_cast<T>(1)), v);
        }
        template <typename T>
        inline Matrix4<T> translate(const Matrix4<T>& m, const Vector3<T>& v) {
            Matrix4<T> rv(m);
            rv[3] = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
            return rv;
        }

        template <typename T>
        inline Matrix4<T> rotate(const Matrix4<T>& m, T angle, const Vector3<T>& v) {
            const T a = angle;
            const T c = cos(a);
            const T s = sin(a);

            const Vector3<T> axis(normalize(v));
            const Vector3<T> temp((T(1) - c) * axis);

            Matrix3<T> rotate(temp[0] * axis[0] + c,
                              temp[0] * axis[1] + s * axis[2],
                              temp[0] * axis[2] - s * axis[1],
                              temp[1] * axis[0] - s * axis[2],
                              temp[1] * axis[1] + c,
                              temp[1] * axis[2] + s * axis[0],
                              temp[2] * axis[0] + s * axis[1],
                              temp[2] * axis[1] - s * axis[0],
                              temp[2] * axis[2] + c);

            return Matrix4<T>(m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2],
                              m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2],
                              m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2],
                              m[3]);
        }

        template<typename T>
        inline Matrix4<T> scale(const Matrix4<T>& m, const Vector3<T>& v) {
            return Matrix4<T>(m[0] * v[0],
                              m[1] * v[1],
                              m[2] * v[2],
                              m[3]);
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_MatrixMath_hpp__
