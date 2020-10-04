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

const vg::SpriteFont* vui::TextWidgetScriptFuncs::impl::getHoverFont(TextWidget* textWidget) {
    return textWidget->getHoverFont();
}

nString vui::TextWidgetScriptFuncs::impl::getHoverText(TextWidget* textWidget) {
    return textWidget->getHoverText();
}

vg::TextAlign vui::TextWidgetScriptFuncs::impl::getHoverTextAlign(TextWidget* textWidget) {
    return textWidget->getHoverTextAlign();
}

color4 vui::TextWidgetScriptFuncs::impl::getHoverTextColor(TextWidget* textWidget) {
    return textWidget->getHoverTextColor();
}

f32v2 vui::TextWidgetScriptFuncs::impl::getHoverTextScale(TextWidget* textWidget) {
    return textWidget->getHoverTextScale();
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

void vui::TextWidgetScriptFuncs::impl::setHoverFont(TextWidget* textWidget, vg::SpriteFont* font) {
    textWidget->setHoverFont(font);
}

void vui::TextWidgetScriptFuncs::impl::setHoverText(TextWidget* textWidget, nString text) {
    textWidget->setHoverText(text);
}

void vui::TextWidgetScriptFuncs::impl::setHoverTextAlign(TextWidget* textWidget, vg::TextAlign textAlign) {
    textWidget->setHoverTextAlign(textAlign);
}

void vui::TextWidgetScriptFuncs::impl::setHoverTextColor(TextWidget* textWidget, color4 color) {
    textWidget->setHoverTextColor(color);
}

void vui::TextWidgetScriptFuncs::impl::setHoverTextScale(TextWidget* textWidget, f32v2 scale) {
    textWidget->setHoverTextScale(scale);
}
