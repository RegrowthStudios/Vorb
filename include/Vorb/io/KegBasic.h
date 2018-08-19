//
// KegBasic.h
// Vorb Engine
//
// Created by Cristian Zaloj on 17 Feb 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file KegBasic.h
 * @brief 
 */

#pragma once

#ifndef Vorb_KegBasic_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegBasic_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>

#include "../types.h"
#endif // !VORB_USING_PCH

namespace keg {
    /*! @brief These are all the types that can be parsed directly into a value.
     */
    enum class BasicType {
        /************************************************************************/
        /* Simple types                                                         */
        /************************************************************************/
#define KEG_BASIC_NUM_VEC(TYPE) TYPE, TYPE##_V2, TYPE##_V3, TYPE##_V4
        // Signed Integral Types
        KEG_BASIC_NUM_VEC(I8),
        KEG_BASIC_NUM_VEC(I16),
        KEG_BASIC_NUM_VEC(I32),
        KEG_BASIC_NUM_VEC(I64),
        // Unsigned Integral Types
        KEG_BASIC_NUM_VEC(UI8),
        KEG_BASIC_NUM_VEC(UI16),
        KEG_BASIC_NUM_VEC(UI32),
        KEG_BASIC_NUM_VEC(UI64),
        // Floating-Point Types
        KEG_BASIC_NUM_VEC(F32),
        KEG_BASIC_NUM_VEC(F64),
        BOOL, ///< Boolean
        C_STRING, ///< C-style string (char* array)
        STRING, ///< New-style C++ string

        /************************************************************************/
        /* Complex types                                                        */
        /************************************************************************/
        ARRAY, ///< Array of values
        PTR, ///< Pointer to a value
        ENUM, ///< Enum
        CUSTOM, ///< Custom user-defined type
        VORB_INTERNAL COUNT, ///< Number of defined types
#undef KEG_BASIC_NUM_VEC
    };

    extern std::map<BasicType, nString> basicTypes;
}

#endif // !Vorb_KegBasic_h__
