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
        class ResourceAllocator;

        /* @brief
         */
        class IContext {
            friend class IResource;
        public:
            /*! @brief Signal that this thread will be used by the context
             * 
             */
            void branchThread(OUT ResourceAllocator* allocator);

#pragma region Resource creation methods
            virtual IBuffer* create(const BufferDescription& desc) = 0;
            virtual IBuffer* create(const CBufferDescription* desc) = 0;
            virtual IConstantBlock* create(const ConstantBlockDescription& desc) = 0;
            virtual ITexture1D* create(const Texture1DDescription& desc) = 0;
            virtual ITexture2D* create(const Texture2DDescription& desc) = 0;
            virtual ITexture3D* create(const Texture3DDescription& desc) = 0;
            IShader* create(const IShaderCode* code) {
                switch (code->getType()) {
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
            virtual ITessGenShader* createTessGenShader(const IShaderCode* code) = 0;
            virtual ITessEvalShader* createTessEvalShader(const IShaderCode* code) = 0;
            virtual IGeometryShader* createGeometryShader(const IShaderCode* code) = 0;
            virtual IPixelShader* createPixelShader(const IShaderCode* code) = 0;
            virtual IComputeShader* createComputeShader(const IShaderCode* code) = 0;
#pragma endregion

#pragma region Resource view creation methods
            virtual IBufferView* makeView(IBuffer* res) = 0;
            virtual IConstantBlockView* makeView(IConstantBlock* res) = 0;
            virtual ITexture1DView* makeView(ITexture1D* res) = 0;
            virtual ITexture2DView* makeView(ITexture2D* res) = 0;
            virtual ITexture3DView* makeView(ITexture3D* res) = 0;
            virtual IComputeResourceView* makeComputeView(IBuffer* res) = 0;
            virtual IComputeResourceView* makeComputeView(ITexture1D* res) = 0;
            virtual IComputeResourceView* makeComputeView(ITexture2D* res) = 0;
            virtual IComputeResourceView* makeComputeView(ITexture3D* res) = 0;

#pragma endregion

            virtual ShaderBytecode compileShaderSource(const cString data, size_t length, ShaderType type, ShaderCompilerInfo headerInfo) = 0;
            virtual IShaderCode* loadCompiledShader(ShaderBytecode code) = 0;

            virtual void present() = 0;
        protected:
            virtual IContext* getResourceBuilderContext() = 0;

            virtual void add(IResource* resource) = 0;
            virtual void remove(IResource* resource) = 0;
            virtual void free(IResource* resource) = 0;
        };

        class ResourceAllocator {
            friend class IContext;
        public:
            IBuffer* create(const BufferDescription& desc) {
                return m_context->create(desc);
            }
            IBuffer* create(const CBufferDescription* desc) {
                return m_context->create(desc);
            }
            IConstantBlock* create(const ConstantBlockDescription& desc) {
                return m_context->create(desc);
            }
            ITexture1D* create(const Texture1DDescription& desc) {
                return m_context->create(desc);
            }
            ITexture2D* create(const Texture2DDescription& desc) {
                return m_context->create(desc);
            }
            ITexture3D* create(const Texture3DDescription& desc) {
                return m_context->create(desc);
            }
            IShader* create(const IShaderCode* code) {
                return m_context->create(code);
            }
            IVertexShader* createVertexShader(const IShaderCode* code) {
                return m_context->createVertexShader(code);
            }
            ITessGenShader* createTessGenShader(const IShaderCode* code) {
                return m_context->createTessGenShader(code);
            }
            ITessEvalShader* createTessEvalShader(const IShaderCode* code) {
                return m_context->createTessEvalShader(code);
            }
            IGeometryShader* createGeometryShader(const IShaderCode* code) {
                return m_context->createGeometryShader(code);
            }
            IPixelShader* createPixelShader(const IShaderCode* code) {
                return m_context->createPixelShader(code);
            }
            IComputeShader* createComputeShader(const IShaderCode* code) {
                return m_context->createComputeShader(code);
            }
            IBufferView* makeView(IBuffer* res) {
                return m_context->makeView(res);
            }
            IConstantBlockView* makeView(IConstantBlock* res) {
                return m_context->makeView(res);
            }
            ITexture1DView* makeView(ITexture1D* res) {
                return m_context->makeView(res);
            }
            ITexture2DView* makeView(ITexture2D* res) {
                return m_context->makeView(res);
            }
            ITexture3DView* makeView(ITexture3D* res) {
                return m_context->makeView(res);
            }
        private:
            ResourceAllocator(IContext* parent) :
                m_context(parent) {
                // Empty
            }

            IContext* m_context;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IContext_h__
