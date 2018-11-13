#include "Vorb/stdafx.h"
#include "Vorb/ui/script/ComboBoxScriptFuncs.h"

#include "Vorb/graphics/SpriteFont.h"
#include "Vorb/ui/ComboBox.h"


void vui::ComboBoxScriptFuncs::impl::addItem(ComboBox* comboBox, nString item) {
    comboBox->addItem(item);
}

bool vui::ComboBoxScriptFuncs::impl::addItemAtIndex(ComboBox* comboBox, size_t index, nString item) {
    return comboBox->addItemAtIndex(index, item);
}

bool vui::ComboBoxScriptFuncs::impl::removeItem(ComboBox* comboBox, nString item) {
    return comboBox->removeItem(item);
}

bool vui::ComboBoxScriptFuncs::impl::removeItemAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->removeItem(index);
}

bool vui::ComboBoxScriptFuncs::impl::selectItem(ComboBox* comboBox, nString item) {
    return comboBox->selectItem(item);
}

bool vui::ComboBoxScriptFuncs::impl::selectItemAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->selectItem(index);
}

bool vui::ComboBoxScriptFuncs::impl::isInDropBounds(ComboBox* comboBox, f32 x, f32 y) {
    return comboBox->isInDropBounds(x, y);
}

VGTexture vui::ComboBoxScriptFuncs::impl::getTexture(ComboBox* comboBox) {
    return comboBox->getTexture();
}

const vorb::graphics::SpriteFont* vui::ComboBoxScriptFuncs::impl::getFont(ComboBox* comboBox) {
    return comboBox->getFont();
}

