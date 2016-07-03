//
// Enums.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file Enums.h
 * 
 * \brief 
 */

#pragma once

#ifndef Vorb_Enums_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Enums_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        union ClearBits {
        public:
            enum {
                COLOR = 1 << 0,
                DEPTH = 1 << 1,
                STENCIL = 1 << 2,
            };

            ClearBits(ui8 v = 0) : 
                value(v) {
                // Empty
            }
            ClearBits(bool c, bool d, bool s) :
                color(c), 
                depth(d),
                stencil(s) {
                // Empty
            }

            struct {
                bool color : 1;
                bool depth : 1;
                bool stencil : 1;
            };
            ui8 value;
        };
    }
}

#endif // !Vorb_Enums_h__
