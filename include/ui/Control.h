//
// Control.h
// Vorb Engine
//
// Created by Benjamin Arnold on 27 Apr 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Control.h
* @brief 
* Base class for all Controls.
*
*/

#pragma once

#ifndef Vorb_Control_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Control_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <vector>
#include "../Events.hpp"
#include "../VorbPreDecl.inl"

DECL_VG(class SpriteFont;
        class SpriteBatch)

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
        struct DockStyle {
            bool left : 1; ///< If true, docked to the left of parent
            bool right : 1; ///< If true, docked to the right of parent
            bool bottom : 1; ///< If true, docked to the bottom of parent
            bool top : 1; ///< If true, anchored to the top of parent
        };

        class Control {
        public:
            /*! @brief Default constructor. */
            Control();
            /*! @brief Constructor that sets name, position, and dimensions.
             *
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Control(const nString& name, const ui32v4& destRect = ui32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
             *
             * The control will be made a child of parent.
             *
             * @param parent: Parent control object.
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Control(Control* parent, const nString& name, const ui32v4& destRect = ui32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~Control();

            /*! @brief Initializes the rendering
            *
            * @param sb: The spritebatch to use.
            * @param font: The spritefont to use.
            * @return true on success.
            */
            virtual bool initRendering(vorb::graphics::SpriteBatch* sb,
                                       vorb::graphics::SpriteFont* font);

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
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            virtual bool addChild(Control* child);
            
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual bool getFixedHeight() const { return m_style.fixedHeight; }
            virtual bool getFixedWidth() const { return m_style.fixedWidth; }
            virtual bool getSelectable() const { return m_style.selectable; }
            virtual const ControlStyle& getStyle() const { return m_style; }
            virtual std::vector<Control*>& getControls() { return m_controls; }
            virtual const ui32v4& getDestRect() const { return m_destRect; }
            virtual const ui32& getX() const { return m_destRect.x; }
            virtual const ui32& getY() const { return m_destRect.y; }
            virtual const ui32& getWidth() const { return m_destRect.z; }
            virtual const ui32& getHeight() const { return m_destRect.w; }
            virtual const AnchorStyle& getAnchor() const { return m_anchor; }
            virtual const DockStyle& getDock() const { return m_dock; }
            virtual const bool& isRenderInit() const { return m_isRenderInit; }
            virtual const vorb::graphics::SpriteBatch* getSpriteBatch() const { return m_sb; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            
            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            // TODO(Ben): Propagate changes to children
            virtual void setFixedHeight(bool fixedHeight) { m_style.fixedHeight = fixedHeight; }
            virtual void setFixedWidth(bool fixedWidth) { m_style.fixedWidth = fixedWidth; }
            virtual void setSelectable(bool selectable) { m_style.selectable = selectable; }
            virtual void setStyle(const ControlStyle& style) { m_style = style; }
            virtual void setDestRect(const ui32v4& destRect) { m_destRect = destRect; }
            virtual void setX(ui32 x) { m_destRect.x = x; }
            virtual void setY(ui32 y) { m_destRect.y = y; }
            virtual void setWidth(ui32 width) { m_destRect.z = width; }
            virtual void setHeight(ui32 height) { m_destRect.w = height; }
            virtual void setAnchor(const AnchorStyle& anchor) { m_anchor = anchor; }
            virtual void setDock(const DockStyle& dock) { m_dock = dock; }
            virtual void setSpriteBatch(vorb::graphics::SpriteBatch* sb);
            virtual void setFont(vorb::graphics::SpriteFont* font);

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
            virtual void onMouseClick(Sender s, const MouseButtonEvent& e);
            virtual void onMouseDown(Sender s, const MouseButtonEvent& e);
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e);
            virtual void onMouseEnter(Sender s, const MouseEvent& e);
            virtual void onMouseLeave(Sender s, const MouseEvent& e);
            virtual void onMouseMove(Sender s, const MouseEvent& e);

            vorb::graphics::SpriteFont* m_font = nullptr;
            vorb::graphics::SpriteBatch* m_sb = nullptr;
            AnchorStyle m_anchor; ///< The anchor data
            ControlStyle m_style = {}; ///< The current style
            DockStyle m_dock; ///< The dock data
            std::vector<Control*> m_controls; ///< All child controls
            ui32v4 m_destRect; ///< The position and dimensions

            bool m_isRenderInit = false; ///< True when rendering is initialized
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Control_h__
