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

void vui::ScriptedUI::init(const nString& startFormPath, IGameScreen* ownerScreen,
                           const GameWindow* window, const f32v4& destRect,
                           vg::SpriteFont* defaultFont /*= nullptr*/) {
    // Set up the first form
    m_ownerScreen = ownerScreen;
    m_window = window;
    m_destRect = destRect;
    m_defaultFont = defaultFont;
    Form* mainForm = makeForm("main", startFormPath);
    m_activeForms.push_back(std::make_pair(mainForm, m_forms[0].second));
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

void vui::ScriptedUI::onOptionsChanged() {
    for (auto& it : m_activeForms) {
        it.second->onOptionsChanged();
    }
}

void vui::ScriptedUI::setDimensions(const f32v2& dimensions) {
    for (auto& it : m_forms) {
        it.first->setDimensions(dimensions);
    }
}

// TODO(Ben): use name
vui::Form* vui::ScriptedUI::makeForm(nString name, nString filePath) {
    // Make the form
    Form* newForm = new Form;
    newForm->init(name, m_ownerScreen, m_destRect, m_defaultFont);
    FormScriptEnvironment* newFormEnv = new FormScriptEnvironment;
    newFormEnv->init(newForm, m_window);
    m_forms.push_back(std::make_pair(newForm, newFormEnv));

    // Register callbacks
    // Register other functions with the env
    registerScriptValues(newFormEnv);

    // Load the script
    newForm->disable();
    newFormEnv->loadForm(filePath.c_str());

    return newForm;
}

void vui::ScriptedUI::registerScriptValues(FormScriptEnvironment* newFormEnv) {
    vscript::Environment* env = newFormEnv->getEnv();
    env->setNamespaces();
    env->addCRDelegate("makeForm", makeRDelegate(*this, &ScriptedUI::makeForm));
    env->addCRDelegate("changeForm", makeRDelegate(*this, &ScriptedUI::changeFormString));
    env->addCRDelegate("changeForm", makeRDelegate(*this, &ScriptedUI::changeForm));
    env->addCRDelegate("enableForm", makeRDelegate(*this, &ScriptedUI::enableForm));
    env->addCRDelegate("disableForm", makeRDelegate(*this, &ScriptedUI::disableForm));
}

vui::Form* vui::ScriptedUI::changeFormString(nString nextForm) {
    for (auto& it : m_forms) {
        if (it.first->getName() == nextForm) {
            // TODO(Ben): This won't work in all situations. Give this more thought
            auto& mainForm = m_activeForms[0];
            mainForm.first->disable();
            mainForm.first = it.first;
            mainForm.second = it.second;
            mainForm.first->enable();
            return mainForm.first;
        }
    }
    return nullptr;
}

vui::Form* vui::ScriptedUI::changeForm(vui::Form* nextForm) {
    for (auto& it : m_forms) {
        if (it.first == nextForm) {
            // TODO(Ben): This won't work in all situations. Give this more thought
            auto& mainForm = m_activeForms[0];
            mainForm.first->disable();
            mainForm.first = it.first;
            mainForm.second = it.second;
            mainForm.first->enable();
            return mainForm.first;
        }
    }
    return nullptr;
}

vui::Form* vui::ScriptedUI::enableForm(vui::Form* form) {
    // This is O(n) but its fine.
    for (auto& it : m_forms) {
        // What am I doing.
        if (it.first == form) {
            if (!it.first->isEnabled()) {
                it.first->enable();
                m_activeForms.push_back(it);
                return it.first;
            }
        }
    }
    return nullptr;
}

vui::Form* vui::ScriptedUI::disableForm(vui::Form* form) {
    // This is O(n) but its fine.
    for (auto& it : m_forms) {
        if (it.first == form) {
            it.first->disable();
            // Remove from active forms list
            for (auto& it2 = m_activeForms.begin(); it2 != m_activeForms.end(); it2++) {
                if ((*it2).first == it.first) {
                    m_activeForms.erase(it2);
                    break;
                }
            }
            return it.first;
        }
    }
    return nullptr;
}
