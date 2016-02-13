//
// TypeManip.hpp
// Vorb Engine
//
// Created by Cristian Zaloj on 20 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file TypeManip.hpp
 * @brief Contains facilities for manipulating C++ types and to aid in meta-template programming.
 */

#pragma once

#ifndef Vorb_TypeManip_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TypeManip_hpp__
//! @endcond

/*! @brief Reinterprets the value of a pointer as any other type that is of the same size.
 * 
 * @tparam T Type to which to interpret the pointer.
 */
template<typename T>
union PointerCast {
    // Pointer conversion is only allowed for types the same size as the pointer.
    static_assert(sizeof(T) == sizeof(void*), "Conversion type must be the same size as a pointer");

    PointerCast(T v) {
        value = v;
    }
    PointerCast(void* v) {
        ptr = v;
    }

    T value;
    void* ptr;
};

/*! @brief Syntactic sugar for ghosting member functions of unknown classes.
 */
class TypelessMember {
public:
    template<typename T>
    static T TypelessMember::* value(size_t offset) {
        return *(T TypelessMember::**)((void**)&offset);
    }
    template<typename T, typename C>
    static T TypelessMember::* value(T C::* offset) {
        return (T TypelessMember::*)offset;
    }

    template<typename R, typename... Params>
    using Func = R(TypelessMember::*)(Params...);
    template<typename C, typename R, typename... Params>
    using FuncOther = R(C::*)(Params...);

    template<typename C, typename T, typename... Params>
    static Func<T, Params...> func(FuncOther<C, T, Params...> offset) {
        return (Func<T, Params...>)offset;
    }

    template<typename Ret, typename... Params>
    Ret function(Params... args);
};

inline TypelessMember* typeless(void* value) {
    return (TypelessMember*)value;
}
template<typename T>
typename std::enable_if<!std::is_pointer<T>::value, TypelessMember*>::type typeless(T value) {
    return (TypelessMember*)(PointerCast<T>(value).ptr);
}

#endif // !Vorb_TypeManip_hpp__
