
//
// SpriteFontScriptFuncs.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 12 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file SpriteFontScriptFuncs.hpp
* \brief Registers functions and consts for sprite batch to a script environment.
*/

#pragma once

#ifndef Vorb_SpriteFontScriptFuncs_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SpriteFontScriptFuncs_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/VorbPreDecl.inl"

DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace graphics {
        namespace impl {
            namespace SpriteFontScriptFuncs {
                template <typename ScriptEnvironmentImpl>
                void registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

                template <typename ScriptEnvironmentImpl>
                void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);
            }
        }
    }
}

namespace vg = vorb::graphics;
template <typename ScriptEnvironmentImpl>
void vg::impl::SpriteFontScriptFuncs::registerFuncs(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

template <typename ScriptEnvironmentImpl>
void vg::impl::SpriteFontScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("Graphics", "TextAlign");
    env->addValue("NONE",         vg::TextAlign::NONE);
    env->addValue("LEFT",         vg::TextAlign::LEFT);
    env->addValue("TOP_LEFT",     vg::TextAlign::TOP_LEFT);
    env->addValue("TOP",          vg::TextAlign::TOP);
    env->addValue("TOP_RIGHT",    vg::TextAlign::TOP_RIGHT);
    env->addValue("RIGHT",        vg::TextAlign::RIGHT);
    env->addValue("BOTTOM_RIGHT", vg::TextAlign::BOTTOM_RIGHT);
    env->addValue("BOTTOM",       vg::TextAlign::BOTTOM);
    env->addValue("BOTTOM_LEFT",  vg::TextAlign::BOTTOM_LEFT);
    env->addValue("CENTER",       vg::TextAlign::CENTER);
    env->setNamespaces();
}

#endif // !Vorb_SpriteFontScriptFuncs_hpp__
