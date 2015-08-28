//
// QuaternionMath.hpp
// Vorb Engine
//
// Created by Benjamin Arnold on 26 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file QuaternionMath.hpp
* @brief Common quaternion mathematical functions.
*/

#pragma once

#ifndef Vorb_QuaternionMath_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_QuaternionMath_hpp__
//! @endcond

namespace vorb {
    namespace math {
        template <typename T>
        inline T length(const Quaternion<T>& q) {
            return vmath::sqrt(vmath::dot(q, q));
        }

        template <typename T>
        inline Quaternion<T> normalize(const Quaternion<T>& q) {
            T len = length(q);
            if (len <= T(0)) return Quaternion<T>(0, 0, 0, 1);
            T oneOverLen = static_cast<T>(1) / len;
            return Quaternion<T>(q.x * oneOverLen, q.y * oneOverLen, q.z * oneOverLen, q.w * oneOverLen);
        }

        template <typename T>
        inline Quaternion<T> cross(const Quaternion<T>& q1, const Quaternion<T>& q2) {
            return Quaternion<T>(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
                                 q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
                                 q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
                                 q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
        }

        template <typename T>
        inline T dot(const Quaternion<T>& q1, const Quaternion<T>& q2) {
            return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
        }

        template <typename T>
        inline Quaternion<T> conjugate(const Quaternion<T>& q) {
            return Quaternion<T>(-q.x, -q.y, -q.z, q.w);
        }

        template <typename T>
        inline Quaternion<T> inverse(const Quaternion<T>& q) {
            return conjugate(q) / dot(q, q);
        }

        template<typename T>
        inline Quaternion<T> mix(const Quaternion<T>& x, const Quaternion<T>& y, T angle) {
            T cosTheta = dot(x, y);

            // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
            if (cosTheta > static_cast<T>(1) - epsilon<T>()) {
                // Linear interpolation
                return Quaternion<T>(mix(x.x, y.x, angle),
                                     mix(x.y, y.y, angle),
                                     mix(x.z, y.z, angle),
                                     mix(x.w, y.w, angle));
            } else {
                // Essential Mathematics, page 467
                T theta = acos(cosTheta);
                return (sin((static_cast<T>(1) - angle) * theta) * x + sin(angle * theta) * y) / sin(theta);
            }
        }

        template<typename T>
        inline Quaternion<T> lerp(const Quaternion<T>& x, const Quaternion<T>& y, T angle) {
            // Lerp is only defined in [0, 1]
            assert(angle >= static_cast<T>(0));
            assert(angle <= static_cast<T>(1));

            return x * (static_cast<T>(1) - angle) + (y * angle);
        }

        template<typename T>
        inline Quaternion<T> slerp(const Quaternion<T>& x, const Quaternion<T>& y, T angle) {
            Quaternion<T> z = y;

            T cosTheta = dot(x, y);

            // If cosTheta < 0, the interpolation will take the long way around the sphere. 
            // To fix this, one quat must be negated.
            if (cosTheta < static_cast<T>(0)) {
                z = -y;
                cosTheta = -cosTheta;
            }

            // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
            if (cosTheta > static_cast<T>(1) - epsilon<T>()) {
                // Linear interpolation
                return Quaternion<T>(mix(x.x, z.x, angle),
                                     mix(x.y, z.y, angle),
                                     mix(x.z, z.z, angle),
                                     mix(x.w, z.w, angle));
            } else {
                // Essential Mathematics, page 467
                T angle = acos(cosTheta);
                return (sin((static_cast<T>(1) - angle) * angle) * x + sin(angle * angle) * z) / sin(angle);
            }
        }

        template<typename T>
        inline Quaternion<T> rotate(const Quaternion<T>& q, const T& angle, const Vector3<T>& v) {
            Vector3<T> tmp = v;

            // Axis of rotation must be normalized
            T len = vmath::length(tmp);
            if (abs(len - static_cast<T>(1)) > static_cast<T>(0.001)) {
                T oneOverLen = static_cast<T>(1) / len;
                tmp.x *= oneOverLen;
                tmp.y *= oneOverLen;
                tmp.z *= oneOverLen;
            }

            const T s = sin(angle * static_cast<T>(0.5));

            return q * Quaternion<T>(tmp.x * s, tmp.y * s, tmp.z * s, cos(angle * static_cast<T>(0.5)));
        }

        template<typename T>
        inline Vector3<T> eulerAngles(const Quaternion<T>& x) {
            return Vector3<T>(pitch(x), yaw(x), roll(x));
        }

        template<typename T>
        inline T roll(const Quaternion<T>& q) {
            return T(atan2(static_cast<T>(2) * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z));
        }

        template<typename T>
        inline T pitch(const Quaternion<T>& q) {
            return T(atan2(static_cast<T>(2) * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z));
        }

        template<typename T>
        inline T yaw(const Quaternion<T>& q) {
            return asin(static_cast<T>(-2) * (q.x * q.z - q.w * q.y));
        }

        template<typename T>
        inline Matrix3<T> mat3_cast(const Quaternion<T>& q) {
            Matrix3<T> rv(static_cast<T>(1));
            T qxx(q.x * q.x);
            T qyy(q.y * q.y);
            T qzz(q.z * q.z);
            T qxz(q.x * q.z);
            T qxy(q.x * q.y);
            T qyz(q.y * q.z);
            T qwx(q.w * q.x);
            T qwy(q.w * q.y);
            T qwz(q.w * q.z);

            rv[0][0] = 1 - 2 * (qyy + qzz);
            rv[0][1] = 2 * (qxy + qwz);
            rv[0][2] = 2 * (qxz - qwy);

            rv[1][0] = 2 * (qxy - qwz);
            rv[1][1] = 1 - 2 * (qxx + qzz);
            rv[1][2] = 2 * (qyz + qwx);

            rv[2][0] = 2 * (qxz + qwy);
            rv[2][1] = 2 * (qyz - qwx);
            rv[2][2] = 1 - 2 * (qxx + qyy);
            return rv;
        }

        template<typename T>
        inline Matrix4<T> mat4_cast(const Quaternion<T>& q) {
            return Matrix4<T>(mat3_cast(q));
        }

        template<typename T>
        inline Quaternion<T> quat_cast(const Matrix3<T>& m) {
            T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
            T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
            T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
            T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

            int biggestIndex = 0;
            T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
            if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
                fourBiggestSquaredMinus1 = fourXSquaredMinus1;
                biggestIndex = 1;
            }
            if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
                fourBiggestSquaredMinus1 = fourYSquaredMinus1;
                biggestIndex = 2;
            }
            if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
                fourBiggestSquaredMinus1 = fourZSquaredMinus1;
                biggestIndex = 3;
            }

