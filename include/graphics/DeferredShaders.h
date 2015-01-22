///
/// DeferredShaders.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 18 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// A container for deferred shading shaders
///

#pragma once

#ifndef DeferredShaders_h__
#define DeferredShaders_h__

#include "GLProgram.h"

namespace vorb {
    namespace graphics {
        /// GLProgram container
        class DeferredShaders {
        public:
            /// Constructor which will not initialize programs
            DeferredShaders();

            /// Disposes and removes all shaders found in this containers
            void dispose();

            GLProgram clear; ///< Shader used to clear GBuffer
            std::unordered_map<nString, GLProgram> geometry; ///< Shaders that draw onto the GBuffer
            std::unordered_map<nString, GLProgram> light; ///< Shaders that add lighting into the lighting buffer
            GLProgram composition; /// Shader used to compose the GBuffer and lighting elements
        };
    }
}
namespace vg = vorb::graphics;

#endif // DeferredShaders_h__