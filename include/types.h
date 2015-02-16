//
// types.h
// Vorb Engine
//
// Created by Cristian Zaloj on 16 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file types.h
 * @brief A header containing Vorb's specially defined types.
 */

#pragma once

#ifndef Vorb_types_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_types_h__
//! @endcond

#include <cstdint>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "compat.h"

// Use the correct glm private namespace
#if defined(OS_WINDOWS)
namespace glm_d = glm::detail;
#else
namespace glm_d = glm;
#endif

/************************************************************************/
/* Integer values                                                       */
/************************************************************************/
typedef int8_t i8; ///< 8-bit signed integer
typedef int8_t sbyte; ///< 8-bit signed integer
typedef int16_t i16; ///< 16-bit signed integer
typedef int32_t i32; ///< 32-bit signed integer
typedef int64_t i64; ///< 64-bit signed integer
typedef uint8_t ui8; ///< 8-bit unsigned integer
typedef uint8_t ubyte; ///< 8-bit unsigned integer
typedef uint16_t ui16; ///< 16-bit unsigned integer
typedef uint32_t ui32; ///< 32-bit unsigned integer
typedef uint64_t ui64; ///< 64-bit unsigned integer

/************************************************************************/
/* Floating point values                                                */
/************************************************************************/
typedef float f32; ///< 32-bit floating point value (single)
typedef double f64; ///< 64-bit floating point value (double)

/************************************************************************/
/* GLM types                                                            */
/************************************************************************/
/*! @brief 8-bit signed integer 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm_d::tvec2<i8> i8v2;
/*! @brief 16-bit signed integer 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm::lowp_ivec2 i16v2;
/*! @brief 32-bit signed integer 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm::mediump_ivec2 i32v2;
/*! @brief 64-bit signed integer 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm::highp_ivec2 i64v2;
/*! @brief 8-bit unsigned integer 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm_d::tvec2<ui8> ui8v2;
/*! @brief 16-bit unsigned integer 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm::lowp_uvec2 ui16v2;
/*! @brief 32-bit unsigned integer 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm::mediump_uvec2 ui32v2;
/*! @brief 64-bit unsigned integer 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm::highp_uvec2 ui64v2;
/*! @brief 32-bit floating point (single) 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm::mediump_vec2 f32v2;
/*! @brief 64-bit floating point (double) 2-value vector.
 * 
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1]
 *  x  ,  y
 *  r  ,  g
 *  s  ,  t
 * </pre>
 */
typedef glm::highp_vec2 f64v2;
/*! @brief 8-bit signed integer 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm_d::tvec3<i8> i8v3;
/*! @brief 16-bit signed integer 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm::lowp_ivec3 i16v3;
/*! @brief 32-bit signed integer 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm::mediump_ivec3 i32v3;
/*! @brief 64-bit signed integer 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm::highp_ivec3 i64v3;
/*! @brief 8-bit unsigned integer 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm_d::tvec3<ui8> ui8v3;
/*! @brief 16-bit unsigned integer 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm::lowp_uvec3 ui16v3;
/*! @brief 32-bit unsigned integer 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm::mediump_uvec3 ui32v3;
/*! @brief 64-bit unsigned integer 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm::highp_uvec3 ui64v3;
/*! @brief 32-bit floating point (single) 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm::mediump_vec3 f32v3;
/*! @brief 64-bit floating point (double) 3-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2]
 *  x  ,  y  ,  z
 *  r  ,  g  ,  b
 *  s  ,  t  ,  p
 * </pre>
 */
typedef glm::highp_vec3 f64v3;
/*! @brief 8-bit signed integer 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm_d::tvec4<i8> i8v4;
/*! @brief 16-bit signed integer 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm::lowp_ivec4 i16v4;
/*! @brief 32-bit signed integer 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm::mediump_ivec4 i32v4;
/*! @brief 64-bit signed integer 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm::highp_ivec4 i64v4;
/*! @brief 8-bit unsigned integer 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm_d::tvec4<ui8> ui8v4;
/*! @brief 16-bit unsigned integer 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm::lowp_uvec4 ui16v4;
/*! @brief 32-bit unsigned integer 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm::mediump_uvec4 ui32v4;
/*! @brief 64-bit unsigned integer 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm::highp_uvec4 ui64v4;
/*! @brief 32-bit floating point (single) 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm::mediump_vec4 f32v4;
/*! @brief 64-bit floating point (double) 4-value vector.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 *  r  ,  g  ,  b  ,  a
 *  s  ,  t  ,  p  ,  q
 * </pre>
 */
