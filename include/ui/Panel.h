//
// Panel.h
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Panel.h
* @brief 
* Panel widget implementation, used for
* grouping controls inside a parent region.
*/

#pragma once

#ifndef Vorb_Panel_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Panel_h__
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

        class Panel : public Widget {
        public:
            /*! @brief Default constructor. */
            Panel();
            /*! @brief Constructor that sets name, position, and dimensions.
            *
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Panel(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * @param parent: Parent control object.
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Panel(Widget* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Default destructor. */
            virtual ~Panel();

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) override;

            /*! @brief Updates the position relative to parent */
            virtual void updatePosition() override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_drawableRect.getTexture(); }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTexture(VGTexture texture);
            virtual void setDestRect(const f32v4& destRect) override;
            virtual void setDimensions(const f32v2& dimensions) override;
            virtual void setHeight(f32 height) override;
            virtual void setPosition(const f32v2& position) override;
            virtual void setWidth(f32 width) override;
            virtual void setX(f32 x) override;
            virtual void setY(f32 y) override;
            virtual void setColor(const color4& color);
            virtual void setHoverColor(const color4& color);

        protected:
            virtual void updateColor();
            virtual void refreshDrawables();

            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e) override;

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect m_drawableRect, m_drawnRect;
            // Has no color by default
            color4 m_backColor = color::Transparent, m_backHoverColor = color::Transparent;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Panel_h__
