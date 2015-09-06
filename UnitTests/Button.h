//
// Button.h
// Vorb Engine
//
// Created by Dylan Scheltens on 27 August 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Button.h
* @brief
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

#include <include/ui/Drawables.h>

#include "Widget.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class Button : public Widget {
        public:
            /*! @brief Default constructor. */
            Button();

            /*! @brief Constructor that sets name, position, and dimensions.
            *
            * @param name: Name of the container.
            */
            Button(const nString& name);

            /*! @brief Default destructor. */
            virtual ~Button();

            /*! @brief Adds all drawables to the UIRenderer.
            *
            * @param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) override;

            /*! @brief Updates the widget. */
            virtual void update(int flags, f32 dt) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_drawableRect.getTexture(); }
            virtual const vorb::graphics::SpriteFont* getFont() const override { return m_drawableText.getFont(); }
            virtual const nString& getText() const { return m_drawableText.getText(); }
            virtual const vg::TextAlign& getTextAlign() const { return m_drawableText.getTextAlign(); }
            virtual const f32v2& getTextScale() const { return m_drawableText.getTextScale(); }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setFont(const vorb::graphics::SpriteFont* font) override;
            virtual void setDimensions(const f32v2& dimensions) override;
            virtual void setWidth(f32 width) override;
            virtual void setHeight(f32 height) override;
            virtual void setPosition(const f32v2& position) override;
            virtual void setX(f32 x) override;
            virtual void setY(f32 y) override;
            virtual void setText(const nString& text);
            virtual void setTextAlign(vg::TextAlign textAlign);
            virtual void setTextScale(const f32v2& textScale);

        protected:
            virtual void updateTextPosition();
            virtual void refreshDrawables();

            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e) override;

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect m_drawableRect, m_drawnRect;
            DrawableText m_drawableText, m_drawnText;
            const vg::SpriteFont* m_defaultFont = nullptr;
        };
    }
}

namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__