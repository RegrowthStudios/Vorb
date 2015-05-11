#include "stdafx.h"
#include "ui/FormScriptEnvironment.h"
#include "ui/IButton.h"
#include "ui/Form.h"

#define INIT_FUNCTION_NAME "init"

vui::FormScriptEnvironment::FormScriptEnvironment() {
    // Empty
}

vui::FormScriptEnvironment::~FormScriptEnvironment() {
    // Empty
}

bool vui::FormScriptEnvironment::init(Form* form, const cString filePath) {
    m_form = form;

    { // Initialize callbacks
        m_env.setNamespaces("Form");
        m_env.addCRDelegate("makeButton", makeRDelegate(*this, &FormScriptEnvironment::makeButton));
        m_env.setNamespaces();
    }

    // Load script
    if (!m_env.load(filePath)) {
        fprintf(stderr, "Failed to load FormScriptEnvironment script %s\n", filePath);
        return false;
    }

    { // Call init function
        m_init = m_env[INIT_FUNCTION_NAME];
        m_init();
    }

    return true;
}

int vui::FormScriptEnvironment::makeButton(nString name, f32 x, f32 y, f32 width, f32 height) {
    vui::IButton* b = new vui::IButton(name, f32v4(x, y, width, height));
    m_form->addWidget(b);
    return 0;
}
