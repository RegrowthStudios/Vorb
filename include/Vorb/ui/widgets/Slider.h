//
// Slider.h
// Vorb Engine
//
// Created by Benjamin Arnold on 5 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file Slider.h
* @brief 
* Defines the slider widget.
*
*/

#pragma once

#ifndef Vorb_Slider_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Slider_h__
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

        class Slider : public Widget {
        public:
            /*! @brief Default constructor. */
            Slider();
            /*! @brief Default destructor. */
            virtual ~Slider();

            /*! @brief Adds all drawables to the UIRenderer
            */
            virtual void addDrawables(UIRenderer& renderer) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getSlideTexture()    const { return m_drawableSlide.getTexture(); }
            virtual const VGTexture& getBarTexture()      const { return m_drawableBar.getTexture(); }
            virtual    const color4& getSlideColor()      const { return m_slideColor; }
            virtual    const color4& getSlideHoverColor() const { return m_slideHoverColor; }
            virtual    const color4& getBarColor()        const { return m_barColor; }
            virtual              i32 getValue()           const { return m_value; }
            virtual              i32 getMin()             const { return m_min; }
            virtual              i32 getMax()             const { return m_max; }
            /// Gets slider value scaled between 0.0f and 1.0f
            virtual              f32 getValueScaled()     const { return (f32)(m_value - m_min) / (m_max - m_min); }
            virtual     const f32v2& getSlideSize()       const { return m_slideSize; }
            virtual   const Length2& getRawSlideSize()    const { return m_rawSlideSize; }
            virtual             bool isHorizontal()       const { return !m_isVertical; }
            virtual             bool isVertical()         const { return m_isVertical; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setSlideTexture(VGTexture texture);
            virtual void setBarTexture(VGTexture texture);
            virtual void setSlideColor(const color4& color);
            virtual void setSlideHoverColor(const color4& color);
            virtual void setBarColor(const color4& color);
            virtual void setValue(i32 value);
            virtual void setRange(i32 min, i32 max);
            virtual void setMin(i32 min);
            virtual void setMax(i32 max);
            virtual void setSlideSize(const Length2& size);
            virtual void setSlideSize(const f32v2& size);
            virtual void setIsVertical(bool isVertical);

            virtual bool isInSlideBounds(const f32v2& point) const { return isInSlideBounds(point.x, point.y); }
            virtual bool isInSlideBounds(f32 x, f32 y) const;

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<i32> ValueChange; ///< Occurs when the value changes

        protected:
            /*! \brief Initialiser for setting up events. */
            virtual void initBase() override;

            virtual void updateDimensions(f32 dt) override;

            virtual void calculateDrawables() override;

            virtual void updateSlidePosition();
            virtual void updateColor();

            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseDown(Sender, const MouseButtonEvent& e) override;
            virtual void onMouseMove(Sender, const MouseMotionEvent& e) override;

            /************************************************************************/
            /* LUA Callbacks                                                        */
            /************************************************************************/
// #ifdef VORB_USING_SCRIPT
//             std::vector<script::Function> m_valueChangeFuncs;
// #endif
            DrawableRect m_drawableBar;
            DrawableRect m_drawableSlide;
            color4       m_barColor;
            color4       m_slideColor, m_slideHoverColor;
            Length2      m_rawSlideSize;
            f32v2        m_slideSize;
            i32          m_value;
            i32          m_min;
            i32          m_max;
            bool         m_isVertical;
            f32v2        m_clickPoint;
            f32          m_clickValue;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Slider_h__