typedef glm::highp_vec4 f64v4;
/*! @brief 32-bit floating point (single) quaternion.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 * </pre>
 */
typedef glm::mediump_quat f32q;
/*! @brief 64-bit floating point (double) quaternion.
 *
 * Elements can be accessed via: @n
 * <pre>
 * [0] , [1] , [2] , [3]
 *  x  ,  y  ,  z  ,  w
 * </pre>
 */
typedef glm::highp_quat f64q;
/*! @brief 32-bit floating point (single) 2x2 matrix.
 *
 * Elements are stored in column major order and column vector notation.
 * 
 * Given axes (X, Y), the matrix: @n
 * <pre>
 * M = 
 * [ X.x , Y.x ]
 * [ X.y , Y.y ]
 * </pre>
 * is accessed in the following fashion: @n
 * <pre>
 * M[0][0] = X.x
 * M[0][1] = X.y
 * M[1][0] = Y.x
 * M[1][1] = Y.y
 * </pre>
 */
typedef glm::mediump_mat2 f32m2;
/*! @brief 64-bit floating point (double) 2x2 matrix.
 *
 * Elements are stored in column major order and column vector notation.
 *
 * Given axes (X, Y), the matrix: @n
 * <pre>
 * M =
 * [ X.x , Y.x ]
 * [ X.y , Y.y ]
 * </pre>
 * is accessed in the following fashion: @n
 * <pre>
 * M[0][0] = X.x
 * M[0][1] = X.y
 * M[1][0] = Y.x
 * M[1][1] = Y.y
 * </pre>
 */
typedef glm::highp_mat2 f64m2;
/*! @brief 32-bit floating point (single) 3x3 matrix.
 *
 * Elements are stored in column major order and column vector notation.
 *
 * Given axes (X, Y, Z), the matrix: @n
 * <pre>
 * M =
 * [ X.x , Y.x , Z.x ]
 * [ X.y , Y.y , Z.y ]
 * [ X.z , Y.z , Z.z ]
 * </pre>
 * is accessed in the following fashion: @n
 * <pre>
 * M[0][0] = X.x
 * M[0][1] = X.y
 * M[0][2] = X.z
 * M[1][0] = Y.x
 * M[1][1] = Y.y
 * M[1][2] = Y.z
 * M[2][0] = Z.x
 * M[2][1] = Z.y
 * M[2][2] = Z.z
 * </pre>
 */
typedef glm::mediump_mat3 f32m3;
/*! @brief 64-bit floating point (double) 3x3 matrix.
 *
 * Elements are stored in column major order and column vector notation.
 *
 * Given axes (X, Y, Z), the matrix: @n
 * <pre>
 * M =
 * [ X.x , Y.x , Z.x ]
 * [ X.y , Y.y , Z.y ]
 * [ X.z , Y.z , Z.z ]
 * </pre>
 * is accessed in the following fashion: @n
 * <pre>
 * M[0][0] = X.x
 * M[0][1] = X.y
 * M[0][2] = X.z
 * M[1][0] = Y.x
 * M[1][1] = Y.y
 * M[1][2] = Y.z
 * M[2][0] = Z.x
 * M[2][1] = Z.y
 * M[2][2] = Z.z
 * </pre>
 */
typedef glm::highp_mat3 f64m3;
/*! @brief 32-bit floating point (single) 4x4 matrix.
 *
 * Elements are stored in column major order and column vector notation.
 *
 * Given axes (X, Y, Z) and translation T, the matrix: @n
 * <pre>
 * M =
 * [ X.x , Y.x , Z.x , T.x ]
 * [ X.y , Y.y , Z.y , T.y ]
 * [ X.z , Y.z , Z.z , T.z ]
 * [  0  ,  0  ,  0  ,  1  ]
 * </pre>
 * is accessed in the following fashion: @n
 * <pre>
 * M[0][0] = X.x
 * M[0][1] = X.y
 * M[0][2] = X.z
 * M[0][3] = 0
 * M[1][0] = Y.x
 * M[1][1] = Y.y
 * M[1][2] = Y.z
 * M[1][3] = 0
 * M[2][0] = Z.x
 * M[2][1] = Z.y
 * M[2][2] = Z.z
 * M[2][3] = 0
 * M[3][0] = T.x
 * M[3][1] = T.y
 * M[3][2] = T.z
 * M[3][3] = 1
 * </pre>
 */
