//
// Random.h
// Vorb Engine
//
// Created by Cristian Zaloj on 8 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file Random.h
 * 
 * \brief A simple random number generator.
 */

#pragma once

#ifndef Vorb_Random_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Random_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

// Like Goldilocks, Just Right
#define MERSENNE_ARRAY_SIZE 624

/// A seeded random number generator
class Random {
public:
    /// Create a random generator using a seed value
    /// \param s: Seed value
    Random(const i32& s);
    /// Create a random generator with seed value of clock ticks
    Random();
    /// Destroys internal array used for mersenne twister
    ~Random();

    /// Reform the random generator
    /// \param s: Seed value
    void seed(const i32& s);

    /// \return: A random number between [0-1] inclusive using mersenne twister
    f32 genMT();
    /// \return: A random number between [0-1] inclusive using murmur hash
    f32 genMH();
private:
    /// ... Because we want to keep generating random stuff after MERSENNE_ARRAY_SIZE uses
    void perturbMT();

    i32 m_seed; ///< The seed value of the generator
    
    i32* m_mtArr; ///< Array of randomly generated numbers for the twister to rampage upon
    i32 m_mtIndex; ///< Current position in the twister's array
    
    i32 m_mhKey; ///< An updating key for murmur hash
};

#endif // !Vorb_Random_h__
