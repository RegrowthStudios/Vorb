#include "Vorb/stdafx.h"
#include "Vorb/ui/script/PanelScriptFuncs.h"

#include "Vorb/ui/Panel.h"

VGTexture vui::PanelScriptFuncs::impl::getTexture(Panel* panel) {
    return panel->getTexture();
}

bool vui::PanelScriptFuncs::impl::getAutoScroll(Panel* panel) {
    return panel->getAutoScroll();
}

f32 vui::PanelScriptFuncs::impl::getSliderWidth(Panel* panel) {
    return panel->getSliderWidth();
}

color4 vui::PanelScriptFuncs::impl::getColor(Panel* panel) {
    return panel->getColor();
}

color4 vui::PanelScriptFuncs::impl::getHoverColor(Panel* panel) {
    return panel->getHoverColor();
}

void vui::PanelScriptFuncs::impl::setTexture(Panel* panel, VGTexture texture) {
    panel->setTexture(texture);
}

void vui::PanelScriptFuncs::impl::setAutoScroll(Panel* panel, bool autoScroll) {
    panel->setAutoScroll(autoScroll);
}

void vui::PanelScriptFuncs::impl::setSliderWidth(Panel* panel, f32 width) {
    panel->setSliderWidth(width);
}

void vui::PanelScriptFuncs::impl::setColor(Panel* panel, color4 color) {
    panel->setColor(color);
}

void vui::PanelScriptFuncs::impl::setHoverColor(Panel* panel, color4 color) {
    panel->setHoverColor(color);
}
