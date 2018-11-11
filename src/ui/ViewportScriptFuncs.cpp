#include "Vorb/stdafx.h"
#include "Vorb/ui/ViewportScriptFuncs.h"

#include "Vorb/ui/Viewport.h"
#include "Vorb/ui/WidgetScriptFuncs.h"
#include "Vorb/script/IEnvironment.hpp"

template <typename ScriptEnvironmentImpl>
void vui::ViewportScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::ViewportScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    // Empty
}
