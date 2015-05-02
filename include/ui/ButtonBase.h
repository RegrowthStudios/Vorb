//
// ButtonBase.h
// Vorb Engine
//
// Created by Benjamin Arnold on 2 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ButtonBase.h
* @brief 
* Abstract base class for all button widget.
*
*/

#pragma once

#ifndef Vorb_ButtonBase_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ButtonBase_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "Widget.h"
#include "../graphics/gtypes.h"
#include "Drawables.h"

namespace vorb {
    namespace ui {
        class ButtonBase : public Widget {
        public:
            ButtonBase();
            virtual ~ButtonBase() = 0;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_drawableRect.getTexture(); }
            virtual const vorb::graphics::SpriteFont* getFont() const override { return m_drawableText.getFont(); }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setDimensions(const f32v2& dimensions) override;
            virtual void setFont(vorb::graphics::SpriteFont* font) override { m_drawableText.setFont(font); }
            virtual void setHeight(f32 height) override;
            virtual void setPosition(const f32v2& position) override;
            virtual void setTexture(VGTexture texture) { m_drawableRect.setTexture(texture); }
            virtual void setWidth(f32 width) override;
            virtual void setX(f32 x) override;
            virtual void setY(f32 y) override;
        protected:
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect m_drawableRect;
            DrawableText m_drawableText;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ButtonBase_h__