//
// WidgetStyle.h
// Vorb Engine
//
// Created by Benjamin Arnold on 2 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file WidgetStyle.h
* @brief 
* StyleSheet definition for widgets
*
*/

#pragma once

#ifndef Vorb_WidgetStyle_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetStyle_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/colors.h"

#define W_BORDER_LEFT 0
#define W_BORDER_RIGHT 1
#define W_BORDER_TOP 2
#define W_BORDER_BOTTOM 3

namespace vorb {
    namespace ui {
        enum class BorderStyle {
            NONE, DOTTED, DASHED, SOLID,
            DOUBLE, GROOVE, RIDGE, INSET,
            OUTSET, INITIAL
        };
        // TODO(Ben): rethink this
       /* class WidgetStyle {
        public:
            WidgetStyle() {
                memset(borderColor, 0, sizeof(borderColor));
                memset(borderStyle, 0, sizeof(borderStyle));
            }

            color4 textColor = color::Red; ///< Color of the text
            color4 backgroundColor = color::White; ///< Color of the background
            f32v4 borderWidth = f32v4(0.0f);
            color4 borderColor[4];
            BorderStyle borderStyle[4];
        };*/
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetStyle_h__
