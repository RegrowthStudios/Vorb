#include "stdafx.h"
#include "ui/WidgetScriptFuncs.h"
#include "ui/Widget.h"
#include "ui/ButtonScriptFuncs.h"
#include "ui/SliderScriptFuncs.h"
#include "ui/CheckBoxScriptFuncs.h"
#include "ui/ComboBoxScriptFuncs.h"
#include "ui/PanelScriptFuncs.h"
#include "script/Environment.h"
#include "VorbPreDecl.inl"


// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env.addCRDelegate(#name, makeRDelegate(*this, &T::name));
#define REGISTER_DEL(env, name) env.addCDelegate(#name, makeDelegate(*this, &T::name));

template <class T>
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
        REGISTER_RDEL(env, getNumWidgets);
        REGISTER_RDEL(env, isEnabled);
        REGISTER_RDEL(env, getHeight);
        REGISTER_RDEL(env, getWidth);
        REGISTER_RDEL(env, getX);
        REGISTER_RDEL(env, getY);
        REGISTER_RDEL(env, getDimensions);
        REGISTER_RDEL(env, getPosition);
        REGISTER_RDEL(env, getRelativePosition);
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
template void vui::WidgetScriptFuncs::registerFuncs<vui::SliderScriptFuncs>(const cString nSpace, vscript::Environment& env);
template void vui::WidgetScriptFuncs::registerFuncs<vui::CheckBoxScriptFuncs>(const cString nSpace, vscript::Environment& env);
template void vui::WidgetScriptFuncs::registerFuncs<vui::ComboBoxScriptFuncs>(const cString nSpace, vscript::Environment& env);
template void vui::WidgetScriptFuncs::registerFuncs<vui::PanelScriptFuncs>(const cString nSpace, vscript::Environment& env);

#undef REGISTER_RDEL
#undef REGISTER_DEL

void vui::WidgetScriptFuncs::dispose(Widget* w) {
    w->dispose();
}

void vui::WidgetScriptFuncs::enable(Widget* w) {
    w->enable();
}

void vui::WidgetScriptFuncs::disable(Widget* w) {
    w->disable();
}

bool vui::WidgetScriptFuncs::getFixedHeight(Widget* w) const {
    return w->getFixedHeight();
}

bool vui::WidgetScriptFuncs::getFixedWidth(Widget* w) const {
    return w->getFixedWidth();
}

bool vui::WidgetScriptFuncs::getSelectable(Widget* w) const {
    return w->getSelectable();
}

bool vui::WidgetScriptFuncs::isMouseIn(Widget* w) const {
    return w->isMouseIn();
}

int vui::WidgetScriptFuncs::getAnchor(Widget* w) const {
    return 0; // TODO(Ben): Implement
}

int vui::WidgetScriptFuncs::getStyle(Widget* w) const {
    return 0; // TODO(Ben): Implement
}

int vui::WidgetScriptFuncs::getDock(Widget* w) const {
    return 0; // TODO(Ben): Implement
}

int vui::WidgetScriptFuncs::getNumWidgets(Widget* w) const {
    return w->getWidgets().size();
}

bool vui::WidgetScriptFuncs::isEnabled(Widget* w) const {
    return w->isEnabled();
}

f32 vui::WidgetScriptFuncs::getHeight(Widget* w) const {
    return w->getHeight();
}

f32 vui::WidgetScriptFuncs::getWidth(Widget* w) const {
    return w->getWidth();
}

f32 vui::WidgetScriptFuncs::getX(Widget* w) const {
    return w->getX();
}

f32 vui::WidgetScriptFuncs::getY(Widget* w) const {
    return w->getY();
}

f32v2 vui::WidgetScriptFuncs::getDimensions(Widget* w) const {
    return w->getDimensions();
}

f32v2 vui::WidgetScriptFuncs::getPosition(Widget* w) const {
    return w->getPosition();
}

f32v2 vui::WidgetScriptFuncs::getRelativePosition(Widget* w) const {
    return w->getRelativePosition();
}

void vui::WidgetScriptFuncs::setAnchor(Widget* w, int anchor) const {
    // TODO(Ben): Implement
}

void vui::WidgetScriptFuncs::setDestRect(Widget* w, f32v4 destRect) const {
    w->setDestRect(destRect);
}

void vui::WidgetScriptFuncs::setDimensions(Widget* w, f32v2 dims) const {
    w->setDimensions(dims);
}

void vui::WidgetScriptFuncs::setDock(Widget* w, DockStyle dock) const {
    w->setDock(dock);
}

void vui::WidgetScriptFuncs::setFixedHeight(Widget* w, bool fixedHeight) const {
    w->setFixedHeight(fixedHeight);
}

void vui::WidgetScriptFuncs::setFixedWidth(Widget* w, bool fixedWidth) const {
    w->setFixedWidth(fixedWidth);
}

void vui::WidgetScriptFuncs::setHeight(Widget* w, f32 height) const {
    w->setHeight(height);
}

void vui::WidgetScriptFuncs::setPosition(Widget* w, f32v2 pos) const {
    w->setPosition(pos);
}

void vui::WidgetScriptFuncs::setSelectable(Widget* w, bool selectable) const {
    w->setSelectable(selectable);
}

void vui::WidgetScriptFuncs::setStyle(Widget* w, int style) const {
    // TODO(Ben): Implement
}

void vui::WidgetScriptFuncs::setWidth(Widget* w, f32 width) const {
    w->setWidth(width);
}

void vui::WidgetScriptFuncs::setX(Widget* w, f32 x) const {
    w->setX(x);
}

void vui::WidgetScriptFuncs::setY(Widget* w, f32 y) const {
    w->setY(y);
}
