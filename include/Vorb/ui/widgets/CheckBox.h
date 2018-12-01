//
// CheckBox.h
// Vorb Engine
//
// Created by Benjamin Arnold on 6 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file CheckBox.h
* \brief 
* Defines the CheckBox widget.
*
*/

#pragma once

#ifndef Vorb_CheckBox_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_CheckBox_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/ui/Drawables.h"
#include "Vorb/ui/widgets/TextWidget.h"

namespace vorb {
    namespace ui {

        /// Forward Declarations
        class UIRenderer;

        class CheckBox : public TextWidget {
        public:
            /*! \brief Default constructor. */
            CheckBox();
            /*! \brief Default destructor. */
            virtual ~CheckBox();

            /*! \brief Adds all drawables to the UIRenderer */
            virtual void addDrawables(UIRenderer& renderer) override;

            virtual void check();
            virtual void uncheck();
            virtual void toggleChecked();

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual      const VGTexture& getUncheckedTexture()      const { return m_uncheckedTexture;      }
            virtual      const VGTexture& getCheckedTexture()        const { return m_checkedTexture;        }
            virtual      const VGTexture& getUncheckedHoverTexture() const { return m_uncheckedHoverTexture; }
            virtual      const VGTexture& getCheckedHoverTexture()   const { return m_checkedHoverTexture;   }
            virtual         const color4& getBoxColor()              const { return m_boxColor;              }
            virtual         const color4& getBoxHoverColor()         const { return m_boxHoverColor;         }
            virtual         const color4& getBoxCheckedColor()       const { return m_boxCheckedColor;       }
            virtual         const color4& getBoxCheckedHoverColor()  const { return m_boxCheckedHoverColor;  }
            virtual         const color4& getTextColor()             const override { return m_textColor;             }
            virtual         const color4& getTextHoverColor()        const { return m_textHoverColor;        }
            virtual           const bool& isChecked()                const { return m_isChecked;             }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setCheckedTexture(VGTexture texture);
            virtual void setUncheckedTexture(VGTexture texture);
            virtual void setCheckedHoverTexture(VGTexture texture);
            virtual void setUncheckedHoverTexture(VGTexture texture);
            virtual void setBoxColor(const color4& color);
            virtual void setBoxHoverColor(const color4& color);
            virtual void setBoxCheckedColor(const color4& color);
            virtual void setBoxCheckedHoverColor(const color4& color);
            virtual void setTextColor(const color4& color) override;
            virtual void setTextHoverColor(const color4& color);
            virtual void setChecked(bool checked);

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<bool> ValueChange; ///< Occurs when value of the box is changed

        protected:
            /*! \brief Initialiser for general set-up. */
            virtual void initBase() override;

            virtual void calculateDrawables() override;

            /*! Updates the colours set for the drawables. */
            virtual void updateColor();

            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e) override;
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e) override;

            /************************************************************************/
            /* LUA Callbacks                                                        */
            /************************************************************************/
// #ifdef VORB_USING_SCRIPT
//             std::vector<script::Function> m_valueChangeFuncs;
// #endif

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            DrawableRect          m_drawableRect;
            color4                m_boxColor,            m_boxHoverColor;
            color4                m_boxCheckedColor,     m_boxCheckedHoverColor;
            color4                m_textColor,           m_textHoverColor;
            VGTexture             m_checkedTexture,      m_uncheckedTexture;
            VGTexture             m_checkedHoverTexture, m_uncheckedHoverTexture;
            bool                  m_isChecked;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_CheckBox_h__
