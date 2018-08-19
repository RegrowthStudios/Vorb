//
// YAMLConverters.h
// Vorb Engine
//
// Created by Cristian Zaloj on 21 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file YAMLConverters.h
 * @brief Declarations of types supported for node conversion.
 */

#pragma once

#ifndef Vorb_YAMLConverters_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAMLConverters_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "YAMLNode.h"
#include "KegTypes.h"

namespace keg {
    /// Interface for node conversion
#define KEG_NODE_CONVERTER(TYPE) \
    template<> struct NodeValueConverter<TYPE> { \
    public: \
            static TYPE convert(Node node); \
            static void write(YAMLEmitter* emitter, TYPE value); \
    }

    /************************************************************************/
    /* System-defined YAML node converters                                  */
    /************************************************************************/
#define KEG_NODE_CONVERTER_NUM(TYPE) \
    KEG_NODE_CONVERTER(TYPE ## 8); \
    KEG_NODE_CONVERTER(TYPE ## 16); \
    KEG_NODE_CONVERTER(TYPE ## 32); \
    KEG_NODE_CONVERTER(TYPE ## 64); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 8v2); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 16v2); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 32v2); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 64v2); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 8v3); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 16v3); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 32v3); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 64v3); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 8v4); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 16v4); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 32v4); \
    KEG_NODE_CONVERTER(keg::keg ## TYPE ## 64v4)
    KEG_NODE_CONVERTER_NUM(i);
    KEG_NODE_CONVERTER_NUM(ui);
    KEG_NODE_CONVERTER(f32);
    KEG_NODE_CONVERTER(keg::kegf32v2);
    KEG_NODE_CONVERTER(keg::kegf32v3);
    KEG_NODE_CONVERTER(keg::kegf32v4);
    KEG_NODE_CONVERTER(f64);
    KEG_NODE_CONVERTER(keg::kegf64v2);
    KEG_NODE_CONVERTER(keg::kegf64v3);
    KEG_NODE_CONVERTER(keg::kegf64v4);
    KEG_NODE_CONVERTER(bool);
    KEG_NODE_CONVERTER(nString);
    KEG_NODE_CONVERTER(cString);
}

#endif // !Vorb_YAMLConverters_h__
