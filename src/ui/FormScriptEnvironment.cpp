#include "Vorb/stdafx.h"

#include <SDL2/SDL.h>

#include "Vorb/ui/Button.h"
#include "Vorb/ui/CheckBox.h"
#include "Vorb/ui/ComboBox.h"
#include "Vorb/ui/Form.h"
#include "Vorb/ui/FormScriptEnvironment.h"
#include "Vorb/ui/Label.h"
#include "Vorb/ui/Panel.h"
#include "Vorb/ui/Slider.h"
#include "Vorb/ui/WidgetList.h"

#define INIT_FUNCTION_NAME "init"
#define OPTIONS_CHANGE_FUNCTION_NAME "onOptionsChanged"

vui::FormScriptEnvironment::FormScriptEnvironment() {
    // Empty
}

vui::FormScriptEnvironment::~FormScriptEnvironment() {
    dispose();
}

bool vui::FormScriptEnvironment::init(Form* form, const GameWindow* gameWindow) {
    m_form = form;
    dispose();
    m_env = new vscript::Environment();

    { // Initialize callbacks
        // Form functions
        m_env->setNamespaces("Form");
        m_env->addCDelegate("enable", makeDelegate(*this, &FormScriptEnvironment::enableForm));
        m_env->addCDelegate("disable", makeDelegate(*this, &FormScriptEnvironment::disableForm));
        m_env->addCRDelegate("getName", makeRDelegate(*this, &FormScriptEnvironment::getFormName));
        m_env->addCRDelegate("makeButton", makeRDelegate(*this, &FormScriptEnvironment::makeButton));
        m_env->addCRDelegate("makeSlider", makeRDelegate(*this, &FormScriptEnvironment::makeSlider));
        m_env->addCRDelegate("makeCheckBox", makeRDelegate(*this, &FormScriptEnvironment::makeCheckBox));
        m_env->addCRDelegate("makeComboBox", makeRDelegate(*this, &FormScriptEnvironment::makeComboBox));
        m_env->addCRDelegate("makePanel", makeRDelegate(*this, &FormScriptEnvironment::makePanel));
        m_env->addCRDelegate("makeLabel", makeRDelegate(*this, &FormScriptEnvironment::makeLabel));
        m_env->addCRDelegate("makeWidgetList", makeRDelegate(*this, &FormScriptEnvironment::makeWidgetList));
        m_env->addCRDelegate("setCallback", makeRDelegate(*m_form, &Form::registerCallback));
        
        m_env->setNamespaces();
        // Widget functions
        m_widgetFuncs.init("Widget", m_env);
        // Button functions
        m_buttonFuncs.init("Button", m_env);
        // Slider functions
        m_sliderFuncs.init("Slider", m_env);
        // CheckBox functions
        m_checkBoxFuncs.init("CheckBox", m_env);
        // ComboBox functions
        m_comboBoxFuncs.init("ComboBox", m_env);
        // Panel functions
        m_panelFuncs.init("Panel", m_env);
        // Label functions
        m_labelFuncs.init("Label", m_env);
        // WidgetList functions
        m_widgetListFuncs.init("WidgetList", m_env);
        // Window functions
        m_windowFuncs.init("Window", gameWindow, m_env);
        // Graphics functions
        m_graphicsInterface.registerGraphics(*m_env);
    }

    // Initialize enums and constants
    registerConstants();

    return true;
}

bool vui::FormScriptEnvironment::loadForm(const cString filePath) {
    // Load script
    if (!m_env->load(filePath)) {
        fprintf(stderr, "Failed to load FormScriptEnvironment script %s\n", filePath);
        return false;
    }

    { // Call init function
        m_init = (*m_env)[INIT_FUNCTION_NAME];
        m_init();
    }
    return true;
}

void vui::FormScriptEnvironment::dispose() {
    for (auto& w : m_widgetsToDelete) {
        delete w;
    }
    std::vector <Widget*>().swap(m_widgetsToDelete);
    delete m_env;
    m_env = nullptr;
}

void vui::FormScriptEnvironment::onOptionsChanged() {
    vscript::Function f = (*m_env)[OPTIONS_CHANGE_FUNCTION_NAME];
    if (!f.isNil()) f();
}

