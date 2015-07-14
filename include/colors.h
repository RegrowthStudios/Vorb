//
// colors.h
// Vorb Engine
//
// Created by Cristian Zaloj on 16 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file colors.h
 * @brief Large list of colors and color space conversion functions.
 */

#pragma once

#ifndef Vorb_colors_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_colors_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

/*! @brief Collection of named colors.
 * 
 * Provides the definition of 141 named
 * colors. Values taken from XNA.
 */
namespace color {
    extern const color4 Transparent; ///< Color "Transparent" with value (0, 0, 0, 0)
    extern const color4 AliceBlue; ///< Color "AliceBlue" with value (240, 248, 255, 255)
    extern const color4 AntiqueWhite; ///< Color "AntiqueWhite" with value (250, 235, 215, 255)
    extern const color4 Aqua; ///< Color "Aqua" with value (0, 255, 255, 255)
    extern const color4 Aquamarine; ///< Color "Aquamarine" with value (127, 255, 212, 255)
    extern const color4 Azure; ///< Color "Azure" with value (240, 255, 255, 255)
    extern const color4 Beige; ///< Color "Beige" with value (245, 245, 220, 255)
    extern const color4 Bisque; ///< Color "Bisque" with value (255, 228, 196, 255)
    extern const color4 Black; ///< Color "Black" with value (0, 0, 0, 255)
    extern const color4 BlanchedAlmond; ///< Color "BlanchedAlmond" with value (255, 235, 205, 255)
    extern const color4 Blue; ///< Color "Blue" with value (0, 0, 255, 255)
    extern const color4 BlueViolet; ///< Color "BlueViolet" with value (138, 43, 226, 255)
    extern const color4 Brown; ///< Color "Brown" with value (165, 42, 42, 255)
    extern const color4 BurlyWood; ///< Color "BurlyWood" with value (222, 184, 135, 255)
    extern const color4 CadetBlue; ///< Color "CadetBlue" with value (95, 158, 160, 255)
    extern const color4 Chartreuse; ///< Color "Chartreuse" with value (127, 255, 0, 255)
    extern const color4 Chocolate; ///< Color "Chocolate" with value (210, 105, 30, 255)
    extern const color4 Coral; ///< Color "Coral" with value (255, 127, 80, 255)
    extern const color4 CornflowerBlue; ///< Color "CornflowerBlue" with value (100, 149, 237, 255)
    extern const color4 Cornsilk; ///< Color "Cornsilk" with value (255, 248, 220, 255)
    extern const color4 Crimson; ///< Color "Crimson" with value (220, 20, 60, 255)
    extern const color4 Cyan; ///< Color "Cyan" with value (0, 255, 255, 255)
    extern const color4 DarkBlue; ///< Color "DarkBlue" with value (0, 0, 139, 255)
    extern const color4 DarkCyan; ///< Color "DarkCyan" with value (0, 139, 139, 255)
    extern const color4 DarkGoldenrod; ///< Color "DarkGoldenrod" with value (184, 134, 11, 255)
    extern const color4 DarkGray; ///< Color "DarkGray" with value (169, 169, 169, 255)
    extern const color4 DarkGreen; ///< Color "DarkGreen" with value (0, 100, 0, 255)
    extern const color4 DarkKhaki; ///< Color "DarkKhaki" with value (189, 183, 107, 255)
    extern const color4 DarkMagenta; ///< Color "DarkMagenta" with value (139, 0, 139, 255)
    extern const color4 DarkOliveGreen; ///< Color "DarkOliveGreen" with value (85, 107, 47, 255)
    extern const color4 DarkOrange; ///< Color "DarkOrange" with value (255, 140, 0, 255)
    extern const color4 DarkOrchid; ///< Color "DarkOrchid" with value (153, 50, 204, 255)
    extern const color4 DarkRed; ///< Color "DarkRed" with value (139, 0, 0, 255)
    extern const color4 DarkSalmon; ///< Color "DarkSalmon" with value (233, 150, 122, 255)
    extern const color4 DarkSeaGreen; ///< Color "DarkSeaGreen" with value (143, 188, 139, 255)
    extern const color4 DarkSlateBlue; ///< Color "DarkSlateBlue" with value (72, 61, 139, 255)
    extern const color4 DarkSlateGray; ///< Color "DarkSlateGray" with value (47, 79, 79, 255)
    extern const color4 DarkTurquoise; ///< Color "DarkTurquoise" with value (0, 206, 209, 255)
    extern const color4 DarkViolet; ///< Color "DarkViolet" with value (148, 0, 211, 255)
    extern const color4 DeepPink; ///< Color "DeepPink" with value (255, 20, 147, 255)
    extern const color4 DeepSkyBlue; ///< Color "DeepSkyBlue" with value (0, 191, 255, 255)
    extern const color4 DimGray; ///< Color "DimGray" with value (105, 105, 105, 255)
    extern const color4 DodgerBlue; ///< Color "DodgerBlue" with value (30, 144, 255, 255)
    extern const color4 Firebrick; ///< Color "Firebrick" with value (178, 34, 34, 255)
    extern const color4 FloralWhite; ///< Color "FloralWhite" with value (255, 250, 240, 255)
    extern const color4 ForestGreen; ///< Color "ForestGreen" with value (34, 139, 34, 255)
    extern const color4 Fuchsia; ///< Color "Fuchsia" with value (255, 0, 255, 255)
    extern const color4 Gainsboro; ///< Color "Gainsboro" with value (220, 220, 220, 255)
    extern const color4 GhostWhite; ///< Color "GhostWhite" with value (248, 248, 255, 255)
    extern const color4 Gold; ///< Color "Gold" with value (255, 215, 0, 255)
    extern const color4 Goldenrod; ///< Color "Goldenrod" with value (218, 165, 32, 255)
    extern const color4 Gray; ///< Color "Gray" with value (128, 128, 128, 255)
    extern const color4 Green; ///< Color "Green" with value (0, 128, 0, 255)
    extern const color4 GreenYellow; ///< Color "GreenYellow" with value (173, 255, 47, 255)
    extern const color4 Honeydew; ///< Color "Honeydew" with value (240, 255, 240, 255)
    extern const color4 HotPink; ///< Color "HotPink" with value (255, 105, 180, 255)
    extern const color4 IndianRed; ///< Color "IndianRed" with value (205, 92, 92, 255)
    extern const color4 Indigo; ///< Color "Indigo" with value (75, 0, 130, 255)
    extern const color4 Ivory; ///< Color "Ivory" with value (255, 255, 240, 255)
    extern const color4 Khaki; ///< Color "Khaki" with value (240, 230, 140, 255)
    extern const color4 Lavender; ///< Color "Lavender" with value (230, 230, 250, 255)
    extern const color4 LavenderBlush; ///< Color "LavenderBlush" with value (255, 240, 245, 255)
    extern const color4 LawnGreen; ///< Color "LawnGreen" with value (124, 252, 0, 255)
    extern const color4 LemonChiffon; ///< Color "LemonChiffon" with value (255, 250, 205, 255)
    extern const color4 LightBlue; ///< Color "LightBlue" with value (173, 216, 230, 255)
    extern const color4 LightCoral; ///< Color "LightCoral" with value (240, 128, 128, 255)
    extern const color4 LightCyan; ///< Color "LightCyan" with value (224, 255, 255, 255)
    extern const color4 LightGoldenrodYellow; ///< Color "LightGoldenrodYellow" with value (250, 250, 210, 255)
    extern const color4 LightGreen; ///< Color "LightGreen" with value (144, 238, 144, 255)
    extern const color4 LightGray; ///< Color "LightGray" with value (211, 211, 211, 255)
    extern const color4 LightPink; ///< Color "LightPink" with value (255, 182, 193, 255)
    extern const color4 LightSalmon; ///< Color "LightSalmon" with value (255, 160, 122, 255)
    extern const color4 LightSeaGreen; ///< Color "LightSeaGreen" with value (32, 178, 170, 255)
    extern const color4 LightSkyBlue; ///< Color "LightSkyBlue" with value (135, 206, 250, 255)
    extern const color4 LightSlateGray; ///< Color "LightSlateGray" with value (119, 136, 153, 255)
    extern const color4 LightSteelBlue; ///< Color "LightSteelBlue" with value (176, 196, 222, 255)
    extern const color4 LightYellow; ///< Color "LightYellow" with value (255, 255, 224, 255)
    extern const color4 Lime; ///< Color "Lime" with value (0, 255, 0, 255)
    extern const color4 LimeGreen; ///< Color "LimeGreen" with value (50, 205, 50, 255)
    extern const color4 Linen; ///< Color "Linen" with value (250, 240, 230, 255)
    extern const color4 Magenta; ///< Color "Magenta" with value (255, 0, 255, 255)
    extern const color4 Maroon; ///< Color "Maroon" with value (128, 0, 0, 255)
    extern const color4 MediumAquamarine; ///< Color "MediumAquamarine" with value (102, 205, 170, 255)
    extern const color4 MediumBlue; ///< Color "MediumBlue" with value (0, 0, 205, 255)
    extern const color4 MediumOrchid; ///< Color "MediumOrchid" with value (186, 85, 211, 255)
    extern const color4 MediumPurple; ///< Color "MediumPurple" with value (147, 112, 219, 255)
    extern const color4 MediumSeaGreen; ///< Color "MediumSeaGreen" with value (60, 179, 113, 255)
    extern const color4 MediumSlateBlue; ///< Color "MediumSlateBlue" with value (123, 104, 238, 255)
    extern const color4 MediumSpringGreen; ///< Color "MediumSpringGreen" with value (0, 250, 154, 255)
    extern const color4 MediumTurquoise; ///< Color "MediumTurquoise" with value (72, 209, 204, 255)
    extern const color4 MediumVioletRed; ///< Color "MediumVioletRed" with value (199, 21, 133, 255)
    extern const color4 MidnightBlue; ///< Color "MidnightBlue" with value (25, 25, 112, 255)
    extern const color4 MintCream; ///< Color "MintCream" with value (245, 255, 250, 255)
    extern const color4 MistyRose; ///< Color "MistyRose" with value (255, 228, 225, 255)
    extern const color4 Moccasin; ///< Color "Moccasin" with value (255, 228, 181, 255)
    extern const color4 NavajoWhite; ///< Color "NavajoWhite" with value (255, 222, 173, 255)
    extern const color4 Navy; ///< Color "Navy" with value (0, 0, 128, 255)
    extern const color4 OldLace; ///< Color "OldLace" with value (253, 245, 230, 255)
    extern const color4 Olive; ///< Color "Olive" with value (128, 128, 0, 255)
    extern const color4 OliveDrab; ///< Color "OliveDrab" with value (107, 142, 35, 255)
    extern const color4 Orange; ///< Color "Orange" with value (255, 165, 0, 255)
    extern const color4 OrangeRed; ///< Color "OrangeRed" with value (255, 69, 0, 255)
    extern const color4 Orchid; ///< Color "Orchid" with value (218, 112, 214, 255)
    extern const color4 PaleGoldenrod; ///< Color "PaleGoldenrod" with value (238, 232, 170, 255)
    extern const color4 PaleGreen; ///< Color "PaleGreen" with value (152, 251, 152, 255)
    extern const color4 PaleTurquoise; ///< Color "PaleTurquoise" with value (175, 238, 238, 255)
    extern const color4 PaleVioletRed; ///< Color "PaleVioletRed" with value (219, 112, 147, 255)
    extern const color4 PapayaWhip; ///< Color "PapayaWhip" with value (255, 239, 213, 255)
    extern const color4 PeachPuff; ///< Color "PeachPuff" with value (255, 218, 185, 255)
    extern const color4 Peru; ///< Color "Peru" with value (205, 133, 63, 255)
    extern const color4 Pink; ///< Color "Pink" with value (255, 192, 203, 255)
    extern const color4 Plum; ///< Color "Plum" with value (221, 160, 221, 255)
    extern const color4 PowderBlue; ///< Color "PowderBlue" with value (176, 224, 230, 255)
    extern const color4 Purple; ///< Color "Purple" with value (128, 0, 128, 255)
    extern const color4 Red; ///< Color "Red" with value (255, 0, 0, 255)
    extern const color4 RosyBrown; ///< Color "RosyBrown" with value (188, 143, 143, 255)
    extern const color4 RoyalBlue; ///< Color "RoyalBlue" with value (65, 105, 225, 255)
    extern const color4 SaddleBrown; ///< Color "SaddleBrown" with value (139, 69, 19, 255)
    extern const color4 Salmon; ///< Color "Salmon" with value (250, 128, 114, 255)
    extern const color4 SandyBrown; ///< Color "SandyBrown" with value (244, 164, 96, 255)
    extern const color4 SeaGreen; ///< Color "SeaGreen" with value (46, 139, 87, 255)
    extern const color4 SeaShell; ///< Color "SeaShell" with value (255, 245, 238, 255)
    extern const color4 Sienna; ///< Color "Sienna" with value (160, 82, 45, 255)
    extern const color4 Silver; ///< Color "Silver" with value (192, 192, 192, 255)
    extern const color4 SkyBlue; ///< Color "SkyBlue" with value (135, 206, 235, 255)
    extern const color4 SlateBlue; ///< Color "SlateBlue" with value (106, 90, 205, 255)
    extern const color4 SlateGray; ///< Color "SlateGray" with value (112, 128, 144, 255)
    extern const color4 Snow; ///< Color "Snow" with value (255, 250, 250, 255)
    extern const color4 SpringGreen; ///< Color "SpringGreen" with value (0, 255, 127, 255)
    extern const color4 SteelBlue; ///< Color "SteelBlue" with value (70, 130, 180, 255)
    extern const color4 Tan; ///< Color "Tan" with value (210, 180, 140, 255)
    extern const color4 Teal; ///< Color "Teal" with value (0, 128, 128, 255)
    extern const color4 Thistle; ///< Color "Thistle" with value (216, 191, 216, 255)
    extern const color4 Tomato; ///< Color "Tomato" with value (255, 99, 71, 255)
    extern const color4 Turquoise; ///< Color "Turquoise" with value (64, 224, 208, 255)
    extern const color4 Violet; ///< Color "Violet" with value (238, 130, 238, 255)
    extern const color4 Wheat; ///< Color "Wheat" with value (245, 222, 179, 255)
    extern const color4 White; ///< Color "White" with value (255, 255, 255, 255)
    extern const color4 WhiteSmoke; ///< Color "WhiteSmoke" with value (245, 245, 245, 255)
    extern const color4 Yellow; ///< Color "Yellow" with value (255, 255, 0, 255)
    extern const color4 YellowGreen; ///< Color "YellowGreen" with value (154, 205, 50, 255)

    f32v3 convertRGBToHSL(const f32v3& val);
    f32v3 convertHSLToRGB(const f32v3& val);
}

#endif // !Vorb_colors_h__
