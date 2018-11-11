#include "Vorb/stdafx.h"
#include "Vorb/ui/ViewScriptEnvironment.h"

#include "Vorb/ui/Button.h"
#include "Vorb/ui/ButtonScriptFuncs.h"
#include "Vorb/ui/CheckBox.h"
#include "Vorb/ui/CheckBoxScriptFuncs.h"
#include "Vorb/ui/ComboBox.h"
#include "Vorb/ui/ComboBoxScriptFuncs.h"
#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/GameWindowScriptFuncs.h"
#include "Vorb/ui/Label.h"
#include "Vorb/ui/LabelScriptFuncs.h"
#include "Vorb/ui/Panel.h"
#include "Vorb/ui/PanelScriptFuncs.h"
#include "Vorb/ui/Slider.h"
#include "Vorb/ui/SliderScriptFuncs.h"
#include "Vorb/ui/TextWidgetScriptFuncs.h"
#include "Vorb/ui/WidgetScriptFuncs.h"
#include "Vorb/ui/WidgetList.h"
#include "Vorb/ui/WidgetListScriptFuncs.h"

template <typename ScriptEnvironmentImpl>
vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::ViewScriptEnvironment() :
    m_viewport(nullptr),
    m_env(nullptr) {
    // Empty
}

template <typename ScriptEnvironmentImpl>
vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::~ViewScriptEnvironment() {
    // Empty
}

template <typename ScriptEnvironmentImpl>
void vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::init(Viewport* viewport, const GameWindow* window) {
    m_viewport = viewport;
    m_env      = new ScriptEnv();
}

template <typename ScriptEnvironmentImpl>
void vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::prepareScriptEnv() {
    registerFuncs();

    registerConsts();
}

template <typename ScriptEnvironmentImpl>
void vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::registerFuncs() {
    // Set up creation functions for extending the view.
    m_env->setNamespaces("UI", "View");
    m_env->addCDelegate("makeButton",     makeDelegate(this, &FormScriptEnvironment::makeButton));
    m_env->addCDelegate("makeCheckBox",   makeDelegate(this, &FormScriptEnvironment::makeCheckBox));
    m_env->addCDelegate("makeComboBox",   makeDelegate(this, &FormScriptEnvironment::makeComboBox));
    m_env->addCDelegate("makeLabel",      makeDelegate(this, &FormScriptEnvironment::makeLabel));
    m_env->addCDelegate("makePanel",      makeDelegate(this, &FormScriptEnvironment::makePanel));
    m_env->addCDelegate("makeSlider",     makeDelegate(this, &FormScriptEnvironment::makeSlider));
    m_env->addCDelegate("makeWidgetList", makeDelegate(this, &FormScriptEnvironment::makeWidgetList));
    m_env->addCDelegate("destroyWidget",  makeDelegate(this, &FormScriptEnvironment::destroyWidget));
    m_env->setNamespaces();

    ButtonScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Button", m_env);

    CheckBoxScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("CheckBox", m_env);

    ComboBoxScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("ComboBox", m_env);

    GameWindowScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Window", m_env);

    LabelScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Label", m_env);

    PanelScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Panel", m_env);

    SliderScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Slider", m_env);

    WidgetListScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("WidgetList", m_env);
}

