//
// FuncPreDecl.hpp
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Sep 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file FuncPreDecl.hpp
* @brief Predeclares common math operations.
*/

#pragma once

#ifndef Vorb_FuncPreDecl_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FuncPreDecl_hpp__
//! @endcondM


namespace vorb {
    namespace math {
        template<typename T> class Vector2;
        template<typename T> class Vector3;
        template<typename T> class Vector4;
        template<typename T> class Matrix2;
        template<typename T> class Matrix3;
        template<typename T> class Matrix4;
        template<typename T> class Quaternion;

        /*! @brief Computes the dot product of two values.
        */
        template <typename T>
        inline T dot(T x, T y);
        /*! @brief Computes the square root of a number.
        */
        template <typename T>
        inline T sqrt(T a);

        template <typename T>
        inline T sin(T a);
        template <typename T>
        inline T cos(T a);
        template <typename T>
        inline T tan(T a);
        template <typename T>
        inline T acos(T a);
        template <typename T>
        inline T asin(T a);
        template <typename T>
        inline T atan(T a);
        template <typename T>
        inline T atan2(T y, T x);

        template <typename T>
        inline T min(T a, T b);
        template <typename T>
        inline T max(T a, T b);
        template <typename T>
        inline T clamp(T a, T minVal, T maxVal);

        template <typename T>
        inline T abs(T a);
        template<>
        inline int32_t abs<int32_t>(int32_t a);
        template<>
        inline int64_t abs<int64_t>(int64_t a);

        template <typename T>
        inline T floor(T a);
        template <typename T>
        inline T floorApprox(T a);

        template <typename T>
        inline T ceil(T a);
        template <typename T>
        inline T ceilApprox(T a);

        template <typename T>
        inline T trunc(T a);

        template <typename T>
        inline T round(T a);
        template <typename T>
        inline T roundApprox(T a);

        template <typename T>
        inline T fract(T a);
        
        template <typename T>
        inline T mod(T a, T b);
        template<>
        inline float mod<float>(float a, float b);

        template<>
        inline double mod<double>(double a, double b);
        
        template <typename T>
        inline T modf(T a, T& b);

        template <typename T>
        inline T sign(T a);

        template <typename T>
        inline T radians(T a);

        template <typename T>
        inline T degrees(T a);

        template <typename T>
        inline T exp(T a);

        template <typename T>
        inline T exp2(T a);

        template <typename T>
        inline T log(T a);

        template <typename T>
        inline T log2(T a);

        /*! @brief Computes the dot product of two vectors.
        */
        template <typename T>
        inline T dot(const Vector2<T>& v1, const Vector2<T>& v2);
        template <typename T>
        inline T dot(const Vector3<T>& v1, const Vector3<T>& v2);
        template <typename T>
        inline T dot(const Vector4<T>& v1, const Vector4<T>& v2);
        /*! @brief Gets the length of a vector.
        */
        template <typename T>
        inline T length(const Vector2<T>& v);
        template <typename T>
        inline T length(const Vector3<T>& v);
        template <typename T>
        inline T length(const Vector4<T>& v);
        /*! @brief Gets the squared length of a vector. Is faster than getting length.
        */
        template <typename T>
        inline T lengthSquared(const Vector2<T>& v);
        template <typename T>
        inline T lengthSquared(const Vector3<T>& v);
        template <typename T>
        inline T lengthSquared(const Vector4<T>& v);
        /*! @brief Computes the cross product of two 3d vectors.
        */
        template <typename T>
        inline Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2);
        /*! @brief Normalizes a vector, turning it into a unit vector.
        */
        template <typename T>
        inline Vector2<T> normalize(const Vector2<T>& v);
        template <typename T>
        inline Vector3<T> normalize(const Vector3<T>& v);
        template <typename T>
        inline Vector4<T> normalize(const Vector4<T>& v);

        // Basic function calls
#define VEC_FCALL(name, func) \
        template <typename T> \
        inline Vector2<T> name(const Vector2<T>& v); \
        template <typename T> \
        inline Vector3<T> name(const Vector3<T>& v); \
        template <typename T> \
        inline Vector4<T> name(const Vector4<T>& v);

        VEC_FCALL(sin, std::sin);
        VEC_FCALL(cos, std::cos);
        VEC_FCALL(tan, std::tan);
        VEC_FCALL(acos, std::acos);
        VEC_FCALL(asin, std::asin);
        VEC_FCALL(atan, std::atan);
        VEC_FCALL(abs, vmath::abs);
        VEC_FCALL(floor, vmath::floor);
        VEC_FCALL(ceil, vmath::ceil);
        VEC_FCALL(trunc, vmath::trunc);
        VEC_FCALL(round, vmath::round);
        VEC_FCALL(fract, vmath::fract);
        VEC_FCALL(sign, vmath::sign);
        VEC_FCALL(radians, vmath::radians);
        VEC_FCALL(degrees, vmath::degrees);
        VEC_FCALL(sqrt, vmath::sqrt);
        VEC_FCALL(exp, vmath::exp);
        VEC_FCALL(exp2, vmath::exp2);
        VEC_FCALL(log, vmath::log);
        VEC_FCALL(log2, vmath::log2);
#undef VEC_FCALL

