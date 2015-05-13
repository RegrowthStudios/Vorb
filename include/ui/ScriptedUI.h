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
            virtual void init(IGameScreen* ownerScreen, const ui32v4& destRect, const nString& startFormPath);
            virtual void draw();
            virtual void update(f32 dt = 1.0f);
            virtual void dispose();
        protected:
            VORB_NON_COPYABLE(ScriptedUI);
            virtual Form* makeForm(nString name, nString filePath);

            std::vector<std::pair<Form*, FormScriptEnvironment*> > m_forms; ///< The forms and script envs in draw order
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ScriptedUI_cpp__
