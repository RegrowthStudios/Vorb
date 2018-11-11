//
// Viewport.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Viewport.h
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
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/Event.hpp"
#include "Vorb/VorbPreDecl.inl"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/IGameScreen.h"

DECL_VG(class SpriteFont; class SpriteBatch)

namespace vorb {
    namespace ui {

        // Forward Declarations
        class GameWindow;

        // TODO(Matthew): The current rendering model involves traversing the entire UI structure to add drawables on each frame. This isn't obviously too slow so for now it's good enough, and provides a SUPER easy way to get depth support in a way that makes sense (supporting z-index contexts a la CSS). However, we can do better potentially if we can utilise the depth testing of OpenGL. We can probably preserve this and the flat array inside UIRenderer if we store information about how deep we are into the UI structure in each widget, but I haven't delved into the notion yet.
        class Viewport : public Widget {
        public:
            Viewport(const GameWindow* window = nullptr);
            virtual ~Viewport();

            /*! @brief Initializes the viewport and its renderer.
             *
             * @param name: The name of the viewport.
             * @param dimensions: Position and size of the viewport.
             * @param defaultFont: The optional default font to use.
             * @param defaultFont: The optional SpriteBatch to use.
             */
            virtual void init(const nString& name, const f32v4& dimensions, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
            /*! @brief Initializes the viewport and its renderer.
             *
             * @param name: The name of the viewport.
             * @param position: Position of the viewport.
             * @param size: Size of the viewport.
             * @param defaultFont: The optional default font to use.
             * @param defaultFont: The optional SpriteBatch to use.
             */
            virtual void init(const nString& name, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
            /*! @brief Frees all resources. */
            virtual void dispose() override;

            /*! \brief Set up events. */
            virtual void enable() override;
            /*! \brief Unsubscribe from events. */
            virtual void disable() override;

            /*! @brief Updates the viewport and all descendant widgets.
             * 
             * @param dt: Time since the last update.
             */
            virtual void update(f32 dt = 0.0f) override;

            /*! @brief Draws the Form. */
            virtual void draw();

            /*! \brief Adds all drawables to the UIRenderer */
            virtual void addDrawables(UIRenderer&) override { /* Empty */ }

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual       UIRenderer* getRenderer()         { return &m_renderer; }
            virtual const GameWindow* getGameWindow() const { return m_window; }

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual void setGameWindow(const GameWindow* window);
        protected:
            virtual void calculateDrawables() override { /* Empty */ }

            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onResize(Sender, const WindowResizeEvent&);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            UIRenderer        m_renderer; ///< The UI Renderer.
            const GameWindow* m_window;   ///< The window to which this UI is drawn.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Form_h__
