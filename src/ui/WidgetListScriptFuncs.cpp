#include "Vorb/stdafx.h"
#include "Vorb/ui/WidgetListScriptFuncs.h"

#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/WidgetList.h"
#include "Vorb/ui/WidgetScriptFuncs.h"


template <typename ScriptEnvironmentImpl>
void vui::WidgetListScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("getTexture",      makeDelegate(&impl::getTexture));
    env->addCDelegate("setTexture",      makeDelegate(&impl::setTexture));
    env->addCDelegate("getAutoScroll",      makeDelegate(&impl::getAutoScroll));
    env->addCDelegate("setAutoScroll",      makeDelegate(&impl::setAutoScroll));
    env->addCDelegate("getBackColor", makeDelegate(&impl::getBackColor));
    env->addCDelegate("setBackColor", makeDelegate(&impl::setBackColor));
    env->addCDelegate("getBackHoverColor", makeDelegate(&impl::getBackHoverColor));
    env->addCDelegate("setBackHoverColor", makeDelegate(&impl::setBackHoverColor));
    env->addCDelegate("getItemCount", makeDelegate(&impl::getItemCount));
    env->addCDelegate("getSpacing", makeDelegate(&impl::getSpacing));
    env->addCDelegate("setSpacing", makeDelegate(&impl::setSpacing));
    env->addCDelegate("getMaxHeight", makeDelegate(&impl::getMaxHeight));
    env->addCDelegate("setMaxHeight", makeDelegate(&impl::setMaxHeight));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::WidgetListScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    // Empty
}

void vui::WidgetListScriptFuncs::impl::addItem(WidgetList* widgetList, IWidget* item) {
    widgetList->addItem(item);
}

bool vui::WidgetListScriptFuncs::impl::addItemAtIndex(WidgetList* widgetList, size_t index, IWidget* item) {
    return widgetList->addItemAtIndex(index, item);
}

bool vui::WidgetListScriptFuncs::impl::removeItem(WidgetList* widgetList, IWidget* item) {
    return widgetList->removeItem(item);
}

bool vui::WidgetListScriptFuncs::impl::removeItemAtIndex(WidgetList* widgetList, size_t index) {
    return widgetList->removeItem(index);
}

VGTexture vui::WidgetListScriptFuncs::impl::getTexture(WidgetList* widgetList) {
    return widgetList->getTexture();
}

bool vui::WidgetListScriptFuncs::impl::getAutoScroll(WidgetList* widgetList) {
    return widgetList->getAutoScroll();
}

color4 vui::WidgetListScriptFuncs::impl::getBackColor(WidgetList* widgetList) {
    return widgetList->getBackColor();
}

color4 vui::WidgetListScriptFuncs::impl::getBackHoverColor(WidgetList* widgetList) {
    return widgetList->getBackHoverColor();
}

size_t vui::WidgetListScriptFuncs::impl::getItemCount(WidgetList* widgetList) {
    return widgetList->getItemCount();
}

f32 vui::WidgetListScriptFuncs::impl::getSpacing(WidgetList* widgetList) {
    return widgetList->getSpacing();
}

f32 vui::WidgetListScriptFuncs::impl::getMaxHeight(WidgetList* widgetList) {
    return widgetList->getMaxHeight();
}

void vui::WidgetListScriptFuncs::impl::setTexture(WidgetList* widgetList, VGTexture texture) {
    widgetList->setTexture(texture);
}

void vui::WidgetListScriptFuncs::impl::setAutoScroll(WidgetList* widgetList, bool autoScroll) {
    widgetList->setAutoScroll(autoScroll);
}

void vui::WidgetListScriptFuncs::impl::setBackColor(WidgetList* widgetList, color4 color) {
    widgetList->setBackColor(color);
}

void vui::WidgetListScriptFuncs::impl::setBackHoverColor(WidgetList* widgetList, color4 color) {
    widgetList->setBackHoverColor(color);
}

void vui::WidgetListScriptFuncs::impl::setSpacing(WidgetList* widgetList, f32 spacing) {
    widgetList->setSpacing(spacing);
}

void vui::WidgetListScriptFuncs::impl::setMaxHeight(WidgetList* widgetList, f32 maxHeight) {
    widgetList->setMaxHeight(maxHeight);
}
