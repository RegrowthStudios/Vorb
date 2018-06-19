#include "Vorb/stdafx.h"
#include "Vorb/ui/WidgetListScriptFuncs.h"
#include "Vorb/ui/WidgetList.h"
#include "Vorb/script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &WidgetListScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &WidgetListScriptFuncs::name));

void vui::WidgetListScriptFuncs::init(const cString nSpace, vscript::Environment* env) {
    // Call base register
    WidgetScriptFuncs::init(nSpace, env);

    env->setNamespaces(nSpace);

    { // Register all functions
        REGISTER_DEL(env, addItem);
        REGISTER_RDEL(env, addItemAtIndex);
        REGISTER_RDEL(env, removeItem);
        REGISTER_RDEL(env, removeItemAtIndex);
        // Getters
        REGISTER_RDEL(env, getTexture);
        REGISTER_RDEL(env, getBackColor);
        REGISTER_RDEL(env, getBackHoverColor);
        REGISTER_RDEL(env, getNumItems);
        REGISTER_RDEL(env, getSpacing);
        REGISTER_RDEL(env, getAutoScroll);     
        // Setters
        REGISTER_DEL(env, setTexture);
        REGISTER_DEL(env, setBackColor);
        REGISTER_DEL(env, setBackHoverColor);
        REGISTER_DEL(env, setSpacing);
        REGISTER_DEL(env, setAutoScroll);
    }
    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

void vui::WidgetListScriptFuncs::addItem(WidgetList* wl, Widget* w) {
    wl->addItem(w);
}

bool vui::WidgetListScriptFuncs::addItemAtIndex(WidgetList* wl, int index, Widget* w) {
    return wl->addItemAtIndex(index, w);
}

bool vui::WidgetListScriptFuncs::removeItem(WidgetList* wl, Widget* w) {
    return wl->removeItem(w);
}

bool vui::WidgetListScriptFuncs::removeItemAtIndex(WidgetList* wl, int index) {
    return wl->removeItem(index);
}

VGTexture vui::WidgetListScriptFuncs::getTexture(WidgetList* w) const {
    return w->getTexture();
}

color4 vui::WidgetListScriptFuncs::getBackColor(WidgetList* w) const {
    return w->getBackColor();
}

color4 vui::WidgetListScriptFuncs::getBackHoverColor(WidgetList* w) {
    return w->getBackHoverColor();
}

size_t vui::WidgetListScriptFuncs::getNumItems(WidgetList* w) const {
    return w->getNumItems();
}

f32 vui::WidgetListScriptFuncs::getSpacing(WidgetList* w) const {
    return w->getSpacing();
}

bool vui::WidgetListScriptFuncs::getAutoScroll(WidgetList* w) const {
    return w->getAutoScroll();
}

void vui::WidgetListScriptFuncs::setTexture(WidgetList* w, VGTexture texture) const {
    w->setTexture(texture);
}

void vui::WidgetListScriptFuncs::setBackColor(WidgetList* w, color4 color) const {
    w->setBackColor(color);
}

void vui::WidgetListScriptFuncs::setBackHoverColor(WidgetList* w, color4 color) const {
    w->setBackHoverColor(color);
}

void vui::WidgetListScriptFuncs::setSpacing(WidgetList* w, f32 spacing) const {
    w->setSpacing(spacing);
}

void vui::WidgetListScriptFuncs::setAutoScroll(WidgetList* w, bool autoScroll) const {
    w->setAutoScroll(autoScroll);
}