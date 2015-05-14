#include "stdafx.h"
#include "ui/ScriptedUI.h"
#include "ui/Form.h"
#include "ui/FormScriptEnvironment.h"

vui::ScriptedUI::ScriptedUI() {
    // Empty
}

vui::ScriptedUI::~ScriptedUI() {
    dispose();
}

void vui::ScriptedUI::init(const nString& startFormPath, IGameScreen* ownerScreen, const ui32v4& destRect, vg::SpriteFont* defaultFont /*= nullptr*/) {
    // Set up the first form
    m_ownerScreen = ownerScreen;
    m_destRect = destRect;
    m_defaultFont = defaultFont;
    Form* mainForm = makeForm("main", startFormPath);
    m_activeForm = mainForm;
}

void vui::ScriptedUI::draw() {
    for (auto& it : m_forms) {
        it.first->draw();
    }
}

void vui::ScriptedUI::update(f32 dt /*= 1.0f*/) {
    for (auto& it : m_forms) {
        it.first->update(dt);
    }
}

void vui::ScriptedUI::dispose() {
    for (auto& it : m_forms) {
        it.first->dispose();
        delete it.first;
        delete it.second;
    }
    std::vector<std::pair<Form*, FormScriptEnvironment*> >().swap(m_forms);
}

// TODO(Ben): use name
vui::Form* vui::ScriptedUI::makeForm(nString name, nString filePath) {
    // Make the form
    Form* newForm = new Form;
    newForm->init(name, m_ownerScreen, m_destRect, m_defaultFont);
    FormScriptEnvironment* newFormEnv = new FormScriptEnvironment;
    newFormEnv->init(newForm);
    m_forms.push_back(std::make_pair(newForm, newFormEnv));

    // Register callbacks
    // Register other functions with the env
    registerScriptValues(newFormEnv);

    // Load the script
    newFormEnv->loadForm(filePath.c_str());
    return newForm;
}

void vui::ScriptedUI::registerScriptValues(FormScriptEnvironment* newFormEnv) {
    vscript::Environment* env = newFormEnv->getEnv();
    env->setNamespaces();
    env->addCRDelegate("makeForm", makeRDelegate(*this, &ScriptedUI::makeForm));
    env->addCRDelegate("changeForm", makeRDelegate(*this, &ScriptedUI::changeForm));
}

vui::Form* vui::ScriptedUI::changeForm(nString nextForm) {
    for (auto& it : m_forms) {
        if (it.first->getName() == nextForm) {
            m_activeForm->disable();
            m_activeForm = it.first;
            m_activeForm->enable();
            return m_activeForm;
        }
    }
    return nullptr;
}