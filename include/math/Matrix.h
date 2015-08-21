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
    class Matrix2x2 {
    public:
        typename Vector2<T> ColType;

        ColType& operator[](int i);
        const ColType& operator[](int i);

        operator glm::tmat2x2<T>() const;

        template<typename U>
        Matrix2x2& operator=(const glm::tmat2x2<U>& rhs);
        template<typename U>
        Matrix2x2& operator=(const Matrix2x2<U>& rhs);

        bool operator==(const Matrix2x2<T>& rhs) const;
        bool operator!=(const Matrix2x2<T>& rhs) const;

        template<typename U>
        Matrix2x2& operator+=(U a);
        template<typename U>
        Matrix2x2& operator+=(const Matrix2x2<U>& v);

        template<typename U>
        Matrix2x2& operator-=(U a);
        template<typename U>
        Matrix2x2& operator-=(const Matrix2x2<U>& v);

        template<typename U>
        Matrix2x2& operator*=(U a);
        template<typename U>
        Matrix2x2& operator*=(const Matrix2x2<U>& v);

        template<typename U>
        Matrix2x2& operator/=(U a);
        template<typename U>
        Matrix2x2& operator/=(const Matrix2x2<U>& v);

    private:
        ColType data[2];
    };
#include "Matrix.inl"
}

#endif // !Vorb_Matrix_h__
