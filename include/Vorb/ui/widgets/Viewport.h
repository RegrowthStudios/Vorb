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
#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/UIRenderer.h"
#include "Vorb/ui/IGameScreen.h"

DECL_VG(class SpriteFont; class SpriteBatch)

namespace vorb {
    namespace ui {

        // Forward Declarations
        class GameWindow;

        // Some defaults for widgets.
        const PositionType  DEFAULT_POSITION_TYPE = PositionType::STATIC_TO_PARENT;
        const vg::TextAlign DEFAULT_TEXT_ALIGN    = vg::TextAlign::NONE;
        const f32v2         DEFAULT_TEXT_SCALE    = f32v2(1.0f);

        // TODO(Matthew): The current rendering model involves traversing the entire UI structure to add drawables on each frame. This isn't obviously too slow so for now it's good enough, and provides a SUPER easy way to get depth support in a way that makes sense (supporting z-index contexts a la CSS). However, we can do better potentially if we can utilise the depth testing of OpenGL. We can probably preserve this and the flat array inside UIRenderer if we can know how deep the UI structure is at its deepest widget, and where we are in that depth for each widget.
        class Viewport : public Widget {
        public:
            Viewport(const GameWindow* window = nullptr);
            virtual ~Viewport();

#if defined(VORB_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#elif defined(VORB_COMPILER_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
            /*! @brief Initializes the viewport and its renderer.
             *
             * @param name: The name of the viewport.
             * @param dimensions: Position and size of the viewport.
             * @param defaultFont: The optional default font to use.
             * @param spriteBatch: The optional SpriteBatch to use.
             */
            virtual void init(const nString& name, const f32v4& dimensions, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
            /*! @brief Initializes the viewport and its renderer.
             *
             * @param name: The name of the viewport.
             * @param position: Position of the viewport.
             * @param size: Size of the viewport.
             * @param defaultFont: The optional default font to use.
             * @param spriteBatch: The optional SpriteBatch to use.
             */
            virtual void init(const nString& name, const Length2& position, const Length2& size, vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);
#if defined(VORB_COMPILER_CLANG)
#pragma clang diagnostic pop
#elif defined(VORB_COMPILER_GCC)
#pragma GCC diagnostic pop
#endif
            /*! @brief Frees all resources. */
            virtual void dispose(bool thisOnly = false) override;

            /*! \brief Set up events. */
            virtual void enable() override;
            /*! \brief Unsubscribe from events. */
            virtual void disable(bool thisOnly = false) override;

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
