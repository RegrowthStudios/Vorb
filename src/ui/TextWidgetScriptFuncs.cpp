#include "Vorb/stdafx.h"
#include "Vorb/ui/TextWidgetScriptFuncs.h"

template <typename ScriptEnvironmentImpl>
void vui::TextWidgetScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    // TODO(Matthew): Implement.
}

template <typename ScriptEnvironmentImpl>
void vui::TextWidgetScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", "TextAlign");
    env->addValue("LEFT", vg::TextAlign::LEFT);
    env->addValue("TOP_LEFT", vg::TextAlign::TOP_LEFT);
    env->addValue("TOP", vg::TextAlign::TOP);
    env->addValue("TOP_RIGHT", vg::TextAlign::TOP_RIGHT);
    env->addValue("RIGHT", vg::TextAlign::RIGHT);
    env->addValue("BOTTOM_RIGHT", vg::TextAlign::BOTTOM_RIGHT);
    env->addValue("BOTTOM", vg::TextAlign::BOTTOM);
    env->addValue("BOTTOM_LEFT", vg::TextAlign::BOTTOM_LEFT);
    env->addValue("CENTER", vg::TextAlign::CENTER);
    env->setNamespaces();
}
