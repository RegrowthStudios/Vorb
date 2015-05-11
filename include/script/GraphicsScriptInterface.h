//
// GraphicsScriptInterface.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file GraphicsScriptInterface.h
* @brief
* Scripting functions and registration for vorb::graphics
*
*/

#pragma once

#ifndef Vorb_GraphicsScriptInterface_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_GraphicsScriptInterface_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "Environment.h"

namespace vorb {
    namespace script {
        class GraphicsScriptingInterface {
        public:
            virtual void registerGraphics(vscript::Environment& env);
        protected:
            virtual void registerConstants(vscript::Environment& env);
            virtual void registerFunctions(vscript::Environment& env);
        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_GraphicsScriptInterface_h__
