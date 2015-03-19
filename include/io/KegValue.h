//
// KegValue.h
// Vorb Engine
//
// Created by Cristian Zaloj on 17 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file KegValue.h
 * @brief 
 * 
 * 
 */

#pragma once

#ifndef Vorb_KegValue_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegValue_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <memory>
#include "types.h"
#endif // !VORB_USING_PCH

#include "KegFuncs.h"

namespace keg {
    enum class BasicType;


    // A Value Type Bound To An Offset In A Struct Of Data
    struct Value {
    public:
        template<typename T, typename M>
        static Value value(M T::* member) {
            Value kv = {};
            kv.type = BasicType::CUSTOM;
            kv.offset = offsetMember(member);
            kv.typeName.clear();
            kv.evaluator = evaluate<M>;
            return std::move(kv);
        }
        template<typename T, typename... Types, typename M, typename... Members>
        static Value value(M T::* member, Members Types ::*... children) {
            static_assert(sizeof...(Types) == sizeof...(Members) && sizeof...(children) == sizeof...(Members), "Pairs of types must be specified");
            Value val = value(children...);
            val.offset += offsetMember(member);
            return val;
        }

        // Set This As A Basic Parseable Value Type
        static Value basic(size_t off, BasicType t);
        // Set This As A Custom Value Type
        static Value custom(size_t off, const nString& t, bool isEnum = false);
        // Set This As An Array Of Values
        static Value array(size_t off, const Value& interior);
        static Value array(size_t off, const keg::BasicType& t);
        // Set This As A Value That Is Accessed Via Pointer
        static Value ptr(size_t off, const Value& interior);
        static Value ptr(size_t off, const keg::BasicType& t);

        // Type Used To Determine Parsing Method
        BasicType type;
        Evaluator evaluator;

        // If It's A Custom Type
        nString typeName;

        // If It's An Array Or Pointer Type
        std::shared_ptr<Value> interiorValue;

        // Bytes Of Offset Into The Struct To Find The Data
        size_t offset;
    };
}

#endif // !Vorb_KegValue_h__
