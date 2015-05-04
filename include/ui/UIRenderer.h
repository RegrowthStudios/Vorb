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
#include "../graphics/SpriteBatch.h"
#include "Drawables.h"
#include <vector>

DECL_VG(class SpriteFont;
        class DepthState;
        class GLProgram;
        class RasterizerState;
        class SamplerState)

namespace vorb {
    namespace ui {

        // Forward Declarations
        class Widget;

        class UIRenderer {
        public:
            typedef Delegate<vg::SpriteBatch*> DrawFunc;
            typedef Delegate<> RefreshFunc;

            /// Constructor
            UIRenderer();
            /// Destructor
            virtual ~UIRenderer();

            /*! @brief Initializes the renderer
            *
            * Call on the GL thread after GL initialization
            *
            * @param defaultFont: Optional default font to use for rendering.
            * @param spriteBatch: Optional SpriteBatch to use. Will not init if passed in. If nullptr, will init it's own.
            */
            virtual void init(vg::SpriteFont* defaultFont = nullptr, vg::SpriteBatch* spriteBatch = nullptr);

            /*! @brief Adds an drawable to be drawn
             * 
             * The init function should have already been called.
             *
             * @param drawable The drawable to render
             */
            virtual void add(const Widget* widget, const DrawFunc& drawFunc, const RefreshFunc& refreshFunc);
            /*! @brief Removes all drawables for a widget
             * 
             * @param widget: The Widget who's drawables should be removed
             * @return true if successfully removed
             */
            virtual bool remove(const Widget* widget);
            /*! @brief Frees resources used by renderer */
            virtual void dispose();
            /*! @brief Draws all IDrawables held by this renderer */
            virtual void draw(f32m4 mWorld, f32m4 mCamera, const vg::SamplerState* ss = nullptr, const vg::DepthState* ds = nullptr, const vg::RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
            virtual void draw(f32m4 mWorld, const f32v2& screenSize, const vg::SamplerState* ss = nullptr, const vg::DepthState* ds = nullptr, const vg::RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);
            virtual void draw(const f32v2& screenSize, const vg::SamplerState* ss = nullptr, const vg::DepthState* ds = nullptr, const vg::RasterizerState* rs = nullptr, vg::GLProgram* shader = nullptr);

            /*! @brief Gets the default SpriteFont.
             *
             * It is up to  the Widget classes to call this function and set the drawable fonts
             * 
             * @return the SpriteFont
             */
            virtual const vg::SpriteFont* getDefaultFont() const { return m_defaultFont; }
        protected:
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            std::map<const Widget*, std::vector<ui32> > m_widgetLookup; ///< Maps Widgets to vector indices for lookups
            std::vector<DrawFunc> m_drawFuncs; ///< Vector for draw iteration
            std::vector<RefreshFunc> m_refreshFuncs; ///< Vector for refresh iteration
            VGTexture m_defaultTexture = 0; ///< Default texture if drawable doesn't have one
            vg::SpriteFont* m_defaultFont = nullptr; ///< Default font if drawable doesn't have one
            vg::SpriteBatch m_defaultSb; ///< Default SpriteBatch if none specified
            vg::SpriteBatch* m_sb = nullptr; ///< SpriteBatch used for rendering
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UIRenderer_h__
