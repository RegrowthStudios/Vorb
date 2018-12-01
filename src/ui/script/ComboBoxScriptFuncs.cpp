#include "Vorb/stdafx.h"
#include "Vorb/ui/script/ComboBoxScriptFuncs.h"

#include "Vorb/graphics/SpriteFont.h"


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

VGTexture vui::ComboBoxScriptFuncs::impl::getMainButtonTexture(ComboBox* comboBox) {
    return comboBox->getMainButtonTexture();
}

VGTexture vui::ComboBoxScriptFuncs::impl::getDropBoxTexture(ComboBox* comboBox) {
    return comboBox->getDropBoxTexture();
}

VGTexture vui::ComboBoxScriptFuncs::impl::getDropButtonTextureOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonTexture(item);
}

VGTexture vui::ComboBoxScriptFuncs::impl::getDropButtonTextureAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonTexture(index);
}

VGTexture vui::ComboBoxScriptFuncs::impl::getMainButtonHoverTexture(ComboBox* comboBox) {
    return comboBox->getMainButtonHoverTexture();
}

VGTexture vui::ComboBoxScriptFuncs::impl::getDropBoxHoverTexture(ComboBox* comboBox) {
    return comboBox->getDropBoxHoverTexture();
}

VGTexture vui::ComboBoxScriptFuncs::impl::getDropButtonHoverTextureOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonHoverTexture(item);
}

VGTexture vui::ComboBoxScriptFuncs::impl::getDropButtonHoverTextureAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonHoverTexture(index);
}

const vorb::graphics::SpriteFont* vui::ComboBoxScriptFuncs::impl::getMainButtonFont(ComboBox* comboBox) {
    return comboBox->getMainButtonFont();
}

const vorb::graphics::SpriteFont* vui::ComboBoxScriptFuncs::impl::getDropButtonFontOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonFont(item);
}

const vorb::graphics::SpriteFont* vui::ComboBoxScriptFuncs::impl::getDropButtonFontAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonFont(index);
}

color4 vui::ComboBoxScriptFuncs::impl::getMainButtonBackColor(ComboBox* comboBox) {
    return comboBox->getMainButtonBackColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getDropBoxBackColor(ComboBox* comboBox) {
    return comboBox->getDropBoxBackColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getDropButtonBackColorOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonBackColor(item);
}

color4 vui::ComboBoxScriptFuncs::impl::getDropButtonBackColorAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonBackColor(index);
}

color4 vui::ComboBoxScriptFuncs::impl::getMainButtonBackHoverColor(ComboBox* comboBox) {
    return comboBox->getMainButtonBackHoverColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getDropBoxBackHoverColor(ComboBox* comboBox) {
    return comboBox->getDropBoxBackHoverColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getDropButtonBackHoverColorOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonBackHoverColor(item);
}

color4 vui::ComboBoxScriptFuncs::impl::getDropButtonBackHoverColorAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonBackHoverColor(index);
}

color4 vui::ComboBoxScriptFuncs::impl::getMainButtonTextColor(ComboBox* comboBox) {
    return comboBox->getMainButtonTextColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getDropButtonTextColorOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonTextColor(item);
}

color4 vui::ComboBoxScriptFuncs::impl::getDropButtonTextColorAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonTextColor(index);
}

color4 vui::ComboBoxScriptFuncs::impl::getMainButtonTextHoverColor(ComboBox* comboBox) {
    return comboBox->getMainButtonTextHoverColor();
}

color4 vui::ComboBoxScriptFuncs::impl::getDropButtonTextHoverColorOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonTextHoverColor(item);
}

color4 vui::ComboBoxScriptFuncs::impl::getDropButtonTextHoverColorAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonTextHoverColor(index);
}

f32v2 vui::ComboBoxScriptFuncs::impl::getMainButtonTextScale(ComboBox* comboBox) {
    return comboBox->getMainButtonTextScale();
}

f32v2 vui::ComboBoxScriptFuncs::impl::getDropButtonTextScaleOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonTextScale(item);
}

f32v2 vui::ComboBoxScriptFuncs::impl::getDropButtonTextScaleAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonTextScale(index);
}

vg::TextAlign vui::ComboBoxScriptFuncs::impl::getMainButtonTextAlign(ComboBox* comboBox) {
    return comboBox->getMainButtonTextAlign();
}

vg::TextAlign vui::ComboBoxScriptFuncs::impl::getDropButtonTextAlignOfItem(ComboBox* comboBox, nString item) {
    return comboBox->getDropButtonTextAlign(item);
}

vg::TextAlign vui::ComboBoxScriptFuncs::impl::getDropButtonTextAlignAtIndex(ComboBox* comboBox, size_t index) {
    return comboBox->getDropButtonTextAlign(index);
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

nString vui::ComboBoxScriptFuncs::impl::getText(ComboBox* comboBox) {
    return comboBox->getText();
}

f32 vui::ComboBoxScriptFuncs::impl::getMaxDropHeight(ComboBox* comboBox) {
    return comboBox->getMaxDropHeight();
}

void vui::ComboBoxScriptFuncs::impl::setFont(ComboBox* comboBox, vg::SpriteFont* font) {
    comboBox->setFont(font);
}

void vui::ComboBoxScriptFuncs::impl::setMainButtonFont(ComboBox* comboBox, vg::SpriteFont* font) {
    comboBox->setMainButtonFont(font);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonFont(ComboBox* comboBox, vg::SpriteFont* font) {
    comboBox->setDropButtonFont(font);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonFontOfItem(ComboBox* comboBox, vg::SpriteFont* font, nString item) {
    comboBox->setDropButtonFont(font, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonFontAtIndex(ComboBox* comboBox, vg::SpriteFont* font, size_t index) {
    comboBox->setDropButtonFont(font, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonFontByCompare(ComboBox* comboBox, vg::SpriteFont* font, DropButtonComparator comparator) {
    comboBox->setDropButtonFont(font, comparator);
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

void vui::ComboBoxScriptFuncs::impl::setMainButtonHoverTexture(ComboBox* comboBox, VGTexture texture) {
    comboBox->setMainButtonHoverTexture(texture);
}

void vui::ComboBoxScriptFuncs::impl::setDropBoxHoverTexture(ComboBox* comboBox, VGTexture texture) {
    comboBox->setDropBoxHoverTexture(texture);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonHoverTexture(ComboBox* comboBox, VGTexture texture) {
    comboBox->setDropButtonHoverTexture(texture);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonHoverTextureOfItem(ComboBox* comboBox, VGTexture texture, nString item) {
    comboBox->setDropButtonHoverTexture(texture, item);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonHoverTextureAtIndex(ComboBox* comboBox, VGTexture texture, size_t index) {
    comboBox->setDropButtonHoverTexture(texture, index);
}

void vui::ComboBoxScriptFuncs::impl::setDropButtonHoverTextureByCompare(ComboBox* comboBox, VGTexture texture, DropButtonComparator comparator) {
    comboBox->setDropButtonHoverTexture(texture, comparator);
}

void vui::ComboBoxScriptFuncs::impl::setBackColor(ComboBox* comboBox, color4 color) {
    comboBox->setBackColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setMainButtonBackColor(ComboBox* comboBox, color4 color) {
    comboBox->setMainButtonBackColor(color);
}

void vui::ComboBoxScriptFuncs::impl::setDropBoxBackColor(ComboBox* comboBox, color4 color) {
    comboBox->setDropBoxBackColor(color);
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

void vui::ComboBoxScriptFuncs::impl::setDropBoxBackHoverColor(ComboBox* comboBox, color4 color) {
    comboBox->setDropBoxBackHoverColor(color);
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
