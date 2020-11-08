//
// Label.h
// Vorb Engine
//
// Created by Benjamin Arnold on 15 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file Label.h
* \brief 
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
#include "Vorb/ui/widgets/TextWidget.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class Label : public TextWidget {
        public:
            /*! \brief Default constructor. */
            Label();
            /*! \brief Copy constructor. */
            Label(const Label&);
            /*! \brief Move constructor. */
            Label(Label&&);

            /*! \brief Copy assignment operator. */
            Label& operator=(const Label&);
            /*! \brief Move assignment operator. */
            Label& operator=(Label&&);

            /*! \brief Adds all drawables to the UIRenderer. */
            virtual void addDrawables(UIRenderer& renderer) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const color4& getLabelColor()        const { return m_labelColor;         }
            virtual const color4& getLabelHoverColor()   const { return m_labelHoverColor;    }
            virtual     VGTexture getLabelTexture()      const { return m_labelTexture;       }
            virtual     VGTexture getLabelHoverTexture() const { return m_labelHoverTexture;  }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setLabelColor(const color4& color);
            virtual void setLabelHoverColor(const color4& color);
            virtual void setLabelTexture(VGTexture texture);
            virtual void setLabelHoverTexture(VGTexture texture);

        protected:
            /*! \brief Initialiser for adding buttons, panel and setting up events. */
            virtual void initBase() override;

            virtual void calculateDrawables() override;

            virtual void updateColor();

            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect          m_drawableRect;
            color4                m_labelColor,   m_labelHoverColor;
            VGTexture             m_labelTexture, m_labelHoverTexture;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Label_h__