void vui::FormScriptEnvironment::registerConstants() {
    // Set enums
    // { // vui::WidgetAlign
    //     m_env->setNamespaces("WidgetAlign");
    //     m_env->addValue("LEFT", WidgetAlign::LEFT);
    //     m_env->addValue("TOP_LEFT", WidgetAlign::TOP_LEFT);
    //     m_env->addValue("TOP", WidgetAlign::TOP);
    //     m_env->addValue("TOP_RIGHT", WidgetAlign::TOP_RIGHT);
    //     m_env->addValue("RIGHT", WidgetAlign::RIGHT);
    //     m_env->addValue("BOTTOM_RIGHT", WidgetAlign::BOTTOM_RIGHT);
    //     m_env->addValue("BOTTOM", WidgetAlign::BOTTOM);
    //     m_env->addValue("BOTTOM_LEFT", WidgetAlign::BOTTOM_LEFT);
    //     m_env->addValue("CENTER", WidgetAlign::CENTER);
    //     m_env->setNamespaces();
    // }
    { // vg::TextAlign
        m_env->setNamespaces("TextAlign");
        m_env->addValue("LEFT", vg::TextAlign::LEFT);
        m_env->addValue("TOP_LEFT", vg::TextAlign::TOP_LEFT);
        m_env->addValue("TOP", vg::TextAlign::TOP);
        m_env->addValue("TOP_RIGHT", vg::TextAlign::TOP_RIGHT);
        m_env->addValue("RIGHT", vg::TextAlign::RIGHT);
        m_env->addValue("BOTTOM_RIGHT", vg::TextAlign::BOTTOM_RIGHT);
        m_env->addValue("BOTTOM", vg::TextAlign::BOTTOM);
        m_env->addValue("BOTTOM_LEFT", vg::TextAlign::BOTTOM_LEFT);
        m_env->addValue("CENTER", vg::TextAlign::CENTER);
        m_env->setNamespaces();
    }
    { // vui::DockState
        m_env->setNamespaces("DockState");
        m_env->addValue("NONE", DockState::NONE);
        m_env->addValue("LEFT", DockState::LEFT);
        m_env->addValue("RIGHT", DockState::RIGHT);
        m_env->addValue("BOTTOM", DockState::BOTTOM);
        m_env->addValue("TOP", DockState::TOP);
        m_env->addValue("FILL", DockState::FILL);
        m_env->setNamespaces();
    }
    // Misc constants
    m_env->addValue("INT_MAX", INT_MAX);
    m_env->addValue("INT_MIN", INT_MIN);
    // This pointer
    m_env->addValue("this", m_form);
}

// vui::Button* vui::FormScriptEnvironment::makeButton(Form* f, nString name, f32 x, f32 y, f32 width, f32 height) {
//     vui::Button* b = new vui::Button(f, name, f32v4(x, y, width, height));
//     m_widgetsToDelete.push_back(b);
//     m_buttonFuncs.registerWidget(b);
//     return b;
// }

// vui::Slider* vui::FormScriptEnvironment::makeSlider(Form* f, nString name, f32 x, f32 y, f32 width, f32 height) {
//     vui::Slider* s = new vui::Slider(f, name, f32v4(x, y, width, height));
//     m_widgetsToDelete.push_back(s);
//     m_sliderFuncs.registerWidget(s);
//     return s;
// }

// vui::CheckBox* vui::FormScriptEnvironment::makeCheckBox(Form* f, nString name, f32 x, f32 y, f32 width, f32 height) {
//     vui::CheckBox* c = new vui::CheckBox(f, name, f32v4(x, y, width, height));
//     m_widgetsToDelete.push_back(c);
//     m_checkBoxFuncs.registerWidget(c);
//     return c;
// }

// vui::ComboBox* vui::FormScriptEnvironment::makeComboBox(Form* f, nString name, f32 x, f32 y, f32 width, f32 height) {
//     vui::ComboBox* c = new vui::ComboBox(f, name, f32v4(x, y, width, height));
//     m_widgetsToDelete.push_back(c);
//     m_comboBoxFuncs.registerWidget(c);
//     return c;
// }

// vui::Panel* vui::FormScriptEnvironment::makePanel(Form* f, nString name, f32 x, f32 y, f32 width, f32 height) {
//     vui::Panel* p = new vui::Panel(f, name, f32v4(x, y, width, height));
//     m_widgetsToDelete.push_back(p);
//     m_panelFuncs.registerWidget(p);
//     return p;
// }

// vui::Label* vui::FormScriptEnvironment::makeLabel(Form* f, nString name, f32 x, f32 y, f32 width, f32 height) {
//     vui::Label* l = new vui::Label(f, name, f32v4(x, y, width, height));
//     m_widgetsToDelete.push_back(l);
//     m_labelFuncs.registerWidget(l);
//     return l;
// }

// vui::WidgetList* vui::FormScriptEnvironment::makeWidgetList(Form* f, nString name, f32 x, f32 y, f32 width, f32 height) {
//     vui::WidgetList* w = new vui::WidgetList(f, name, f32v4(x, y, width, height));
//     m_widgetsToDelete.push_back(w);
//     m_widgetListFuncs.registerWidget(w);
//     return w;
// }

void vui::FormScriptEnvironment::enableForm(Form* f) {
    f->enable();
}

void vui::FormScriptEnvironment::disableForm(Form* f) {
    f->disable();
}

nString vui::FormScriptEnvironment::getFormName(Form* f) {
    return f->getName();
}
