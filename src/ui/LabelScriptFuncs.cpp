#include "Vorb/stdafx.h"
#include "Vorb/ui/LabelScriptFuncs.h"
#include "Vorb/ui/Label.h"
#include "Vorb/script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &LabelScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &LabelScriptFuncs::name));

void vui::LabelScriptFuncs::init(const cString nSpace, vscript::Environment* env) {
    // Call base register
    WidgetScriptFuncs::init(nSpace, env);

    env->setNamespaces(nSpace);

    { // Register all functions
        // Getters
        REGISTER_RDEL(env, getTextColor);
        REGISTER_RDEL(env, getText);
        REGISTER_RDEL(env, getTextAlign);
        REGISTER_RDEL(env, getTextScale);
        // Setters
        REGISTER_DEL(env, setText);
        REGISTER_DEL(env, setTextColor);
        REGISTER_DEL(env, setTextAlign);
        REGISTER_DEL(env, setTextScale);
    }
    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

color4 vui::LabelScriptFuncs::getTextColor(Label* l) const {
    return l->getTextColor();
}

nString vui::LabelScriptFuncs::getText(Label* l) const {
    return l->getText();
}

vg::TextAlign vui::LabelScriptFuncs::getTextAlign(Label* l) const {
    return l->getTextAlign();
}

f32v2 vui::LabelScriptFuncs::getTextScale(Label* l) const {
    return l->getTextScale();
}

void vui::LabelScriptFuncs::setText(Label* l, nString text) const {
    l->setText(text);
}

void vui::LabelScriptFuncs::setTextColor(Label* l, color4 color) const {
    l->setTextColor(color);
}

void vui::LabelScriptFuncs::setTextAlign(Label* l, vg::TextAlign textAlign) const {
    l->setTextAlign(textAlign);
}

void vui::LabelScriptFuncs::setTextScale(Label* l, f32v2 textScale) const {
    l->setTextScale(textScale);
}
