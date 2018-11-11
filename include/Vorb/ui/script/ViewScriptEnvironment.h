//
// ViewScriptEnvironment.h
// Vorb Engine
//
// Created by Matthew Marshall on 11 Nov 2018
// Based on Ben Arnold's FormScriptEnvironment.
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ViewScriptEnvironment.h
* @brief Manages the script environment of a view.
*/

#pragma once

#ifndef Vorb_ViewScriptEnvironment_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ViewScriptEnvironment_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/io/File.h"
#include "Vorb/script/IEnvironment.hpp"

namespace vorb {
    namespace ui {
        // Forward Declarations
        class Button;
        class CheckBox;
        class ComboBox;
        class GameWindow;
        class IWidget;
        class Label;
        class Panel;
        class Slider;
        class Widget;
        class Viewport;
        class WidgetList;

        // TODO(Matthew): Provide function for loading specified fonts via Lua.
        // TODO(Matthew): Implement update mechanism for script env.
        template <typename ScriptEnvironmentImpl>
        class ViewScriptEnvironment {
            using ScriptEnv = vscript::IEnvironment<ScriptEnvironmentImpl>;
            using IWidgets  = std::vector<IWidget*>;
        public:
            ViewScriptEnvironment();
            ~ViewScriptEnvironment();

            void init(Viewport* viewport, const GameWindow* window);
            void dispose();

            bool load(const vio::File& filepath);

            ScriptEnv* getEnv() { return m_env; }
        protected:
            void prepareScriptEnv();

            void registerFuncs();
            void registerConsts();

                vui::Button* makeButton     (IWidget* parent, nString name, f32v4 dimensions);
              vui::CheckBox* makeCheckBox   (IWidget* parent, nString name, f32v4 dimensions);
              vui::ComboBox* makeComboBox   (IWidget* parent, nString name, f32v4 dimensions);
                 vui::Label* makeLabel      (IWidget* parent, nString name, f32v4 dimensions);
                 vui::Panel* makePanel      (IWidget* parent, nString name, f32v4 dimensions);
                vui::Slider* makeSlider     (IWidget* parent, nString name, f32v4 dimensions);
            vui::WidgetList* makeWidgetList (IWidget* parent, nString name, f32v4 dimensions);

            void destroyWidget(IWidget* widget);

            const GameWindow* m_window;

            Viewport*   m_viewport;
            ScriptEnv*  m_env;
            IWidgets    m_widgets;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ViewScriptEnvironment_h__
