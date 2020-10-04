#include "Vorb/stdafx.h"
#include "Vorb/ui/script/LabelScriptFuncs.h"

#include "Vorb/ui/widgets/Label.h"

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
