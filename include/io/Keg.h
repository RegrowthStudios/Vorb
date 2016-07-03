//
// Keg.h
// Vorb Engine
//
// Created by Cristian Zaloj on 9 Jan 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file Keg.h
 * 
 * \brief Keg YAML utility.
 */

#pragma once

#ifndef Vorb_Keg_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Keg_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "KegBasic.h"
#include "KegEnum.h"
#include "KegEnvironment.h"
#include "KegType.h"
#include "KegTypes.h"
#include "KegValue.h"
#include "YAML.h"

#define KEG_DOC_TYPE_ID "__TYPE__"
#define KEG_DOC_DATA_ID "__DATA__"

/*! \brief Vorb extension for YAML parsing.
 * 
 * Provides utility classes for parsing YAML files as well
 * as basic user-defined reflection capabilities.
 */
namespace keg {
    // These Are Errors Generated From Parsing
    enum class Error {
        // Yippee Ki Yay
        NONE,
        // A Custom Type Is Not Defined Yet Or The Document Is Wrong
        TYPE_NOT_FOUND,
        // An Invalid Argument Was Supplied To The Parser
        BAD_ARGUMENT,
        // A Bad Value Was Given In The Data
        BAD_VALUE,
        // Not Enough Information Provided In The Data
        EARLY_EOF
    };

    struct ReadContext {
    public:
        YAMLReader reader;
        Environment* env;
    };


    // Parse String Of Data Into A Destination Given A Type And Optionally A Separate Environment
    Error parse(void* dest, const cString data, Type* type = nullptr, Environment* env = nullptr);
    // Parse String Of Data Into A Destination Given A Type Name And Optionally A Separate Environment
    Error parse(void* dest, const cString data, const nString& typeName, Environment* env = nullptr);
    // Parse String Of Data Into A Destination Given A Type ID And Optionally A Separate Environment
    Error parse(void* dest, const cString data, const ui32& typeID, Environment* env = nullptr);
    Error parse(ui8* dest, keg::Node& data, ReadContext& context, Type* type);
    void evalData(ui8* dest, const Value* decl, keg::Node& node, ReadContext& context);

    nString write(const void* src, Type* type, Environment* env = nullptr);
    nString write(const void* src, const nString& typeName, Environment* env = nullptr);
    nString write(const void* src, const ui32& typeID, Environment* env = nullptr);
    bool write(const ui8* src, keg::YAMLWriter& e, Environment* env, Type* type);

    VORB_INTERNAL Type& getType(bool& initialized, Type& type, bool (*fInit)());
    VORB_INTERNAL Enum& getEnum(bool& initialized, Enum& type, bool (*fInit)());
}


