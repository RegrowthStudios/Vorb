#include "Vorb/stdafx.h"
#include "Vorb/ui/script/TextWidgetScriptFuncs.h"

#include "Vorb/graphics/SpriteFont.h"
#include "Vorb/ui/widgets/TextWidget.h"

const vg::SpriteFont* vui::TextWidgetScriptFuncs::impl::getFont(TextWidget* textWidget) {
    return textWidget->getFont();
}

nString vui::TextWidgetScriptFuncs::impl::getText(TextWidget* textWidget) {
    return textWidget->getText();
}

vg::TextAlign vui::TextWidgetScriptFuncs::impl::getTextAlign(TextWidget* textWidget) {
    return textWidget->getTextAlign();
}

color4 vui::TextWidgetScriptFuncs::impl::getTextColor(TextWidget* textWidget) {
    return textWidget->getTextColor();
}

f32v2 vui::TextWidgetScriptFuncs::impl::getTextScale(TextWidget* textWidget) {
    return textWidget->getTextScale();
}

void vui::TextWidgetScriptFuncs::impl::setFont(TextWidget* textWidget, vg::SpriteFont* font) {
    textWidget->setFont(font);
}

void vui::TextWidgetScriptFuncs::impl::setText(TextWidget* textWidget, nString text) {
    textWidget->setText(text);
}

void vui::TextWidgetScriptFuncs::impl::setTextAlign(TextWidget* textWidget, vg::TextAlign textAlign) {
    textWidget->setTextAlign(textAlign);
}

void vui::TextWidgetScriptFuncs::impl::setTextColor(TextWidget* textWidget, color4 color) {
    textWidget->setTextColor(color);
}

void vui::TextWidgetScriptFuncs::impl::setTextScale(TextWidget* textWidget, f32v2 scale) {
    textWidget->setTextScale(scale);
}
