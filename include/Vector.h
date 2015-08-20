//
// Vector.h
// Vorb Engine
//
// Created by Benjamin Arnold on 20 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Vector.h
* @brief Defines custom Vorb vector types.
*/

#pragma once

#ifndef Vorb_Vector_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Vector_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    template <typename T>
    class Vector2 {
    public:
        /************************************************************************/
        /* Constructors                                                         */
        /************************************************************************/
        Vector2() : x(0), y(0) {}
        Vector2(T a) : x(a), y(a) {}
        Vector2(T x, T y) : x(x), y(y) {}
        Vector2(const ui8* data) :
            x(static_cast<T>(data[0])),
            y(static_cast<T>(data[1])) {}
        template <typename U>
        Vector2(const glm::tvec2<U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        T& operator[](int i);
        const T& operator[](int i) const;

        operator glm::tvec2<T>();

        template<typename U>
        Vector2& operator=(const glm::tvec2<U>& rhs);
        template<typename U>
        Vector2& operator=(const Vector2<U>& rhs);

        bool operator==(const Vector2<T>& rhs) const;
        bool operator!=(const Vector2<T>& rhs) const;

        template<typename U>
        Vector2& operator+=(U a);
        template<typename U>
        Vector2& operator+=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator-=(U a);
        template<typename U>
        Vector2& operator-=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator*=(U a);
        template<typename U>
        Vector2& operator*=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator/=(U a);
        template<typename U>
        Vector2& operator/=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator%=(U a);
        template<typename U>
        Vector2& operator%=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator&=(U a);
        template<typename U>
        Vector2& operator&=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator|=(U a);
        template<typename U>
        Vector2& operator|=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator^=(U a);
        template<typename U>
        Vector2& operator^=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator<<=(U a);
        template<typename U>
        Vector2& operator<<=(const Vector2<U>& v);

        template<typename U>
        Vector2& operator>>=(U a);
        template<typename U>
        Vector2& operator>>=(const Vector2<U>& v);

        /************************************************************************/
        /* Member Variables                                                     */
        /************************************************************************/
        union {
            struct { T x; T y; };
            struct { T r; T g; };
            struct { T s; T t; };
            T data[2];
        };
    };

    template <typename T>
    class Vector3 {
    public:
        /************************************************************************/
        /* Constructors                                                         */
        /************************************************************************/
        Vector3() : x(0), y(0), z(0) {}
        Vector3(T a) : x(a), y(a), z(a) {}
        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
        Vector3(const ui8* data) :
            x(static_cast<T>(data[0])),
            y(static_cast<T>(data[1])),
            z(static_cast<T>(data[2])) {}
        template <typename U>
        Vector3(const glm::tvec3<U>& v) :
            x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}

        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        T& operator[](int i);
        const T& operator[](int i) const;

        operator glm::tvec3<T>();

        template<typename U>
        Vector3& operator=(const glm::tvec3<U>& rhs);
        template<typename U>
        Vector3& operator=(const Vector3<U>& rhs);

        bool operator==(const Vector3<T>& rhs) const;
        bool operator!=(const Vector3<T>& rhs) const;

        template<typename U>
        Vector3& operator+=(U a);
        template<typename U>
        Vector3& operator+=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator-=(U a);
        template<typename U>
        Vector3& operator-=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator*=(U a);
        template<typename U>
        Vector3& operator*=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator/=(U a);
        template<typename U>
        Vector3& operator/=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator%=(U a);
        template<typename U>
        Vector3& operator%=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator&=(U a);
        template<typename U>
        Vector3& operator&=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator|=(U a);
        template<typename U>
        Vector3& operator|=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator^=(U a);
        template<typename U>
        Vector3& operator^=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator<<=(U a);
        template<typename U>
        Vector3& operator<<=(const Vector3<U>& v);

        template<typename U>
        Vector3& operator>>=(U a);
        template<typename U>
        Vector3& operator>>=(const Vector3<U>& v);

        /************************************************************************/
        /* Member Variables                                                     */
        /************************************************************************/
        union {
            struct { T x; T y; T z; };
            struct { T r; T g; T b; };
            struct { T s; T t; T u; };
            T data[3];
        };
    };

    template <typename T>
    class Vector4 {
    public:
        /************************************************************************/
        /* Constructors                                                         */
        /************************************************************************/
        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(T a) : x(a), y(a), z(a), w(a) {}
        Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        template <typename U>
        Vector4(const glm::tvec4<U>& v) :
            x(static_cast<T>(v.x)), y(static_cast<T>(v.y)),
            z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {}

        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        T& operator[](int i);
        const T& operator[](int i) const;

        operator glm::tvec4<T>();

        template<typename U>
        Vector4& operator=(const glm::tvec4<U>& rhs);
        template<typename U>
        Vector4& operator=(const Vector4<U>& rhs);

        bool operator==(const Vector4<T>& rhs) const;
        bool operator!=(const Vector4<T>& rhs) const;

        template<typename U>
        Vector4& operator+=(U a);
        template<typename U>
        Vector4& operator+=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator-=(U a);
        template<typename U>
        Vector4& operator-=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator*=(U a);
        template<typename U>
        Vector4& operator*=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator/=(U a);
        template<typename U>
        Vector4& operator/=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator%=(U a);
        template<typename U>
        Vector4& operator%=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator&=(U a);
        template<typename U>
        Vector4& operator&=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator|=(U a);
        template<typename U>
        Vector4& operator|=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator^=(U a);
        template<typename U>
        Vector4& operator^=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator<<=(U a);
        template<typename U>
        Vector4& operator<<=(const Vector4<U>& v);

        template<typename U>
        Vector4& operator>>=(U a);
        template<typename U>
        Vector4& operator>>=(const Vector4<U>& v);

        /************************************************************************/
        /* Member Variables                                                     */
        /************************************************************************/
        union {
            struct { T x; T y; T z; T w; };
            struct { T r; T g; T b; T a; };
            struct { T s; T t; T u; T v; };
            T data[4];
        };
    };

#include "Vector.inl"
}


#endif // !Vorb_Vector_h__
