//
// Matrix.h
// Vorb Engine
//
// Created by Benjamin Arnold on 21 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Matrix.h
* @brief 
*/

#pragma once

#ifndef Vorb_Matrix_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Matrix_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "Vector.h"

namespace vorb {
    template <typename T>
    class Matrix2 {
    public:
        typedef Vector2<T> RowType;
        typedef Vector2<T> ColType;

        /************************************************************************/
        /* Constructors                                                         */
        /************************************************************************/
        Matrix2() : c0(1, 0), c1(0, 1) {}
        Matrix2(const Matrix2& m) : c0(m[0]), c1(m[1]) {}
        Matrix2(const T& a) : c0(a, 0), c1(0, a) {}
        Matrix2(const T& x0, const T& y0,
                const T& x1, const T& y1) :
            c0(x0, y0), c1(x1, y1) {}
        Matrix2(const ColType& c0, const ColType& c1) : c0(c0), c1(c1) {}
        Matrix2(const glm::tmat2x2<T>& m) : c0(m[0]), c1(m[1]) {}

        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        ColType& operator[](int i);
        const ColType& operator[](int i) const;

        operator glm::tmat2x2<T>() const;


        template<typename U>
        Matrix2& operator=(const glm::tmat2x2<U>& rhs);
        template<typename U>
        Matrix2& operator=(const Matrix2<U>& rhs);

        bool operator==(const Matrix2<T>& rhs) const;
        bool operator!=(const Matrix2<T>& rhs) const;

        template<typename U>
        Matrix2& operator+=(U a);
        template<typename U>
        Matrix2& operator+=(const Matrix2<U>& v);

        template<typename U>
        Matrix2& operator-=(U a);
        template<typename U>
        Matrix2& operator-=(const Matrix2<U>& v);

        template<typename U>
        Matrix2& operator*=(U a);
        template<typename U>
        Matrix2& operator*=(const Matrix2<U>& v);

        template<typename U>
        Matrix2& operator/=(U a);
        template<typename U>
        Matrix2& operator/=(const Matrix2<U>& v);

    private:
        union {
            UNIONIZE(ColType data[2]);
            UNIONIZE(ColType c0; ColType c1);
        };
    };

    template <typename T>
    class Matrix3 {
    public:
        typedef Vector3<T> RowType;
        typedef Vector3<T> ColType;

        /************************************************************************/
        /* Constructors                                                         */
        /************************************************************************/
        Matrix3() : c0(1, 0, 0), c1(0, 1, 0), c2(0, 0, 1) {}
        Matrix3(const Matrix3& m) : c0(m[0]), c1(m[1]), c2(m[2]) {}
        Matrix3(const T& a) : c0(a, 0, 0), c1(0, a, 0), c2(0, 0, a) {}
        Matrix3(const T& x0, const T& y0, const T& z0,
                const T& x1, const T& y1, const T& z1,
                const T& x2, const T& y2, const T& z2) :
                c0(x0, y0, z0), c1(x1, y1, z1), c2(x2, y2, z2) {}
        Matrix3(const ColType& c0, const ColType& c1, const ColType& c2) :
                c0(c0), c1(c1), c2(c2) {}
        Matrix3(const glm::tmat3x3<T>& m) : c0(m[0]), c1(m[1]), c2(m[2]) {}

        /************************************************************************/
        /* Operators                                                            */
        /************************************************************************/
        ColType& operator[](int i);
        const ColType& operator[](int i) const;

        operator glm::tmat3x3<T>() const;


        template<typename U>
        Matrix3& operator=(const glm::tmat3x3<U>& rhs);
        template<typename U>
        Matrix3& operator=(const Matrix3<U>& rhs);

        bool operator==(const Matrix3<T>& rhs) const;
        bool operator!=(const Matrix3<T>& rhs) const;

        template<typename U>
        Matrix3& operator+=(U a);
        template<typename U>
        Matrix3& operator+=(const Matrix3<U>& v);

        template<typename U>
        Matrix3& operator-=(U a);
        template<typename U>
        Matrix3& operator-=(const Matrix3<U>& v);

        template<typename U>
        Matrix3& operator*=(U a);
        template<typename U>
        Matrix3& operator*=(const Matrix3<U>& v);

        template<typename U>
        Matrix3& operator/=(U a);
        template<typename U>
        Matrix3& operator/=(const Matrix3<U>& v);

    private:
        union {
            UNIONIZE(ColType data[3]);
            UNIONIZE(ColType c0; ColType c1; ColType c2);
        };
    };
#include "Matrix.inl"
}

#endif // !Vorb_Matrix_h__
