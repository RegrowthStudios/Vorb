//
// ShaderInterface.h
// Vorb Engine
//
// Created by Benjamin Arnold on 29 Mar 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file ShaderInterface.h
 * 
 * \brief Interface for communicating semantic information between programs and vertex buffer.
 */

#pragma once

#ifndef Vorb_ShaderInterface_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ShaderInterface_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <vector>
#include <unordered_map>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "GLEnums.h"

namespace vorb {
    namespace graphics {

        class GLProgram;

        /// Data for an attribute
        struct AttributeBind {
            VGAttribute location;
            VGSemantic semantic;
            ui32 stride;
            ui32 offset;
            i32 size;
            vg::VertexAttribPointerType type;
            bool normalized;
        };
        typedef std::vector<AttributeBind> AttributeBindings;
        typedef std::unordered_map<VGSemantic, VGAttribute> AttributeSemBinding;
             
        class ShaderInterface {
        public:
            ShaderInterface(const AttributeBindings& bindings) : m_bindings(bindings) {
                // Empty
            };

            /// Disposes VAO and clears bindings
            void dispose();

            /// Builds the VAO from semantic bindings
            /// \param semBinds: The semantic to attribute bindings
            /// \return number of bindings successfully linked
            i32 build(const AttributeSemBinding& semBinds);

            /// Builds the VAO from a GLProgram
            /// \return number of bindings successfully linked
            i32 build(const GLProgram* program);

            /// Enables the VAO for this interface
            void use();

            /// Disables VAO
            static void unuse();

            /// Gets the attribute bindings
            const AttributeBindings& getBindings() const { return m_bindings; }
        private:
            VGVertexArray m_vao = 0;
            AttributeBindings m_bindings;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_ShaderInterface_h__
