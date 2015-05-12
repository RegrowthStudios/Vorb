#include "stdafx.h"
#include "ui/PanelScriptFuncs.h"
#include "ui/Panel.h"
#include "script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env.addCRDelegate(#name, makeRDelegate(*this, &PanelScriptFuncs::name));
#define REGISTER_DEL(env, name) env.addCDelegate(#name, makeDelegate(*this, &PanelScriptFuncs::name));

void vui::PanelScriptFuncs::registerFuncs(const cString nSpace, vscript::Environment& env) {
    env.setNamespaces(nSpace);
    // Call base register
    WidgetScriptFuncs::registerFuncs(nSpace, env);

    { // Register all functions
        // Getters
        REGISTER_RDEL(env, getTexture);
        // Setters
        REGISTER_DEL(env, setTexture);
        REGISTER_DEL(env, setColor);
        REGISTER_DEL(env, setHoverColor);
    }
    env.setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

VGTexture vui::PanelScriptFuncs::getTexture(Panel* p) const {
    return p->getTexture();
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
