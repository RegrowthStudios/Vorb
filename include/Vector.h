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

#include "Vector.inl"
}


#endif // !Vorb_Vector_h__
