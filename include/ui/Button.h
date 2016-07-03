//
// Button.h
// Vorb Engine
//
// Created by Benjamin Arnold on 2 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file Button.h
 * 
* \brief 
* Abstract base class for all button widget.
*
*/

#pragma once

#ifndef Vorb_Button_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Button_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "Drawables.h"
#include "Widget.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class Button : public Widget {
        public:
            /*! \brief Default constructor. */
            Button(InputDispatcher* dispatcher);
            /*! \brief Constructor that sets name, position, and dimensions.
            *
            * \param name: Name of the control.
            * \param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Button(InputDispatcher* dispatcher, const nString& name, const f32v4& destRect = f32v4(0));
            /*! \brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * \param parent: Parent control object.
            * \param name: Name of the control.
            * \param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Button(IWidgetContainer* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! \brief Default destructor. */
            virtual ~Button(); // TODO(Ben): Maybe make abstract?

            /*! \brief Adds all drawables to the UIRenderer
            *
            * \param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) override;

            /*! \brief Updates the position relative to parent */
            virtual void updatePosition() override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_drawableRect.getTexture(); }
            virtual const vorb::graphics::SpriteFont* getFont() const override { return m_drawableText.getFont(); }
            virtual const color4& getBackColor() const { return m_backColor1; }
            virtual const color4& getBackHoverColor() const { return m_backHoverColor1; }
            virtual const color4& getTextColor() const { return m_textColor; }
            virtual const color4& getTextHoverColor() const { return m_textHoverColor; }
            virtual const nString& getText() const { return m_drawableText.getText(); }
            virtual const vg::TextAlign& getTextAlign() const { return m_drawableText.getTextAlign(); }
            virtual const f32v2& getTextScale() const { return m_drawableText.getTextScale(); }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setDestRect(const f32v4& destRect) override;
            virtual void setDimensions(const f32v2& dimensions) override;
            virtual void setFont(const vorb::graphics::SpriteFont* font) override;
            virtual void setHeight(f32 height) override;
            virtual void setPosition(const f32v2& position) override;
            virtual void setTexture(VGTexture texture);
            virtual void setWidth(f32 width) override;
            virtual void setX(f32 x) override;
            virtual void setY(f32 y) override;
            virtual void setBackColor(const color4& color);
            virtual void setBackColorGrad(const color4& color1, const color4& color2, vg::GradientType grad);
            virtual void setBackHoverColor(const color4& color);
            virtual void setBackHoverColorGrad(const color4& color1, const color4& color2, vg::GradientType grad);
            virtual void setText(const nString& text);
            virtual void setTextColor(const color4& color);
            virtual void setTextHoverColor(const color4& color);
            virtual void setTextAlign(vg::TextAlign textAlign);
            virtual void setTextScale(const f32v2& textScale);

        protected:
            virtual void updateColor();
            virtual void updateTextPosition();
            virtual void refreshDrawables();

            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e) override;
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect m_drawableRect, m_drawnRect;
            DrawableText m_drawableText, m_drawnText;
            vg::GradientType gradBack = vg::GradientType::NONE, gradHover = vg::GradientType::NONE;
            color4 m_backColor1 = color::LightGray, m_backColor2 = color::LightGray;
            color4 m_backHoverColor1 = color::AliceBlue, m_backHoverColor2 = color::AliceBlue;
            color4 m_textColor = color::Black, m_textHoverColor = color::Black;
            const vg::SpriteFont* m_defaultFont = nullptr;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Button_h__
