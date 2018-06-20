//
// WidgetList.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file WidgetList.h
* @brief 
* A widget that is a vertical list of other widgets.
*
*/

#pragma once

#ifndef Vorb_WidgetList_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_WidgetList_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/Drawables.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/Panel.h"

namespace vorb {
    namespace ui {

        // TODO(Ben): Proper combo box
        class WidgetList : public Widget {
            friend class ComboBoxScriptFuncs;
        public:
            /*! @brief Default constructor. */
            WidgetList();
            /*! @brief Constructor that sets name, position, and dimensions.
            *
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            WidgetList(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * @param parent: Parent control object.
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            WidgetList(IWidgetContainer* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Default destructor. */
            virtual ~WidgetList();

            virtual void dispose() override;

            /*!@brief Updates the position relative to parent */
            virtual void updatePosition() override;

            /*! @brief Adds a Widget to the combo box
            *
            * @param w: The Widget to add
            */
            virtual void addItem(Widget* w);
            /*! @brief Adds a Widget to the combo box at a specific index
            * and shifts other items accordingly.
            *
            * @param index: The index to add at
            * @param w: The Widget to add
            * @return true if successfully added
            */
            virtual bool addItemAtIndex(int index, Widget* w);
            /*! @brief Removes a Widget from the combo box
            * If there are multiple instances of the Widget, only the
            * first will be removed
            * @param w: The Widget to remove
            * @return true if successfully removed
            */
            virtual bool removeItem(Widget* w);
            /*! @brief Removes a Widget from the combo box
            *
            * @param index: The index of the Widget to remove
            * @return true if successfully removed
            */
            virtual bool removeItem(int index);
            /*! @brief Adds a series of items to the combo box
            *
            * @param widgetsToAdd: The Widgets to add
            */
            virtual void addItems(const std::vector <Widget*>& widgetsToAdd);
   
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_panel.getTexture(); }
            virtual const bool& getAutoScroll() const { return m_panel.getAutoScroll(); }
            virtual const color4& getBackColor() const { return m_panel.getColor(); }
            virtual const color4& getBackHoverColor() const { return m_panel.getHoverColor(); }
            virtual size_t getNumItems() const { return m_listedWidgets.size(); }
            virtual const f32& getSpacing() const { return m_spacing; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setDimensions(const f32v2& dimensions) override;
            virtual void setHeight(f32 height) override;
            virtual void setTexture(VGTexture texture);
            virtual void setWidth(f32 width) override;
            virtual void setBackColor(const color4& color);
            virtual void setBackHoverColor(const color4& color);
            virtual void setSpacing(f32 spacing);
            virtual void setAutoScroll(bool autoScroll);

        protected:  
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            f32 m_spacing;
            Panel m_panel; // Panel that holds the drop buttons
            std::vector<Widget*> m_listedWidgets; ///< The widgets in the list
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetList_h__
