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
#include "../IDGenerator.h"
#include "../VorbPreDecl.inl"

DECL_VSCRIPT(class Environment)

typedef int WidgetID;

namespace vorb {
    namespace ui {

        // Forward Declarations
        class Widget;

        class WidgetScriptFuncs {
        public:
            virtual void registerFuncs(const cString nSpace, vscript::Environment& env);
            virtual WidgetID registerWidget(Widget* widget);
            virtual void unregisterWidget(WidgetID id);
            Widget* getWidget(WidgetID id) const;
        protected:

            /*! @brief Releases all resources used by the Widget.
            *
            * Gets called in the destructor.
            */
            virtual void dispose(WidgetID id);
            /*! @brief Enables events* */
            virtual void enable(WidgetID id);
            /*! @brief Disables events* */
            virtual void disable(WidgetID id);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual bool getFixedHeight(WidgetID id) const;
            virtual bool getFixedWidth(WidgetID id) const;
            virtual bool getSelectable(WidgetID id) const;
            virtual bool isMouseIn(WidgetID id) const;
            virtual const int getAnchor(WidgetID id) const;
            virtual const int getStyle(WidgetID id) const;
            virtual const int getDock(WidgetID id) const;
            virtual bool isEnabled(WidgetID id) const;
            virtual f32 getHeight(WidgetID id) const;
            virtual f32 getWidth(WidgetID id) const;
            virtual f32 getX(WidgetID id) const;
            virtual f32 getY(WidgetID id) const;
            virtual f32v2 getDimensions(WidgetID id) const;
            virtual f32v2 getPosition(WidgetID id) const;
            virtual f32v2 getRelativePosition(WidgetID id) const;
            virtual const std::vector<Widget*>& getWidgets(WidgetID id) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setAnchor(WidgetID id, int anchor);
            virtual void setDimensions(WidgetID id, f32 width, f32 height);
            virtual void setDock(WidgetID id, int dock);
            virtual void setFixedHeight(WidgetID id, bool fixedHeight);
            virtual void setFixedWidth(WidgetID id, bool fixedWidth);
            virtual void setHeight(WidgetID id, f32 height);
            virtual void setPosition(WidgetID id, f32 x, f32 y);
            virtual void setSelectable(WidgetID id, bool selectable);
            virtual void setStyle(WidgetID id, int style);
            virtual void setWidth(WidgetID id, f32 width);
            virtual void setX(WidgetID id, f32 x);
            virtual void setY(WidgetID id, f32 y);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            vcore::IDGenerator<WidgetID> m_idGenerator;
            std::map<WidgetID, Widget*> m_widgets;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetScriptFuncs_h__