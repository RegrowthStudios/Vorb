//
// lua/LFunction.h
// Vorb Engine
//
// Created by Matthew Marshall on 8 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file LFunction.h
 * \brief Classes wrapping Lua functions, providing call procedures for C++.
 */

#pragma once

#ifndef Vorb_Lua_LFunction_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Lua_LFunction_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#define SCRIPT_FUNCTION_TABLE "ScriptFunctions"

namespace vorb {
    namespace script {
        namespace lua {

        }
    }
}
namespace vscript  = vorb::script;

#endif // VORB_USING_SCRIPT

#endif // !Vorb_Lua_LFunction_h__
