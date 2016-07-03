//
// GraphicsScriptInterface.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file GraphicsScriptInterface.h
 * 
 * \brief Scripting functions and registration for vorb::graphics.
 */

#pragma once

#ifndef Vorb_GraphicsScriptInterface_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_GraphicsScriptInterface_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Environment.h"
#include "../graphics/TextureCache.h"

namespace vorb {
    namespace script {
        class GraphicsScriptInterface {
        public:
            virtual void registerGraphics(vscript::Environment& env);
            virtual void dispose();

            vg::TextureCache textureCache; ///< Texture cache for textures loaded via scripts
        protected:
            virtual void registerConstants(vscript::Environment& env);
            virtual void registerFunctions(vscript::Environment& env);
        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_GraphicsScriptInterface_h__
