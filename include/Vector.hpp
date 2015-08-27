//
// Vector.hpp
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

#ifndef Vorb_Vector_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Vector_hpp__
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
        template <typename U>
        Vector2(const Vector2<U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        T& operator[](int i);
        const T& operator[](int i) const;

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
            UNIONIZE(T x; T y);
            UNIONIZE(T r; T g);
            UNIONIZE(T s; T t);
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
        template <typename U>
        Vector3(const Vector3<U>& v) :
            x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {
        }

        /************************************************************************/
        /* Explicit Conversions                                                 */
        /************************************************************************/
        template<typename A, typename B, typename C>
        explicit Vector3(A a, B b, C c);
        template<typename A, typename B>
        explicit Vector3(const Vector2<A>& a, B b);
        template<typename A, typename B>
        explicit Vector3(A a, const Vector2<B>& b);


        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        T& operator[](int i);
        const T& operator[](int i) const;

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
            UNIONIZE(T x; T y; T z);
            UNIONIZE(T r; T g; T b);
            UNIONIZE(T s; T t; T p);
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
        Vector4(const Vector4<U>& v) :
            x(static_cast<T>(v.x)), y(static_cast<T>(v.y)),
            z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {
        }

        /************************************************************************/
        /* Explicit Conversions                                                 */
        /************************************************************************/
        template<typename A, typename B, typename C, typename D>
        explicit Vector4(A a, B b, C c, D d);
        template<typename A, typename B, typename C>
        explicit Vector4(const Vector2<A>& a, B b, C c);
        template<typename A, typename B, typename C>
        explicit Vector4(A a, const Vector2<B>& b, C c);
        template<typename A, typename B, typename C>
        explicit Vector4(A a, B b, const Vector2<C>& c);
        template<typename A, typename B>
        explicit Vector4(const Vector3<A>& a, B b);
        template<typename A, typename B>
        explicit Vector4(A a, const Vector3<B>& b);
        template<typename A, typename B>
        explicit Vector4(const Vector2<A>& a, const Vector2<B>& b);

        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        T& operator[](int i);
        const T& operator[](int i) const;

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
            UNIONIZE(T x; T y; T z; T w);
            UNIONIZE(T r; T g; T b; T a);
            UNIONIZE(T s; T t; T p; T q);
            T data[4];
        };
    };

#include "Vector.inl"
}

#endif // !Vorb_Vector_hpp__
