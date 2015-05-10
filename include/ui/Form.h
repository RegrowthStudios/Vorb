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
#include "IWidgetContainer.h"
#include "UIRenderer.h"

DECL_VG(class SpriteFont; class SpriteBatch)

namespace vorb {
    namespace ui {

        // Forward declarations
        class Widget;
        class GameWindow;

        class Form : public IWidgetContainer {
        public:
            Form();
            virtual ~Form();
            /*! @brief Initializes the Form and it's renderer.
             * 
             * @param ownerWindow: The game window that owns the Form.
             * @param destRect: Position and size of the Form.
             * @param defaultFont: The optional default font to use.
             * @param defaultFont: The optional SpriteBatch to use.
             */
            virtual void init(const GameWindow* ownerWindow, ui32v4 destRect, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
            /*! @brief Adds a widget to the Form and initializes it for rendering.
             * 
             * @param widget: The Widget to add.
             * @return true on success.
             */
            virtual bool addWidget(Widget* widget) override;
            /*! @brief Removes a widget from the Form.
            *
            * @param widget: The Widget to remove.
            * @return true on success.
            */
            virtual bool removeWidget(Widget* widget) override;
            /* Updates the position of the Form. */
            virtual void updatePosition() override;
            /*! @brief Updates all the widgets in the Form.
             * 
             * @param dt: deltatime
             */
            virtual void update(f32 dt = 1.0f);
            /*! @brief Draws the Form. */
            virtual void draw();
            /*! @brief Frees all resources. */
            virtual void dispose() override;

        protected:
            UIRenderer m_renderer; ///< The UI Renderer.
            const GameWindow* m_ownerWindow = nullptr; ///< The Owning window.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Form_h__
