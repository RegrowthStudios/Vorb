#include "Vorb/graphics/SpriteBatch.h"
#include "Vorb/io/YAMLImpl.h"
#include "Vorb/ui/widgets/TextWidget.h"

namespace vorb {
    namespace ui {
        /****************\
         *    String    *
        \****************/

        bool parseString(keg::Node value, OUT nString& str) {
            if (keg::getType(value) != keg::NodeType::VALUE) return false;

            str = value->data[0].as<nString>();

            return true;
        }

        /*****************\
         *    Vectors    *
        \*****************/

#define PARSE_VEC2(TYPE)                                        \
        bool parseVec2(keg::Node value, OUT TYPE##v2& vec) {    \
            if (keg::getType(value) != keg::NodeType::SEQUENCE  \
                    || value->data.size() != 2) return false;   \
                                                                \
            vec[0] = value->data[0].as<TYPE>();                 \
            vec[1] = value->data[1].as<TYPE>();                 \
                                                                \
            return true;                                        \
        }

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

#define PARSE_VEC3(TYPE)                                        \
        bool parseVec3(keg::Node value, OUT TYPE##v3& vec) {    \
            if (keg::getType(value) != keg::NodeType::SEQUENCE  \
                    || value->data.size() != 3) return false;   \
                                                                \
            vec[0] = value->data[0].as<TYPE>();                 \
            vec[1] = value->data[1].as<TYPE>();                 \
            vec[2] = value->data[2].as<TYPE>();                 \
                                                                \
            return true;                                        \
        }

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

#define PARSE_VEC4(TYPE)                                        \
        bool parseVec4(keg::Node value, OUT TYPE##v4& vec) {    \
            if (keg::getType(value) != keg::NodeType::SEQUENCE  \
                    || value->data.size() != 4) return false;   \
                                                                \
            vec[0] = value->data[0].as<TYPE>();                 \
            vec[1] = value->data[1].as<TYPE>();                 \
            vec[2] = value->data[2].as<TYPE>();                 \
            vec[3] = value->data[3].as<TYPE>();                 \
                                                                \
            return true;                                        \
        }

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

        /********************\
         *    Text Align    *
        \********************/

        bool parsePartTextAlign(keg::Node value, OUT vg::TextAlign& textAlign, size_t offset = 0) {
            nString name = value->data[offset].as<nString>();

            if (name == "none") {
                textAlign = vg::TextAlign::NONE;
                return true;
            } else if (name == "left") {
                textAlign = vg::TextAlign::LEFT;
                return true;
            } else if (name == "top_left") {
                textAlign = vg::TextAlign::TOP_LEFT;
                return true;
            } else if (name == "top") {
                textAlign = vg::TextAlign::TOP;
                return true;
            } else if (name == "top_right") {
                textAlign = vg::TextAlign::TOP_RIGHT;
                return true;
            } else if (name == "right") {
                textAlign = vg::TextAlign::RIGHT;
                return true;
            } else if (name == "bottom_right") {
                textAlign = vg::TextAlign::BOTTOM_RIGHT;
                return true;
            } else if (name == "bottom") {
                textAlign = vg::TextAlign::BOTTOM;
                return true;
            } else if (name == "bottom_left") {
                textAlign = vg::TextAlign::BOTTOM_LEFT;
                return true;
            } else if (name == "center") {
                textAlign = vg::TextAlign::CENTER;
                return true;
            }

            return false;
        }

        bool parseTextAlign(keg::Node value, OUT vg::TextAlign& textAlign) {
            if (keg::getType(value) != keg::NodeType::VALUE) return false;

            return parsePartTextAlign(value, textAlign);
        }

        /*******************\
         *      Color      *
        \*******************/

        bool parsePartColor(keg::Node value, OUT color4& color, size_t offset = 0) {
            color.r = value->data[offset + 0].as<ui8>();
            color.g = value->data[offset + 1].as<ui8>();
            color.b = value->data[offset + 2].as<ui8>();
            color.a = value->data[offset + 3].as<ui8>();

            return true;
        }

        bool parseColor(keg::Node value, OUT color4& color) {
            if (keg::getType(value) != keg::NodeType::SEQUENCE
                    || value->data.size() != 4) return false;

            return parsePartColor(value, color);
        }

        /***********************\
         *    Gradient Type    *
        \***********************/

        bool parsePartGradientType(keg::Node value, OUT vg::GradientType& gradType, size_t offset = 0) {
            nString name = value->data[offset].as<nString>();

            if (name == "none") {
                gradType = vg::GradientType::NONE;
                return true;
            } else if (name == "horizontal") {
                gradType = vg::GradientType::HORIZONTAL;
                return true;
            } else if (name == "vertical") {
                gradType = vg::GradientType::VERTICAL;
                return true;
            } else if (name == "left_diagonal") {
                gradType = vg::GradientType::LEFT_DIAGONAL;
                return true;
            } else if (name == "right_diagonal") {
                gradType = vg::GradientType::RIGHT_DIAGONAL;
                return true;
            }

            return false;
        }

        bool parseGradientType(keg::Node value, OUT vg::GradientType& gradType) {
            if (keg::getType(value) != keg::NodeType::VALUE) return false;

            return parsePartGradientType(value, gradType);
        }

        /******************\
         *    Gradient    *
        \******************/

        bool parseGradient(keg::Node value, OUT color4& color1, OUT color4& color2, OUT vg::GradientType& gradType) {
            if (keg::getType(value) != keg::NodeType::MAP
                    || value->data.size() != 3) return false;

            if (!value->data["color1"] || !value->data["color2"]
                    || !value->data["grad_type"]) return false;

            if (!parseColor({ value->data["color1"] }, color1)) return false;
            if (!parseColor({ value->data["color2"] }, color2)) return false;
            if (!parseGradientType({ value->data["grad_type"] }, gradType)) return false;

            return true;
        }
    }
}
