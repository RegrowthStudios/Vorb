//
// WidgetListScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file WidgetListScriptFuncs.h
* @brief 
* Script functions for WidgetList widget.
*
*/

#pragma once

#ifndef Vorb_WidgetListScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetListScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/graphics/gtypes.h"
#include "Vorb/ui/WidgetScriptFuncs.h"

namespace vorb {
    namespace ui {

        // Forward declarations
        class WidgetList;

        class WidgetListScriptFuncs : public WidgetScriptFuncs {
        public:
            void init(const cString nSpace, vscript::Environment* env) override;
        protected:

            /*! @brief Adds a Widget to the combo box
            *
            * @param w: The Widget to add
            */
            virtual void addItem(WidgetList* wl, Widget* w);
            /*! @brief Adds a Widget to the combo box at a specific index
            * and shifts other items accordingly.
            *
            * @param index: The index to add at
            * @param w: The Widget to add
            * @return true if successfully added
            */
            virtual bool addItemAtIndex(WidgetList* wl, int index, Widget* w);
            /*! @brief Removes a Widget from the combo box
            * If there are multiple instances of the Widget, only the
            * first will be removed
            * @param w: The Widget to remove
            * @return true if successfully removed
            */
            virtual bool removeItem(WidgetList* wl, Widget* w);
            /*! @brief Removes a Widget from the combo box
            *
            * @param index: The index of the Widget to remove
            * @return true if successfully removed
            */
            virtual bool removeItemAtIndex(WidgetList* wl, int index);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual VGTexture getTexture(WidgetList* w) const;
            virtual color4 getBackColor(WidgetList* w) const;
            virtual color4 getBackHoverColor(WidgetList* w);
            virtual size_t getNumItems(WidgetList* w) const;
            virtual f32 getSpacing(WidgetList* w) const;
            virtual bool getAutoScroll(WidgetList* w) const;

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTexture(WidgetList* w, VGTexture texture) const;
            virtual void setBackColor(WidgetList* w, color4 color) const;
            virtual void setBackHoverColor(WidgetList* w, color4 color) const;
            virtual void setSpacing(WidgetList* w, f32 spacing) const;
            virtual void setAutoScroll(WidgetList* w, bool autoScroll) const;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetListScriptFuncs_h__