        template <typename T>
        inline Vector2<T> mod(const Vector2<T>& v, T a);
        template <typename T>
        inline Vector3<T> mod(const Vector3<T>& v, T a);
        template <typename T>
        inline Vector4<T> mod(const Vector4<T>& v, T a);

        template <typename T>
        inline Vector2<T> min(const Vector2<T>& v1, const Vector2<T>& v2);
        template <typename T>
        inline Vector3<T> min(const Vector3<T>& v1, const Vector3<T>& v2);
        template <typename T>
        inline Vector4<T> min(const Vector4<T>& v1, const Vector4<T>& v2);

        template <typename T>
        inline Vector2<T> max(const Vector2<T>& v1, const Vector2<T>& v2);
        template <typename T>
        inline Vector3<T> max(const Vector3<T>& v1, const Vector3<T>& v2);
        template <typename T>
        inline Vector4<T> max(const Vector4<T>& v1, const Vector4<T>& v2);

        template <typename T>
        inline Vector2<T> clamp(const Vector2<T>& v, T minVal, T maxVal);
        template <typename T>
        inline Vector3<T> clamp(const Vector3<T>& v, T minVal, T maxVal);
        template <typename T>
        inline Vector4<T> clamp(const Vector4<T>& v, T minVal, T maxVal);

                template<typename T>
        inline Matrix2<T> inverse(const Matrix2<T>& m);
        template<typename T>
        inline Matrix3<T> inverse(const Matrix3<T>& m);
        template<typename T>
        inline Matrix4<T> inverse(const Matrix4<T>& m);

        template <typename T>
        inline Matrix4<T> lookAt(const Vector3<T>& eye,
                                 const Vector3<T>& center,
                                 const Vector3<T>& up);
        template <typename T>
        inline Matrix4<T> perspective(T yFOV, T aspect, T zNear, T zFar);
        template <typename T>
        inline Matrix4<T> perspectiveFov(T fov, T width, T height, T zNear, T zFar);
        template <typename T>
        inline Matrix4<T> ortho(T left, T right, T bottom, T top);

        template <typename T>
        inline Matrix4<T> translate(const Vector3<T>& v);
        template <typename T>
        inline Matrix4<T> translate(const Matrix4<T>& m, const Vector3<T>& v);

        template <typename T>
        inline Matrix4<T> rotate(T angle, const Vector3<T>& v);
        template <typename T>
        inline Matrix4<T> rotate(const Matrix4<T>& m, T angle, const Vector3<T>& v);

        template<typename T>
        inline Matrix4<T> scale(const Vector3<T>& v);
        template<typename T>
        inline Matrix4<T> scale(const Matrix4<T>& m, const Vector3<T>& v);

        template<typename T>
        inline Matrix2<T> transpose(const Matrix2<T>& m);
        template<typename T>
        inline Matrix3<T> transpose(const Matrix3<T>& m) ;
        template<typename T>
        inline Matrix4<T> transpose(const Matrix4<T>& m);

        template <typename T>
        inline T length(const Quaternion<T>& q);

        template <typename T>
        inline Quaternion<T> normalize(const Quaternion<T>& q);

        template <typename T>
        inline Quaternion<T> cross(const Quaternion<T>& q1, const Quaternion<T>& q2);

        template <typename T>
        inline T dot(const Quaternion<T>& q1, const Quaternion<T>& q2);

        template <typename T>
        inline Quaternion<T> conjugate(const Quaternion<T>& q);

        template <typename T>
        inline Quaternion<T> inverse(const Quaternion<T>& q);

        template<typename T>
        inline Quaternion<T> mix(const Quaternion<T>& x, const Quaternion<T>& y, T angle);

        template<typename T>
        inline Quaternion<T> lerp(const Quaternion<T>& x, const Quaternion<T>& y, T angle);

        template<typename T>
        inline Quaternion<T> slerp(const Quaternion<T>& x, const Quaternion<T>& y, T angle);

        template<typename T>
        inline Quaternion<T> rotate(const Quaternion<T>& q, const T& angle, const Vector3<T>& v);

        template<typename T>
        inline Vector3<T> eulerAngles(const Quaternion<T>& x);

        template<typename T>
        inline T roll(const Quaternion<T>& q);

        template<typename T>
        inline T pitch(const Quaternion<T>& q);

        template<typename T>
        inline T yaw(const Quaternion<T>& q);

        template<typename T>
        inline Matrix3<T> mat3_cast(const Quaternion<T>& q);

        template<typename T>
        inline Matrix4<T> mat4_cast(const Quaternion<T>& q);

        template<typename T>
        inline Quaternion<T> quat_cast(const Matrix3<T>& m);

        template<typename T>
        inline Quaternion<T> quat_cast(const Matrix4<T>& m4);

        template<typename T>
        inline T angle(const Quaternion<T>& x);

        template<typename T>
        inline Vector3<T> axis(const Quaternion<T>& x);

        template<typename T>
        inline Quaternion<T> angleAxis(const T& angle, const Vector3<T>& v);

        template<typename T>
        inline Matrix3<T> toMat3(const Quaternion<T>& q);
        template<typename T>
        inline Matrix4<T> toMat4(const Quaternion<T>& q);
    }
}
namespace vmath = vorb::math;

#endif // !Vorb_FuncPreDecl_hpp__

