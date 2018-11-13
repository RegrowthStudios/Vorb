#include "Vorb/stdafx.h"
#include "Vorb/ui/script/GameWindowScriptFuncs.h"

#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/script/WidgetScriptFuncs.h"
#include "Vorb/script/IEnvironment.hpp"


template <typename ScriptEnvironmentImpl>
void vui::GameWindowScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env, const GameWindow* window) {
    env->setNamespaces("UI", "GameWindow");
    env->addCDelegate("getNumSupportedResolutions", makeDelegate([window] () {
        return impl::getNumSupportedResolutions(window);
    }));
    env->addCDelegate("getSupportedResolution",     makeDelegate([window] (size_t resIndex) {
        return impl::getSupportedResolution(window, resIndex);
    }));
    env->addCDelegate("getCurrentResolution",       makeDelegate([window] () {
        return impl::getCurrentResolution(window);
    }));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::GameWindowScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

size_t vui::GameWindowScriptFuncs::impl::getNumSupportedResolutions(const GameWindow* window) {
    return window->getSupportedResolutions().size();
}

ui32v2 vui::GameWindowScriptFuncs::impl::getSupportedResolution(const GameWindow* window, size_t resIndex) {
    return window->getSupportedResolutions().at(resIndex);
}

ui32v2 vui::GameWindowScriptFuncs::impl::getCurrentResolution(const GameWindow* window) {
    return window->getViewportDims();
}
