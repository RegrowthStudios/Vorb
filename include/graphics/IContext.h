//
// IContext.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IContext.h
 * @brief The context hold graphics resources that multiple devices may share.
 */

#pragma once

#ifndef Vorb_IContext_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IContext_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "IResource.h"
#include "ResourceDescriptions.h"

namespace vorb {
    namespace graphics {
        /* @brief
         */
        class IContext {
            friend class IResource;
        public:
            virtual IBuffer* create(const BufferDescription& desc) = 0;
            virtual IBuffer* create(const CBufferDescription* desc) = 0;

            virtual ShaderBytecode compileShaderSource(const cString data, size_t length, ShaderType type, ShaderCompilerInfo headerInfo) = 0;
            virtual IShaderCode* loadCompiledShader(ShaderBytecode code) = 0;

            IShader* create(vg::ShaderType type, const IShaderCode* code) {
                switch (type) {
                case ShaderType::VERTEX_SHADER:
                    return createVertexShader(code);
                case ShaderType::GEOMETRY_SHADER:
                    return createGeometryShader(code);
                case ShaderType::TESS_CONTROL_SHADER:
                    return createTessGenShader(code);
                case ShaderType::TESS_EVALUATION_SHADER:
                    return createTessEvalShader(code);
                case ShaderType::FRAGMENT_SHADER:
                    return createPixelShader(code);
                case ShaderType::COMPUTE_SHADER:
                    return createComputeShader(code);
                default:
                    return nullptr;
                }
            }
            virtual IVertexShader* createVertexShader(const IShaderCode* code) = 0;
            virtual IGeometryShader* createGeometryShader(const IShaderCode* code) = 0;
            virtual ITessGenShader* createTessGenShader(const IShaderCode* code) = 0;
            virtual ITessEvalShader* createTessEvalShader(const IShaderCode* code) = 0;
            virtual IPixelShader* createPixelShader(const IShaderCode* code) = 0;
            virtual IComputeShader* createComputeShader(const IShaderCode* code) = 0;

            virtual void present() = 0;
        protected:
            void add(IResource* resource);
            void remove(IResource* resource);
            void free(IResource* resource);
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IContext_h__
