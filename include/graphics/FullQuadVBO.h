//
// FullQuadVBO.h
// Vorb Engine
//
// Created by Cristian Zaloj on 6 Nov 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file FullQuadVBO.h
 * 
 * \brief A mesh for a fullscreen quad.
 */

#pragma once

#ifndef Vorb_FullQuadVBO_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FullQuadVBO_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        /// Wrapper over common functionality to draw a quad across the entire screen
        class FullQuadVBO {
        public:
            /// Initialize all OpenGL resources
            /// \param attrLocation: Position attribute location for VAO
            void init(i32 attrLocation = 0);
            /// Dispose all OpenGL resources
            void dispose();

            /// Binds vertex array, index buffer, and issues a draw command
            void draw();
        private:
            union {
                struct {
                    ui32 m_vb; ///< Vertex buffer ID
                    ui32 m_ib; ///< Index buffer ID
                };
                ui32 m_buffers[2]; ///< Storage for both buffers used by this mesh
            };
            ui32 m_vao; ///< VAO with vertex attribute pointing to 0
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_FullQuadVBO_h__
