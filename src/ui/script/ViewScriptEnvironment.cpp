#include "Vorb/stdafx.h"
#include "Vorb/ui/script/ViewScriptEnvironment.h"

#include "Vorb/ui/Button.h"
#include "Vorb/ui/script/ButtonScriptFuncs.h"
#include "Vorb/ui/CheckBox.h"
#include "Vorb/ui/script/CheckBoxScriptFuncs.h"
#include "Vorb/ui/ComboBox.h"
#include "Vorb/ui/script/ComboBoxScriptFuncs.h"
#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/script/GameWindowScriptFuncs.h"
#include "Vorb/ui/Label.h"
#include "Vorb/ui/script/LabelScriptFuncs.h"
#include "Vorb/ui/Panel.h"
#include "Vorb/ui/script/PanelScriptFuncs.h"
#include "Vorb/ui/Slider.h"
#include "Vorb/ui/script/SliderScriptFuncs.h"
#include "Vorb/ui/script/TextWidgetScriptFuncs.h"
#include "Vorb/ui/Viewport.h"
#include "Vorb/ui/script/ViewportScriptFuncs.h"
#include "Vorb/ui/script/WidgetScriptFuncs.h"
#include "Vorb/ui/WidgetList.h"
#include "Vorb/ui/script/WidgetListScriptFuncs.h"

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
    m_window   = window;
    m_viewport = viewport;
    m_env      = new ScriptEnv();
    m_env->init();
}

template <typename ScriptEnvironmentImpl>
void vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::dispose() {
    for (auto& widget : m_widgets) {
        delete widget;
    }
    IWidgets().swap(m_widgets);

    m_env->dispose();

    delete m_env;

    m_env      = nullptr;
    m_window   = nullptr;
    m_viewport = nullptr;
}

template <typename ScriptEnvironmentImpl>
bool vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::load(const vio::File& filepath) {
    m_env->load(filepath);
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
    m_env->addCDelegate("makeButton",     makeDelegate(this, &ViewScriptEnvironment::makeButton));
    m_env->addCDelegate("makeCheckBox",   makeDelegate(this, &ViewScriptEnvironment::makeCheckBox));
    m_env->addCDelegate("makeComboBox",   makeDelegate(this, &ViewScriptEnvironment::makeComboBox));
    m_env->addCDelegate("makeLabel",      makeDelegate(this, &ViewScriptEnvironment::makeLabel));
    m_env->addCDelegate("makePanel",      makeDelegate(this, &ViewScriptEnvironment::makePanel));
    m_env->addCDelegate("makeSlider",     makeDelegate(this, &ViewScriptEnvironment::makeSlider));
    m_env->addCDelegate("makeWidgetList", makeDelegate(this, &ViewScriptEnvironment::makeWidgetList));
    m_env->addCDelegate("destroyWidget",  makeDelegate(this, &ViewScriptEnvironment::destroyWidget));
    m_env->setNamespaces();

    ButtonScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Button", m_env);

    CheckBoxScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("CheckBox", m_env);

    ComboBoxScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("ComboBox", m_env);

    GameWindowScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Window", m_env, m_window);

    LabelScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Label", m_env);

    PanelScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Panel", m_env);

    SliderScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Slider", m_env);

    ViewportScriptFuncs::registerFuncs<ScriptEnvironmentImpl>("Viewport", m_env);

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

    ViewportScriptFuncs::registerFuncs<ScriptEnvironmentImpl>(m_env);

    WidgetScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);

    WidgetListScriptFuncs::registerConsts<ScriptEnvironmentImpl>(m_env);
}

template <typename ScriptEnvironmentImpl>
vui::Button* vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::makeButton(IWidget* parent, nString name, f32v4 dimensions) {
    Button* button = new Button();

    button->init(parent, name, dimensions);

    m_widgets.push_back(button);

    return button;
}

template <typename ScriptEnvironmentImpl>
vui::CheckBox* vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::makeCheckBox(IWidget* parent, nString name, f32v4 dimensions) {
    CheckBox* checkBox = new CheckBox();

    checkBox->init(parent, name, dimensions);

    m_widgets.push_back(checkBox);

    return checkBox;
}

template <typename ScriptEnvironmentImpl>
vui::ComboBox* vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::makeComboBox(IWidget* parent, nString name, f32v4 dimensions) {
    ComboBox* comboBox = new ComboBox();

    comboBox->init(parent, name, dimensions);

    m_widgets.push_back(comboBox);

    return comboBox
}

template <typename ScriptEnvironmentImpl>
vui::Label* vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::makeLabel(IWidget* parent, nString name, f32v4 dimensions) {
    Label* label = new Label();

    label->init(parent, name, dimensions);

    m_widgets.push_back(label);

    return label
}

template <typename ScriptEnvironmentImpl>
vui::Panel* vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::makePanel(IWidget* parent, nString name, f32v4 dimensions) {
    Panel* panel = new Panel();

    panel->init(parent, name, dimensions);

    m_widgets.push_back(panel);

    return panel
}

template <typename ScriptEnvironmentImpl>
vui::Slider* vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::makeSlider(IWidget* parent, nString name, f32v4 dimensions) {
    Slider* slider = new Slider();

    slider->init(parent, name, dimensions);

    m_widgets.push_back(slider);

    return slider
}

template <typename ScriptEnvironmentImpl>
vui::WidgetList* vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::makeWidgetList(IWidget* parent, nString name, f32v4 dimensions) {
    WidgetList* widgetList = new WidgetList();

    widgetList->init(parent, name, dimensions);

    m_widgets.push_back(widgetList);

    return widgetList
}

template <typename ScriptEnvironmentImpl>
void vui::ViewScriptEnvironment<ScriptEnvironmentImpl>::destroyWidget(IWidget* widget) {
    // By default this function acts to just dispose the widget, unless we also manage it.
    widget->dispose();

    // If we manage this widget, clear up memory.
    auto& it = std::find(m_widgets.begin(), m_widgets.end(), widget);
    if (it != m_widgets.end()) {
        m_widgets.erase(it);
        delete widget;
    }
}
