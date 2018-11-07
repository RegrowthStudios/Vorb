//
// WidgetList.h
// Vorb Engine
//
// Created by Benjamin Arnold on 18 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file WidgetList.h
* \brief 
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

        class WidgetList : public Widget {
            friend class WidgetListScriptFuncs;

            using IWidgets = std::vector<IWidget*>;
        public:
            /*! \brief Default constructor. */
            WidgetList();
            /*! \brief Default destructor. */
            virtual ~WidgetList();

            virtual void dispose() override;

            virtual void addDrawables(UIRenderer&) override { /* Empty */ }

            /*! \brief Adds a widget to the list.
             *
             * \param w: The widget to add.
             */
            virtual void addItem(IWidget* item);
            /*! \brief Adds a Widget to the list at a specific index and shifts other items accordingly.
             *
             * \param index: The index to add at.
             * \param w: The widget to add.
             * \return true if successfully added.
             */
            virtual bool addItemAtIndex(size_t index, IWidget* item);
            /*! \brief Adds a series of items to the list.
             *
             * \param widgetsToAdd: The widgets to add.
             */
            virtual void addItems(const IWidgets& items);
            /*! \brief Removes a widget from the list.
             *
             * If there are multiple instances of the widget, only the first will be removed.
             *
             * \param w: The widget to remove.
             * \return true if successfully removed.
             */
            virtual bool removeItem(IWidget* item);
            /*! \brief Removes a widget from the list.
             *
             * \param index: The index of the widget to remove.
             * \return true if successfully removed.
             */
            virtual bool removeItem(size_t index);
   
            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture()        const { return m_panel.getTexture();    }
            virtual             bool getAutoScroll()     const { return m_panel.getAutoScroll(); }
            virtual    const color4& getBackColor()      const { return m_panel.getColor();      }
            virtual    const color4& getBackHoverColor() const { return m_panel.getHoverColor(); }
            virtual           size_t getItemCount()      const { return m_items.size();  }
            virtual       const f32& getSpacing()        const { return m_spacing;               }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setTexture(VGTexture texture);
            virtual void setBackColor(const color4& color);
            virtual void setBackHoverColor(const color4& color);
            virtual void setSpacing(f32 spacing);
            virtual void setAutoScroll(bool autoScroll);

        protected:
            /*! \brief Initialiser for adding panel. */
            virtual void initBase() override;

            /*!
             * \brief Updates the positions of the listed widgets.
             */
            virtual void calculateDrawables() override;

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            f32      m_spacing; ///< Spacing between items.
            Panel    m_panel;   ///< Panel that holds the items.
            IWidgets m_items;   ///< The widgets in the list.
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_WidgetList_h__
