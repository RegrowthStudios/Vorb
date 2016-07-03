//
// FormScriptEnvironment.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file FormScriptEnvironment.h
 * 
* \brief 
* Scripting environment for a UI form.
*
*/

#pragma once

#ifndef Vorb_FormScriptEnvironment_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FormScriptEnvironment_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "../script/Environment.h"
#include "../script/GraphicsScriptInterface.h"
#include "ButtonScriptFuncs.h"
#include "CheckBoxScriptFuncs.h"
#include "ComboBoxScriptFuncs.h"
#include "GameWindow.h"
#include "GameWindowScriptFuncs.h"
#include "LabelScriptFuncs.h"
#include "PanelScriptFuncs.h"
#include "SliderScriptFuncs.h"
#include "WidgetListScriptFuncs.h"

namespace vorb {
    namespace ui {

        // Forward Declarations
        class Form;

        class FormScriptEnvironment {
        public:
            FormScriptEnvironment();
            virtual ~FormScriptEnvironment();
            virtual bool init(Form* form, const GameWindow* gameWindow);
            virtual bool loadForm(const cString filePath);
            virtual void dispose();
            virtual void onOptionsChanged();

            vscript::Environment* getEnv() { return m_env; }
        protected:
            virtual void registerConstants();
            virtual vui::Button* makeButton(Form* f, nString name, f32 x, f32 y, f32 width, f32 height);
            virtual vui::Slider* makeSlider(Form* f, nString name, f32 x, f32 y, f32 width, f32 height);
            virtual vui::CheckBox* makeCheckBox(Form* f, nString name, f32 x, f32 y, f32 width, f32 height);
            virtual vui::ComboBox* makeComboBox(Form* f, nString name, f32 x, f32 y, f32 width, f32 height);
            virtual vui::Panel* makePanel(Form* f, nString name, f32 x, f32 y, f32 width, f32 height);
            virtual vui::Label* makeLabel(Form* f, nString name, f32 x, f32 y, f32 width, f32 height);
            virtual vui::WidgetList* makeWidgetList(Form* f, nString name, f32 x, f32 y, f32 width, f32 height);

            virtual void enableForm(Form* f);
            virtual void disableForm(Form* f);
            virtual nString getFormName(Form* f);

            Form* m_form = nullptr;
            vscript::Environment* m_env = nullptr;
            vscript::Function m_init;
            vscript::GraphicsScriptInterface m_graphicsInterface;
            WidgetScriptFuncs m_widgetFuncs;
            ButtonScriptFuncs m_buttonFuncs;
            SliderScriptFuncs m_sliderFuncs;
            CheckBoxScriptFuncs m_checkBoxFuncs;
            ComboBoxScriptFuncs m_comboBoxFuncs;
            PanelScriptFuncs m_panelFuncs;
            LabelScriptFuncs m_labelFuncs;
            WidgetListScriptFuncs m_widgetListFuncs;
            GameWindowScriptFuncs m_windowFuncs;
            std::vector <Widget*> m_widgetsToDelete;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_FormScriptEnvironment_h__