#define KEG_GLOBAL_TYPE(TYPENAME) kts_##TYPENAME
#define KEG_GLOBAL_TYPE_INIT(TYPENAME) init_kts_##TYPENAME
#define KEG_GLOBAL_TYPE_INIT_FUNC(TYPENAME) kt_init_##TYPENAME
#define KEG_GLOBAL_TYPE_BUILD_FUNC(TYPENAME) kt_build_##TYPENAME
#define KEG_GLOBAL_ENUM(TYPENAME) kes_##TYPENAME
#define KEG_GLOBAL_ENUM_INIT(TYPENAME) init_kes_##TYPENAME
#define KEG_GLOBAL_ENUM_INIT_FUNC(TYPENAME) ke_init_##TYPENAME
#define KEG_GLOBAL_ENUM_BUILD_FUNC(TYPENAME) ke_build_##TYPENAME
#ifdef DEBUG
#define KEG_TYPE_INIT_FUNC_DEF(TYPENAME, STRUCT_TYPE) \
    bool KEG_GLOBAL_TYPE_INIT_FUNC(TYPENAME)() { \
        if (KEG_GLOBAL_TYPE_INIT(TYPENAME)) return true; \
        puts("Initializing Keg Type: "#TYPENAME); \
        KEG_GLOBAL_TYPE(TYPENAME).setStructType<STRUCT_TYPE>(); \
        KEG_GLOBAL_TYPE_BUILD_FUNC(TYPENAME)(KEG_GLOBAL_TYPE(TYPENAME)); \
        return true; \
    }
#define KEG_ENUM_INIT_FUNC_DEF(TYPENAME) \
    bool KEG_GLOBAL_ENUM_INIT_FUNC(TYPENAME)() { \
        if (KEG_GLOBAL_ENUM_INIT(TYPENAME)) return true; \
        puts("Initializing Keg Enum: "#TYPENAME); \
        KEG_GLOBAL_ENUM_BUILD_FUNC(TYPENAME)(KEG_GLOBAL_ENUM(TYPENAME)); \
        return true; \
    }
#else
#define KEG_TYPE_INIT_FUNC_DEF(TYPENAME, STRUCT_TYPE) \
    bool KEG_GLOBAL_TYPE_INIT_FUNC(TYPENAME)() { \
        if (KEG_GLOBAL_TYPE_INIT(TYPENAME)) return true; \
        KEG_GLOBAL_TYPE(TYPENAME).setStructType<STRUCT_TYPE>(); \
        KEG_GLOBAL_TYPE_BUILD_FUNC(TYPENAME)(KEG_GLOBAL_TYPE(TYPENAME)); \
        return true; \
    }
#define KEG_ENUM_INIT_FUNC_DEF(TYPENAME) \
    bool KEG_GLOBAL_ENUM_INIT_FUNC(TYPENAME)() { \
        if (KEG_GLOBAL_ENUM_INIT(TYPENAME)) return true; \
        KEG_GLOBAL_ENUM_BUILD_FUNC(TYPENAME)(KEG_GLOBAL_ENUM(TYPENAME)); \
        return true; \
    }
#endif // DEBUG

// For Use In Header Files:
#define KEG_TYPE_DECL(TYPENAME) extern keg::Type KEG_GLOBAL_TYPE(TYPENAME); extern bool KEG_GLOBAL_TYPE_INIT(TYPENAME)
#define KEG_ENUM_DECL(TYPENAME) extern keg::Enum KEG_GLOBAL_ENUM(TYPENAME); extern bool KEG_GLOBAL_ENUM_INIT(TYPENAME)

// For Use In Source Files:
#define KEG_TYPE_DEF(TYPENAME, STRUCT_TYPE, VAR_NAME) \
    bool KEG_GLOBAL_TYPE_INIT_FUNC(TYPENAME)(); \
    void KEG_GLOBAL_TYPE_BUILD_FUNC(TYPENAME)(keg::Type&); \
    keg::Type KEG_GLOBAL_TYPE(TYPENAME)(#TYPENAME, nullptr); \
    bool KEG_GLOBAL_TYPE_INIT(TYPENAME) = KEG_GLOBAL_TYPE_INIT_FUNC(TYPENAME)(); \
    KEG_TYPE_INIT_FUNC_DEF(TYPENAME, STRUCT_TYPE) \
    void KEG_GLOBAL_TYPE_BUILD_FUNC(TYPENAME)(keg::Type& VAR_NAME)
#define KEG_ENUM_DEF(TYPENAME, STRUCT_TYPE, VAR_NAME) \
    bool KEG_GLOBAL_ENUM_INIT_FUNC(TYPENAME)(); \
    void KEG_GLOBAL_ENUM_BUILD_FUNC(TYPENAME)(keg::Enum&); \
    keg::Enum KEG_GLOBAL_ENUM(TYPENAME)(sizeof(STRUCT_TYPE), #TYPENAME, nullptr); \
    bool KEG_GLOBAL_ENUM_INIT(TYPENAME) = KEG_GLOBAL_ENUM_INIT_FUNC(TYPENAME)(); \
    KEG_ENUM_INIT_FUNC_DEF(TYPENAME) \
    void KEG_GLOBAL_ENUM_BUILD_FUNC(TYPENAME)(keg::Enum& VAR_NAME)

// For Use If The Keg::Type Is Declared In A File
#define KEG_GET_TYPE(TYPENAME) \
    keg::getType(KEG_GLOBAL_TYPE_INIT(TYPENAME), KEG_GLOBAL_TYPE(TYPENAME), KEG_GLOBAL_TYPE_INIT_FUNC(TYPENAME))
#define KEG_GET_ENUM(TYPENAME) \
    keg::getEnum(KEG_GLOBAL_ENUM_INIT(TYPENAME), KEG_GLOBAL_ENUM(TYPENAME), KEG_GLOBAL_ENUM_INIT_FUNC(TYPENAME))

// For Less Verbose Type Initializations
#define KEG_TYPE_DEF_SAME_NAME(TYPENAME, VAR_NAME) KEG_TYPE_DEF(TYPENAME, TYPENAME, VAR_NAME)
#define KEG_TYPE_INIT_ADD_MEMBER(VAR_NAME, TYPENAME, MEMBER, TYPE_ENUM) \
    VAR_NAME.addValue(#MEMBER, keg::Value::basic(offsetof(TYPENAME, MEMBER), keg::BasicType::TYPE_ENUM))

#endif // !Vorb_Keg_h__

/*! \example "Keg Usage"
 * 
 * In the header file, you should declare that you want a type/enum to be part of
 * the keg environment.
 * \include VorbKeg.h
 * 
 * Define the values of all declared keg types/enums in compilation units.
 * \include VorbKeg.cpp
 */
