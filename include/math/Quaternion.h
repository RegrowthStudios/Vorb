//
// Quaternion.h
// Vorb Engine
//
// Created by Benjamin Arnold on 21 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Quaternion.h
* @brief 
*
*
*/

#pragma once

#ifndef Vorb_Quaternion_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Quaternion_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

template <typename T>
class Quaternion {
public:
    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(const Vector3<T>& euler);
    Quaternion(const glm::tvec3<T>& euler);
    Quaternion(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {};


    union {
        struct { T x; T y; T z; T w; };
        T data[4];
    }
};

#endif // !Vorb_Quaternion_h__
