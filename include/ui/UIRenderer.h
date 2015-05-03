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
            /*! @brief Adds an drawable to be drawn
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
            /*! @brief Draws all IDrawables held by this renderer
             * 
             * @param sb: The SpriteBatch to use in rendering
             */
            virtual void draw(vg::SpriteBatch* sb);

        protected:
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            std::map<const Widget*, std::vector<ui32> > m_widgetLookup; ///< Maps Widgets to vector indices for lookups
            std::vector<DrawFunc> m_drawFuncs; ///< Vector for draw iteration
            std::vector<RefreshFunc> m_refreshFuncs; ///< Vector for refresh iteration
            VGTexture m_defaultTexture = 0; ///< Default texture if drawable doesn't have one
            vg::SpriteFont* m_defaultFont = nullptr; ///< Default font if drawable doesn't have one
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_UIRenderer_h__
