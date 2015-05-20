//
// ComboBox.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ComboBox.h
* @brief 
* Defines the ComboBox widget
*
*/

#pragma once

#ifndef Vorb_ComboBox_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ComboBox_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "Drawables.h"
#include "Widget.h"
#include "Button.h"
#include "Panel.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        enum class DropDownStyle {
            SIMPLE,
            DROP_DOWN,
            DROP_DOWN_LIST
        };

        // TODO(Ben): Proper combo box
        class ComboBox : public Widget {
            friend class ComboBoxScriptFuncs;
        public:
            /*! @brief Default constructor. */
            ComboBox();
            /*! @brief Constructor that sets name, position, and dimensions.
            *
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            ComboBox(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The control will be made a child of parent.
            *
            * @param parent: Parent control object.
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            ComboBox(IWidgetContainer* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Default destructor. */
            virtual ~ComboBox();

            virtual void dispose() override;

            /*!@brief Updates the position relative to parent */
            virtual void updatePosition() override;

            /*! @brief Adds an item to the combo box
             * 
             * @param item: The item to add
             */
            virtual void addItem(const nString& item);
            /*! @brief Adds an item to the combo box at a specific index
            * and shifts other items accordingly.
            *
            * @param index: The index to add at
            * @param item: The item to add
            * @return true if successfully added
            */
            virtual bool addItemAtIndex(int index, const nString& item);
            /*! @brief Removes an item from the combo box
            * If there are multiple instances of the item, only the
            * first will be removed
            * @param item: The item to remove
            * @return true if successfully removed
            */
            virtual bool removeItem(const nString& item);
            /*! @brief Removes an item from the combo box
            * 
            * @param item: The index of the item to remove
            * @return true if successfully removed
            */
            virtual bool removeItem(int index);
            /*! @brief Adds a series of items to the combo box
            *
            * @param itemsToAdd: The items to add
            */
            virtual void addItems(const std::vector <nString>& itemsToAdd);
            /*! @brief Selects an item index
            *
            * @param index: The index to select
            * @return true if successfully selected
            */
            virtual bool selectItem(int index);

            /*! @brief Checks if a point is inside the drop window
            *
            * @param point: The point to check
            * @return true if point is in m_destRect
            */
            virtual bool isInDropBounds(const f32v2& point) const { return isInDropBounds(point.x, point.y); }
            virtual bool isInDropBounds(f32 x, f32 y) const;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual const VGTexture& getTexture() const { return m_mainButton.getTexture(); }
            virtual const vorb::graphics::SpriteFont* getFont() const override;
            virtual const color4& getBackColor() const { return m_mainButton.getBackColor(); }
            virtual const color4& getBackHoverColor() const { return m_mainButton.getBackHoverColor(); }
            virtual const color4& getTextColor() const { return m_mainButton.getTextColor(); }
            virtual const color4& getTextHoverColor() const { return m_mainButton.getTextHoverColor(); }
            virtual f32v2 getTextScale() const;
            virtual const std::vector <nString>& getItems() const { return m_items; }
            virtual size_t getNumItems() const { return m_items.size(); }
            virtual const nString& getItem(int index) const;
            virtual const vg::TextAlign& getTextAlign() const { return m_mainButton.getTextAlign(); }
            virtual const nString& getText() const { return m_mainButton.getText(); }
            virtual const f32& getMaxDropHeight() const { return m_maxDropHeight; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setDimensions(const f32v2& dimensions) override;
            virtual void setFont(const vorb::graphics::SpriteFont* font) override;
            virtual void setHeight(f32 height) override;
            virtual void setTexture(VGTexture texture);
            virtual void setDropBoxTexture(VGTexture texture);
            virtual void setDropButtonTexture(VGTexture texture);
            virtual void setWidth(f32 width) override;
            virtual void setBackColor(const color4& color);
            virtual void setBackHoverColor(const color4& color);
            virtual void setTextColor(const color4& color);
            virtual void setTextHoverColor(const color4& color);
            virtual void setTextScale(const f32v2& textScale);
            virtual void setTextAlign(vg::TextAlign align);
            virtual void setText(const nString& text);
            virtual void setMaxDropHeight(f32 maxDropHeight);

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<const nString&> ValueChange; ///< Occurs when selected item is changed
        protected:
            virtual void updateDropButton(vorb::ui::Button* b);
            virtual void computeClipRect(const f32v4& parentClipRect = f32v4(-(FLT_MAX / 2.0f), -(FLT_MAX / 2.0f), FLT_MAX, FLT_MAX)) override;

            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e) override;
            virtual void onSubButtonClick(Sender s, const MouseButtonEvent& e);
            virtual void onMainButtonClick(Sender s, const MouseButtonEvent& e);

            /************************************************************************/
            /* LUA Callbacks                                                        */
            /************************************************************************/
#ifdef VORB_USING_SCRIPT
            std::vector<script::Function> m_valueChangeFuncs;
#endif

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            f32 m_maxDropHeight = FLT_MAX;
            Panel m_dropPanel; // Panel that holds the drop buttons
            Button m_mainButton; // Main button for dropping
            std::vector<Button*> m_buttons; // Sub buttons
            DropDownStyle m_dropDownStyle = DropDownStyle::DROP_DOWN_LIST;
            std::vector <nString> m_items; ///< All combo box items
            bool m_isDropped = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ComboBox_h__
