//
// Widget.h
// Vorb Engine
//
// Created by Benjamin Arnold on 27 Apr 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Widget.h
* @brief 
* Base class for all Widgets.
*
*/

#pragma once

#ifndef Vorb_Widget_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Widget_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <vector>
#include "../Events.hpp"
#include "../VorbPreDecl.inl"

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        // Forward Declarations
        struct MouseButtonEvent;
        struct MouseEvent;

        //! Bitfield of control styling flags
        struct ControlStyle {
            bool fixedHeight : 1; ///< If true, the control has fixed height when auto-scaled.
            bool fixedWidth : 1; ///< If true, the control has fixed width when auto-scaled.
            bool selectable : 1; ///< If true, the control can receive focus.
        };
        //! Bitfield of anchor flags
        struct AnchorStyle {
            bool left : 1; ///< If true, anchored to the left of parent
            bool right : 1; ///< If true, anchored to the right of parent
            bool bottom : 1; ///< If true, anchored to the bottom of parent
            bool top : 1; ///< If true, anchored to the top of parent
        };
        //! Bitfield of dock flags
        enum class DockStyle {
            LEFT, RIGHT, BOTTOM, TOP, FILL
        };

        class Widget {
        public:
            /*! @brief Default constructor. */
            Widget();
            /*! @brief Constructor that sets name, position, and dimensions.
             *
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(const nString& name, const ui32v4& destRect = ui32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
             *
             * The control will be made a child of parent.
             *
             * @param parent: Parent control object.
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(Widget* parent, const nString& name, const ui32v4& destRect = ui32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~Widget();

            /*! @brief Draws the control.
             *
             * Gets called in the destructor.
             */
            virtual void draw();

            /*! @brief Releases all resources used by the Control.
             * 
             * Gets called in the destructor.
             */
            virtual void dispose();

            /*! @brief Adds a child to the control
             *
             * @param child: The control to add
             * @return true on success.
             */
            virtual bool addChild(Widget* child);
            
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual bool getFixedHeight() const { return m_style.fixedHeight; }
            virtual bool getFixedWidth() const { return m_style.fixedWidth; }
            virtual bool getSelectable() const { return m_style.selectable; }
            virtual const AnchorStyle& getAnchor() const { return m_anchor; }
            virtual const ControlStyle& getStyle() const { return m_style; }
            virtual const DockStyle& getDock() const { return m_dock; }
            virtual const bool& isRenderInit() const { return m_isRenderInit; }
            virtual const f32& getHeight() const { return m_destRect.w; }
            virtual const f32& getWidth() const { return m_destRect.z; }
            virtual const f32& getX() const { return m_destRect.x; }
            virtual const f32& getY() const { return m_destRect.y; }
            virtual const f32v2& getDimensions() const { return *(f32v2*)(&m_destRect.z); }
            virtual const f32v2& getPosition() const { return *(f32v2*)(&m_destRect.x); }
            virtual const f32v4& getDestRect() const { return m_destRect; }
            virtual const std::vector<Widget*>& getControls() const { return m_controls; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            
            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            // TODO(Ben): Propagate changes to children
            virtual void setAnchor(const AnchorStyle& anchor) { m_anchor = anchor; }
            virtual void setDestRect(const f32v4& destRect) { m_destRect = destRect; }
            virtual void setDimensions(const f32v2& dimensions) { m_destRect.z = dimensions.x; m_destRect.w = dimensions.y; }
            virtual void setDock(const DockStyle& dock) { m_dock = dock; }
            virtual void setFixedHeight(bool fixedHeight) { m_style.fixedHeight = fixedHeight; }
            virtual void setFixedWidth(bool fixedWidth) { m_style.fixedWidth = fixedWidth; }
            virtual void setFont(vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setHeight(f32 height) { m_destRect.w = height; }
            virtual void setPosition(const f32v2& position) { m_destRect.x = position.x; m_destRect.y = position.y; }
            virtual void setSelectable(bool selectable) { m_style.selectable = selectable; }
            virtual void setStyle(const ControlStyle& style) { m_style = style; }
            virtual void setWidth(f32 width) { m_destRect.z = width; }
            virtual void setX(f32 x) { m_destRect.x = x; }
            virtual void setY(f32 y) { m_destRect.y = y; }

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<const MouseButtonEvent&> onMouseClick; ///< Occurs when control is clicked by mouse.
            Event<const MouseButtonEvent&> onMouseDown; ///< Occurs when mouse button is pressed over control.
            Event<const MouseButtonEvent&> onMouseUp; ///< Occurs when mouse button is released over control.
            Event<const MouseEvent&> onMouseEnter; ///< Occurs when mouse pointer enters the control.
            Event<const MouseEvent&> onMouseLeave; ///< Occurs when mouse pointer leaves the control.
            Event<const MouseEvent&> onMouseMove; ///< Occurs when mouse pointer is moved over control.
            // TODO(Ben): Lots more events!

        protected:
            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseClick(Sender s, const MouseButtonEvent& e);
            virtual void onMouseDown(Sender s, const MouseButtonEvent& e);
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e);
            virtual void onMouseEnter(Sender s, const MouseEvent& e);
            virtual void onMouseLeave(Sender s, const MouseEvent& e);
            virtual void onMouseMove(Sender s, const MouseEvent& e);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            AnchorStyle m_anchor; ///< The anchor data.
            ControlStyle m_style = {}; ///< The current style.
            DockStyle m_dock; ///< The dock type.
            std::vector<Widget*> m_controls; ///< All child controls.
            f32v4 m_destRect; ///< The position and dimensions.

            bool m_isRenderInit = false; ///< True when rendering is initialized.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
