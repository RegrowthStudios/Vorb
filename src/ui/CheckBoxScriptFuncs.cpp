#include "Vorb/stdafx.h"
#include "Vorb/ui/CheckBoxScriptFuncs.h"
#include "Vorb/ui/CheckBox.h"
#include "Vorb/script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &CheckBoxScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &CheckBoxScriptFuncs::name));

void vui::CheckBoxScriptFuncs::init(const cString nSpace, vscript::Environment* env) {
    // Call base register
    WidgetScriptFuncs::init(nSpace, env);

    env->setNamespaces(nSpace);

    { // Register all functions
        // Getters
        REGISTER_RDEL(env, getUncheckedTexture);
        REGISTER_RDEL(env, getCheckedTexture);
        REGISTER_RDEL(env, getBoxColor);
        REGISTER_RDEL(env, getBoxHoverColor);
        REGISTER_RDEL(env, getBoxCheckedColor);
        REGISTER_RDEL(env, getBoxCheckedHoverColor);
        REGISTER_RDEL(env, getTextColor);
        REGISTER_RDEL(env, getText);
        REGISTER_RDEL(env, getTextAlign);
        REGISTER_RDEL(env, getTextScale);
        REGISTER_RDEL(env, isChecked);
        // Setters
        REGISTER_DEL(env, setCheckedTexture);
        REGISTER_DEL(env, setUncheckedTexture);
        REGISTER_DEL(env, setBoxColor);
        REGISTER_DEL(env, setBoxHoverColor);
        REGISTER_DEL(env, setBoxCheckedColor);
        REGISTER_DEL(env, setBoxCheckedHoverColor);
        REGISTER_DEL(env, setText);
        REGISTER_DEL(env, setTextColor);
        REGISTER_DEL(env, setTextAlign);
        REGISTER_DEL(env, setTextScale);
        REGISTER_DEL(env, setChecked);

        REGISTER_DEL(env, check);
        REGISTER_DEL(env, unCheck);
        REGISTER_DEL(env, toggleChecked);
    }
    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

void vui::CheckBoxScriptFuncs::registerWidget(Widget* w) {
    WidgetScriptFuncs::registerWidget(w);
    CheckBox* c = (CheckBox*)w;
    c->ValueChange += makeDelegate(*this, &CheckBoxScriptFuncs::onValueChange);
}

void vui::CheckBoxScriptFuncs::unregisterWidget(Widget* w) {
    WidgetScriptFuncs::unregisterWidget(w);
    CheckBox* c = (CheckBox*)w;
    c->ValueChange -= makeDelegate(*this, &CheckBoxScriptFuncs::onValueChange);
}

bool vui::CheckBoxScriptFuncs::addCallback(Widget* w, EventType eventType, nString funcName) {
    const vscript::Function& f = (*m_env)[funcName];
    if (f.isNil()) return false;
    CheckBox* c = (CheckBox*)w;
    switch (eventType) {
        case EventType::VALUE_CHANGE:
            c->m_valueChangeFuncs.push_back(f); break;
        default:
            return false;
    }
    return true;
}

bool vui::CheckBoxScriptFuncs::removeCallback(Widget* w, EventType eventType, nString funcName) {
    const vscript::Function& f = (*m_env)[funcName];
    if (f.isNil()) return false;
    CheckBox* c = (CheckBox*)w;
    switch (eventType) {
        case EventType::VALUE_CHANGE:
            c->m_valueChangeFuncs.erase(std::find(c->m_valueChangeFuncs.begin(), c->m_valueChangeFuncs.end(), f));
            break;
        default:
            return WidgetScriptFuncs::removeCallback(w, eventType, funcName);
    }
    return true;
}

void vui::CheckBoxScriptFuncs::check(CheckBox* c) const {
    c->check();
}

void vui::CheckBoxScriptFuncs::unCheck(CheckBox* c) const {
    c->unCheck();
}

void vui::CheckBoxScriptFuncs::toggleChecked(CheckBox* c) const {
    c->toggleChecked();
}

VGTexture vui::CheckBoxScriptFuncs::getUncheckedTexture(CheckBox* c) const {
    return c->getUncheckedTexture();
}

VGTexture vui::CheckBoxScriptFuncs::getCheckedTexture(CheckBox* c) const {
    return c->getCheckedTexture();
}

color4 vui::CheckBoxScriptFuncs::getBoxColor(CheckBox* c) const {
    return c->getBoxColor();
}

color4 vui::CheckBoxScriptFuncs::getBoxHoverColor(CheckBox* c) const {
    return c->getBoxHoverColor();
}

color4 vui::CheckBoxScriptFuncs::getBoxCheckedColor(CheckBox* c) const {
    return c->getBoxCheckedColor();
}

color4 vui::CheckBoxScriptFuncs::getBoxCheckedHoverColor(CheckBox* c) const {
    return c->getBoxCheckedHoverColor();
}

color4 vui::CheckBoxScriptFuncs::getTextColor(CheckBox* c) const {
    return c->getTextColor();
}

nString vui::CheckBoxScriptFuncs::getText(CheckBox* c) const {
    return c->getText();
}

vg::TextAlign vui::CheckBoxScriptFuncs::getTextAlign(CheckBox* c) const {
    return c->getTextAlign();
}

f32v2 vui::CheckBoxScriptFuncs::getTextScale(CheckBox* c) const {
    return c->getTextScale();
}

bool vui::CheckBoxScriptFuncs::isChecked(CheckBox* c) const {
    return c->isChecked();
}

void vui::CheckBoxScriptFuncs::setCheckedTexture(CheckBox* c, VGTexture texture) const {
    c->setCheckedTexture(texture);
}

void vui::CheckBoxScriptFuncs::setUncheckedTexture(CheckBox* c, VGTexture texture) const {
    c->setUncheckedTexture(texture);
}

void vui::CheckBoxScriptFuncs::setBoxColor(CheckBox* c, color4 color) const {
    c->setBoxColor(color);
}

void vui::CheckBoxScriptFuncs::setBoxHoverColor(CheckBox* c, color4 color) const {
    c->setBoxHoverColor(color);
}

void vui::CheckBoxScriptFuncs::setBoxCheckedColor(CheckBox* c, color4 color) const {
    c->setBoxCheckedColor(color);
}

void vui::CheckBoxScriptFuncs::setBoxCheckedHoverColor(CheckBox* c, color4 color) const {
    c->setBoxCheckedHoverColor(color);
}

void vui::CheckBoxScriptFuncs::setText(CheckBox* c, nString text) const {
    c->setText(text);
}

void vui::CheckBoxScriptFuncs::setTextColor(CheckBox* c, color4 color) const {
    c->setTextColor(color);
}

void vui::CheckBoxScriptFuncs::setTextAlign(CheckBox* c, vg::TextAlign textAlign) const {
    c->setTextAlign(textAlign);
}

void vui::CheckBoxScriptFuncs::setTextScale(CheckBox* c, f32v2 textScale) const {
    c->setTextScale(textScale);
}

void vui::CheckBoxScriptFuncs::setChecked(CheckBox* c, bool checked) const {
    c->setChecked(checked);
}

void vui::CheckBoxScriptFuncs::onValueChange(Sender s, bool b) const {
    CheckBox* w = (CheckBox*)s;
    size_t sz = w->m_valueChangeFuncs.size();
    for (size_t i = 0; i < sz && i < w->m_valueChangeFuncs.size(); i++) {
        w->m_valueChangeFuncs[i](b);
    }
}
