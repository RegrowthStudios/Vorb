//
// FormScriptEnvironment.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file FormScriptEnvironment.h
* @brief 
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
#include "SliderScriptFuncs.h"

namespace vorb {
    namespace ui {

        // Forward Declarations
        class Form;

        class FormScriptEnvironment {
        public:
            FormScriptEnvironment();
            virtual ~FormScriptEnvironment();
            virtual bool init(Form* form, const cString filePath);
        protected:
            virtual void registerConstants();
            virtual vui::Button* makeButton(nString name, f32 x, f32 y, f32 width, f32 height);
            virtual vui::Slider* makeSlider(nString name, f32 x, f32 y, f32 width, f32 height);


            Form* m_form = nullptr;
            vscript::Environment m_env;
            vscript::Function m_init;
            vscript::GraphicsScriptInterface m_graphicsInterface;
            ButtonScriptFuncs m_buttonFuncs;
            SliderScriptFuncs m_sliderFuncs;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_FormScriptEnvironment_h__
