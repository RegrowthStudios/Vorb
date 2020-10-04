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
#include "Vorb/ui/widgets/Widget.h"
#include "Vorb/ui/widgets/Button.h"
#include "Vorb/ui/widgets/Panel.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        // TODO(Matthew): Different properties on selected button.
        class ComboBox : public Widget {
            using DropButtonComparator = Delegate<bool, size_t, Button*>;
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
            virtual const vg::SpriteFont* getMainButtonFont()           const { return m_mainButton.getFont();           }
            virtual const vg::SpriteFont* getMainButtonHoverFont()      const { return m_mainButton.getHoverFont();      }
            virtual             VGTexture getMainButtonTexture()        const { return m_mainButton.getTexture();        }
            virtual             VGTexture getDropBoxTexture()           const { return m_dropPanel.getTexture();         }
            virtual             VGTexture getMainButtonHoverTexture()   const { return m_mainButton.getHoverTexture();   }
            virtual             VGTexture getDropBoxHoverTexture()      const { return m_dropPanel.getHoverTexture();    }
            virtual                color4 getMainButtonBackColor()      const { return m_mainButton.getBackColor();      }
            virtual                color4 getDropBoxBackColor()         const { return m_dropPanel.getColor();           }
            virtual                color4 getMainButtonBackHoverColor() const { return m_mainButton.getBackHoverColor(); }
            virtual                color4 getDropBoxBackHoverColor()    const { return m_dropPanel.getHoverColor();      }
            virtual                color4 getMainButtonTextColor()      const { return m_mainButton.getTextColor();      }
            virtual                color4 getMainButtonHoverTextColor() const { return m_mainButton.getHoverTextColor(); }
            virtual                 f32v2 getMainButtonTextScale()      const { return m_mainButton.getTextScale();      }
            virtual                 f32v2 getMainButtonHoverTextScale() const { return m_mainButton.getHoverTextScale(); }
            virtual         vg::TextAlign getMainButtonTextAlign()      const { return m_mainButton.getTextAlign();      }
            virtual         vg::TextAlign getMainButtonHoverTextAlign() const { return m_mainButton.getHoverTextAlign(); }
            virtual        const nString& getMainButtonText()           const { return m_mainButton.getText();           }
            virtual        const nString& getMainButtonHoverText()      const { return m_mainButton.getHoverText();      }
            virtual const vg::SpriteFont* getDropButtonFont(const nString& item)           const;
            virtual const vg::SpriteFont* getDropButtonFont(size_t index)                  const;
            virtual const vg::SpriteFont* getDropButtonHoverFont(const nString& item)      const;
            virtual const vg::SpriteFont* getDropButtonHoverFont(size_t index)             const;
            virtual             VGTexture getDropButtonTexture(const nString& item)        const;
            virtual             VGTexture getDropButtonTexture(size_t index)               const;
            virtual             VGTexture getDropButtonHoverTexture(const nString& item)   const;
            virtual             VGTexture getDropButtonHoverTexture(size_t index)          const;
            virtual                color4 getDropButtonBackColor(const nString& item)      const;
            virtual                color4 getDropButtonBackColor(size_t index)             const;
            virtual                color4 getDropButtonBackHoverColor(const nString& item) const;
            virtual                color4 getDropButtonBackHoverColor(size_t index)        const;
            virtual                color4 getDropButtonTextColor(const nString& item)      const;
            virtual                color4 getDropButtonTextColor(size_t index)             const;
            virtual                color4 getDropButtonHoverTextColor(const nString& item) const;
            virtual                color4 getDropButtonHoverTextColor(size_t index)        const;
            virtual                 f32v2 getDropButtonTextScale(const nString& item)      const;
            virtual                 f32v2 getDropButtonTextScale(size_t index)             const;
            virtual                 f32v2 getDropButtonHoverTextScale(const nString& item) const;
            virtual                 f32v2 getDropButtonHoverTextScale(size_t index)        const;
            virtual         vg::TextAlign getDropButtonTextAlign(const nString& item)      const;
            virtual         vg::TextAlign getDropButtonTextAlign(size_t index)             const;
            virtual         vg::TextAlign getDropButtonHoverTextAlign(const nString& item) const;
            virtual         vg::TextAlign getDropButtonHoverTextAlign(size_t index)        const;
            virtual            const f32& getMaxDropHeight() const { return m_maxDropHeight; }
            virtual const std::vector <nString>& getItems()                 const { return m_items;                          }
            virtual                       size_t getItemCount()             const { return m_buttons.size();                 }
            virtual               const nString& getItem(size_t index)      const { return m_items.at(index);                }
            virtual                       size_t getItemIndex(nString item) const { return std::distance(m_items.begin(), std::find(m_items.begin(), m_items.end(), item));                }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setFont(const vorb::graphics::SpriteFont* font);
            virtual void setMainButtonFont(const vorb::graphics::SpriteFont* font);
            virtual void setDropButtonFont(const vorb::graphics::SpriteFont* font);
            virtual void setDropButtonFont(const vorb::graphics::SpriteFont* font, const nString& item);
            virtual void setDropButtonFont(const vorb::graphics::SpriteFont* font, size_t index);
            virtual void setDropButtonFont(const vorb::graphics::SpriteFont* font, DropButtonComparator comparator);
            virtual void setHoverFont(const vorb::graphics::SpriteFont* font);
            virtual void setMainButtonHoverFont(const vorb::graphics::SpriteFont* font);
            virtual void setDropButtonHoverFont(const vorb::graphics::SpriteFont* font);
            virtual void setDropButtonHoverFont(const vorb::graphics::SpriteFont* font, const nString& item);
            virtual void setDropButtonHoverFont(const vorb::graphics::SpriteFont* font, size_t index);
            virtual void setDropButtonHoverFont(const vorb::graphics::SpriteFont* font, DropButtonComparator comparator);
            virtual void setMainButtonTexture(VGTexture texture);
            virtual void setDropBoxTexture(VGTexture texture);
            virtual void setDropButtonTexture(VGTexture texture);
            virtual void setDropButtonTexture(VGTexture texture, const nString& item);
            virtual void setDropButtonTexture(VGTexture texture, size_t index);
            virtual void setDropButtonTexture(VGTexture texture, DropButtonComparator comparator);
            virtual void setMainButtonHoverTexture(VGTexture texture);
            virtual void setDropBoxHoverTexture(VGTexture texture);
            virtual void setDropButtonHoverTexture(VGTexture texture);
            virtual void setDropButtonHoverTexture(VGTexture texture, const nString& item);
            virtual void setDropButtonHoverTexture(VGTexture texture, size_t index);
            virtual void setDropButtonHoverTexture(VGTexture texture, DropButtonComparator comparator);
            virtual void setBackColor(const color4& color);
            virtual void setDropBoxBackColor(const color4& color);
            virtual void setMainButtonBackColor(const color4& color);
            virtual void setDropButtonBackColor(const color4& color);
            virtual void setDropButtonBackColor(const color4& color, const nString& item);
            virtual void setDropButtonBackColor(const color4& color, size_t index);
            virtual void setDropButtonBackColor(const color4& color, DropButtonComparator comparator);
            virtual void setBackHoverColor(const color4& color);
            virtual void setDropBoxBackHoverColor(const color4& color);
            virtual void setMainButtonBackHoverColor(const color4& color);
            virtual void setDropButtonBackHoverColor(const color4& color);
            virtual void setDropButtonBackHoverColor(const color4& color, const nString& item);
            virtual void setDropButtonBackHoverColor(const color4& color, size_t index);
            virtual void setDropButtonBackHoverColor(const color4& color, DropButtonComparator comparator);
            virtual void setTextColor(const color4& color);
            virtual void setMainButtonTextColor(const color4& color);
            virtual void setDropButtonTextColor(const color4& color);
            virtual void setDropButtonTextColor(const color4& color, const nString& item);
            virtual void setDropButtonTextColor(const color4& color, size_t index);
            virtual void setDropButtonTextColor(const color4& color, DropButtonComparator comparator);
            virtual void setHoverTextColor(const color4& color);
            virtual void setMainButtonHoverTextColor(const color4& color);
            virtual void setDropButtonHoverTextColor(const color4& color);
            virtual void setDropButtonHoverTextColor(const color4& color, const nString& item);
            virtual void setDropButtonHoverTextColor(const color4& color, size_t index);
            virtual void setDropButtonHoverTextColor(const color4& color, DropButtonComparator comparator);
            virtual void setTextScale(const f32v2& textScale);
            virtual void setMainButtonTextScale(const f32v2& textScale);
            virtual void setDropButtonTextScale(const f32v2& textScale);
            virtual void setDropButtonTextScale(const f32v2& textScale, const nString& item);
            virtual void setDropButtonTextScale(const f32v2& textScale, size_t index);
            virtual void setDropButtonTextScale(const f32v2& textScale, DropButtonComparator comparator);
            virtual void setHoverTextScale(const f32v2& textScale);
            virtual void setMainButtonHoverTextScale(const f32v2& textScale);
            virtual void setDropButtonHoverTextScale(const f32v2& textScale);
            virtual void setDropButtonHoverTextScale(const f32v2& textScale, const nString& item);
            virtual void setDropButtonHoverTextScale(const f32v2& textScale, size_t index);
            virtual void setDropButtonHoverTextScale(const f32v2& textScale, DropButtonComparator comparator);
            virtual void setTextAlign(vg::TextAlign align);
            virtual void setMainButtonTextAlign(vg::TextAlign align);
            virtual void setDropButtonTextAlign(vg::TextAlign align);
            virtual void setDropButtonTextAlign(vg::TextAlign align, const nString& item);
            virtual void setDropButtonTextAlign(vg::TextAlign align, size_t index);
            virtual void setDropButtonTextAlign(vg::TextAlign align, DropButtonComparator comparator);
            virtual void setHoverTextAlign(vg::TextAlign align);
            virtual void setMainButtonHoverTextAlign(vg::TextAlign align);
            virtual void setDropButtonHoverTextAlign(vg::TextAlign align);
            virtual void setDropButtonHoverTextAlign(vg::TextAlign align, const nString& item);
            virtual void setDropButtonHoverTextAlign(vg::TextAlign align, size_t index);
            virtual void setDropButtonHoverTextAlign(vg::TextAlign align, DropButtonComparator comparator);
            virtual void setMainButtonText(const nString& text);
            virtual void setDropButtonText(const nString& text);
            virtual void setDropButtonText(const nString& text, const nString& item);
            virtual void setDropButtonText(const nString& text, size_t index);
            virtual void setDropButtonText(const nString& text, DropButtonComparator comparator);
            virtual void setMainButtonHoverText(const nString& text);
            virtual void setDropButtonHoverText(const nString& text);
            virtual void setDropButtonHoverText(const nString& text, const nString& item);
            virtual void setDropButtonHoverText(const nString& text, size_t index);
            virtual void setDropButtonHoverText(const nString& text, DropButtonComparator comparator);
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
// #ifdef VORB_USING_SCRIPT
//             std::vector<script::Function> m_valueChangeFuncs;
// #endif

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            f32                  m_maxDropHeight;
            Panel                m_dropPanel;  // Panel that holds the drop buttons
            Button               m_mainButton; // Main button for dropping
            std::vector<Button*> m_buttons;    // Sub buttons
            std::vector<nString> m_items;
            bool                 m_isDropped;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ComboBox_h__
