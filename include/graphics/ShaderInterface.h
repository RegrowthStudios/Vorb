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

#include "graphics/GLEnums.h"
#include <vector>

namespace vorb {
    namespace graphics {

        class GLProgram;

        struct AttributeBind {
            VGAttribute location;
            ui32 semantic;
            ui32 stride;
            ui32 offset;
            i32 size;
            vg::VertexAttribPointerType type;
            bool normalized;
        };
        typedef std::vector<AttributeBind> AttributeBindings;
             
        class ShaderInterface {
        public:
            ShaderInterface(const AttributeBindings& bindings) : m_bindings(bindings) {
                // Empty
            };

            i32 build(const std::unordered_map<ui32, VGAttribute>& semBinds);

            i32 build(const GLProgram* program);

            const AttributeBindings& getBindings() const { return m_bindings; }
        private:
            AttributeBindings m_bindings;
        };
    }
}
#endif // ShaderInterface_h__