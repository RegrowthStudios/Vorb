//
// Button.h
// Vorb Engine
//
// Created by Benjamin Arnold on 2 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file Button.h
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
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/Drawables.h"
#include "Vorb/ui/widgets/TextWidget.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class Button : public TextWidget {
        public:
            /*! \brief Default constructor. */
            Button();
            /*! \brief Copy constructor. */
            Button(const Button&);
            /*! \brief Move constructor. */
            Button(Button&&);

            /*! \brief Copy assignment operator. */
            Button& operator=(const Button&);
            /*! \brief Move assignment operator. */
            Button& operator=(Button&&);

            /*! \brief Adds all drawables to the UIRenderer */
            virtual void addDrawables(UIRenderer& renderer) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            // TODO(Matthew): Get gradients back (update script funcs).
            virtual      const VGTexture& getTexture()        const { return m_texture;         }
            virtual      const VGTexture& getHoverTexture()   const { return m_hoverTexture;    }
            virtual         const color4& getBackColor()      const { return m_backColor1;      }
            virtual         const color4& getBackHoverColor() const { return m_backHoverColor1; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTexture(VGTexture texture);
            virtual void setHoverTexture(VGTexture texture);
            virtual void setBackColor(const color4& color);
            virtual void setBackColorGrad(const color4& color1, const color4& color2, vg::GradientType grad);
            virtual void setBackHoverColor(const color4& color);
            virtual void setBackHoverColorGrad(const color4& color1, const color4& color2, vg::GradientType grad);

        protected:
            /*! \brief Initialiser for general set-up. */
            virtual void initBase() override;

            virtual void calculateDrawables() override;

            /*! Updates the colours set for the drawables. */
            virtual void updateColor();

            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e) override;

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect          m_drawableRect;
            vg::GradientType      m_gradBack,        m_gradHover;
            color4                m_backColor1,      m_backColor2;
            color4                m_backHoverColor1, m_backHoverColor2;
            VGTexture             m_texture,         m_hoverTexture;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Button_h__
