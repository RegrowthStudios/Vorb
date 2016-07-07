//
// FullQuadVBO.h
// Vorb Engine
//
// Created by Cristian Zaloj on 6 Nov 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file FullQuadVBO.h
 * @brief A mesh for a fullscreen quad.
 */

#pragma once

#ifndef Vorb_FullQuadVBO_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_FullQuadVBO_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "gtypes.h"

namespace vorb {
    namespace graphics {
        /// Wrapper over common functionality to draw a quad across the entire screen
        class FullQuadVBO {
        public:
            /*! @brief Initialize all graphics resources.
             * @param attrLocation: Position attribute location for VAO
             */
            void init(i32 attrLocation = 0);
            /*! @brief Dispose all graphics resources
             */
            void dispose();

            /*! @brief Binds vertex array, index buffer, and issues a draw command.
             */
            void draw();

            bool isInitialized() const { return m_vao != 0; }
        private:
            union {
                struct {
                    VGVertexBuffer m_vb;
                    VGIndexBuffer  m_ib;
                };
                VGVertexBuffer m_buffers[2];
            };
            VGVertexArray m_vao = 0;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_FullQuadVBO_h__
