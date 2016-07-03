//
// Panel.h
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file Panel.h
 * 
* \brief 
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
#include "Slider.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class Panel : public Widget {
        public:
            /*! \brief Default constructor. */
            Panel(InputDispatcher* dispatcher);
            /*! \brief Constructor that sets name, position, and dimensions.
            *
            * \param name: Name of the control.
            * \param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Panel(InputDispatcher* dispatcher, const nString& name, const f32v4& destRect = f32v4(0));
            /*! \brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * \param parent: Parent control object.
            * \param name: Name of the control.
            * \param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Panel(IWidgetContainer* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! \brief Default destructor. */
            virtual ~Panel();

            /*! \brief Adds all drawables to the UIRenderer
            *
            * \param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) override;
            /*! \brief Removes all drawables from the UIRenderer
            *
            * \param renderer: UIRenderer to remove from
            */
            virtual void removeDrawables() override;


            bool addWidget(Widget* child) override;

            /*! \brief Updates the position relative to parent */
            virtual void updatePosition() override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_drawableRect.getTexture(); }
            virtual const bool& getAutoScroll() const { return m_autoScroll; }
            virtual const f32& getSliderWidth() const { return m_sliderWidth; }
            virtual const color4& getColor() const { return m_backColor; }
            virtual const color4& getHoverColor() const { return m_backHoverColor; }

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
            virtual void setAutoScroll(bool autoScroll);

        protected:
            virtual void updateColor();
            virtual void updateSliders();
            virtual void refreshDrawables();
           
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e) override;
            virtual void onSliderValueChange(Sender s, int v);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            f32 minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX; ///< Used for auto scroll
            Slider m_sliders[2];
            f32 m_sliderWidth = 15.0f;
            bool m_autoScroll = false;
            f32v2 m_childOffset = f32v2(0.0f);
            DrawableRect m_drawableRect, m_drawnRect;
            // Has no color by default
            color4 m_backColor = color::Transparent, m_backHoverColor = color::Transparent;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Panel_h__
