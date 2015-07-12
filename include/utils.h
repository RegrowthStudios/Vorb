///
/// utils.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 7 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Simple utility functions
///

#pragma once

#ifndef utils_h__
#define utils_h__

// Used for excluding complex utilities
#ifdef UTIL_SIMPLE
#define NO_UTIL_RADIX
#define NO_UTIL_BUFFER
#define NO_UTIL_INTERSECTION
#endif

/************************************************************************/
/* Inlined Utility Files                                                */
/************************************************************************/
#include "FastConversion.inl"
#ifndef NO_UTIL_RADIX
#include "Radix.inl"
#endif
#ifndef NO_UTIL_BUFFER
#include "BufferUtils.inl"
#endif
#ifndef NO_UTIL_INTERSECTION
#include "IntersectionUtils.inl"
#endif

// Inlined math functions
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(a) (((a) < 0) ?(-(a)):(a))
#define INTERPOLATE(a, min, max) (((max)-(min))*(a)+(min))

/************************************************************************/
/* Type Ranges                                                          */
/************************************************************************/
template<typename T> struct Range { T min; T max; };
#define RANGE_STRUCT(NAME, TYPE) typedef Range<TYPE> NAME##Range
RANGE_STRUCT(I8, i8);
RANGE_STRUCT(I16, i16);
RANGE_STRUCT(I32, i32);
RANGE_STRUCT(I64, i64);
RANGE_STRUCT(UI8, ui8);
RANGE_STRUCT(UI16, ui16);
RANGE_STRUCT(UI32, ui32);
RANGE_STRUCT(UI64, ui64);
RANGE_STRUCT(F32, f32);
RANGE_STRUCT(F64, f64);

/************************************************************************/
/* String Utilites                                                      */
/************************************************************************/
inline void convertWToMBString(const cwString ws, nString& resultString) {
    size_t l = wcslen(ws);
    resultString.resize(l + 1);
    size_t numConverted = 0;
    #if defined(__APPLE__) || defined(__linux__)
    wcstombs(&(resultString[0]), ws, numConverted);
    #elif defined(WIN32) || defined(WIN64)
    wcstombs_s(&numConverted, &(resultString[0]), l + 1, ws, l);
    #endif   // win32

    resultString[l] = '\0';
}
inline const cwString convertMBToWString(const cString s) {
    size_t l = strlen(s);
    cwString resultString = new wchar_t[l + 1];
    size_t numConverted = 0;
#if defined(__APPLE__) || defined(__linux__)
    wcstombs((char *)&(resultString[0]), (const wchar_t *)s, numConverted);
#elif defined(WIN32) || defined(WIN64)
    mbstowcs(resultString, s, l);
#endif   // win32

    resultString[l] = '\0';
    return resultString;
}

/************************************************************************/
/* Interpolation Utilities                                              */
/************************************************************************/
template<typename T, typename R>
inline T lerp(const T& v0, const T& v1, const R& r) {
    return (T)((v1 - v0) * r + v0);
}
template<typename T, typename R>
inline T bilerp(const T& v00, const T& v01, const T& v10, const T& v11, const R& r1, const R& r2) {
    T v0 = lerp(v00, v01, r2);
    T v1 = lerp(v10, v11, r2);
    return lerp(v0, v1, r1);
}
template<typename T, typename R>
inline T trilerp(const T& v000, const T& v001, const T& v010, const T& v011, const T& v100, const T& v101, const T& v110, const T& v111, const R& r1, const R& r2, const R& r3) {
    T v0 = bilerp(v000, v001, v010, v011, r2, r3);
    T v1 = bilerp(v100, v101, v110, v111, r2, r3);
    return lerp(v0, v1, r1);
}
template<size_t D, typename T, size_t N>
inline T upSampleArray(size_t i, T (&data)[N]) {
    f64 r = (f64)(i % D) / (f64)D;
    i /= D;
    return lerp(data[i], data[i + 1], r);
}
template<size_t D1, size_t D2, typename T, size_t N1, size_t N2>
inline T upSampleArray(size_t i1, size_t i2, T(&data)[N1][N2]) {
    f64 r1 = (f64)(i1 % D1) / (f64)D1;
    f64 r2 = (f64)(i2 % D2) / (f64)D2;
    i1 /= D1;
    i2 /= D2;
    return bilerp(
        data[i1][i2],
        data[i1][i2 + 1],
        data[i1 + 1][i2],
        data[i1 + 1][i2 + 1],
        r1, r2
        );
}
template<size_t D1, size_t D2, size_t D3, typename T, size_t N1, size_t N2, size_t N3>
inline T upSampleArray(size_t i1, size_t i2, size_t i3, T (&data)[N1][N2][N3]) {
    f64 r1 = (f64)(i1 % D1) / (f64)D1;
    f64 r2 = (f64)(i2 % D2) / (f64)D2;
    f64 r3 = (f64)(i3 % D3) / (f64)D3;
    i1 /= D1;
    i2 /= D2;
    i3 /= D3;
    return trilerp(
        data[i1][i2][i3],
        data[i1][i2][i3 + 1],
        data[i1][i2 + 1][i3],
        data[i1][i2 + 1][i3 + 1],
        data[i1 + 1][i2][i3],
        data[i1 + 1][i2][i3 + 1],
        data[i1 + 1][i2 + 1][i3],
        data[i1 + 1][i2 + 1][i3 + 1],
        r1, r2, r3
        );
}

