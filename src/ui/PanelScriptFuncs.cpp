#include "Vorb/stdafx.h"
#include "Vorb/ui/PanelScriptFuncs.h"

#include "Vorb/ui/Panel.h"
#include "Vorb/script/IEnvironment.hpp"

template <typename ScriptEnvironmentImpl>
void vui::PanelScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getTexture",     makeDelegate(&impl::getTexture));
    env->addCDelegate("setTexture",     makeDelegate(&impl::setTexture));
    env->addCDelegate("getAutoScroll",  makeDelegate(&impl::getAutoScroll));
    env->addCDelegate("setAutoScroll",  makeDelegate(&impl::setAutoScroll));
    env->addCDelegate("getSliderWidth", makeDelegate(&impl::getSliderWidth));
    env->addCDelegate("setSliderWidth", makeDelegate(&impl::setSliderWidth));
    env->addCDelegate("getColor",       makeDelegate(&impl::getColor));
    env->addCDelegate("setColor",       makeDelegate(&impl::setColor));
    env->addCDelegate("getHoverColor",  makeDelegate(&impl::getHoverColor));
    env->addCDelegate("setHoverColor",  makeDelegate(&impl::setHoverColor));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::PanelScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    // Empty
}

VGTexture vui::PanelScriptFuncs::impl::getTexture(Panel* panel) {
    return panel->getTexture();
}

bool vui::PanelScriptFuncs::impl::getAutoScroll(Panel* panel) {
    return panel->getAutoScroll();
}

f32 vui::PanelScriptFuncs::impl::getSliderWidth(Panel* panel) {
    return panel->getSliderWidth();
}

color4 vui::PanelScriptFuncs::impl::getColor(Panel* panel) {
    return panel->getColor();
}

color4 vui::PanelScriptFuncs::impl::getHoverColor(Panel* panel) {
    return panel->getHoverColor();
}

void vui::PanelScriptFuncs::impl::setTexture(Panel* panel, VGTexture texture) {
    panel->setTexture(texture);
}

void vui::PanelScriptFuncs::impl::setAutoScroll(Panel* panel, bool autoScroll) {
    panel->setAutoScroll(autoScroll);
}

void vui::PanelScriptFuncs::impl::setSliderWidth(Panel* panel, f32 width) {
    panel->setSliderWidth(width);
}

void vui::PanelScriptFuncs::impl::setColor(Panel* panel, color4 color) {
    panel->setColor(color);
}

void vui::PanelScriptFuncs::impl::setHoverColor(Panel* panel, color4 color) {
    panel->setHoverColor(color);
}
