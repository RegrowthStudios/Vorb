//
// IndexSequence.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 7 Nov 2018
// Lifted directly from Cristian Zaloj's original script environment implementation.
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file IndexSequence.hpp
 * \brief Provides a index sequence trick which allows for unpacking of variadic templates and of types built from them.
 */

#pragma once

#ifndef Vorb_IndexSequence_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IndexSequence_h__
//! @endcond

// Index sequencing trick to enable flexible variadic pack expansion. (Taken from C++14).
// Given a variadic template of <int, int, int>, this trick takes the number of types in the pack (3)
// and then recursively expands that value to return a struct index_sequence<0, 1, 2>. 
// By exploiting type inference, we can then have a variadic indices template in functions that need
// to call a function for each element in a container whom's contents are defined by a variadic pack.
template<size_t ...Indices>
struct index_sequence {};
template<size_t N, size_t ...Indices>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, Indices...> {};
template<size_t ...Indices>
struct make_index_sequence<0, Indices...> : index_sequence<Indices...> {};
template<typename ...Types>
struct index_sequence_for : make_index_sequence<sizeof...(Types)> {};

#endif // !Vorb_IndexSequence_h__
