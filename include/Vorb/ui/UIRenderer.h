//
// UIRenderer.h
// Vorb Engine
//
// Created by Benjamin Arnold on 28 Apr 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file UIRenderer.h
* \brief 
* Renderer for Vorb UI
*
*/

#pragma once

#ifndef Vorb_UIRenderer_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UIRenderer_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <vector>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "../Event.hpp"
#include "../VorbPreDecl.inl"
#include "../graphics/SpriteBatch.h"
#include "../graphics/GLRenderTarget.h"
#include "Drawables.h"

DECL_VG(class SpriteFont; class DepthState; class GLProgram; class RasterizerState; class SamplerState)

namespace vorb {
    namespace ui {

        // Forward Declarations
        class IWidget;
        class GameWindow;

        // TODO(Matthew): The spritebatch does FUNKY shit when text extends beyond clip rect, trying to do some wrapping but it's borked.
        class UIRenderer {
        public:
            using DrawFunc = Delegate<vg::SpriteBatch*>;

            /// Constructor
            UIRenderer();
            /// Destructor
            virtual ~UIRenderer();

            /*! \brief Initializes the renderer
            *
            * Call on the GL thread after GL initialization
            *
            * \param defaultFont: Optional default font to use for rendering.
            * \param spriteBatch: Optional SpriteBatch to use. Will not init if passed in. If nullptr, will init it's own.
            */
            virtual void init(vg::SpriteFont* defaultFont = nullptr,
                              vg::SpriteBatch* spriteBatch = nullptr);

            /*! \brief Frees resources used by renderer */
            virtual void dispose();

            /*!
             * \brief Prepares the UIRenderer for rendering a UI.
             */
            virtual void prepare() { m_sb->begin(); }

            /*! \brief Adds a drawable to be rendererd. */
            virtual void add(const DrawFunc& drawFunc);

            /*!
             * \brief Renders the UI that has been prepared.
             */
            virtual void render(const f32m4& mWorld, const f32m4& mCamera, const vg::SamplerState* ss /*= nullptr*/, const vg::DepthState* ds /*= nullptr*/, const vg::RasterizerState* rs /*= nullptr*/, vg::GLProgram* shader /*= nullptr*/);
            virtual void render(const f32m4& mWorld, const f32v2& screenSize, const vg::SamplerState* ss = nullptr, const vg::DepthState* ds = nullptr, const vg::RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
            virtual void render(const f32v2& screenSize, const vg::SamplerState* ss = nullptr, const vg::DepthState* ds = nullptr, const vg::RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);

            /*! \brief Gets the default SpriteFont.
             *
             * It is up to the Widget classes to call this function and set the drawable fonts
             * 
             * \return the SpriteFont
             */
            virtual const vg::SpriteFont* getDefaultFont() const { return m_defaultFont; }
        protected:
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            VGTexture m_defaultTexture = 0; ///< Default texture if drawable doesn't have one
            vg::SpriteFont* m_defaultFont = nullptr; ///< Default font if drawable doesn't have one
            vg::SpriteBatch m_defaultSb; ///< Default SpriteBatch if none specified
            vg::SpriteBatch* m_sb = nullptr; ///< SpriteBatch used for rendering
            bool m_shouldRenderToTexture = false; ///< When true, renders to a framebuffer instead
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UIRenderer_h__
