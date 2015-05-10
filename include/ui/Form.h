//
// Form.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Form.h
* @brief 
* Represents an in-game UI window.
*
*/

#pragma once

#ifndef Vorb_Form_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Form_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH\

#include "../VorbPreDecl.inl"
#include "UIRenderer.h"

DECL_VG(class SpriteFont; class SpriteBatch)

namespace vorb {
    namespace ui {

        // Forward declarations
        class Widget;
        class GameWindow;

        class Form {
        public:
            Form();
            virtual ~Form();

            void init(const GameWindow* ownerWindow, ui32v4 destRect, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);

            void addWidget(Widget* widget);

            void removeWidget(Widget* widget);

            void update(f32 dt = 1.0f);

            void draw();

            void dispose();

        protected:
            UIRenderer m_renderer;
            const GameWindow* m_ownerWindow = nullptr;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Form_h__
