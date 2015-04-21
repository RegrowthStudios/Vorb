///
/// ShaderInterface.h
/// Vorb
///
/// Created by Benjamin Arnold on 29 Mar 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Interface for communicating semantic information between programs and vertex buffer.
///

#pragma once

#ifndef ShaderInterface_h__
#define ShaderInterface_h__

#include "../Vorb.h"
#include "GLEnums.h"
#include <vector>

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
            /// @param semBinds: The semantic to attribute bindings
            /// @return number of bindings successfully linked
            i32 build(const AttributeSemBinding& semBinds);

            /// Builds the VAO from a GLProgram
            /// @return number of bindings successfully linked
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
#endif // ShaderInterface_h__