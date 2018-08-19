//
// KegTypes.h
// Vorb Engine
//
// Created by Benjamin Arnold on 19 Aug 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file KegTypes.h
* @brief Defines custom vector types for keg since glm needs alignment.
*/

#pragma once

#ifndef Vorb_KegTypes_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegTypes_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

namespace keg {

    template <typename T, typename GLMCONV>
    struct KegVec2 {
        KegVec2() : x(0), y(0) {}
        KegVec2(const ui8* data) :
            x(static_cast<T>(data[0])),
            y(static_cast<T>(data[1])) {}
        KegVec2(const GLMCONV& v) : x(v.x), y(v.y) {}

        union {
            struct {
                T x;
                T y;
            };
            T data[2];
        };
        T& operator[](int i) {
            return data[i];
        }
        const T& operator[](int i) const {
            return data[i];
        }
        operator GLMCONV() {
            GLMCONV rv;
            rv.x = x;
            rv.y = y;
            return rv;
        }
        KegVec2& operator=(const GLMCONV& rhs) {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }
        KegVec2& operator=(const KegVec2<T, GLMCONV>& rhs) {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }
    };

    template <typename T, typename GLMCONV>
    struct KegVec3 {
        KegVec3() : x(0), y(0), z(0) {}
        KegVec3(const ui8* data) :
            x(static_cast<T>(data[0])),
            y(static_cast<T>(data[1])),
            z(static_cast<T>(data[2])) {}
        KegVec3(const GLMCONV& v) : x(v.x), y(v.y), z(v.z) {}

        union {
            struct {
                T x;
                T y;
                T z;
            };
            T data[3];
        };
        T& operator[](int i) {
            return data[i];
        }
        const T& operator[](int i) const {
            return data[i];
        }
        operator GLMCONV() {
            GLMCONV rv;
            rv.x = x;
            rv.y = y;
            rv.z = z;
            return rv;
        }
        KegVec3& operator=(const GLMCONV& rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }
        KegVec3& operator=(const KegVec3<T, GLMCONV>& rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }
    };

    template <typename T, typename GLMCONV>
    struct KegVec4 {
        KegVec4() : x(0), y(0), z(0), w(0) {}
        KegVec4(const ui8* data) :
            x(static_cast<T>(data[0])),
            y(static_cast<T>(data[1])),
            z(static_cast<T>(data[2])),
            w(static_cast<T>(data[3])) {}
        KegVec4(const GLMCONV& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

        union {
            struct {
                T x;
                T y;
                T z;
                T w;
            };
            T data[4];
        };
        T& operator[](int i) {
            return data[i];
        }
        const T& operator[](int i) const {
            return data[i];
        }
        operator GLMCONV() {
            GLMCONV rv;
            rv.x = x;
            rv.y = y;
            rv.z = z;
            rv.w = w;
            return rv;
        }
        KegVec4& operator=(const GLMCONV& rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
            return *this;
        }
        KegVec4& operator=(const KegVec4<T, GLMCONV>& rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
            return *this;
        }
    };

    typedef KegVec2<i8, i8v2> kegi8v2;
    typedef KegVec3<i8, i8v3> kegi8v3;
    typedef KegVec4<i8, i8v4> kegi8v4;
    typedef KegVec2<ui8, ui8v2> kegui8v2;
    typedef KegVec3<ui8, ui8v3> kegui8v3;
    typedef KegVec4<ui8, ui8v4> kegui8v4;

    typedef KegVec2<i16, i16v2> kegi16v2;
    typedef KegVec3<i16, i16v3> kegi16v3;
    typedef KegVec4<i16, i16v4> kegi16v4;
    typedef KegVec2<ui16, ui16v2> kegui16v2;
    typedef KegVec3<ui16, ui16v3> kegui16v3;
    typedef KegVec4<ui16, ui16v4> kegui16v4;

    typedef KegVec2<i32, i32v2> kegi32v2;
    typedef KegVec3<i32, i32v3> kegi32v3;
    typedef KegVec4<i32, i32v4> kegi32v4;
    typedef KegVec2<ui32, ui32v2> kegui32v2;
    typedef KegVec3<ui32, ui32v3> kegui32v3;
    typedef KegVec4<ui32, ui32v4> kegui32v4;

    typedef KegVec2<i64, i64v2> kegi64v2;
    typedef KegVec3<i64, i64v3> kegi64v3;
    typedef KegVec4<i64, i64v4> kegi64v4;
    typedef KegVec2<ui64, ui64v2> kegui64v2;
    typedef KegVec3<ui64, ui64v3> kegui64v3;
    typedef KegVec4<ui64, ui64v4> kegui64v4;

    typedef KegVec2<f32, f32v2> kegf32v2;
    typedef KegVec3<f32, f32v3> kegf32v3;
    typedef KegVec4<f32, f32v4> kegf32v4;
    typedef KegVec2<f64, f64v2> kegf64v2;
    typedef KegVec3<f64, f64v3> kegf64v3;
    typedef KegVec4<f64, f64v4> kegf64v4;
}

#endif // !Vorb_KegTypes_h__
