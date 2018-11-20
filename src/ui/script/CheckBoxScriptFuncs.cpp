#include "Vorb/stdafx.h"
#include "Vorb/ui/script/CheckBoxScriptFuncs.h"

#include "Vorb/ui/widgets/CheckBox.h"

void vui::CheckBoxScriptFuncs::impl::check(CheckBox* checkBox) {
    checkBox->check();
}

void vui::CheckBoxScriptFuncs::impl::uncheck(CheckBox* checkBox) {
    checkBox->uncheck();
}

void vui::CheckBoxScriptFuncs::impl::toggleChecked(CheckBox* checkBox) {
    checkBox->toggleChecked();
}

VGTexture vui::CheckBoxScriptFuncs::impl::getUncheckedTexture(CheckBox* checkBox) {
    return checkBox->getUncheckedTexture();
}

VGTexture vui::CheckBoxScriptFuncs::impl::getCheckedTexture(CheckBox* checkBox) {
    return checkBox->getCheckedTexture();
}

VGTexture vui::CheckBoxScriptFuncs::impl::getUncheckedHoverTexture(CheckBox* checkBox) {
    return checkBox->getUncheckedHoverTexture();
}

VGTexture vui::CheckBoxScriptFuncs::impl::getCheckedHoverTexture(CheckBox* checkBox) {
    return checkBox->getCheckedHoverTexture();
}

color4 vui::CheckBoxScriptFuncs::impl::getBoxColor(CheckBox* checkBox) {
    return checkBox->getBoxColor();
}

color4 vui::CheckBoxScriptFuncs::impl::getBoxHoverColor(CheckBox* checkBox) {
    return checkBox->getBoxHoverColor();
}

color4 vui::CheckBoxScriptFuncs::impl::getBoxCheckedColor(CheckBox* checkBox) {
    return checkBox->getBoxCheckedColor();
}

color4 vui::CheckBoxScriptFuncs::impl::getBoxCheckedHoverColor(CheckBox* checkBox) {
    return checkBox->getBoxCheckedHoverColor();
}

color4 vui::CheckBoxScriptFuncs::impl::getTextHoverColor(CheckBox* checkBox) {
    return checkBox->getTextHoverColor();
}

bool vui::CheckBoxScriptFuncs::impl::isChecked(CheckBox* checkBox) {
    return checkBox->isChecked();
}

void vui::CheckBoxScriptFuncs::impl::setCheckedTexture(CheckBox* checkBox, VGTexture texture) {
    checkBox->setCheckedTexture(texture);
}

void vui::CheckBoxScriptFuncs::impl::setUncheckedTexture(CheckBox* checkBox, VGTexture texture) {
    checkBox->setUncheckedTexture(texture);
}

void vui::CheckBoxScriptFuncs::impl::setCheckedHoverTexture(CheckBox* checkBox, VGTexture texture) {
    checkBox->setCheckedHoverTexture(texture);
}

void vui::CheckBoxScriptFuncs::impl::setUncheckedHoverTexture(CheckBox* checkBox, VGTexture texture) {
    checkBox->setUncheckedHoverTexture(texture);
}

void vui::CheckBoxScriptFuncs::impl::setBoxColor(CheckBox* checkBox, color4 color) {
    checkBox->setBoxColor(color);
}

void vui::CheckBoxScriptFuncs::impl::setBoxHoverColor(CheckBox* checkBox, color4 color) {
    checkBox->setBoxHoverColor(color);
}

void vui::CheckBoxScriptFuncs::impl::setBoxCheckedColor(CheckBox* checkBox, color4 color) {
    checkBox->setBoxCheckedColor(color);
}

void vui::CheckBoxScriptFuncs::impl::setBoxCheckedHoverColor(CheckBox* checkBox, color4 color) {
    checkBox->setBoxCheckedHoverColor(color);
}

void vui::CheckBoxScriptFuncs::impl::setTextHoverColor(CheckBox* checkBox, color4 color) {
    checkBox->setTextHoverColor(color);
}

void vui::CheckBoxScriptFuncs::impl::setChecked(CheckBox* checkBox, bool checked) {
    checkBox->setChecked(checked);
}

// void vui::CheckBoxScriptFuncs::impl::onValueChange(Sender s, bool b) {
//     CheckBox* w = (CheckBox*)s;
//     size_t sz = w->m_valueChangeFuncs.size();
//     for (size_t i = 0; i < sz && i < w->m_valueChangeFuncs.size(); i++) {
//         w->m_valueChangeFuncs[i](b);
//     }
// }
