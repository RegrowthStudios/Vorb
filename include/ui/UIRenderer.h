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
#include "Drawables.h"
#include <vector>

DECL_VG(class SpriteBatch;
        class SpriteFont)

namespace vorb {
    namespace ui {
        class UIRenderer {
        public:
            UIRenderer();
            virtual ~UIRenderer();
            /*! @brief Adds an IDrawable to be drawn
             * 
             * @param drawable The IDrawable to render
             */
            virtual void add(const IDrawable* drawable);
            /*! @brief Removes an IDrawable from render list
             * 
             * @param drawable: The IDrawable to remove 
             * @return true if successfully removed
             */
            virtual void remove(const IDrawable* drawable);
            /*! @brief Frees resources used by renderer */
            virtual void dispose();
            /*! @brief Draws all IDrawables held by this renderer
             * 
             * @param sb: The spritebatch to use in rendering
             */
            virtual void draw(vg::SpriteBatch* sb);

        protected:
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            std::map<const IDrawable*, ui32> m_drawableLookup; ///< Maps IDrawables to vector indices for m_drawables
            std::vector<const IDrawable*> m_drawables; ///< Vector for draw iteration
            VGTexture m_defaultTexture = 0; ///< Default texture if drawable doesn't have one
            vg::SpriteFont* m_defaultFont = nullptr; ///< Default font if drawable doesn't have one
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UIRenderer_h__
