///
/// init.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 18 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Compiler voodoo for initializing types statically
///

#pragma once

#ifndef init_h__
#define init_h__

template<typename T, typename... Params> struct typeinit; ///< Initializes a type with a certain set of arguments

/// Initializes a data type with some input information
/// @tparam T: Type of initializing object
/// @tparam Params: Argument types
/// @param data: Object
/// @param args: Arguments
/// @return True if initialized correctly
template<typename T, typename... Params>
bool init(T& data, Params... args) {
    return typeinit<T, Params...>::init(data, args...);
}

#endif // init_h__