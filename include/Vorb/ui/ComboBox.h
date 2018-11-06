//
// ComboBox.h
// Vorb Engine
//
// Created by Benjamin Arnold on 10 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ComboBox.h
* \brief 
* Defines the ComboBox widget
*
*/

#pragma once

#ifndef Vorb_ComboBox_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ComboBox_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/Drawables.h"
#include "Vorb/ui/Widget.h"
#include "Vorb/ui/Button.h"
#include "Vorb/ui/Panel.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        enum class DropDownStyle {
            SIMPLE,
            DROP_DOWN,
            DROP_DOWN_LIST
        };

        // TODO(Matthew): Different properties on selected button.
        class ComboBox : public Widget {
            friend class ComboBoxScriptFuncs;
        public:
            /*! \brief Default constructor. */
            ComboBox();
            /*! \brief Default destructor. */
            virtual ~ComboBox();

            virtual void dispose() override;

            /*! \brief Set up events. */
            virtual void enable() override;
            /*! \brief Unsubscribe from events. */
            virtual void disable() override;

            virtual void addDrawables(UIRenderer&) override { /* Empty */ }

            /*!\brief Updates the position relative to parent */
            // virtual void updatePosition() override;

            /*! \brief Adds an item to the combo box.
             * 
             * \param item: The item to add.
             * 
             * \return pointer to button created.
             */
            virtual Button* addItem(const nString& item);
            /*! \brief Adds an item to the combo box at a specific index
             * and shifts other items accordingly.
             *
             * \param index: The index to add at.
             * \param item: The item to add.
             * 
             * \return pointer to button created, nullptr if failed.
             */
            virtual Button* addItemAtIndex(size_t index, const nString& item);
            /*! \brief Adds a series of items to the combo box.
             *
             * \param itemsToAdd: The items to add.
             * 
             * \return vector of pointers to buttons created.
             */
            virtual std::vector<Button*> addItems(const std::vector<nString>& itemsToAdd);
            /*! \brief Removes an item from the combo box.
            * If there are multiple instances of the item, only the
            * first will be removed.
            * \param item: The item to remove.
            * \return true if successfully removed.
            */
            virtual bool removeItem(const nString& item);
            /*! \brief Removes an item from the combo box.
            * 
            * \param item: The index of the item to remove.
            * \return true if successfully removed.
            */
            virtual bool removeItem(size_t index);
            /*! \brief Selects an item by text.
            *
            * \param item: The item text.
            * \return true if successfully selected.
            */
            virtual bool selectItem(const nString& item);
            /*! \brief Selects an item by index.
            *
            * \param index: The index to select.
            * \return true if successfully selected.
            */
            virtual bool selectItem(size_t index);

            /*! \brief Checks if a point is inside the drop window
            *
            * \param point: The point to check
            * \return true if point is in m_destRect
            */
            virtual bool isInDropBounds(const f32v2& point) const { return isInDropBounds(point.x, point.y); }
            virtual bool isInDropBounds(f32 x, f32 y) const;

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            // TODO(Matthew): Write getters to match each of the kinds of setter.
            virtual                  const VGTexture& getTexture()           const { return m_mainButton.getTexture();        }
            virtual const vorb::graphics::SpriteFont* getFont()              const { return m_mainButton.getFont();           }
            virtual                     const color4& getBackColor()         const { return m_mainButton.getBackColor();      }
            virtual                     const color4& getBackHoverColor()    const { return m_mainButton.getBackHoverColor(); }
            virtual                     const color4& getTextColor()         const { return m_mainButton.getTextColor();      }
            virtual                     const color4& getTextHoverColor()    const { return m_mainButton.getTextHoverColor(); }
            virtual                             f32v2 getTextScale()         const { return m_mainButton.getTextScale();      }
            virtual      const std::vector <nString>& getItems()             const { return m_items;                          }
            virtual                            size_t getItemCount()         const { return m_buttons.size();                 }
            virtual                    const nString& getItem(size_t index)  const { return m_items.at(index);                }
            virtual              const vg::TextAlign& getTextAlign()         const { return m_mainButton.getTextAlign();      }
            virtual                    const nString& getText()              const { return m_mainButton.getText();           }
            virtual                        const f32& getMaxDropHeight()     const { return m_maxDropHeight;                  }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            // TODO(Matthew): We need to store data about these calls in this class, so that future added items also get given the right settings.
            virtual void setFont(const vorb::graphics::SpriteFont* font);
            virtual void setMainButtonTexture(VGTexture texture);
            virtual void setDropBoxTexture(VGTexture texture);
            virtual void setDropButtonTexture(VGTexture texture);
            virtual void setDropButtonTexture(VGTexture texture, const nString& item);
            virtual void setDropButtonTexture(VGTexture texture, size_t index);
            virtual void setBackColor(const color4& color);
            virtual void setMainButtonBackColor(const color4& color);
            virtual void setDropButtonBackColor(const color4& color);
            virtual void setDropButtonBackColor(const color4& color, const nString& item);
            virtual void setDropButtonBackColor(const color4& color, size_t index);
            virtual void setBackHoverColor(const color4& color);
            virtual void setMainButtonBackHoverColor(const color4& color);
            virtual void setDropButtonBackHoverColor(const color4& color);
            virtual void setDropButtonBackHoverColor(const color4& color, const nString& item);
            virtual void setDropButtonBackHoverColor(const color4& color, size_t index);
            virtual void setTextColor(const color4& color);
            virtual void setMainButtonTextColor(const color4& color);
            virtual void setDropButtonTextColor(const color4& color);
            virtual void setDropButtonTextColor(const color4& color, const nString& item);
            virtual void setDropButtonTextColor(const color4& color, size_t index);
            virtual void setTextHoverColor(const color4& color);
            virtual void setMainButtonTextHoverColor(const color4& color);
            virtual void setDropButtonTextHoverColor(const color4& color);
            virtual void setDropButtonTextHoverColor(const color4& color, const nString& item);
            virtual void setDropButtonTextHoverColor(const color4& color, size_t index);
            virtual void setTextScale(const f32v2& textScale);
            virtual void setMainButtonTextScale(const f32v2& textScale);
            virtual void setDropButtonTextScale(const f32v2& textScale);
            virtual void setDropButtonTextScale(const f32v2& textScale, const nString& item);
            virtual void setDropButtonTextScale(const f32v2& textScale, size_t index);
            virtual void setTextAlign(vg::TextAlign align);
            virtual void setMainButtonTextAlign(vg::TextAlign align);
            virtual void setDropButtonTextAlign(vg::TextAlign align);
            virtual void setDropButtonTextAlign(vg::TextAlign align, const nString& item);
            virtual void setDropButtonTextAlign(vg::TextAlign align, size_t index);
            virtual void setText(const nString& text);
            virtual void setMaxDropHeight(f32 maxDropHeight);

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<const nString&> ValueChange; ///< Occurs when selected item is changed

        protected:
            /*! \brief Initialiser for adding buttons, panel and setting up events. */
            virtual void initBase() override;

            /*!
             * \brief Updates the dimensions of the new IWidget according to specific widget rules.
             * 
             * \param dt: Change in time since last update.
             */
            virtual void updateDimensions(f32 dt) override;

            /*! \brief Calculates the properties of the drawables. */
            virtual void calculateDrawables() override;

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
            f32                  m_maxDropHeight;
            Panel                m_dropPanel;  // Panel that holds the drop buttons
            Button               m_mainButton; // Main button for dropping
            std::vector<Button*> m_buttons;    // Sub buttons
            std::vector<nString> m_items;
            DropDownStyle        m_dropDownStyle;
            bool                 m_isDropped;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ComboBox_h__