            T biggestVal = sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1)) * static_cast<T>(0.5);
            T mult = static_cast<T>(0.25) / biggestVal;

            Quaternion<T> rv(0);
            switch (biggestIndex) {
                case 0:
                    rv.w = biggestVal;
                    rv.x = (m[1][2] - m[2][1]) * mult;
                    rv.y = (m[2][0] - m[0][2]) * mult;
                    rv.z = (m[0][1] - m[1][0]) * mult;
                    break;
                case 1:
                    rv.w = (m[1][2] - m[2][1]) * mult;
                    rv.x = biggestVal;
                    rv.y = (m[0][1] + m[1][0]) * mult;
                    rv.z = (m[2][0] + m[0][2]) * mult;
                    break;
                case 2:
                    rv.w = (m[2][0] - m[0][2]) * mult;
                    rv.x = (m[0][1] + m[1][0]) * mult;
                    rv.y = biggestVal;
                    rv.z = (m[1][2] + m[2][1]) * mult;
                    break;
                case 3:
                    rv.w = (m[0][1] - m[1][0]) * mult;
                    rv.x = (m[2][0] + m[0][2]) * mult;
                    rv.y = (m[1][2] + m[2][1]) * mult;
                    rv.z = biggestVal;
                    break;

                default:					// Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
                    assert(false);
                    break;
            }
            return rv;
        }

        template<typename T>
        inline Quaternion<T> quat_cast(const Matrix4<T>& m4) {
            return quat_cast(Matrix3<T>(m4));
        }

        template<typename T>
        inline T angle(const Quaternion<T>& x) {
            return acos(x.w) * static_cast<T>(2);
        }

        template<typename T>
        inline Vector3<T> axis(const Quaternion<T>& x) {
            T tmp1 = static_cast<T>(1) - x.w * x.w;
            if (tmp1 <= static_cast<T>(0))
                return Vector3<T>(0, 0, 1);
            T tmp2 = static_cast<T>(1) / sqrt(tmp1);
            return Vector3<T>(x.x * tmp2, x.y * tmp2, x.z * tmp2);
        }

        template<typename T>
        inline Quaternion<T> angleAxis(const T& angle, const Vector3<T>& v) {
            const T s = vmath::sin(angle * static_cast<T>(0.5));
            return Quaternion<T>(v.x * s,
                                 v.y * s,
                                 v.z * s,
                                 vmath::cos(angle * static_cast<T>(0.5)));
        }

        template<typename T>
        inline Matrix3<T> toMat3(const Quaternion<T>& q) {
            return vmath::mat3_cast(q);
        }

        template<typename T>
        inline Matrix4<T> toMat4(const Quaternion<T>& q) {
            return vmath::mat4_cast(q);
        }
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_QuaternionMath_hpp__
