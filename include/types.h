#pragma once

#ifndef types_h__
#define types_h__

#include <cstdint>
#include <memory>
#include <string>

#ifdef TYPES_GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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

#ifdef TYPES_GLM

/************************************************************************/
/* GLM types                                                            */
/************************************************************************/
#ifdef glm_core_type_gentype2
// GLM Vec2 integer values
typedef glm::detail::tvec2<i8> i8v2;
typedef glm::lowp_ivec2 i16v2;
typedef glm::mediump_ivec2 i32v2;
typedef glm::highp_ivec2 i64v2;
typedef glm::detail::tvec2<ui8> ui8v2;
typedef glm::lowp_uvec2 ui16v2;
typedef glm::mediump_uvec2 ui32v2;
typedef glm::highp_uvec2 ui64v2;
// GLM Vec2 floating point values
typedef glm::mediump_vec2 f32v2;
typedef glm::highp_vec2 f64v2;
#endif
#ifdef glm_core_type_gentype3
// GLM Vec3 integer values
typedef glm::detail::tvec3<i8> i8v3;
typedef glm::lowp_ivec3 i16v3;
typedef glm::mediump_ivec3 i32v3;
typedef glm::highp_ivec3 i64v3;
typedef glm::detail::tvec3<ui8> ui8v3;
typedef glm::lowp_uvec3 ui16v3;
typedef glm::mediump_uvec3 ui32v3;
typedef glm::highp_uvec3 ui64v3;
// GLM Vec3 floating point values
typedef glm::mediump_vec3 f32v3;
typedef glm::highp_vec3 f64v3;
#endif
#ifdef glm_core_type_gentype4
// GLM Vec4 integer values
typedef glm::detail::tvec4<i8> i8v4;
typedef glm::lowp_ivec4 i16v4;
typedef glm::mediump_ivec4 i32v4;
typedef glm::highp_ivec4 i64v4;
typedef glm::detail::tvec4<ui8> ui8v4;
typedef glm::lowp_uvec4 ui16v4;
typedef glm::mediump_uvec4 ui32v4;
typedef glm::highp_uvec4 ui64v4;
// GLM Vec4 floating point values
typedef glm::mediump_vec4 f32v4;
typedef glm::highp_vec4 f64v4;
#endif

#ifdef GLM_GTC_quaternion
// GLM floating point quaternions
typedef glm::mediump_quat f32q;
typedef glm::highp_quat f64q;
#endif

// GLM floating point matrices
#ifdef glm_core_type_mat2x2
typedef glm::mediump_mat2 f32m2;
typedef glm::highp_mat2 f64m2;
#endif
#ifdef glm_core_type_mat3x3
typedef glm::mediump_mat3 f32m3;
typedef glm::highp_mat3 f64m3;
#endif
#ifdef glm_core_type_mat4x4
typedef glm::mediump_mat4 f32m4;
typedef glm::highp_mat4 f64m4;
#endif

#endif

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

#endif // types_h__
