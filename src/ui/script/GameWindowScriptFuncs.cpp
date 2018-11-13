#include "Vorb/stdafx.h"
#include "Vorb/ui/script/GameWindowScriptFuncs.h"

#include "Vorb/ui/GameWindow.h"

size_t vui::GameWindowScriptFuncs::impl::getNumSupportedResolutions(const GameWindow* window) {
    return window->getSupportedResolutions().size();
}

ui32v2 vui::GameWindowScriptFuncs::impl::getSupportedResolution(const GameWindow* window, size_t resIndex) {
    return window->getSupportedResolutions().at(resIndex);
}

ui32v2 vui::GameWindowScriptFuncs::impl::getCurrentResolution(const GameWindow* window) {
    return window->getViewportDims();
}