color4 vui::ComboBoxScriptFuncs::impl::getBackColor(ComboBox* comboBox) {
    return comboBox->getBackColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getBackHoverColor(ComboBox* comboBox) {
    return comboBox->getBackHoverColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getTextColor(ComboBox* comboBox) {
    return comboBox->getTextColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getTextHoverColor(ComboBox* comboBox) {
    return comboBox->getTextHoverColor();
}

f32v2 vui::ComboBoxScriptFuncs::impl::getTextScale(ComboBox* comboBox) {
    return comboBox->getTextScale();
}

nString vui::ComboBoxScriptFuncs::impl::getItem(ComboBox* comboBox, size_t index) {
    return comboBox->getItem(index);
}

size_t vui::ComboBoxScriptFuncs::impl::getItemIndex(ComboBox* comboBox, nString item) {
    return comboBox->getItemIndex(item);
}

size_t vui::ComboBoxScriptFuncs::impl::getItemCount(ComboBox* comboBox) {
    return comboBox->getItemCount();
}

vg::TextAlign vui::ComboBoxScriptFuncs::impl::getTextAlign(ComboBox* comboBox) {
    return comboBox->getTextAlign();
}

nString vui::ComboBoxScriptFuncs::impl::getText(ComboBox* comboBox) {
    return comboBox->getText();
}

f32 vui::ComboBoxScriptFuncs::impl::getMaxDropHeight(ComboBox* comboBox) {
    return comboBox->getMaxDropHeight();
}

void vui::ComboBoxScriptFuncs::impl::setFont(ComboBox* comboBox, vg::SpriteFont* font) {
    comboBox->setFont(font);
}

void vui::ComboBoxScriptFuncs::impl::setMainButtonTexture(ComboBox* comboBox, VGTexture texture) {
    comboBox->setMainButtonTexture(texture);
}

void vui::ComboBoxScriptFuncs::impl::setDropBoxTexture(ComboBox* comboBox, VGTexture texture) {
    comboBox->setDropBoxTexture(texture);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTexture(ComboBox* comboBox, VGTexture texture) {
    comboBox->setDropButtonTexture(texture);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextureOfItem(ComboBox* comboBox, VGTexture texture, nString item) {
    comboBox->setDropButtonTexture(texture, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextureAtIndex(ComboBox* comboBox, VGTexture texture, size_t index) {
    comboBox->setDropButtonTexture(texture, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextureByCompare(ComboBox* comboBox, VGTexture texture, DropButtonComparator comparator) {
    comboBox->setDropButtonTexture(texture, comparator);
}

void vui::ComboBoxScriptFuncs::impl::setBackColor(ComboBox* comboBox, color4 color) {
    comboBox->setBackColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setMainButtonBackColor(ComboBox* comboBox, color4 color) {
    comboBox->setMainButtonBackColor(color);
}
void vui::ComboBoxScriptFuncs::impl::setDropButtonBackColor(ComboBox* comboBox, color4 color) {
    comboBox->setDropButtonBackColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonBackColorOfItem(ComboBox* comboBox, color4 color, nString item) {
    comboBox->setDropButtonBackColor(color, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonBackColorAtIndex(ComboBox* comboBox, color4 color, size_t index) {
    comboBox->setDropButtonBackColor(color, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonBackColorByCompare(ComboBox* comboBox, color4 color, DropButtonComparator comparator) {
    comboBox->setDropButtonBackColor(color, comparator);
}

void vui::ComboBoxScriptFuncs::impl::setBackHoverColor(ComboBox* comboBox, color4 color) {
    comboBox->setBackHoverColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setMainButtonBackHoverColor(ComboBox* comboBox, color4 color) {
    comboBox->setMainButtonBackHoverColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonBackHoverColor(ComboBox* comboBox, color4 color) {
    comboBox->setDropButtonBackHoverColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonBackHoverColorOfItem(ComboBox* comboBox, color4 color, nString item) {
    comboBox->setDropButtonBackHoverColor(color, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonBackHoverColorAtIndex(ComboBox* comboBox, color4 color, size_t index) {
    comboBox->setDropButtonBackHoverColor(color, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonBackHoverColorByCompare(ComboBox* comboBox, color4 color, DropButtonComparator comparator) {
    comboBox->setDropButtonBackHoverColor(color, comparator);
}

void vui::ComboBoxScriptFuncs::impl::setTextColor(ComboBox* comboBox, color4 color) {
    comboBox->setTextColor(color);
}
void vui::ComboBoxScriptFuncs::impl::setMainButtonTextColor(ComboBox* comboBox, color4 color) {
    comboBox->setMainButtonTextColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextColor(ComboBox* comboBox, color4 color) {
    comboBox->setDropButtonTextColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextColorOfItem(ComboBox* comboBox, color4 color, nString item) {
    comboBox->setDropButtonTextColor(color, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextColorAtIndex(ComboBox* comboBox, color4 color, size_t index) {
    comboBox->setDropButtonTextColor(color, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextColorByCompare(ComboBox* comboBox, color4 color, DropButtonComparator comparator) {
    comboBox->setDropButtonTextColor(color, comparator);
}

void vui::ComboBoxScriptFuncs::impl::setTextHoverColor(ComboBox* comboBox, color4 color) {
    comboBox->setTextHoverColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setMainButtonTextHoverColor(ComboBox* comboBox, color4 color) {
    comboBox->setMainButtonTextHoverColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextHoverColor(ComboBox* comboBox, color4 color) {
    comboBox->setDropButtonTextHoverColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextHoverColorOfItem(ComboBox* comboBox, color4 color, nString item) {
    comboBox->setDropButtonTextHoverColor(color, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextHoverColorAtIndex(ComboBox* comboBox, color4 color, size_t index) {
    comboBox->setDropButtonTextHoverColor(color, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextHoverColorByCompare(ComboBox* comboBox, color4 color, DropButtonComparator comparator) {
    comboBox->setDropButtonTextHoverColor(color, comparator);
}

void vui::ComboBoxScriptFuncs::impl::setTextScale(ComboBox* comboBox, f32v2 textScale) {
    comboBox->setTextScale(textScale);
}

void vui::ComboBoxScriptFuncs::impl::setMainButtonTextScale(ComboBox* comboBox, f32v2 textScale) {
    comboBox->setMainButtonTextScale(textScale);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextScale(ComboBox* comboBox, f32v2 textScale) {
    comboBox->setDropButtonTextScale(textScale);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextScaleOfItem(ComboBox* comboBox, f32v2 textScale, nString item) {
    comboBox->setDropButtonTextScale(textScale, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextScaleAtIndex(ComboBox* comboBox, f32v2 textScale, size_t index) {
    comboBox->setDropButtonTextScale(textScale, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextScaleByCompare(ComboBox* comboBox, f32v2 textScale, DropButtonComparator comparator) {
    comboBox->setDropButtonTextScale(textScale, comparator);
}

void vui::ComboBoxScriptFuncs::impl::setTextAlign(ComboBox* comboBox, vg::TextAlign align) {
    comboBox->setTextAlign(align);
}

void vui::ComboBoxScriptFuncs::impl::setMainButtonTextAlign(ComboBox* comboBox, vg::TextAlign align) {
    comboBox->setMainButtonTextAlign(align);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextAlign(ComboBox* comboBox, vg::TextAlign align) {
    comboBox->setDropButtonTextAlign(align);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextAlignOfItem(ComboBox* comboBox, vg::TextAlign align, nString item) {
    comboBox->setDropButtonTextAlign(align, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextAlignAtIndex(ComboBox* comboBox, vg::TextAlign align, size_t index) {
    comboBox->setDropButtonTextAlign(align, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonTextAlignByCompare(ComboBox* comboBox, vg::TextAlign align, DropButtonComparator comparator) {
    comboBox->setDropButtonTextAlign(align, comparator);
}

void vui::ComboBoxScriptFuncs::impl::setText(ComboBox* comboBox, nString text) {
    comboBox->setText(text);
}

void vui::ComboBoxScriptFuncs::impl::setMaxDropHeight(ComboBox* comboBox, f32 maxDropHeight) {
    comboBox->setMaxDropHeight(maxDropHeight);
}

// void vui::ComboBoxScriptFuncs::impl::onValueChange(Sender s, nString& v) {
//     ComboBox* w = (ComboBox*)s;
//     size_t sz = w->m_valueChangeFuncs.size();
//     for (size_t i = 0; i < sz && i < w->m_valueChangeFuncs.size(); i++) {
//         w->m_valueChangeFuncs[i](v);
//     }
// }
