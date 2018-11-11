#include "Vorb/stdafx.h"
#include "Vorb/ui/TextWidgetScriptFuncs.h"

#include "Vorb/graphics/SpriteFont.h"
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/TextWidget.h"
#include "Vorb/ui/WidgetScriptFuncs.h"

template <typename ScriptEnvironmentImpl>
void vui::TextWidgetScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getFont",      makeDelegate(&impl::getFont));
    env->addCDelegate("setFont",      makeDelegate(&impl::setFont));
    env->addCDelegate("getText",      makeDelegate(&impl::getText));
    env->addCDelegate("setText",      makeDelegate(&impl::setText));
    env->addCDelegate("getTextAlign", makeDelegate(&impl::getTextAlign));
    env->addCDelegate("setTextAlign", makeDelegate(&impl::setTextAlign));
    env->addCDelegate("getTextColor", makeDelegate(&impl::getTextColor));
    env->addCDelegate("setTextColor", makeDelegate(&impl::setTextColor));
    env->addCDelegate("getTextScale", makeDelegate(&impl::getTextScale));
    env->addCDelegate("setTextScale", makeDelegate(&impl::setTextScale));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::TextWidgetScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", "TextAlign");
    env->addValue("LEFT", vg::TextAlign::LEFT);
    env->addValue("TOP_LEFT", vg::TextAlign::TOP_LEFT);
    env->addValue("TOP", vg::TextAlign::TOP);
    env->addValue("TOP_RIGHT", vg::TextAlign::TOP_RIGHT);
    env->addValue("RIGHT", vg::TextAlign::RIGHT);
    env->addValue("BOTTOM_RIGHT", vg::TextAlign::BOTTOM_RIGHT);
    env->addValue("BOTTOM", vg::TextAlign::BOTTOM);
    env->addValue("BOTTOM_LEFT", vg::TextAlign::BOTTOM_LEFT);
    env->addValue("CENTER", vg::TextAlign::CENTER);
    env->setNamespaces();
}

const vg::SpriteFont* vui::TextWidgetScriptFuncs::impl::getFont(TextWidget* textWidget) {
    return textWidget->getFont();
}

nString vui::TextWidgetScriptFuncs::impl::getText(TextWidget* textWidget) {
    return textWidget->getText();
}

vg::TextAlign vui::TextWidgetScriptFuncs::impl::getTextAlign(TextWidget* textWidget) {
    return textWidget->getTextAlign();
}

color4 vui::TextWidgetScriptFuncs::impl::getTextColor(TextWidget* textWidget) {
    return textWidget->getTextColor();
}

f32v2 vui::TextWidgetScriptFuncs::impl::getTextScale(TextWidget* textWidget) {
    return textWidget->getTextScale();
}

void vui::TextWidgetScriptFuncs::impl::setFont(TextWidget* textWidget, vg::SpriteFont* font) {
    textWidget->setFont(font);
}

void vui::TextWidgetScriptFuncs::impl::setText(TextWidget* textWidget, nString text) {
    textWidget->setText(text);
}

void vui::TextWidgetScriptFuncs::impl::setTextAlign(TextWidget* textWidget, vg::TextAlign textAlign) {
    textWidget->setTextAlign(textAlign);
}

void vui::TextWidgetScriptFuncs::impl::setTextColor(TextWidget* textWidget, color4 color) {
    textWidget->setTextColor(color);
}

void vui::TextWidgetScriptFuncs::impl::setTextScale(TextWidget* textWidget, f32v2 scale) {
    textWidget->setTextScale(scale);
}
