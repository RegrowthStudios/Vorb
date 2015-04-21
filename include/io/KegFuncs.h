//
// KegFuncs.h
// Vorb Engine
//
// Created by Cristian Zaloj on 18 Mar 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file KegFuncs.h
 * @brief 
 * 
 * 
 */

#pragma once

#ifndef Vorb_KegFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "YAML.h"
#include "KegBasic.h"

namespace keg {
    typedef void(*Evaluator)(void* dest, keg::Node n);

    template<typename T>
    void evaluate(void* dest, Node node) {
        T* value = (T*)dest;
        *value = keg::convert<T>(node);
    }

    template<>
    inline void evaluate<ui8>(void* dest, Node node) {
        ui8* value = (ui8*)dest;
        *value = static_cast<ui8>(keg::convert<ui16>(node));
    }
    template<>
    inline void evaluate<cString>(void* dest, Node node) {
        cString* value = (cString*)dest;

        nString data = keg::convert<nString>(node);
        cString dataCopy = new char[data.length() + 1];
        memcpy(dataCopy, data.c_str(), data.length());
        dataCopy[data.length()] = 0;

        *value = dataCopy;
    }

    VORB_INTERNAL extern Evaluator simpleEvaluators[(size_t)BasicType::COUNT];
}

#endif // !Vorb_KegFuncs_h__
