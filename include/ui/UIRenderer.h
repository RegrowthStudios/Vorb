//
// UIRenderer.h
// Vorb Engine
//
// Created by Benjamin Arnold on 28 Apr 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file UIRenderer.h
* @brief 
* Renderer for Vorb UI
*
*/

#pragma once

#ifndef Vorb_UIRenderer_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UIRenderer_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "../VorbPreDecl.inl"

DECL_VG(class SpriteBatch;
        class SpriteFont)

namespace vorb {
    namespace ui {
        class UIRenderer {
        public:
            UIRenderer();
            virtual ~UIRenderer();

        protected:

        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UIRenderer_h__
