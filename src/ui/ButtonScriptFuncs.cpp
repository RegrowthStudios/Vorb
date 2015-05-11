#include "stdafx.h"
#include "ui/ButtonScriptFuncs.h"
#include "ui/IButton.h"
#include "script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env.addCRDelegate(#name, makeRDelegate(*this, &T::name));
#define REGISTER_DEL(env, name) env.addCDelegate(#name, makeDelegate(*this, &T::name));

template <class T = ButtonScriptFuncs>
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

VGTexture vorb::ui::ButtonScriptFuncs::getTexture(Widget* w) const {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    return b->getTexture();
}

color4 vorb::ui::ButtonScriptFuncs::getBackColor(Widget* w) const {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    return b->getBackColor();
}

color4 vorb::ui::ButtonScriptFuncs::getBackHoverColor(Widget* w) const {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    return b->getBackHoverColor();
}

color4 vorb::ui::ButtonScriptFuncs::getTextColor(Widget* w) const {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    return b->getTextColor();
}

color4 vorb::ui::ButtonScriptFuncs::getTextHoverColor(Widget* w) const {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    return b->getTextHoverColor();
}

nString vorb::ui::ButtonScriptFuncs::getText(Widget* w) const {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    return b->getText();
}

vg::TextAlign vorb::ui::ButtonScriptFuncs::getTextAlign(Widget* w) const {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    return b->getTextAlign();
}

f32v2 vorb::ui::ButtonScriptFuncs::getTextScale(Widget* w) const {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    return b->getTextScale();
}

void vorb::ui::ButtonScriptFuncs::setBackColor(Widget* w, color4 color) {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    b->setBackColor(color);
}

void vorb::ui::ButtonScriptFuncs::setBackHoverColor(Widget* w, color4 color) {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    b->setBackHoverColor(color);
}

void vorb::ui::ButtonScriptFuncs::setText(Widget* w, nString text) {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    b->setText(text);
}

void vorb::ui::ButtonScriptFuncs::setTextColor(Widget* w, color4 color) {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    b->setTextColor(color);
}

void vorb::ui::ButtonScriptFuncs::setTextHoverColor(Widget* w, color4 color) {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    b->setTextHoverColor(color);
}

void vorb::ui::ButtonScriptFuncs::setTextAlign(Widget* w, int textAlign) {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    b->setTextAlign((vg::TextAlign)textAlign);
}

void vorb::ui::ButtonScriptFuncs::setTextScale(Widget* w, f32v2 textScale) {
    vui::IButton* b = static_cast<vui::IButton*>(w);
    b->setTextScale(textScale);
}
