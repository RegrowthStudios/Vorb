#include "stdafx.h"
#include "ui/ButtonScriptFuncs.h"
#include "ui/Button.h"
#include "script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env.addCRDelegate(#name, makeRDelegate(*this, &T::name));
#define REGISTER_DEL(env, name) env.addCDelegate(#name, makeDelegate(*this, &T::name));

template <class T>
void vui::ButtonScriptFuncs::registerFuncs(const cString nSpace, vscript::Environment& env) {
    // Call base register
    WidgetScriptFuncs::registerFuncs<ButtonScriptFuncs>(nSpace, env);

    env.setNamespaces(nSpace);

    { // Register all functions
        // Getters
        REGISTER_RDEL(env, getTexture);
        REGISTER_RDEL(env, getBackColor);
        REGISTER_RDEL(env, getBackHoverColor);
        REGISTER_RDEL(env, getTextColor);
        REGISTER_RDEL(env, getTextHoverColor);
        REGISTER_RDEL(env, getText);
        REGISTER_RDEL(env, getTextAlign);
        REGISTER_RDEL(env, getTextScale);
        // Setters
        REGISTER_DEL(env, setBackColor);
        REGISTER_DEL(env, setBackHoverColor);
        REGISTER_DEL(env, setText);
        REGISTER_DEL(env, setTextColor);
        REGISTER_DEL(env, setTextHoverColor);
        REGISTER_DEL(env, setTextAlign);
        REGISTER_DEL(env, setTextScale);
    }
    env.setNamespaces();
}
// Explicit templates so we can have code in the cpp file
template void vui::ButtonScriptFuncs::registerFuncs<vui::ButtonScriptFuncs>(const cString nSpace, vscript::Environment& env);

#undef REGISTER_RDEL
#undef REGISTER_DEL

VGTexture vorb::ui::ButtonScriptFuncs::getTexture(Button* b) const {
    return b->getTexture();
}

color4 vorb::ui::ButtonScriptFuncs::getBackColor(Button* b) const {
    return b->getBackColor();
}

color4 vorb::ui::ButtonScriptFuncs::getBackHoverColor(Button* b) const {
    return b->getBackHoverColor();
}

color4 vorb::ui::ButtonScriptFuncs::getTextColor(Button* b) const {
    return b->getTextColor();
}

color4 vorb::ui::ButtonScriptFuncs::getTextHoverColor(Button* b) const {
    return b->getTextHoverColor();
}

nString vorb::ui::ButtonScriptFuncs::getText(Button* b) const {
    return b->getText();
}

vg::TextAlign vorb::ui::ButtonScriptFuncs::getTextAlign(Button* b) const {
    return b->getTextAlign();
}

f32v2 vorb::ui::ButtonScriptFuncs::getTextScale(Button* b) const {
    return b->getTextScale();
}

void vorb::ui::ButtonScriptFuncs::setBackColor(Button* b, color4 color) {
    b->setBackColor(color);
}

void vorb::ui::ButtonScriptFuncs::setBackHoverColor(Button* b, color4 color) {
    b->setBackHoverColor(color);
}

void vorb::ui::ButtonScriptFuncs::setText(Button* b, nString text) {
    b->setText(text);
}

void vorb::ui::ButtonScriptFuncs::setTextColor(Button* b, color4 color) {
    b->setTextColor(color);
}

void vorb::ui::ButtonScriptFuncs::setTextHoverColor(Button* b, color4 color) {
    b->setTextHoverColor(color);
}

void vorb::ui::ButtonScriptFuncs::setTextAlign(Button* b, vg::TextAlign textAlign) {
    b->setTextAlign(textAlign);
}

void vorb::ui::ButtonScriptFuncs::setTextScale(Button* b, f32v2 textScale) {
    b->setTextScale(textScale);
}
