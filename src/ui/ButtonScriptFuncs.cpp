#include "Vorb/stdafx.h"
#include "Vorb/ui/ButtonScriptFuncs.h"
#include "Vorb/ui/Button.h"
#include "Vorb/script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &ButtonScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &ButtonScriptFuncs::name));

void vui::ButtonScriptFuncs::init(const cString nSpace, vscript::Environment* env) {
    // Call base register
    WidgetScriptFuncs::init(nSpace, env);

    env->setNamespaces(nSpace);

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
        REGISTER_DEL(env, setBackColorGrad);
        REGISTER_DEL(env, setBackHoverColor);
        REGISTER_DEL(env, setBackHoverColorGrad);
        REGISTER_DEL(env, setText);
        REGISTER_DEL(env, setTextColor);
        REGISTER_DEL(env, setTextHoverColor);
        REGISTER_DEL(env, setTextAlign);
        REGISTER_DEL(env, setTextScale);
        REGISTER_DEL(env, setTexture);
        
    }
    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

VGTexture vui::ButtonScriptFuncs::getTexture(Button* b) const {
    return b->getTexture();
}

color4 vui::ButtonScriptFuncs::getBackColor(Button* b) const {
    return b->getBackColor();
}

color4 vui::ButtonScriptFuncs::getBackHoverColor(Button* b) const {
    return b->getBackHoverColor();
}

color4 vui::ButtonScriptFuncs::getTextColor(Button* b) const {
    return b->getTextColor();
}

color4 vui::ButtonScriptFuncs::getTextHoverColor(Button* b) const {
    return b->getTextHoverColor();
}

nString vui::ButtonScriptFuncs::getText(Button* b) const {
    return b->getText();
}

vg::TextAlign vui::ButtonScriptFuncs::getTextAlign(Button* b) const {
    return b->getTextAlign();
}

f32v2 vui::ButtonScriptFuncs::getTextScale(Button* b) const {
    return b->getTextScale();
}

void vui::ButtonScriptFuncs::setBackColor(Button* b, color4 color) const {
    b->setBackColor(color);
}

void vui::ButtonScriptFuncs::setBackColorGrad(Button* b, color4 color1, color4 color2, vg::GradientType grad) const {
    b->setBackColorGrad(color1, color2, grad);
}

void vui::ButtonScriptFuncs::setBackHoverColor(Button* b, color4 color) const {
    b->setBackHoverColor(color);
}

void vui::ButtonScriptFuncs::setBackHoverColorGrad(Button* b, color4 color1, color4 color2, vg::GradientType grad) const {
    b->setBackHoverColorGrad(color1, color2, grad);
}

void vui::ButtonScriptFuncs::setText(Button* b, nString text) const {
    b->setText(text);
}

void vui::ButtonScriptFuncs::setTextColor(Button* b, color4 color) const {
    b->setTextColor(color);
}

void vui::ButtonScriptFuncs::setTextHoverColor(Button* b, color4 color) const {
    b->setTextHoverColor(color);
}

void vui::ButtonScriptFuncs::setTextAlign(Button* b, vg::TextAlign textAlign) const {
    b->setTextAlign(textAlign);
}

void vui::ButtonScriptFuncs::setTextScale(Button* b, f32v2 textScale) const {
    b->setTextScale(textScale);
}

void vui::ButtonScriptFuncs::setTexture(Button* b, VGTexture texture) const {
    b->setTexture(texture);
}
