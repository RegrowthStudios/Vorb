///
/// colors.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 8 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Large list of colors and color space conversion functions
///

#pragma once

#ifndef colors_h__
#define colors_h__

/** @brief Collection of named colors.
 * 
 * Provides the definition of 141 named
 * colors. Values taken from XNA.
 */
namespace color {
    extern const color4 Transparent;
    extern const color4 AliceBlue;
    extern const color4 AntiqueWhite;
    extern const color4 Aqua;
    extern const color4 Aquamarine;
    extern const color4 Azure;
    extern const color4 Beige;
    extern const color4 Bisque;
    extern const color4 Black;
    extern const color4 BlanchedAlmond;
    extern const color4 Blue;
    extern const color4 BlueViolet;
    extern const color4 Brown;
    extern const color4 BurlyWood;
    extern const color4 CadetBlue;
    extern const color4 Chartreuse;
    extern const color4 Chocolate;
    extern const color4 Coral;
    extern const color4 CornflowerBlue;
    extern const color4 Cornsilk;
    extern const color4 Crimson;
    extern const color4 Cyan;
    extern const color4 DarkBlue;
    extern const color4 DarkCyan;
    extern const color4 DarkGoldenrod;
    extern const color4 DarkGray;
    extern const color4 DarkGreen;
    extern const color4 DarkKhaki;
    extern const color4 DarkMagenta;
    extern const color4 DarkOliveGreen;
    extern const color4 DarkOrange;
    extern const color4 DarkOrchid;
    extern const color4 DarkRed;
    extern const color4 DarkSalmon;
    extern const color4 DarkSeaGreen;
    extern const color4 DarkSlateBlue;
    extern const color4 DarkSlateGray;
    extern const color4 DarkTurquoise;
    extern const color4 DarkViolet;
    extern const color4 DeepPink;
    extern const color4 DeepSkyBlue;
    extern const color4 DimGray;
    extern const color4 DodgerBlue;
    extern const color4 Firebrick;
    extern const color4 FloralWhite;
    extern const color4 ForestGreen;
    extern const color4 Fuchsia;
    extern const color4 Gainsboro;
    extern const color4 GhostWhite;
    extern const color4 Gold;
    extern const color4 Goldenrod;
    extern const color4 Gray;
    extern const color4 Green;
    extern const color4 GreenYellow;
    extern const color4 Honeydew;
    extern const color4 HotPink;
    extern const color4 IndianRed;
    extern const color4 Indigo;
    extern const color4 Ivory;
    extern const color4 Khaki;
    extern const color4 Lavender;
    extern const color4 LavenderBlush;
    extern const color4 LawnGreen;
    extern const color4 LemonChiffon;
    extern const color4 LightBlue;
    extern const color4 LightCoral;
    extern const color4 LightCyan;
    extern const color4 LightGoldenrodYellow;
    extern const color4 LightGreen;
    extern const color4 LightGray;
    extern const color4 LightPink;
    extern const color4 LightSalmon;
    extern const color4 LightSeaGreen;
    extern const color4 LightSkyBlue;
    extern const color4 LightSlateGray;
    extern const color4 LightSteelBlue;
    extern const color4 LightYellow;
    extern const color4 Lime;
    extern const color4 LimeGreen;
    extern const color4 Linen;
    extern const color4 Magenta;
    extern const color4 Maroon;
    extern const color4 MediumAquamarine;
    extern const color4 MediumBlue;
    extern const color4 MediumOrchid;
    extern const color4 MediumPurple;
    extern const color4 MediumSeaGreen;
    extern const color4 MediumSlateBlue;
    extern const color4 MediumSpringGreen;
    extern const color4 MediumTurquoise;
    extern const color4 MediumVioletRed;
    extern const color4 MidnightBlue;
    extern const color4 MintCream;
    extern const color4 MistyRose;
    extern const color4 Moccasin;
    extern const color4 NavajoWhite;
    extern const color4 Navy;
    extern const color4 OldLace;
    extern const color4 Olive;
    extern const color4 OliveDrab;
    extern const color4 Orange;
    extern const color4 OrangeRed;
    extern const color4 Orchid;
    extern const color4 PaleGoldenrod;
    extern const color4 PaleGreen;
    extern const color4 PaleTurquoise;
    extern const color4 PaleVioletRed;
    extern const color4 PapayaWhip;
    extern const color4 PeachPuff;
    extern const color4 Peru;
    extern const color4 Pink;
    extern const color4 Plum;
    extern const color4 PowderBlue;
    extern const color4 Purple;
    extern const color4 Red;
    extern const color4 RosyBrown;
    extern const color4 RoyalBlue;
    extern const color4 SaddleBrown;
    extern const color4 Salmon;
    extern const color4 SandyBrown;
    extern const color4 SeaGreen;
    extern const color4 SeaShell;
    extern const color4 Sienna;
    extern const color4 Silver;
    extern const color4 SkyBlue;
    extern const color4 SlateBlue;
    extern const color4 SlateGray;
    extern const color4 Snow;
    extern const color4 SpringGreen;
    extern const color4 SteelBlue;
    extern const color4 Tan;
    extern const color4 Teal;
    extern const color4 Thistle;
    extern const color4 Tomato;
    extern const color4 Turquoise;
    extern const color4 Violet;
    extern const color4 Wheat;
    extern const color4 White;
    extern const color4 WhiteSmoke;
    extern const color4 Yellow;
    extern const color4 YellowGreen;

    f32v3 convertRGBToHSL(const f32v3& val);
    f32v3 convertHSLToRGB(const f32v3& val);
}

#endif // colors_h__