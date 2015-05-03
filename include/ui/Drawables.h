//
// Drawables.h
// Vorb Engine
//
// Created by Benjamin Arnold on 2 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Drawables.h
* @brief 
* Defines drawable components for controls
*
*/

#pragma once

#ifndef Vorb_Drawables_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Drawables_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "../VorbPreDecl.inl"
#include "../graphics/gtypes.h"
#include "WidgetStyle.h"

DECL_VG(class SpriteBatch;
        class SpriteFont)

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class IDrawable {
        public:
            virtual void draw(vg::SpriteBatch* spriteBatch) const = 0;
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const color4& getTextColor() const { return m_style.textColor; }
            virtual const color4& getBackgroundColor() const { return m_style.backgroundColor; }
            virtual const f32& getLayerDepth() const { return m_layerDepth; }
            virtual const f32& getX() const { return m_position.x; }
            virtual const f32& getY() const { return m_position.y; }
            virtual const f32v2& getPosition() const { return m_position; }
            virtual const UIRenderer* getRenderer() const { return m_renderer; }
            virtual const WidgetStyle& getStyle() const { return m_style; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTextColor(const color4& color) { m_style.textColor = color; }
            virtual void setBackgroundColor(const color4& color) { m_style.backgroundColor = color; }
            virtual void setLayerDepth(f32 layerDepth) { m_layerDepth = layerDepth; }
            virtual void setPosition(const f32v2& position) { m_position = position; }
            virtual void setX(f32 x) { m_position.x = x; }
            virtual void setY(f32 y) { m_position.y = y; }
            virtual void setRenderer(const UIRenderer* renderer) { m_renderer = renderer; }
            virtual void setStyle(const WidgetStyle& style) { m_style = style; }
        protected:
            WidgetStyle m_style; ///< The style of the widget
            const UIRenderer* m_renderer = nullptr; ///< Parent renderer TODO(Ben): Multiple renderers?
            f32 m_layerDepth = 0.0f; ///< Depth used in SpriteBatch rendering
            f32v2 m_position = f32v2(0.0f); ///< Position of the control
        };
        class DrawableRect : public IDrawable {
        public:
            virtual void draw(vg::SpriteBatch* spriteBatch) const override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_texture; }
            virtual const f32v2& getDimensions() const { return m_dimensions; }
            virtual const f32& getWidth() const { return m_dimensions.x; }
            virtual const f32& getHeight() const { return m_dimensions.y; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setDimensions(const f32v2& dimensions) { m_dimensions = dimensions; }
            virtual void setTexture(VGTexture texture) { m_texture = texture; }
            virtual void setHeight(f32 height) { m_dimensions.y = height; }
            virtual void setWidth(f32 width) { m_dimensions.x = width; }

        protected:
            f32v2 m_dimensions; ///< Dimensions of the rectangle
            VGTexture m_texture = 0; ///< The background texture
        };
        class DrawableText : public IDrawable {
        public:
            virtual void draw(vg::SpriteBatch* spriteBatch) const override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setFont(vorb::graphics::SpriteFont* font);

        private:
            vg::SpriteFont* m_font = nullptr; ///< SpriteFont handle
            f32 m_scale = 1.0f; ///< Scale for font rendering
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Drawables_h__