/// Simple hermite interpolater for smoothing the range 0-1
template<typename T>
inline T hermite(T v) { return static_cast<T>(3.0) * (v * v) - static_cast<T>(2.0) * (v * v * v); }

/************************************************************************/
/* Quaternion Utilities                                                 */
/************************************************************************/
/// Finds the shortest arc rotation quat between two directions
/// @param v1: Starting direction
/// @param v2: End direction
/// @pre: v1 and v2 are normalized
/// @pre: v1 != -v2
inline f64q quatBetweenVectors(const f64v3& v1, const f64v3& v2) {
    f64q q;
    f64v3 a = glm::cross(v1, v2);
    q.x = a.x;
    q.y = a.y;
    q.z = a.z;
    q.w = 1.0 + glm::dot(v1, v2);
    return glm::normalize(q);
}
/// Finds the shortest arc rotation quat between two directions
/// @param v1: Starting direction
/// @param v2: End direction
/// @pre: v1 and v2 are normalized
/// @pre: v1 != -v2
inline f32q quatBetweenVectors(const f32v3& v1, const f32v3& v2) {
    f32q q;
    f32v3 a = glm::cross(v1, v2);
    q.x = a.x;
    q.y = a.y;
    q.z = a.z;
    f32 l1 = glm::length(v1);
    f32 l2 = glm::length(v2);
    q.w = sqrt((l1 * l1) * (l2 * l2)) + glm::dot(v1, v2);
    return glm::normalize(q);
}
/************************************************************************/
/* Clip utilities                                                       */
/************************************************************************/
/*! @brief Computes clipping of a rect.
 * 
 * @param clipRect: The clipping rectangle
 * @param position: The rectangle position to check. Will be clipped. 
 * @param position: The rectangle size to check. Will be clipped.
 * @param uvRect: The rectangle UVs. Will be clipped.
 * @return true if it clipped.
 */
inline bool computeClipping(const f32v4& clipRect, f32v2& position, f32v2& size, f32v4& uvRect) {
    bool rv = false;
    if (position.x < clipRect.x) {
        f32 t = clipRect.x - position.x;
        uvRect.x += uvRect.z * (t / size.x);
        uvRect.z *= 1.0f - (t / size.x);
        position.x = clipRect.x;
        size.x -= t;
        rv = true;
    }
    if (position.x + size.x > clipRect.x + clipRect.z) {
        f32 t = position.x + size.x - (clipRect.x + clipRect.z);
        uvRect.z *= 1.0f - (t / size.x);
        size.x -= t;
        rv = true;
    }
    if (position.y < clipRect.y) {
        f32 t = clipRect.y - position.y;
        uvRect.y += uvRect.w * (t / size.y);
        uvRect.w *= 1.0f - (t / size.y);
        position.y = clipRect.y;
        size.y -= t;
        rv = true;
    }
    if (position.y + size.y > clipRect.y + clipRect.w) {
        f32 t = position.y + size.y - (clipRect.y + clipRect.w);
        uvRect.w *= 1.0f - (t / size.y);
        size.y -= t;
        rv = true;
    }
    return rv;
}
inline bool computeClipping(const f32v4& clipRect, f32v2& position, f32v2& size) {
    bool rv = false;
    if (position.x < clipRect.x) {
        f32 t = clipRect.x - position.x;
        position.x = clipRect.x;
        size.x -= t;
        rv = true;
    }
    if (position.x + size.x > clipRect.x + clipRect.z) {
        f32 t = position.x + size.x - (clipRect.x + clipRect.z);
        size.x -= t;
        rv = true;
    }
    if (position.y < clipRect.y) {
        f32 t = clipRect.y - position.y;
        position.y = clipRect.y;
        size.y -= t;
        rv = true;
    }
    if (position.y + size.y > clipRect.y + clipRect.w) {
        f32 t = position.y + size.y - (clipRect.y + clipRect.w);
        size.y -= t;
        rv = true;
    }
    return rv;
}
inline bool computeClipping(f32 clipPos, f32 clipWidth, f32& position, f32& width) {
    bool rv = false;
    if (position < clipPos) {
        f32 t = clipPos - position;
        position = clipPos;
        width -= t;
        rv = true;
    }
    if (position + width > clipPos + clipWidth) {
        f32 t = position + width - (clipPos + clipWidth);
        width -= t;
        rv = true;
    }
    return rv;
}
/************************************************************************/
/* Hash functions                                                       */
/************************************************************************/
template <>
struct std::hash<i32v3> {
    size_t operator()(const i32v3& k) const {
        std::hash<i32> h;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:
        return ((h(k.x) ^ (h(k.y) << 1)) >> 1) ^ (h(k.z) << 1);
    }
};
template <>
struct std::hash<i32v2> {
    size_t operator()(const i32v2& k) const {
        std::hash<i32> h;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:
        return ((h(k.x) ^ (h(k.y) << 1)) >> 1);
    }
};
template <>
struct std::hash<ui32v3> {
    size_t operator()(const ui32v3& k) const {
        std::hash<ui32> h;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:
        return ((h(k.x) ^ (h(k.y) << 1)) >> 1) ^ (h(k.z) << 1);
    }
};
template <>
struct std::hash<ui32v2> {
    size_t operator()(const ui32v2& k) const {
        std::hash<ui32> h;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:
        return ((h(k.x) ^ (h(k.y) << 1)) >> 1);
    }
};

#endif // utils_h__
