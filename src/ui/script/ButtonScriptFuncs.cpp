#include "Vorb/stdafx.h"
#include "Vorb/ui/script/ButtonScriptFuncs.h"

#include "Vorb/graphics/SpriteBatch.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/Button.h"
#include "Vorb/ui/script/TextWidgetScriptFuncs.h"

template <typename ScriptEnvironmentImpl>
void vui::ButtonScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getTexture",            makeDelegate(&impl::getTexture));
    env->addCDelegate("setTexture",            makeDelegate(&impl::setTexture));
    env->addCDelegate("getHoverTexture",       makeDelegate(&impl::getHoverTexture));
    env->addCDelegate("setHoverTexture",       makeDelegate(&impl::setHoverTexture));
    env->addCDelegate("getBackColor",          makeDelegate(&impl::getBackColor));
    env->addCDelegate("setBackColor",          makeDelegate(&impl::setBackColor));
    env->addCDelegate("getBackHoverColor",     makeDelegate(&impl::getBackHoverColor));
    env->addCDelegate("setBackHoverColor",     makeDelegate(&impl::setBackHoverColor));
    env->addCDelegate("getTextHoverColor",     makeDelegate(&impl::getTextHoverColor));
    env->addCDelegate("setTextHoverColor",     makeDelegate(&impl::setTextHoverColor));
    env->addCDelegate("setBackColorGrad",      makeDelegate(&impl::setBackColorGrad));
    env->addCDelegate("setBackHoverColorGrad", makeDelegate(&impl::setBackHoverColorGrad));
    env->setNamespaces();

    TextWidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::TextWidgetScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

VGTexture  vui::ButtonScriptFuncs::impl::getTexture(Button* button) {
    return button->getTexture();
}

VGTexture  vui::ButtonScriptFuncs::impl::getHoverTexture(Button* button) {
    return button->getHoverTexture();
}

color4  vui::ButtonScriptFuncs::impl::getBackColor(Button* button) {
    return button->getBackColor();
}

color4  vui::ButtonScriptFuncs::impl::getBackHoverColor(Button* button) {
    return button->getBackHoverColor();
}

color4  vui::ButtonScriptFuncs::impl::getTextHoverColor(Button* button) {
    return button->getTextHoverColor();
}

void  vui::ButtonScriptFuncs::impl::setTexture(Button* button, VGTexture texture) {
    button->setTexture(texture);
}

void  vui::ButtonScriptFuncs::impl::setHoverTexture(Button* button, VGTexture texture) {
    button->setTexture(texture);
}

void  vui::ButtonScriptFuncs::impl::setBackColor(Button* button, color4 color) {
    button->setBackColor(color);
}

void  vui::ButtonScriptFuncs::impl::setBackColorGrad(Button* button, color4 color1, color4 color2, vg::GradientType grad) {
    button->setBackColorGrad(color1, color2, grad);
}

void  vui::ButtonScriptFuncs::impl::setBackHoverColor(Button* button, color4 color) {
    button->setBackHoverColor(color);
}

void  vui::ButtonScriptFuncs::impl::setBackHoverColorGrad(Button* button, color4 color1, color4 color2, vg::GradientType grad) {
    button->setBackHoverColorGrad(color1, color2, grad);
}

void  vui::ButtonScriptFuncs::impl::setTextHoverColor(Button* button, color4 color) {
    button->setTextHoverColor(color);
}
