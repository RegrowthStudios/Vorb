//
// CheckBox.h
// Vorb Engine
//
// Created by Benjamin Arnold on 6 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file CheckBox.h
* @brief 
* Defines the CheckBox widget.
*
*/

#pragma once

#ifndef Vorb_CheckBox_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_CheckBox_h__
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

        class CheckBox : public Widget {
            friend class CheckBoxScriptFuncs;
        public:
            /*! @brief Default constructor. */
            CheckBox();
            /*! @brief Constructor that sets name, position, and dimensions.
            *
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            CheckBox(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * @param parent: Parent control object.
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            CheckBox(IWidgetContainer* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Default destructor. */
            virtual ~CheckBox();

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) override;

            /*! @brief Updates the position relative to parent */
            virtual void updatePosition() override;

            virtual void check();
            virtual void unCheck();
            virtual void toggleChecked();
            virtual void computeClipRect(const f32v4& parentClipRect = f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX)) override;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getUncheckedTexture() const { return m_uncheckedTexture; }
            virtual const VGTexture& getCheckedTexture() const { return m_checkedTexture; }
            virtual const vorb::graphics::SpriteFont* getFont() const override { return m_drawableText.getFont(); }
            virtual const color4& getBoxColor() const { return m_boxColor; }
            virtual const color4& getBoxHoverColor() const { return m_boxHoverColor; }
            virtual const color4& getBoxCheckedColor() const { return m_boxCheckedColor; }
            virtual const color4& getBoxCheckedHoverColor() const { return m_boxCheckedHoverColor; }
            virtual const color4& getTextColor() const { return m_drawableText.getColor(); }
            virtual const nString& getText() const { return m_drawableText.getText(); }
            virtual const vg::TextAlign& getTextAlign() const { return m_drawableText.getTextAlign(); }
            virtual const f32v2& getTextScale() const { return m_drawableText.getTextScale(); }
            virtual const bool& isChecked() const { return m_isChecked; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setDimensions(const f32v2& dimensions) override;
            virtual void setFont(const vorb::graphics::SpriteFont* font) override;
            virtual void setHeight(f32 height) override;
            virtual void setPosition(const f32v2& position) override;
            virtual void setCheckedTexture(VGTexture texture);
            virtual void setUncheckedTexture(VGTexture texture);
            virtual void setWidth(f32 width) override;
            virtual void setX(f32 x) override;
            virtual void setY(f32 y) override;
            virtual void setBoxColor(const color4& color);
            virtual void setBoxHoverColor(const color4& color);
            virtual void setBoxCheckedColor(const color4& color);
            virtual void setBoxCheckedHoverColor(const color4& color);
            virtual void setText(const nString& text);
            virtual void setTextColor(const color4& color);
            virtual void setTextAlign(vg::TextAlign textAlign);
            virtual void setTextScale(const f32v2& textScale);
            virtual void setChecked(bool checked);

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<bool> ValueChange; ///< Occurs when value of the box is changed

        protected:
            virtual void updateColor();
            virtual void updateTextPosition();
            virtual void refreshDrawables();

            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e) override;
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;

            /************************************************************************/
            /* LUA Callbacks                                                        */
            /************************************************************************/
#ifdef VORB_USING_SCRIPT
            std::vector<script::Function> m_valueChangeFuncs;
#endif

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect m_drawableRect, m_drawnRect;
            DrawableText m_drawableText, m_drawnText;
            color4 m_boxColor = color::DarkGray, m_boxHoverColor = color::AliceBlue;
            color4 m_boxCheckedColor = color::LightGray, m_boxCheckedHoverColor = color::AliceBlue;
            const vg::SpriteFont* m_defaultFont = nullptr;
            VGTexture m_checkedTexture = 0, m_uncheckedTexture = 0;
            bool m_isChecked = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_CheckBox_h__
