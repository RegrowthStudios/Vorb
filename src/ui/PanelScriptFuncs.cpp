#include "Vorb/stdafx.h"
#include "Vorb/ui/PanelScriptFuncs.h"
#include "Vorb/ui/Panel.h"
#include "Vorb/script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &PanelScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &PanelScriptFuncs::name));

void vui::PanelScriptFuncs::init(const cString nSpace, vscript::Environment* env) {
    
    // Call base register
    WidgetScriptFuncs::init(nSpace, env);

    env->setNamespaces(nSpace);
    { // Register all functions
        // Getters
        REGISTER_RDEL(env, getTexture);
        REGISTER_RDEL(env, getColor);
        REGISTER_RDEL(env, getHoverColor);
        REGISTER_RDEL(env, getAutoScroll);
        // Setters
        REGISTER_DEL(env, setTexture);
        REGISTER_DEL(env, setColor);
        REGISTER_DEL(env, setHoverColor);
        REGISTER_DEL(env, setAutoScroll);
    }
    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

VGTexture vui::PanelScriptFuncs::getTexture(Panel* p) const {
    return p->getTexture();
}

color4 vui::PanelScriptFuncs::getColor(Panel* p) const {
    return p->getColor();
}

color4 vui::PanelScriptFuncs::getHoverColor(Panel* p) const {
    return p->getHoverColor();
}

bool vui::PanelScriptFuncs::getAutoScroll(Panel* p) const {
    return p->getAutoScroll();
}

void vui::PanelScriptFuncs::setTexture(Panel* p, VGTexture texture) const {
    p->setTexture(texture);
}

void vui::PanelScriptFuncs::setColor(Panel* p, color4 color) const {
    p->setColor(color);
}

void vui::PanelScriptFuncs::setHoverColor(Panel* p, color4 color) const {
    p->setHoverColor(color);
}

void vui::PanelScriptFuncs::setAutoScroll(Panel* p, bool autoScroll) const {
    p->setAutoScroll(autoScroll);
}
