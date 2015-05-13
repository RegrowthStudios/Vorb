//
// WidgetScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file WidgetScriptFuncs.h
* @brief
* Scripts functions for Widgets
*
*/

#pragma once

#ifndef Vorb_WidgetScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <map>
#include "IWidgetContainer.h"
#include "../IDGenerator.h"
#include "../VorbPreDecl.inl"

DECL_VSCRIPT(class Environment)


namespace vorb {
    namespace ui {

        enum class EventType {
            NONE,
            MOUSE_CLICK,
            MOUSE_DOWN,
            MOUSE_UP,
            MOUSE_ENTER,
            MOUSE_LEAVE,
            MOUSE_MOVE
        };

        // Forward Declarations
        class Widget;

        class WidgetScriptFuncs {
        public:
            virtual void init(const cString nSpace, vscript::Environment* env);
        protected:

            /*! @brief Releases all resources used by the Widget.
            *
            * Gets called in the destructor.
            */
            virtual void dispose(Widget* w);
            /*! @brief Enables events* */
            virtual void enable(Widget* w);
            /*! @brief Disables events* */
            virtual void disable(Widget* w);
            /*! @brief Registers a LUA callback with a widget* */
            virtual bool addCallback(Widget* w, EventType eventType, nString funcName);
            /*! @brief Deregisters a LUA callback with a widget* */
            virtual bool removeCallback(Widget* w, EventType eventType, nString funcName);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual bool getFixedHeight(Widget* w) const;
            virtual bool getFixedWidth(Widget* w) const;
            virtual bool getSelectable(Widget* w) const;
            virtual bool isMouseIn(Widget* w) const;
            virtual int getAnchor(Widget* w) const;
            virtual int getStyle(Widget* w) const;
            virtual int getDock(Widget* w) const;
            virtual int getNumWidgets(Widget* w) const;
            virtual bool isEnabled(Widget* w) const;
            virtual f32 getHeight(Widget* w) const;
            virtual f32 getWidth(Widget* w) const;
            virtual f32 getX(Widget* w) const;
            virtual f32 getY(Widget* w) const;
            virtual f32v2 getDimensions(Widget* w) const;
            virtual f32v2 getPosition(Widget* w) const;
            virtual f32v2 getRelativePosition(Widget* w) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setAnchor(Widget* w, int anchor) const;
            virtual void setDestRect(Widget* w, f32v4 destRect) const;
            virtual void setDimensions(Widget* w, f32v2 dims) const;
            virtual void setDock(Widget* w, DockStyle dock) const;
            virtual void setFixedHeight(Widget* w, bool fixedHeight) const;
            virtual void setFixedWidth(Widget* w, bool fixedWidth) const;
            virtual void setHeight(Widget* w, f32 height) const;
            virtual void setPosition(Widget* w, f32v2 pos) const;
            virtual void setSelectable(Widget* w, bool selectable) const;
            virtual void setStyle(Widget* w, int style) const;
            virtual void setWidth(Widget* w, f32 width) const;
            virtual void setX(Widget* w, f32 x) const;
            virtual void setY(Widget* w, f32 y) const;

            /************************************************************************/
            /* Widget Event Handlers                                                */
            /************************************************************************/
            virtual void onMouseClick(Sender s, const MouseButtonEvent& e);
            virtual void onMouseDown(Sender s, const MouseButtonEvent& e);
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e);
            virtual void onMouseEnter(Sender s, const MouseMotionEvent& e);
            virtual void onMouseLeave(Sender s, const MouseMotionEvent& e);
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e);
            vscript::Environment* m_env = nullptr;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetScriptFuncs_h__