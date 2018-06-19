#include "Vorb/stdafx.h"
#include "Vorb/ui/GameWindowScriptFuncs.h"
#include "Vorb/ui/GameWindow.h"
#include "Vorb/script/Environment.h"

// Helper macros for smaller code
#define REGISTER_RDEL(env, name) env->addCRDelegate(#name, makeRDelegate(*this, &GameWindowScriptFuncs::name));
#define REGISTER_DEL(env, name) env->addCDelegate(#name, makeDelegate(*this, &GameWindowScriptFuncs::name));

void vui::GameWindowScriptFuncs::init(const cString nSpace, const GameWindow* gameWindow, vscript::Environment* env) {
    m_window = gameWindow; 
    env->setNamespaces(nSpace);
    REGISTER_RDEL(env, getNumSupportedResolutions);
    REGISTER_RDEL(env, getSupportedResolution);
    REGISTER_RDEL(env, getCurrentResolution);
    env->setNamespaces();
}

#undef REGISTER_RDEL
#undef REGISTER_DEL

i32 vui::GameWindowScriptFuncs::getNumSupportedResolutions() const {
    return (i32)m_window->getSupportedResolutions().size();
}

ui32v2 vui::GameWindowScriptFuncs::getSupportedResolution(int resIndex) const {
    return m_window->getSupportedResolutions().at(resIndex);
}

ui32v2 vui::GameWindowScriptFuncs::getCurrentResolution() const {
    return m_window->getViewportDims();
}