template <typename ScriptEnvironmentImpl>
void vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::registerConsts() {
    ButtonScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    CheckBoxScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    ComboBoxScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    GameWindowScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    LabelScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    PanelScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    SliderScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    WidgetListScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    m_env->setNamespaces("UI", "ClippingState");
    m_env->addValue("VISIBLE", vui::IWidget::ClippingState::VISIBLE);
    m_env->addValue("HIDDEN",  vui::IWidget::ClippingState::HIDDEN);
    m_env->addValue("INHERIT", vui::IWidget::ClippingState::INHERIT);
    m_env->setNamespaces();

    m_env->setNamespaces("UI", "DockState");
    m_env->addValue("NONE",   vui::IWidget::DockState::NONE);
    m_env->addValue("LEFT",   vui::IWidget::DockState::LEFT);
    m_env->addValue("RIGHT",  vui::IWidget::DockState::RIGHT);
    m_env->addValue("BOTTOM", vui::IWidget::DockState::BOTTOM);
    m_env->addValue("TOP",    vui::IWidget::DockState::TOP);
    m_env->addValue("FILL",   vui::IWidget::DockState::FILL);
    m_env->setNamespaces();

    m_env->setNamespaces("UI", "PositionType");
    m_env->addValue("STATIC_TO_WINDOW",     vui::Widget::PositionType::STATIC_TO_WINDOW);
    m_env->addValue("STATIC_TO_VIEWPORT",   vui::Widget::PositionType::STATIC_TO_VIEWPORT);
    m_env->addValue("STATIC_TO_PARENT",     vui::Widget::PositionType::STATIC_TO_PARENT);
    m_env->addValue("RELATIVE_TO_WINDOW",   vui::Widget::PositionType::RELATIVE_TO_WINDOW);
    m_env->addValue("RELATIVE_TO_VIEWPORT", vui::Widget::PositionType::RELATIVE_TO_VIEWPORT);
    m_env->addValue("RELATIVE_TO_PARENT",   vui::Widget::PositionType::RELATIVE_TO_PARENT);
    m_env->setNamespaces();

    m_env->setNamespaces("UI", "DimensionType");
    m_env->addValue("PIXEL",                      vui::Widget::DimensionType::PIXEL);
    m_env->addValue("WIDTH_PERCENTAGE",           vui::Widget::DimensionType::WIDTH_PERCENTAGE);
    m_env->addValue("HEIGHT_PERCENTAGE",          vui::Widget::DimensionType::HEIGHT_PERCENTAGE);
    m_env->addValue("MIN_PERCENTAGE",             vui::Widget::DimensionType::MIN_PERCENTAGE);
    m_env->addValue("MAX_PERCENTAGE",             vui::Widget::DimensionType::MAX_PERCENTAGE);
    m_env->addValue("PARENT_WIDTH_PERCENTAGE",    vui::Widget::DimensionType::PARENT_WIDTH_PERCENTAGE);
    m_env->addValue("PARENT_HEIGHT_PERCENTAGE",   vui::Widget::DimensionType::PARENT_HEIGHT_PERCENTAGE);
    m_env->addValue("PARENT_MIN_PERCENTAGE",      vui::Widget::DimensionType::PARENT_MIN_PERCENTAGE);
    m_env->addValue("PARENT_MAX_PERCENTAGE",      vui::Widget::DimensionType::PARENT_MAX_PERCENTAGE);
    m_env->addValue("VIEWPORT_WIDTH_PERCENTAGE",  vui::Widget::DimensionType::VIEWPORT_WIDTH_PERCENTAGE);
    m_env->addValue("VIEWPORT_HEIGHT_PERCENTAGE", vui::Widget::DimensionType::VIEWPORT_HEIGHT_PERCENTAGE);
    m_env->addValue("VIEWPORT_MIN_PERCENTAGE",    vui::Widget::DimensionType::VIEWPORT_MIN_PERCENTAGE);
    m_env->addValue("VIEWPORT_MAX_PERCENTAGE",    vui::Widget::DimensionType::VIEWPORT_MAX_PERCENTAGE);
    m_env->addValue("WINDOW_WIDTH_PERCENTAGE",    vui::Widget::DimensionType::WINDOW_WIDTH_PERCENTAGE);
    m_env->addValue("WINDOW_HEIGHT_PERCENTAGE",   vui::Widget::DimensionType::WINDOW_HEIGHT_PERCENTAGE);
    m_env->addValue("WINDOW_MIN_PERCENTAGE",      vui::Widget::DimensionType::WINDOW_MIN_PERCENTAGE);
    m_env->addValue("WINDOW_MAX_PERCENTAGE",      vui::Widget::DimensionType::WINDOW_MAX_PERCENTAGE);
    m_env->setNamespaces();
}
