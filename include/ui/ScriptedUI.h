//
// ScriptedUI.cpp
// Vorb Engine
//
// Created by Benjamin Arnold on 12 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ScriptedUI.cpp
* @brief
* High level UI class that is defined
* with scripting.
*/

#pragma once

#ifndef Vorb_ScriptedUI_cpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ScriptedUI_cpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <vector>

namespace vorb {
    namespace ui {

        // Forward declarations
        class FormScriptEnvironment;
        class IGameScreen;
        class Form;

        class ScriptedUI {
        public:
            ScriptedUI();
            virtual ~ScriptedUI();
            virtual void init(IGameScreen* ownerScreen, const ui32v4& destRect, const cString startFormPath);
            virtual void dispose();
        protected:
            vui::FormScriptEnvironment* m_env = nullptr; ///< Scripting Environment
            std::vector<vui::Form*> m_currentForm; ///< The forms in draw order
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ScriptedUI_cpp__
