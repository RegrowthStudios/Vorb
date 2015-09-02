//
// Quaternion.hpp
// Vorb Engine
//
// Created by Benjamin Arnold on 21 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Quaternion.hpp
* @brief Implements Quaternion for rotation math.
*/

#pragma once

#ifndef Vorb_Quaternion_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Quaternion_hpp__
//! @endcond

#include "Vector.hpp"

namespace vorb {
    template <typename T>
    class Quaternion {
    public:
        /************************************************************************/
        /* Constructors                                                         */
        /************************************************************************/
        Quaternion() : x(0), y(0), z(0), w(1) {}
        Quaternion(const vorb::Vector3<T>& euler);
        template<typename U>
        Quaternion(Quaternion<U> q) :
            x(static_cast<T>(q.x)),
            y(static_cast<T>(q.y)),
            z(static_cast<T>(q.z)),
            w(static_cast<T>(q.w)) {}
        Quaternion(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {};

        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        T& operator[](int i);
        const T& operator[](int i) const;

        template<typename U>
        Quaternion& operator=(const Quaternion<U>& rhs);

        bool operator==(const Quaternion<T>& rhs) const;
        bool operator!=(const Quaternion<T>& rhs) const;

        template<typename U>
        Quaternion& operator+=(const Quaternion<U>& q);

        template<typename U>
        Quaternion& operator-=(const Quaternion<U>& q);

        template<typename U>
        Quaternion& operator*=(U a);
        template<typename U>
        Quaternion& operator*=(const Quaternion<U>& q);

        template<typename U>
        Quaternion& operator/=(U a);

        union {
            UNIONIZE(T x; T y; T z; T w);
            T data[4];
        };
    };
#include "Quaternion.inl"
}

#endif // !Vorb_Quaternion_hpp__
