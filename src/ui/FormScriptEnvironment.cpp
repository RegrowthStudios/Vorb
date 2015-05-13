#include "stdafx.h"
#include "ui/FormScriptEnvironment.h"
#include "ui/Button.h"
#include "ui/CheckBox.h"
#include "ui/ComboBox.h"
#include "ui/Form.h"
#include "ui/Slider.h"
#include "ui/Panel.h"

#define INIT_FUNCTION_NAME "init"

vui::FormScriptEnvironment::FormScriptEnvironment() {
    // Empty
}

vui::FormScriptEnvironment::~FormScriptEnvironment() {
    dispose();
}

bool vui::FormScriptEnvironment::init(Form* form) {
    m_form = form;
    dispose();
    m_env = new vscript::Environment();

    { // Initialize callbacks
        // Form functions
        m_env->setNamespaces("Form");
        m_env->addCRDelegate("makeButton", makeRDelegate(*this, &FormScriptEnvironment::makeButton));
        m_env->addCRDelegate("makeSlider", makeRDelegate(*this, &FormScriptEnvironment::makeSlider));
        m_env->addCRDelegate("makeCheckBox", makeRDelegate(*this, &FormScriptEnvironment::makeCheckBox));
        m_env->addCRDelegate("makeComboBox", makeRDelegate(*this, &FormScriptEnvironment::makeComboBox));
        m_env->addCRDelegate("makePanel", makeRDelegate(*this, &FormScriptEnvironment::makePanel));
        m_env->addCRDelegate("setCallback", makeRDelegate(*m_form, &Form::registerCallback));
        m_env->setNamespaces();
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
    delete m_env;
    m_env = nullptr;
}

void vui::FormScriptEnvironment::registerConstants() {
    // Set enums
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
    { // vui::DockStyle
        m_env->setNamespaces("DockStyle");
        m_env->addValue("NONE", DockStyle::NONE);
        m_env->addValue("LEFT", DockStyle::LEFT);
        m_env->addValue("RIGHT", DockStyle::RIGHT);
        m_env->addValue("BOTTOM", DockStyle::BOTTOM);
        m_env->addValue("TOP", DockStyle::TOP);
        m_env->addValue("FILL", DockStyle::FILL);
        m_env->setNamespaces();
    }
    // Misc constants
    m_env->addValue("INT_MAX", INT_MAX);
    m_env->addValue("INT_MIN", INT_MIN);
}

vui::Button* vui::FormScriptEnvironment::makeButton(nString name, f32 x, f32 y, f32 width, f32 height) {
    vui::Button* b = new vui::Button(name, f32v4(x, y, width, height));
    m_buttonFuncs.registerWidget(b);
    m_form->addWidget(b); 
    return b;
}

vui::Slider* vui::FormScriptEnvironment::makeSlider(nString name, f32 x, f32 y, f32 width, f32 height) {
    vui::Slider* s = new vui::Slider(name, f32v4(x, y, width, height));
    m_sliderFuncs.registerWidget(s);
    m_form->addWidget(s);
    return s;
}

vui::CheckBox* vui::FormScriptEnvironment::makeCheckBox(nString name, f32 x, f32 y, f32 width, f32 height) {
    vui::CheckBox* c = new vui::CheckBox(name, f32v4(x, y, width, height));
    m_checkBoxFuncs.registerWidget(c);
    m_form->addWidget(c);
    return c;
}

vui::ComboBox* vui::FormScriptEnvironment::makeComboBox(nString name, f32 x, f32 y, f32 width, f32 height) {
    vui::ComboBox* c = new vui::ComboBox(name, f32v4(x, y, width, height));
    m_checkBoxFuncs.registerWidget(c);
    m_form->addWidget(c);
    return c;
}

vui::Panel* vui::FormScriptEnvironment::makePanel(nString name, f32 x, f32 y, f32 width, f32 height) {
    vui::Panel* p = new vui::Panel(name, f32v4(x, y, width, height));
    m_panelFuncs.registerWidget(p);
    m_form->addWidget(p);
    return p;
}
