//
// Panel.h
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file Panel.h
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
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/Drawables.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/Slider.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class Panel : public Widget {
        public:
            /*! \brief Default constructor. */
            Panel();
            /*! \brief Constructor that sets name, position, and dimensions.
            *
            * \param name: Name of the control.
            * \param dimensions: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Panel(const nString& name, const f32v4& dimensions = f32v4(0.0f));
            /*! \brief Constructor that sets name, position, and dimensions.
             *
             * \param name: Name of the control.
             * \param position: The position of the widget.
             * \param size: The size of the widget.
             */
            Panel(const nString& name, const Length2& position, const Length2& size);
            /*! \brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * \param parent: Parent control object.
            * \param name: Name of the control.
            * \param dimensions: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Panel(IWidget* parent, const nString& name, const f32v4& dimensions = f32v4(0.0f));
            /*! \brief Constructor that sets parent control, name, position, and dimensions.
             *
             * The widget will be made a child of parent.
             *
             * \param parent: Parent widget.
             * \param name: Name of the control.
             * \param position: The position of the widget.
             * \param size: The size of the widget.
             */
            Panel(IWidget* parent, const nString& name, const Length2& position, const Length2& size);
            /*! \brief Default destructor. */
            virtual ~Panel();

            /*! \brief Enables events panel widgets are interested in. */
            virtual void enable() override;
            /*! \brief Disables events panel widgets are interested in. */
            virtual void disable() override;

            /*! \brief Adds all drawables to the UIRenderer. */
            virtual void addDrawables() override;
            /*! \brief Refresh drawables. */
            virtual void refreshDrawables() override;

            bool addWidget(IWidget* child) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture()     const { return m_drawableRect.getTexture(); }
            virtual             bool getAutoScroll()  const { return m_autoScroll;                }
            virtual              f32 getSliderWidth() const { return m_sliderWidth;               }
            virtual    const color4& getColor()       const { return m_backColor;                 }
            virtual    const color4& getHoverColor()  const { return m_backHoverColor;            }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTexture(VGTexture texture);
            virtual void setColor(const color4& color);
            virtual void setHoverColor(const color4& color);
            virtual void setAutoScroll(bool autoScroll);
        protected:
            virtual void updateDimensions(f32 dt) override;

            virtual void calculateDrawables() override;

            virtual void updateColor();
            virtual void updateSliders();

            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;
            virtual void onMouseFocusLost(Sender s, const MouseEvent& e) override;
            virtual void onSliderValueChange(Sender s, int v);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            f32 m_minX =  FLT_MAX, m_minY =  FLT_MAX;
            f32 m_maxX = -FLT_MAX, m_maxY = -FLT_MAX; ///< The minimum and maximum coordinates of child widgets relative to panel.

            bool m_autoScroll     = false; ///< Whether to automatically add sliders if content overflows the panel.
            struct {
                Slider horizontal;
                Slider vertical;
            } m_sliders; ///< The sliders that will be added on content overflow.

            bool m_flipHorizontal = false; ///< Whether we should place the horizontal slider on the left or right (true = left, false = right).
            bool m_flipVertical   = false; ///< Whether we should place the vertical slider on the top or bottom (true = top, false = bottom).
            f32  m_sliderWidth    = 15.0f; ///< The width, in pixels, of the sliders.

            f32v2 m_childOffset = f32v2(0.0f);

            DrawableRect m_drawableRect, m_drawnRect;
            color4 m_backColor      = color::Transparent; ///< Colour of the panel.
            color4 m_backHoverColor = color::Transparent; ///< Colour of the panel on hover.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Panel_h__
