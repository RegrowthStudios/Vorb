#include "stdafx.h"
#include "Vorb/graphics/script/SamplerStateScriptFuncs.h"

template <typename ScriptEnvironmentImpl>
void vg::SamplerStateScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    // Empty
}

template <typename ScriptEnvironmentImpl>
void vg::SamplerStateScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env.setNamespaces("Graphics", "SamplerState");
    env.addValue("POINT_WRAP",          &vg::SamplerState::POINT_WRAP);
    env.addValue("POINT_CLAMP",         &vg::SamplerState::POINT_CLAMP);
    env.addValue("LINEAR_WRAP",         &vg::SamplerState::LINEAR_WRAP);
    env.addValue("LINEAR_CLAMP",        &vg::SamplerState::LINEAR_CLAMP);
    env.addValue("POINT_WRAP_MIPMAP",   &vg::SamplerState::POINT_WRAP_MIPMAP);
    env.addValue("POINT_CLAMP_MIPMAP",  &vg::SamplerState::POINT_CLAMP_MIPMAP);
    env.addValue("LINEAR_WRAP_MIPMAP",  &vg::SamplerState::LINEAR_WRAP_MIPMAP);
    env.addValue("LINEAR_CLAMP_MIPMAP", &vg::SamplerState::LINEAR_CLAMP_MIPMAP);
    env.setNamespaces();
}
