//
// IButton.h
// Vorb Engine
//
// Created by Benjamin Arnold on 2 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IButton.h
* @brief 
* Abstract base class for all button widget.
*
*/

#pragma once

#ifndef Vorb_IButton_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IButton_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "Widget.h"
#include "../graphics/gtypes.h"
#include "Drawables.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class IButton : public Widget {
        public:
            /*! @brief Default constructor. */
            IButton();
            /*! @brief Constructor that sets name, position, and dimensions.
            *
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            IButton(const nString& name, const ui32v4& destRect = ui32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * @param parent: Parent control object.
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            IButton(Widget* parent, const nString& name, const ui32v4& destRect = ui32v4(0));
            /*! @brief Default destructor. */
            virtual ~IButton(); // TODO(Ben): Maybe make abstract?

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) override;

            /*! @brief Removes all drawables from the UIRenderer
            *
            * @param renderer: UIRenderer to remove from
            */
            virtual void removeDrawables(UIRenderer* renderer) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_drawableRect.getTexture(); }
            virtual const vorb::graphics::SpriteFont* getFont() const override { return m_drawableText.getFont(); }
            virtual const WidgetStyle& getBackgroundStyle() const { return m_drawableRect.getStyle(); }
            virtual const WidgetStyle& getTextStyle() const { return m_drawableText.getStyle(); }
            virtual const nString& getText() const { return m_drawableText.getText(); }

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
            virtual void setRenderer(const UIRenderer* renderer) { m_drawableRect.setRenderer(renderer); m_drawableText.setRenderer(renderer); }
            virtual void setBackgroundStyle(const WidgetStyle& style) { m_drawableRect.setStyle(style); }
            virtual void setText(const nString& text) { m_drawableText.setText(text); }
            virtual void setTextStyle(const WidgetStyle& style) { m_drawableText.setStyle(style); }

        protected:
            virtual void refreshDrawable();
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect m_drawableRect, m_drawnRect;
            DrawableText m_drawableText, m_drawnText;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_IButton_h__