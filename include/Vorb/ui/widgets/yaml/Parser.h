
//
// Parser.h
// Vorb Engine
//
// Created by Matthew Marshall on 29 June 2019
// Copyright 2019 Regrowth Studios
// MIT License
//

/*! \file Parser.h
 * @brief Provides useful helper functions for loading UI elements from YAML.
 */

#pragma once

#ifndef Vorb_UI_YAML_Parser_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UI_YAML_Parser_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/graphics/SpriteBatch.h"
#include "Vorb/io/Keg.h"
#include "Vorb/io/YAMLImpl.h"
#include "Vorb/ui/widgets/TextWidget.h"

namespace vorb {
    namespace ui {
        /***************\
         *    Basic    *
        \***************/

        template <typename Type>
        bool parseValue(keg::YAMLNode value, OUT Type& parsed) {
            if (keg::getType(&value) != keg::NodeType::VALUE) return false;

            parsed = value.data[0].as<Type>();

            return true;
        }

        /*****************\
         *    Vectors    *
        \*****************/

#define PARSE_VEC2(TYPE) bool parseVec2(keg::YAMLNode value, OUT TYPE##v2& vec);

        PARSE_VEC2(i8)
        PARSE_VEC2(i16)
        PARSE_VEC2(i32)
        PARSE_VEC2(i64)
        PARSE_VEC2(ui8)
        PARSE_VEC2(ui16)
        PARSE_VEC2(ui32)
        PARSE_VEC2(ui64)
        PARSE_VEC2(f32)
        PARSE_VEC2(f64)

#undef PARSE_VEC2

#define PARSE_VEC3(TYPE) bool parseVec3(keg::YAMLNode value, OUT TYPE##v3& vec);

        PARSE_VEC3(i8)
        PARSE_VEC3(i16)
        PARSE_VEC3(i32)
        PARSE_VEC3(i64)
        PARSE_VEC3(ui8)
        PARSE_VEC3(ui16)
        PARSE_VEC3(ui32)
        PARSE_VEC3(ui64)
        PARSE_VEC3(f32)
        PARSE_VEC3(f64)

#undef PARSE_VEC3

#define PARSE_VEC4(TYPE) bool parseVec4(keg::YAMLNode value, OUT TYPE##v4& vec);

        PARSE_VEC4(i8)
        PARSE_VEC4(i16)
        PARSE_VEC4(i32)
        PARSE_VEC4(i64)
        PARSE_VEC4(ui8)
        PARSE_VEC4(ui16)
        PARSE_VEC4(ui32)
        PARSE_VEC4(ui64)
        PARSE_VEC4(f32)
        PARSE_VEC4(f64)

#undef PARSE_VEC4

        /********************\
         *    Text Align    *
        \********************/

        bool parsePartTextAlign(keg::YAMLNode value, OUT vg::TextAlign& textAlign, size_t offset = 0);

        bool parseTextAlign(keg::YAMLNode value, OUT vg::TextAlign& textAlign);

        /************************\
         *    Clipping State    *
        \************************/

        bool parsePartClippingState(keg::YAMLNode value, OUT vui::ClippingState& clippingState, size_t offset = 0);

        bool parseClippingState(keg::YAMLNode value, OUT vui::ClippingState& clippingState);

        /******************\
         *    Clipping    *
        \******************/

        bool parseClipping(keg::YAMLNode value, OUT vui::ClippingState* clipping);

        /********************\
         *    Dock State    *
        \********************/

        bool parsePartDockState(keg::YAMLNode value, OUT vui::DockState& dockState, size_t offset = 0);

        bool parseDockState(keg::YAMLNode value, OUT vui::DockState& dockState);

        /********************\
         *       Dock       *
        \********************/

        bool parseDock(keg::YAMLNode value, OUT vui::Dock& dock);

        /***********************\
         *    Position Type    *
        \***********************/

        bool parsePartPositionType(keg::YAMLNode value, OUT vui::PositionType& positionType, size_t offset = 0);

        bool parsePositionType(keg::YAMLNode value, OUT vui::PositionType& positionType);

        /************************\
         *    Dimension Type    *
        \************************/

        bool parsePartDimensionType(keg::YAMLNode value, OUT vui::DimensionType& dimensionType, size_t offset = 0);

        bool parseDimensionType(keg::YAMLNode value, OUT vui::DimensionType& dimensionType);

        /**********************\
         *       Length       *
        \**********************/

        bool parseLength(keg::YAMLNode value, OUT vui::Length& length);

        bool parseLength2(keg::YAMLNode value, OUT vui::Length2& length);

        bool parseLength4(keg::YAMLNode value, OUT vui::Length4& length);

        /****************************\
         *       Length or Raw      *
        \****************************/

        union LengthOrRaw {
            vui::Length length;
            f32         raw;
        };

        i8 parseLengthOrRaw(keg::YAMLNode value, OUT LengthOrRaw& lengthOrRaw);

        union Length2OrRaw {
            vui::Length2 length;
            f32v2        raw;
        };

        i8 parseLength2OrRaw(keg::YAMLNode value, OUT Length2OrRaw& lengthOrRaw);

        union Length4OrRaw {
            vui::Length4 length;
            f32v4        raw;
        };

        i8 parseLength4OrRaw(keg::YAMLNode value, OUT Length4OrRaw& lengthOrRaw);

        /*******************\
         *      Color      *
        \*******************/

        bool parsePartColor(keg::YAMLNode value, OUT color4& color, size_t offset = 0);

        bool parseColor(keg::YAMLNode value, OUT color4& color);

        /***********************\
         *    Gradient Type    *
        \***********************/

        bool parsePartGradientType(keg::YAMLNode value, OUT vg::GradientType& gradType, size_t offset = 0);

        bool parseGradientType(keg::YAMLNode value, OUT vg::GradientType& gradType);

        /******************\
         *    Gradient    *
        \******************/

        bool parseGradient(keg::YAMLNode value, OUT color4& color1, OUT color4& color2, OUT vg::GradientType& gradType);
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UI_YAML_Parser_h__
