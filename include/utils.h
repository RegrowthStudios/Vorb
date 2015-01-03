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
    i32 l = wcslen(ws);
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
    i32 l = strlen(s);
    cwString resultString = new wchar_t[l + 1];
    size_t numConverted = 0;
#if defined(__APPLE__) || defined(__linux__)
    wcstombs(&(resultString[0]), ws, numConverted);
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

#endif // utils_h__