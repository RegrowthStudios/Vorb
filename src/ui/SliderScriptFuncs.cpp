#include "Vorb/stdafx.h"
#include "Vorb/ui/SliderScriptFuncs.h"
#include "Vorb/ui/Slider.h"
#include "Vorb/script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &SliderScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &SliderScriptFuncs::name));

void vui::SliderScriptFuncs::init(const cString nSpace, vscript::Environment* env) {
    // Call base register
    WidgetScriptFuncs::init(nSpace, env);

    env->setNamespaces(nSpace);

    { // Register all functions
        // Getters
        REGISTER_RDEL(env, getSlideTexture);
        REGISTER_RDEL(env, getBarTexture);
        REGISTER_RDEL(env, getSlideColor);
        REGISTER_RDEL(env, getSlideHoverColor);
        REGISTER_RDEL(env, getBarColor);
        REGISTER_RDEL(env, getValue);
        REGISTER_RDEL(env, getMin);
        REGISTER_RDEL(env, getMax);
        REGISTER_RDEL(env, getValueScaled);
        REGISTER_RDEL(env, isInSlideBounds);
        REGISTER_RDEL(env, isVertical);
        // Setters
        REGISTER_DEL(env, setSlideDimensions);
        REGISTER_DEL(env, setSlideTexture);
        REGISTER_DEL(env, setBarTexture);
        REGISTER_DEL(env, setBarColor);
        REGISTER_DEL(env, setSlideColor);
        REGISTER_DEL(env, setSlideHoverColor);
        REGISTER_DEL(env, setValue);
        REGISTER_DEL(env, setRange);
        REGISTER_DEL(env, setMin);
        REGISTER_DEL(env, setMax);
        REGISTER_DEL(env, setIsVertical);
    }
    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

void vui::SliderScriptFuncs::registerWidget(Widget* w) {
    WidgetScriptFuncs::registerWidget(w);
    Slider* s = (Slider*)w;
    s->ValueChange += makeDelegate(*this, &SliderScriptFuncs::onValueChange);
}

void vui::SliderScriptFuncs::unregisterWidget(Widget* w) {
    WidgetScriptFuncs::unregisterWidget(w);
    Slider* s = (Slider*)w;
    s->ValueChange -= makeDelegate(*this, &SliderScriptFuncs::onValueChange);
}

bool vui::SliderScriptFuncs::addCallback(Widget* w, EventType eventType, nString funcName) {
    const vscript::Function& f = (*m_env)[funcName];
    if (f.isNil()) return false;
    Slider* s = (Slider*)w;
    switch (eventType) {
        case EventType::VALUE_CHANGE:
            s->m_valueChangeFuncs.push_back(f); break;
        default:
            return false;
    }
    return true;
}

bool vui::SliderScriptFuncs::removeCallback(Widget* w, EventType eventType, nString funcName) {
    const vscript::Function& f = (*m_env)[funcName];
    if (f.isNil()) return false;
    Slider* s = (Slider*)w;
    switch (eventType) {
        case EventType::VALUE_CHANGE:
            s->m_valueChangeFuncs.erase(std::find(s->m_valueChangeFuncs.begin(), s->m_valueChangeFuncs.end(), f));
            break;
        default:
            return WidgetScriptFuncs::removeCallback(w, eventType, funcName);
    }
    return true;
}

VGTexture vui::SliderScriptFuncs::getSlideTexture(Slider* s) const {
    return s->getSlideTexture();
}

VGTexture vui::SliderScriptFuncs::getBarTexture(Slider* s) const {
    return s->getBarTexture();
}

color4 vui::SliderScriptFuncs::getSlideColor(Slider* s) const {
    return s->getSlideColor();
}

color4 vui::SliderScriptFuncs::getSlideHoverColor(Slider* s) const {
    return s->getSlideHoverColor();
}

color4 vui::SliderScriptFuncs::getBarColor(Slider* s) const {
    return s->getBarColor();
}

int vui::SliderScriptFuncs::getValue(Slider* s) const {
    return s->getValue();
}

int vui::SliderScriptFuncs::getMin(Slider* s) const {
    return s->getMin();
}

int vui::SliderScriptFuncs::getMax(Slider* s) const {
    return s->getMax();
}

f32 vui::SliderScriptFuncs::getValueScaled(Slider* s) const {
    return s->getValueScaled();
}

bool vui::SliderScriptFuncs::isVertical(Slider* s) const {
    return s->isVertical();
}

void vui::SliderScriptFuncs::setSlideDimensions(Slider* s, f32v2 dimensions) const {
    s->setSlideDimensions(dimensions);
}

void vui::SliderScriptFuncs::setSlideTexture(Slider* s, VGTexture texture) const {
    s->setSlideTexture(texture);
}

void vui::SliderScriptFuncs::setBarTexture(Slider* s, VGTexture texture) const {
    s->setBarTexture(texture);
}

void vui::SliderScriptFuncs::setBarColor(Slider* s, color4 color) const {
    s->setBarColor(color);
}

void vui::SliderScriptFuncs::setSlideColor(Slider* s, color4 color) const {
    s->setSlideColor(color);
}

void vui::SliderScriptFuncs::setSlideHoverColor(Slider* s, color4 color) const {
    s->setSlideHoverColor(color);
}

void vui::SliderScriptFuncs::setValue(Slider* s, int value) const {
    s->setValue(value);
}

void vui::SliderScriptFuncs::setRange(Slider* s, int min, int max) const {
    s->setRange(min, max);
}

void vui::SliderScriptFuncs::setMin(Slider* s, int min) const {
    s->setMin(min);
}

void vui::SliderScriptFuncs::setMax(Slider* s, int max) const {
    s->setMax(max);
}

void vui::SliderScriptFuncs::setIsVertical(Slider* s, bool isVertical) const {
    s->setIsVertical(isVertical);
}

bool vui::SliderScriptFuncs::isInSlideBounds(Slider* s, f32v2 point) const {
    return s->isInSlideBounds(point);
}

void vui::SliderScriptFuncs::onValueChange(Sender s, int v) const {
    Slider* w = (Slider*)s;
    size_t sz = w->m_valueChangeFuncs.size();
    for (size_t i = 0; i < sz && i < w->m_valueChangeFuncs.size(); i++) {
        w->m_valueChangeFuncs[i](v);
    }
}