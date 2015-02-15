///
/// types.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 14 Feb 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef Vorb_types_h__
#define Vorb_types_h__

#include <cstdint>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "compat.h"

#if defined(OS_WINDOWS)
namespace glm_d = glm::detail;
#else
namespace glm_d = glm;
#endif

/************************************************************************/
/* Integer values                                                       */
/************************************************************************/
typedef int8_t i8;
typedef int8_t sbyte;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t ui8;
typedef uint8_t ubyte;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

/************************************************************************/
/* Floating point values                                                */
/************************************************************************/
typedef float f32;
typedef double f64;

/************************************************************************/
/* GLM types                                                            */
/************************************************************************/
// GLM Vec2 integer values
typedef glm_d::tvec2<i8> i8v2;
typedef glm::lowp_ivec2 i16v2;
typedef glm::mediump_ivec2 i32v2;
typedef glm::highp_ivec2 i64v2;
typedef glm_d::tvec2<ui8> ui8v2;
typedef glm::lowp_uvec2 ui16v2;
typedef glm::mediump_uvec2 ui32v2;
typedef glm::highp_uvec2 ui64v2;
// GLM Vec2 floating point values
typedef glm::mediump_vec2 f32v2;
typedef glm::highp_vec2 f64v2;
// GLM Vec3 integer values
typedef glm_d::tvec3<i8> i8v3;
typedef glm::lowp_ivec3 i16v3;
typedef glm::mediump_ivec3 i32v3;
typedef glm::highp_ivec3 i64v3;
typedef glm_d::tvec3<ui8> ui8v3;
typedef glm::lowp_uvec3 ui16v3;
typedef glm::mediump_uvec3 ui32v3;
typedef glm::highp_uvec3 ui64v3;
// GLM Vec3 floating point values
typedef glm::mediump_vec3 f32v3;
typedef glm::highp_vec3 f64v3;
// GLM Vec4 integer values
typedef glm_d::tvec4<i8> i8v4;
typedef glm::lowp_ivec4 i16v4;
typedef glm::mediump_ivec4 i32v4;
typedef glm::highp_ivec4 i64v4;
typedef glm_d::tvec4<ui8> ui8v4;
typedef glm::lowp_uvec4 ui16v4;
typedef glm::mediump_uvec4 ui32v4;
typedef glm::highp_uvec4 ui64v4;
// GLM Vec4 floating point values
typedef glm::mediump_vec4 f32v4;
typedef glm::highp_vec4 f64v4;

// GLM floating point quaternions
typedef glm::mediump_quat f32q;
typedef glm::highp_quat f64q;

// GLM floating point matrices
typedef glm::mediump_mat2 f32m2;
typedef glm::highp_mat2 f64m2;
typedef glm::mediump_mat3 f32m3;
typedef glm::highp_mat3 f64m3;
typedef glm::mediump_mat4 f32m4;
typedef glm::highp_mat4 f64m4;

// Colors
#include "typesColor.inl"

/************************************************************************/
/* Enum class ops                                                       */
/************************************************************************/
#define ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, OP) \
    inline CLASS operator OP (const CLASS& a, const CLASS& b) { return (CLASS)((PROXY_TYPE)a OP (PROXY_TYPE)b); } \
    inline CLASS& operator OP## = (CLASS& a, const CLASS& b) { a = a OP b; return a; }
#define ENUM_CLASS_OPS_INL(CLASS, PROXY_TYPE) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, &) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, | ) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, ^) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, +) \
    ENUM_CLASS_OP_INL(CLASS, PROXY_TYPE, -) \
    inline CLASS operator << (const CLASS& a, const size_t& b) { return (CLASS)((PROXY_TYPE)a << b); } \
    inline CLASS operator >> (const CLASS& a, const size_t& b) { return (CLASS)((PROXY_TYPE)a >> b); }

// Array
#include "typesArray.inl"

// String
#define cString char*
#define cwString wchar_t*
#define nString std::string

/// Pointer offset
#define offsetptr(s, m) ((void*)offsetof(s, m))

#endif // Vorb_types_h__
