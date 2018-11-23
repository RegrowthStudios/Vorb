#include "Vorb/stdafx.h"
#include "Vorb/ui/script/SliderScriptFuncs.h"

VGTexture vui::SliderScriptFuncs::impl::getSlideTexture(Slider* slider) {
    return slider->getSlideTexture();
}

VGTexture vui::SliderScriptFuncs::impl::getBarTexture(Slider* slider) {
    return slider->getBarTexture();
}

color4 vui::SliderScriptFuncs::impl::getSlideColor(Slider* slider) {
    return slider->getSlideColor();
}

color4 vui::SliderScriptFuncs::impl::getSlideHoverColor(Slider* slider) {
    return slider->getSlideHoverColor();
}

color4 vui::SliderScriptFuncs::impl::getBarColor(Slider* slider) {
    return slider->getBarColor();
}

i32 vui::SliderScriptFuncs::impl::getValue(Slider* slider) {
    return slider->getValue();
}

i32 vui::SliderScriptFuncs::impl::getMin(Slider* slider) {
    return slider->getMin();
}

i32 vui::SliderScriptFuncs::impl::getMax(Slider* slider) {
    return slider->getMax();
}

f32 vui::SliderScriptFuncs::impl::getValueScaled(Slider* slider) {
    return slider->getValueScaled();
}

f32v2 vui::SliderScriptFuncs::impl::getSlideSize(Slider* slider) {
    return slider->getSlideSize();
}

vui::Length2 vui::SliderScriptFuncs::impl::getRawSlideSize(Slider* slider) {
    return slider->getRawSlideSize();
}

bool vui::SliderScriptFuncs::impl::isHorizontal(Slider* slider) {
    return slider->isHorizontal();
}

bool vui::SliderScriptFuncs::impl::isVertical(Slider* slider) {
    return slider->isVertical();
}

bool vui::SliderScriptFuncs::impl::isInSlideBounds(Slider* slider, f32 x, f32 y) {
    return slider->isInSlideBounds(x, y);
}

void vui::SliderScriptFuncs::impl::setSlideSize(Slider* slider, f32v2 size) {
    slider->setSlideSize(size);
}

void vui::SliderScriptFuncs::impl::setSlideTexture(Slider* slider, VGTexture texture) {
    slider->setSlideTexture(texture);
}

void vui::SliderScriptFuncs::impl::setBarTexture(Slider* slider, VGTexture texture) {
    slider->setBarTexture(texture);
}

void vui::SliderScriptFuncs::impl::setBarColor(Slider* slider, color4 color) {
    slider->setBarColor(color);
}

void vui::SliderScriptFuncs::impl::setSlideColor(Slider* slider, color4 color) {
    slider->setSlideColor(color);
}

void vui::SliderScriptFuncs::impl::setSlideHoverColor(Slider* slider, color4 color) {
    slider->setSlideHoverColor(color);
}

void vui::SliderScriptFuncs::impl::setValue(Slider* slider, i32 value) {
    slider->setValue(value);
}

void vui::SliderScriptFuncs::impl::setRange(Slider* slider, i32 min, i32 max) {
    slider->setRange(min, max);
}

void vui::SliderScriptFuncs::impl::setMin(Slider* slider, i32 min) {
    slider->setMin(min);
}

void vui::SliderScriptFuncs::impl::setMax(Slider* slider, i32 max) {
    slider->setMax(max);
}

void vui::SliderScriptFuncs::impl::setIsVertical(Slider* slider, bool isVertical) {
    slider->setIsVertical(isVertical);
}

// void vui::SliderScriptFuncs::impl::onValueChange(Sender s, i32 v) {
//     Slider* w = (Slider*)s;
//     size_t sz = w->m_valueChangeFuncs.size();
//     for (size_t i = 0; i < sz && i < w->m_valueChangeFuncs.size(); i++) {
//         w->m_valueChangeFuncs[i](v);
//     }
// }