#include "Vorb/stdafx.h"
#include "Vorb/ui/ScriptedUI.h"
#include "Vorb/ui/FormScriptEnvironment.h"

vui::ScriptedUI::ScriptedUI() {
    // Empty
}

vui::ScriptedUI::~ScriptedUI() {
    dispose();
}

void vui::ScriptedUI::init(const nString& startFormPath VORB_MAYBE_UNUSED, IGameScreen* ownerScreen,
                           const GameWindow* window, const f32v4& destRect,
                           vg::SpriteFont* defaultFont /*= nullptr*/) {
    // Set up the first form
    m_ownerScreen = ownerScreen;
    m_window = window;
    m_destRect = destRect;
    m_defaultFont = defaultFont;
    // makeForm("main", startFormPath);
}

void vui::ScriptedUI::draw() {
    // for (auto& it : m_forms) {
    //     it.first->draw();
    // }
}

void vui::ScriptedUI::update(f32 dt VORB_MAYBE_UNUSED /*= 1.0f*/) {
    // for (auto& it : m_forms) {
    //     it.first->update(dt);
    // }
}

void vui::ScriptedUI::dispose() {
    // for (auto& it : m_forms) {
    //     it.first->dispose();
    //     delete it.first;
    //     delete it.second;
    // }
    // std::vector<std::pair<Form*, FormScriptEnvironment*> >().swap(m_forms);
}

void vui::ScriptedUI::onOptionsChanged() {
    // for (auto& it : m_forms) {
    //     if (it.first->isEnabled()) it.second->onOptionsChanged();
    // }
}

void vui::ScriptedUI::setDimensions(VORB_MAYBE_UNUSED const f32v2& dimensions) {
    // for (VORB_MAYBE_UNUSED auto& it : m_forms) {
    //     // it.first->setDimensions(dimensions);
    // }
}

// TODO(Ben): use name
// vui::Form* vui::ScriptedUI::makeForm(VORB_MAYBE_UNUSED nString name, VORB_MAYBE_UNUSED nString filePath) {
//     // // Make the form
//     // Form* newForm = new Form;
//     // newForm->init(name, m_ownerScreen, m_destRect, m_defaultFont);
//     // FormScriptEnvironment* newFormEnv = new FormScriptEnvironment;
//     // newFormEnv->init(newForm, m_window);
//     // m_forms.push_back(std::make_pair(newForm, newFormEnv));

//     // // Register callbacks
//     // // Register other functions with the env
//     // registerScriptValues(newFormEnv);

//     // // Load the script
//     // newForm->disable();
//     // newFormEnv->loadForm(filePath.c_str());

//     // return newForm;
//     return nullptr;
// }

void vui::ScriptedUI::registerScriptValues(FormScriptEnvironment* newFormEnv VORB_MAYBE_UNUSED) {
//     vscript::Environment* env = newFormEnv->getEnv();
//     env->setNamespaces();
//     env->addCRDelegate("makeForm", makeRDelegate(*this, &ScriptedUI::makeForm));
//     env->addCRDelegate("enableForm", makeRDelegate(*this, &ScriptedUI::enableForm));
//     env->addCRDelegate("disableForm", makeRDelegate(*this, &ScriptedUI::disableForm));
//     env->addCRDelegate("getForm", makeRDelegate(*this, &ScriptedUI::getForm));
}

// vui::Form* vui::ScriptedUI::enableForm(nString name) {
//     // This is O(n) but its fine.
//     for (auto& it : m_forms) {
//         // What am I doing.
//         if (it.first->getName() == name) {
//             if (!it.first->isEnabled()) {
//                 it.first->enable();
//                 return it.first;
//             }
//         }
//     }
//     return nullptr;
// }

// vui::Form* vui::ScriptedUI::disableForm(nString name) {
//     // This is O(n) but its fine.
//     for (auto& it : m_forms) {
//         if (it.first->getName() == name) {
//             it.first->disable();
//             return it.first;
//         }
//     }
//     return nullptr;
// }

// vui::Form* vui::ScriptedUI::getForm(nString name) {
//     // This is O(n) but its fine.
//     for (auto& it : m_forms) {
//         if (it.first->getName() == name) {
//             return it.first;
//         }
//     }
//     return nullptr;
// }