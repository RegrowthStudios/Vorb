//
// TextWidget.h
// Vorb Engine
//
// Created by Matthew Marshall on 3 November 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file TextWidget.h
* \brief 
* 
*
*/

#pragma once

#ifndef Vorb_TextWidget_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TextWidget_h__
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

        class TextWidget : public Widget {
        public:
            /*! \brief Default constructor. */
            TextWidget();
            /*! \brief Default destructor. */
            virtual ~TextWidget();

            /*! \brief Adds all drawables to the UIRenderer. */
            virtual void addDrawables() override;
            /*! \brief Refresh drawables. */
            virtual void refreshDrawables() override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const vg::SpriteFont* getFont()      const { return m_drawableText.getFont();      }
            virtual         const color4& getTextColor() const { return m_drawableText.getColor();     }
            virtual        const nString& getText()      const { return m_drawableText.getText();      }
            virtual  const vg::TextAlign& getTextAlign() const { return m_drawableText.getTextAlign(); }
            virtual          const f32v2& getTextScale() const { return m_drawableText.getTextScale(); }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setFont(const vg::SpriteFont* font);
            virtual void setText(const nString& text);
            virtual void setTextColor(const color4& color);
            virtual void setTextAlign(vg::TextAlign textAlign);
            virtual void setTextScale(const f32v2& textScale);

        protected:
            virtual void calculateDrawables() override;

            virtual void updateTextPosition();

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableText          m_drawableText, m_drawnText;
            const vg::SpriteFont* m_defaultFont;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Label_h__
