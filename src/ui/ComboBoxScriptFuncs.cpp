#include "stdafx.h"
#include "ui/ComboBoxScriptFuncs.h"
#include "ui/ComboBox.h"
#include "script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &ComboBoxScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &ComboBoxScriptFuncs::name));

void vui::ComboBoxScriptFuncs::init(const cString nSpace, vscript::Environment* env) {
    // Call base register
    WidgetScriptFuncs::init(nSpace, env);

    env->setNamespaces(nSpace);

    { // Register all functions
        // Item functions
        REGISTER_DEL(env, addItem);
        REGISTER_RDEL(env, addItemAtIndex);
        REGISTER_RDEL(env, removeItem);
        REGISTER_RDEL(env, removeItemAtIndex);
        REGISTER_RDEL(env, selectItemAtIndex);

        // Getters
        REGISTER_RDEL(env, getTexture);
        REGISTER_RDEL(env, getBackColor);
        REGISTER_RDEL(env, getBackHoverColor);
        REGISTER_RDEL(env, getTextColor);
        REGISTER_RDEL(env, getTextHoverColor);
        REGISTER_RDEL(env, getTextScale);
        REGISTER_RDEL(env, getItem);
        REGISTER_RDEL(env, getNumItems);
        REGISTER_RDEL(env, getTextAlign);

        REGISTER_RDEL(env, isInDropBounds);
        // Setters
        REGISTER_DEL(env, setTexture);
        REGISTER_DEL(env, setDropBoxTexture);
        REGISTER_DEL(env, setDropButtonTexture);
        REGISTER_DEL(env, setBackColor);
        REGISTER_DEL(env, setBackHoverColor);
        REGISTER_DEL(env, setTextColor);
        REGISTER_DEL(env, setTextHoverColor);
        REGISTER_DEL(env, setTextScale);
        REGISTER_DEL(env, setTextAlign);
    }
    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

void vui::ComboBoxScriptFuncs::addItem(ComboBox* c, nString item) const {
    c->addItem(item);
}

bool vui::ComboBoxScriptFuncs::addItemAtIndex(ComboBox* c, int index, nString item) const {
    return c->addItemAtIndex(index, item);
}

bool vui::ComboBoxScriptFuncs::removeItem(ComboBox* c, nString item) const {
    return c->removeItem(item);
}

bool vui::ComboBoxScriptFuncs::removeItemAtIndex(ComboBox* c, int index) const {
    return c->removeItem(index);
}

bool vui::ComboBoxScriptFuncs::selectItemAtIndex(ComboBox* c, int index) const {
    return c->selectItem(index);
}

bool vui::ComboBoxScriptFuncs::isInDropBounds(ComboBox* c, f32v2 point) const {
    return c->isInDropBounds(point);
}

VGTexture vui::ComboBoxScriptFuncs::getTexture(ComboBox* c) const {
    return c->getTexture();
}

color4 vui::ComboBoxScriptFuncs::getBackColor(ComboBox* c) const {
    return c->getBackColor();
}

color4 vui::ComboBoxScriptFuncs::getBackHoverColor(ComboBox* c) const {
    return c->getBackHoverColor();
}

color4 vui::ComboBoxScriptFuncs::getTextColor(ComboBox* c) const {
    return c->getTextColor();
}

color4 vui::ComboBoxScriptFuncs::getTextHoverColor(ComboBox* c) const {
    return c->getTextHoverColor();
}

f32v2 vui::ComboBoxScriptFuncs::getTextScale(ComboBox* c) const {
    return c->getTextScale();
}

nString vui::ComboBoxScriptFuncs::getItem(ComboBox* c, int index) const {
    return c->getItem(index);
}

size_t vui::ComboBoxScriptFuncs::getNumItems(ComboBox* c) const {
    return c->getNumItems();
}

vg::TextAlign vui::ComboBoxScriptFuncs::getTextAlign(ComboBox* c) const {
    return c->getTextAlign();
}

void vui::ComboBoxScriptFuncs::setTexture(ComboBox* c, VGTexture texture) const {
    c->setTexture(texture);
}

void vui::ComboBoxScriptFuncs::setDropBoxTexture(ComboBox* c, VGTexture texture) const {
    c->setDropBoxTexture(texture);
}

void vui::ComboBoxScriptFuncs::setDropButtonTexture(ComboBox* c, VGTexture texture) const {
    c->setDropButtonTexture(texture);
}

void vui::ComboBoxScriptFuncs::setBackColor(ComboBox* c, color4 color) const {
    c->setBackColor(color);
}

void vui::ComboBoxScriptFuncs::setBackHoverColor(ComboBox* c, color4 color) const {
    c->setBackHoverColor(color);
}

void vui::ComboBoxScriptFuncs::setTextColor(ComboBox* c, color4 color) const {
    c->setTextColor(color);
}

void vui::ComboBoxScriptFuncs::setTextHoverColor(ComboBox* c, color4 color) const {
    c->setTextHoverColor(color);
}

void vui::ComboBoxScriptFuncs::setTextScale(ComboBox* c, f32v2 textScale) const {
    c->setTextScale(textScale);
}

void vui::ComboBoxScriptFuncs::setTextAlign(ComboBox* c, vg::TextAlign align) const {
    c->setTextAlign(align);
}