#include "stdafx.h"
#include "ui/WidgetScriptFuncs.h"
#include "ui/Widget.h"
#include "ui/ButtonScriptFuncs.h"
#include "script/Environment.h"
#include "VorbPreDecl.inl"


// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env.addCRDelegate(#name, makeRDelegate(*this, &T::name));
#define REGISTER_DEL(env, name) env.addCDelegate(#name, makeDelegate(*this, &T::name));

template <class T = WidgetScriptFuncs>
void vui::WidgetScriptFuncs::registerFuncs(const cString nSpace, vscript::Environment& env) {
    env.setNamespaces(nSpace);

    { // Register all functions
        // Getters
        REGISTER_RDEL(env, getFixedHeight);
        REGISTER_RDEL(env, getFixedWidth);
        REGISTER_RDEL(env, getSelectable);
        REGISTER_RDEL(env, isMouseIn);
        REGISTER_RDEL(env, getAnchor);
        REGISTER_RDEL(env, getStyle);
        REGISTER_RDEL(env, getDock);
        REGISTER_RDEL(env, isEnabled);
        REGISTER_RDEL(env, getHeight);
        REGISTER_RDEL(env, getWidth);
        REGISTER_RDEL(env, getX);
        REGISTER_RDEL(env, getY);
        REGISTER_RDEL(env, getDimensions);
        REGISTER_RDEL(env, getPosition);
        REGISTER_RDEL(env, getRelativePosition);
        //REGISTER_RDEL(env, getWidgets);
        // Setters
        REGISTER_DEL(env, setAnchor);
        REGISTER_DEL(env, setDestRect);
        REGISTER_DEL(env, setDimensions);
        REGISTER_DEL(env, setDock);
        REGISTER_DEL(env, setFixedHeight);
        REGISTER_DEL(env, setFixedWidth);
        REGISTER_DEL(env, setHeight);
        REGISTER_DEL(env, setPosition);
        REGISTER_DEL(env, setSelectable);
        REGISTER_DEL(env, setStyle);
        REGISTER_DEL(env, setWidth);
        REGISTER_DEL(env, setX);
        REGISTER_DEL(env, setY);
        // Misc
        REGISTER_DEL(env, dispose);
        REGISTER_DEL(env, enable);
        REGISTER_DEL(env, disable);
    }
    env.setNamespaces();
}
// Explicit templates so we can have code in the cpp file
template void vui::WidgetScriptFuncs::registerFuncs<vui::WidgetScriptFuncs>(const cString nSpace, vscript::Environment& env);
template void vui::WidgetScriptFuncs::registerFuncs<vui::ButtonScriptFuncs>(const cString nSpace, vscript::Environment& env);

#undef REGISTER_RDEL
#undef REGISTER_DEL

vui::WidgetID vui::WidgetScriptFuncs::registerWidget(Widget* widget) {
    WidgetID id = m_idGenerator.generate();
    m_widgets[id] = widget;
    return id;
}

void vui::WidgetScriptFuncs::unregisterWidget(WidgetID id) {
    m_widgets.erase(id);
    m_idGenerator.recycle(id);
}

vui::Widget* vui::WidgetScriptFuncs::getWidget(WidgetID id) const {
    auto& it = m_widgets.find(id);
    if (it == m_widgets.end()) return nullptr;
    return it->second;
}

void vui::WidgetScriptFuncs::dispose(WidgetID id) {
    Widget* w = getWidget(id);
    w->dispose();
}

void vui::WidgetScriptFuncs::enable(WidgetID id) {
    Widget* w = getWidget(id);
    w->enable();
}

void vui::WidgetScriptFuncs::disable(WidgetID id) {
    Widget* w = getWidget(id);
    w->disable();
}

bool vui::WidgetScriptFuncs::getFixedHeight(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getFixedHeight();
}

bool vui::WidgetScriptFuncs::getFixedWidth(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getFixedWidth();
}

bool vui::WidgetScriptFuncs::getSelectable(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getSelectable();
}

bool vui::WidgetScriptFuncs::isMouseIn(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->isMouseIn();
}

int vui::WidgetScriptFuncs::getAnchor(WidgetID id) const {
    return 0; // TODO(Ben): Implement
}

int vui::WidgetScriptFuncs::getStyle(WidgetID id) const {
    return 0; // TODO(Ben): Implement
}

int vui::WidgetScriptFuncs::getDock(WidgetID id) const {
    return 0; // TODO(Ben): Implement
}

bool vui::WidgetScriptFuncs::isEnabled(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->isEnabled();
}

f32 vui::WidgetScriptFuncs::getHeight(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getHeight();
}

f32 vui::WidgetScriptFuncs::getWidth(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getWidth();
}

f32 vui::WidgetScriptFuncs::getX(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getX();
}

f32 vui::WidgetScriptFuncs::getY(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getY();
}

f32v2 vui::WidgetScriptFuncs::getDimensions(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getDimensions();
}

f32v2 vui::WidgetScriptFuncs::getPosition(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getPosition();
}

f32v2 vui::WidgetScriptFuncs::getRelativePosition(WidgetID id) const {
    Widget* w = getWidget(id);
    return w->getRelativePosition();
}

std::vector<vui::WidgetID> vui::WidgetScriptFuncs::getWidgets(WidgetID id) const {
    return std::vector<vui::WidgetID>(); // TODO(Ben): Implement
}

void vui::WidgetScriptFuncs::setAnchor(WidgetID id, int anchor) {
    // TODO(Ben): Implement
}

void vui::WidgetScriptFuncs::setDestRect(WidgetID id, f32v4 destRect) {
    Widget* w = getWidget(id);
    w->setDestRect(destRect);
}

void vui::WidgetScriptFuncs::setDimensions(WidgetID id, f32v2 dims) {
    Widget* w = getWidget(id);
    w->setDimensions(dims);
}

void vui::WidgetScriptFuncs::setDock(WidgetID id, int dock) {
    // TODO(Ben): Implement
}

void vui::WidgetScriptFuncs::setFixedHeight(WidgetID id, bool fixedHeight) {
    Widget* w = getWidget(id);
    w->setFixedHeight(fixedHeight);
}

void vui::WidgetScriptFuncs::setFixedWidth(WidgetID id, bool fixedWidth) {
    Widget* w = getWidget(id);
    w->setFixedWidth(fixedWidth);
}

void vui::WidgetScriptFuncs::setHeight(WidgetID id, f32 height) {
    Widget* w = getWidget(id);
    w->setHeight(height);
}

void vui::WidgetScriptFuncs::setPosition(WidgetID id, f32v2 pos) {
    Widget* w = getWidget(id);
    w->setPosition(pos);
}

void vui::WidgetScriptFuncs::setSelectable(WidgetID id, bool selectable) {
    Widget* w = getWidget(id);
    w->setSelectable(selectable);
}

void vui::WidgetScriptFuncs::setStyle(WidgetID id, int style) {
    // TODO(Ben): Implement
}

void vui::WidgetScriptFuncs::setWidth(WidgetID id, f32 width) {
    Widget* w = getWidget(id);
    w->setWidth(width);
}

void vui::WidgetScriptFuncs::setX(WidgetID id, f32 x) {
    Widget* w = getWidget(id);
    w->setX(x);
}

void vui::WidgetScriptFuncs::setY(WidgetID id, f32 y) {
    Widget* w = getWidget(id);
    w->setY(y);
}
