//
// Label.h
// Vorb Engine
//
// Created by Benjamin Arnold on 15 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Label.h
* @brief 
* A simple text label widget.
*
*/

#pragma once

#ifndef Vorb_Label_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Label_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/Drawables.h"
#include "Vorb/ui/Widget.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class Label : public Widget {
        public:
            /*! @brief Default constructor. */
            Label();
            /*! @brief Constructor that sets name, position, and dimensions.
            *
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Label(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * @param parent: Parent control object.
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Label(IWidget* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Default destructor. */
            virtual ~Label();

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) override;

            /*! @brief Updates the position relative to parent */
            // virtual void updatePosition() override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const vorb::graphics::SpriteFont* getFont() const override { return m_drawableText.getFont(); }
            virtual const color4& getTextColor() const { return m_drawableText.getColor(); }
            virtual const nString& getText() const { return m_drawableText.getText(); }
            virtual const vg::TextAlign& getTextAlign() const { return m_drawableText.getTextAlign(); }
            virtual const f32v2& getTextScale() const { return m_drawableText.getTextScale(); }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            // virtual void setDestRect(const f32v4& destRect) override;
            // virtual void setDimensions(const f32v2& dimensions) override;
            virtual void setFont(const vorb::graphics::SpriteFont* font) override;
            // virtual void setHeight(f32 height) override;
            // virtual void setPosition(const f32v2& position) override;
            // virtual void setWidth(f32 width) override;
            // virtual void setX(f32 x) override;
            // virtual void setY(f32 y) override;
            virtual void setText(const nString& text);
            virtual void setTextColor(const color4& color);
            virtual void setTextAlign(vg::TextAlign textAlign);
            virtual void setTextScale(const f32v2& textScale);

        protected:
            virtual void updateTextPosition();
            virtual void refreshDrawables();

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableText m_drawableText, m_drawnText;
            const vg::SpriteFont* m_defaultFont = nullptr;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Label_h__
