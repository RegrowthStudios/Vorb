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
#include "../graphics/SpriteBatch.h"
#include "../graphics/gtypes.h"
#include "../colors.h"
#include "WidgetStyle.h"

#define CLIP_RECT_DEFAULT f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX)

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        class IDrawable {
        public:
            virtual void draw(vg::SpriteBatch* spriteBatch) const = 0;
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const color4& getColor() const { return m_color1; }
            virtual const color4& getColor1() const { return m_color1; }
            virtual const color4& getColor2() const { return m_color2; }
            virtual const f32& getLayerDepth() const { return m_layerDepth; }
            virtual const f32& getX() const { return m_position.x; }
            virtual const f32& getY() const { return m_position.y; }
            virtual const f32v2& getPosition() const { return m_position; }
            virtual const vg::GradientType& getGradientType() const { return m_grad; }
            virtual const f32v4& getClipRect() const { return m_clipRect; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setColor(const color4& color) { m_color1 = m_color2 = color; }
            virtual void setColor1(const color4& color) { m_color1 = color; }
            virtual void setColor2(const color4& color) { m_color2 = color; }
            virtual void setLayerDepth(f32 layerDepth) { m_layerDepth = layerDepth; }
            virtual void setPosition(const f32v2& position) { m_position = position; }
            virtual void setX(f32 x) { m_position.x = x; }
            virtual void setY(f32 y) { m_position.y = y; }
            virtual void setGradientType(vg::GradientType grad) { m_grad = grad; }
            virtual void setClipRect(const f32v4& clipRect) { m_clipRect = clipRect; }

        protected:
            vg::GradientType m_grad = vg::GradientType::NONE;
            color4 m_color1 = color::LightGray;
            color4 m_color2 = color::LightGray;
            f32 m_layerDepth = 0.0f; ///< Depth used in SpriteBatch rendering
            f32v2 m_position = f32v2(0.0f); ///< Position of the control
            f32v4 m_clipRect = CLIP_RECT_DEFAULT;
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
            DrawableText() { m_color1 = color::Black; }
            virtual void draw(vg::SpriteBatch* spriteBatch) const override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            virtual const nString& getText() const { return m_text; }
            virtual const vg::TextAlign& getTextAlign() const { return m_textAlign; }
            virtual const f32v2& getTextScale() const { return m_scale; }
            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setText(const nString& text) { m_text = text; }
            virtual void setTextAlign(vg::TextAlign textAlign) { m_textAlign = textAlign; }
            virtual void setTextScale(const f32v2& textScale) { m_scale = textScale; }
            
        private:
            const vg::SpriteFont* m_font = nullptr; ///< SpriteFont handle
            f32v2 m_scale = f32v2(1.0f); ///< Scale for font rendering
            nString m_text = ""; ///< Text to be drawn
            vg::TextAlign m_textAlign = vg::TextAlign::TOP_LEFT; ///< Alignment of the text
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Drawables_h__