typedef glm::mediump_mat4 f32m4;
/*! @brief 64-bit floating point (double) 4x4 matrix.
 *
 * Elements are stored in column major order and column vector notation.
 *
 * Given axes (X, Y, Z) and translation T, the matrix: @n
 * <pre>
 * M =
 * [ X.x , Y.x , Z.x , T.x ]
 * [ X.y , Y.y , Z.y , T.y ]
 * [ X.z , Y.z , Z.z , T.z ]
 * [  0  ,  0  ,  0  ,  1  ]
 * </pre>
 * is accessed in the following fashion: @n
 * <pre>
 * M[0][0] = X.x
 * M[0][1] = X.y
 * M[0][2] = X.z
 * M[0][3] = 0
 * M[1][0] = Y.x
 * M[1][1] = Y.y
 * M[1][2] = Y.z
 * M[1][3] = 0
 * M[2][0] = Z.x
 * M[2][1] = Z.y
 * M[2][2] = Z.z
 * M[2][3] = 0
 * M[3][0] = T.x
 * M[3][1] = T.y
 * M[3][2] = T.z
 * M[3][3] = 1
 * </pre>
 */
typedef glm::highp_mat4 f64m4;

// Colors
#include "typesColor.inl"

/*! @brief Implements a binary operator for an enum class.
 * 
 * The operator performs its operation using an operator that is defined for
 * a different proxy type. The enum value is first converted to the proxy type,
 * the operation is performed, and then the proxy result is converted back to
 * the enum type.
 * 
 * @param CLASS: The type of the enum class
 * @param PROXY_TYPE: The proxy type used for back-and-forth conversion on the enum values
 * @param OP: The operator that will be defined for this enum: (&, |, ^, +, -, *, /)
 */
#define ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, OP) \
    inline CLASS operator OP (const CLASS& a, const CLASS& b) { return (CLASS)static_cast<PROXY_TYPE>((PROXY_TYPE)a OP(PROXY_TYPE)b); } \
    inline CLASS& operator OP## = (CLASS& a, const CLASS& b) { a = a OP b; return a; }
/*! @brief Implements a binary operators (&, |, ^, +, -) and the shift operators for an enum class.
 * 
 * @param CLASS: The type of the enum class
 * @param PROXY_TYPE: The proxy type used for back-and-forth conversion on the enum values
 */
#define ENUM_CLASS_OPS_INL(CLASS, PROXY_TYPE) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, &) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, | ) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, ^) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, +) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, -) \
    inline CLASS operator << (const CLASS& a, const size_t& b) { return (CLASS)((PROXY_TYPE)a << b); } \
    inline CLASS& operator <<= (CLASS& a, const size_t& b) { a = a << b; return a; } \
    inline CLASS operator >> (const CLASS& a, const size_t& b) { return (CLASS)((PROXY_TYPE)a >> b); } \
    inline CLASS& operator >>= (CLASS& a, const size_t& b) { a = a >> b; return a; }
/*! \example "Enum Class Proxy Operators"
 * 
 * When using enum classes, it may be useful to
 * allow operators to take effect on them. By default,
 * enum classes have no defined operators, but with a useful
 * macro and a proxy type that has the desired operators defined,
 * creating operator definitions for enum classes is a cinch:
 * \include VorbEnumOps.cpp
 */

// Array
#include "typesArray.inl"

/*! @brief Type of a C-style string
 */
#define cString char*
/*! @brief Type of a C-style string made of wide characters
 */
#define cwString wchar_t*
/*! @brief Type of a C++-style string
 */
#define nString std::string

/*! @brief Pointer offset
 * 
 * Converts the integral offset value of a member into a pointer representation
 */
#define offsetptr(s, m) ((void*)offsetof(s, m))

#endif // !Vorb_types_h__
