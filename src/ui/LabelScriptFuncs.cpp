#include "Vorb/stdafx.h"
#include "Vorb/ui/LabelScriptFuncs.h"

#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/Label.h"
#include "Vorb/ui/TextWidgetScriptFuncs.h"

template <typename ScriptEnvironmentImpl>
void vui::LabelScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getLabelColor",      makeDelegate(&impl::getLabelColor));
    env->addCDelegate("setLabelColor",      makeDelegate(&impl::setLabelColor));
    env->addCDelegate("getLabelHoverColor",      makeDelegate(&impl::getLabelHoverColor));
    env->addCDelegate("setLabelHoverColor",      makeDelegate(&impl::setLabelHoverColor));
    env->addCDelegate("getLabelTexture", makeDelegate(&impl::getLabelTexture));
    env->addCDelegate("setLabelTexture", makeDelegate(&impl::setLabelTexture));
    env->addCDelegate("getLabelHoverTexture", makeDelegate(&impl::getLabelHoverTexture));
    env->addCDelegate("setLabelHoverTexture", makeDelegate(&impl::setLabelHoverTexture));
    env->addCDelegate("getTextHoverColor", makeDelegate(&impl::getTextHoverColor));
    env->addCDelegate("setTextHoverColor", makeDelegate(&impl::setTextHoverColor));
    env->setNamespaces();

    TextWidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::LabelScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    // Empty
}

color4 vui::LabelScriptFuncs::impl::getLabelColor(Label* label) {
    return label->getLabelColor();
}

color4 vui::LabelScriptFuncs::impl::getLabelHoverColor(Label* label) {
    return label->getLabelHoverColor();
}

VGTexture vui::LabelScriptFuncs::impl::getLabelTexture(Label* label) {
    return label->getLabelTexture();
}

VGTexture vui::LabelScriptFuncs::impl::getLabelHoverTexture(Label* label) {
    return label->getLabelHoverTexture();
}

color4 vui::LabelScriptFuncs::impl::getTextHoverColor(Label* label) {
    return label->getTextHoverColor();
}

void vui::LabelScriptFuncs::impl::setLabelColor(Label* label, color4 color) {
    label->setLabelColor(color);
}

void vui::LabelScriptFuncs::impl::setLabelHoverColor(Label* label, color4 color) {
    label->setLabelHoverColor(color);
}

void vui::LabelScriptFuncs::impl::setLabelTexture(Label* label, VGTexture texture) {
    label->setLabelTexture(texture);
}

void vui::LabelScriptFuncs::impl::setLabelHoverTexture(Label* label, VGTexture texture) {
    label->setLabelHoverTexture(texture);
}

void vui::LabelScriptFuncs::impl::setTextHoverColor(Label* label, color4 color) {
    label->setTextHoverColor(color);
}
