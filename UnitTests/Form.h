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
#include <include/types.h>
#endif // !VORB_USING_PCH\

#include <include/VorbPreDecl.inl>
#include <include/ui/UIRenderer.h>
#include <include/ui/IGameScreen.h>

#include "WidgetContainer.h"

DECL_VG(class SpriteFont; class SpriteBatch)

namespace vorb {
    namespace ui {

        // Forward declarations
        class Widget;

        // Template is the screen type
        class Form : public WidgetContainer {
        public:
            Form();
            virtual ~Form();

            /*! @brief Initializes the Form and it's renderer.
             * 
             * @param name: The name of the screen.
             * @param ownerScreen: The game screen that owns the Form.
             * @param destRect: Position and size of the Form.
             * @param defaultFont: The optional default font to use.
             * @param defaultFont: The optional SpriteBatch to use.
             */
            virtual void init(const nString& name, IGameScreen* ownerScreen, const f32v4& destRect, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
            
            /*! @brief Adds a widget to the Form and initializes it for rendering.
             * 
             * @param widget: The Widget to add.
             * @return true on success.
             */
            virtual void addWidget(Widget* widget) override;
            
            /*! @brief Removes a widget from the Form.
            *
            * @param widget: The Widget to remove.
            * @return true on success.
            */
            virtual void removeWidget(Widget* widget) override;
            
            /*! @brief Updates the widget. */
            virtual void update(int flags, f32 dt) override;
            
            /*! @brief Draws the Form. */
            virtual void draw();
            
            /*! @brief Frees all resources. */
            virtual void dispose() override;

            /*! @brief Registers a custom callback with a widget event.
             * Override this in custom forms to set up callbacks
             * @param callbackName: Name of the custom callback 
             * @return true if callbackName was the name of a function
             */
            virtual bool registerCallback(Widget* w, nString callback) { return false; }

        protected:
            UIRenderer m_renderer; ///< The UI Renderer.
            IGameScreen* m_ownerIGameScreen = nullptr; ///< The Owning screen.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Form_h__
