//
// TextWidget.h
// Vorb Engine
//
// Created by Matthew Marshall on 3 November 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file TextWidget.h
* \brief Provides a simple extension of Widget to incorporate text.
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
#include "Vorb/ui/widgets/Widget.h"

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


            /*! \brief Initialiser that sets name, position, size, and Z-index.
            *
            * \param name: Name of the widget.
            * \param dimensions: Position and size of widget.
            * \param zIndex: Z-index of widget.
            */
            virtual void init(const nString& name, const f32v4& dimensions = f32v4(0.0f), ui16 zIndex = 0) override;
            /*! \brief Initialiser that sets name, position, size, and Z-index.
             *
            * \param name: Name of the widget.
            * \param position: Position of widget.
            * \param size: Size of widget.
            * \param zIndex: Z-index of widget.
             */
            virtual void init(const nString& name, const Length2& position, const Length2& size, ui16 zIndex = 0);
            /*! \brief Initialiser that sets parent, name, position, size, and Z-index.
            *
            * \param parent: Parent of the widget.
            * \param name: Name of the widget.
            * \param dimensions: Position and size of widget.
            * \param zIndex: Z-index of widget.
            */
            virtual void init(IWidget* parent, const nString& name, const f32v4& dimensions = f32v4(0.0f), ui16 zIndex = 0) override;
            /*! \brief Initialiser that sets parent, name, position, size, and Z-index.
             *
             * \param parent: Parent of the widget.
             * \param name: Name of the widget.
             * \param position: Position of widget.
             * \param size: Size of widget.
             * \param zIndex: Z-index of widget.
             */
            virtual void init(IWidget* parent, const nString& name, const Length2& position, const Length2& size, ui16 zIndex = 0);

            /*! \brief Adds all drawables to the UIRenderer. */
            virtual void addDrawables(UIRenderer& renderer) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const vg::SpriteFont* getFont()           const { return m_drawableText.getFont();           }
            virtual         const color4& getTextColor()      const { return m_drawableText.getColor();          }
            virtual        const nString& getText()           const { return m_drawableText.getText();           }
            virtual  const vg::TextAlign& getTextAlign()      const { return m_drawableText.getTextAlign();      }
            virtual          const f32v2& getTextScale()      const { return m_drawableText.getTextScale();      }
            virtual const vg::SpriteFont* getHoverFont()      const { return m_drawableHoverText.getFont();      }
            virtual         const color4& getHoverTextColor() const { return m_drawableHoverText.getColor();     }
            virtual        const nString& getHoverText()      const { return m_drawableHoverText.getText();      }
            virtual  const vg::TextAlign& getHoverTextAlign() const { return m_drawableHoverText.getTextAlign(); }
            virtual          const f32v2& getHoverTextScale() const { return m_drawableHoverText.getTextScale(); }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setFont(const vg::SpriteFont* font);
            virtual void setText(const nString& text);
            virtual void setTextColor(const color4& color);
            virtual void setTextAlign(vg::TextAlign textAlign);
            virtual void setTextScale(const f32v2& textScale);
            virtual void setHoverFont(const vg::SpriteFont* font);
            virtual void setHoverText(const nString& text);
            virtual void setHoverTextColor(const color4& color);
            virtual void setHoverTextAlign(vg::TextAlign textAlign);
            virtual void setHoverTextScale(const f32v2& textScale);

        protected:
            virtual void calculateDrawables() override;

            virtual void updateTextPosition();

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableText          m_drawableText;
            DrawableText          m_drawableHoverText;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Label_h__
