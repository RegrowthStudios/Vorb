//
// IContext.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file IContext.h
 * 
 * \brief The context hold graphics resources that multiple devices may share.
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
#include "IAdapter.h"

namespace vorb {
    namespace graphics {
        class ResourceAllocator;

        /* \brief
         */
        class IContext {
            friend class IAdapter;
            friend class IResource;
        public:
            const APIVersion& getAPIVersion() const {
                return m_api;
            }

            /*! \brief Signal that this thread will be used by the context
             * 
             */
            void branchThread(OUT ResourceAllocator* allocator);

#pragma region Resource creation methods
            virtual vorb::graphics::IBuffer* create(const BufferDescription& desc, OPT InitalResourceData* data = nullptr) = 0;
            virtual vorb::graphics::IConstantBlock* create(const ConstantBlockDescription& desc, OPT InitalResourceData* data = nullptr) = 0;
            virtual vorb::graphics::ITexture1D* create(const Texture1DDescription& desc, OPT InitalResourceData* data = nullptr) = 0;
            virtual vorb::graphics::ITexture2D* create(const Texture2DDescription& desc, OPT InitalResourceData* data = nullptr) = 0;
            virtual vorb::graphics::ITexture3D* create(const Texture3DDescription& desc, OPT InitalResourceData* data = nullptr) = 0;
            virtual vorb::graphics::IVertexDeclaration* create(VertexElementDescription* desc, IShaderCode* vertexShaderCode, size_t numElements) = 0;
            vorb::graphics::IShader* create(const IShaderCode* code) {
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
            virtual vorb::graphics::IVertexShader* createVertexShader(const IShaderCode* code) = 0;
            virtual vorb::graphics::ITessGenShader* createTessGenShader(const IShaderCode* code) = 0;
            virtual vorb::graphics::ITessEvalShader* createTessEvalShader(const IShaderCode* code) = 0;
            virtual vorb::graphics::IGeometryShader* createGeometryShader(const IShaderCode* code) = 0;
            virtual vorb::graphics::IPixelShader* createPixelShader(const IShaderCode* code) = 0;
            virtual vorb::graphics::IComputeShader* createComputeShader(const IShaderCode* code) = 0;
#pragma endregion

#pragma region Resource view creation methods
            virtual vorb::graphics::IBufferView* makeView(IBuffer* res) = 0;
            virtual vorb::graphics::IConstantBlockView* makeView(IConstantBlock* res) = 0;
            virtual vorb::graphics::ITexture1DView* makeView(ITexture1D* res) = 0;
            virtual vorb::graphics::ITexture2DView* makeView(ITexture2D* res) = 0;
            virtual vorb::graphics::ITexture3DView* makeView(ITexture3D* res) = 0;
            virtual vorb::graphics::IComputeResourceView* makeComputeView(IBuffer* res) = 0;
            virtual vorb::graphics::IComputeResourceView* makeComputeView(ITexture1D* res) = 0;
            virtual vorb::graphics::IComputeResourceView* makeComputeView(ITexture2D* res) = 0;
            virtual vorb::graphics::IComputeResourceView* makeComputeView(ITexture3D* res) = 0;
#pragma endregion

            virtual vorb::graphics::ShaderBytecode compileShaderSource(const cString data, size_t length, ShaderType type, ShaderCompilerInfo headerInfo) = 0;
            virtual vorb::graphics::IShaderCode* loadCompiledShader(ShaderBytecode code) = 0;

            virtual void present() = 0;

            virtual void dispose() = 0;
        protected:
            virtual vorb::graphics::IContext* getResourceBuilderContext() = 0;

            virtual void add(IResource* resource) = 0;
            virtual void remove(IResource* resource) = 0;
            virtual void free(IResource* resource) = 0;

            APIVersion m_api;
        };

        class ResourceAllocator {
            friend class IContext;
        public:
            vorb::graphics::IBuffer* create(const BufferDescription& desc) {
                return m_context->create(desc);
            }
            vorb::graphics::IConstantBlock* create(const ConstantBlockDescription& desc) {
                return m_context->create(desc);
            }
            vorb::graphics::ITexture1D* create(const Texture1DDescription& desc) {
                return m_context->create(desc);
            }
            vorb::graphics::ITexture2D* create(const Texture2DDescription& desc) {
                return m_context->create(desc);
            }
            vorb::graphics::ITexture3D* create(const Texture3DDescription& desc) {
                return m_context->create(desc);
            }
            vorb::graphics::IShader* create(const IShaderCode* code) {
                return m_context->create(code);
            }
            vorb::graphics::IVertexShader* createVertexShader(const IShaderCode* code) {
                return m_context->createVertexShader(code);
            }
            vorb::graphics::ITessGenShader* createTessGenShader(const IShaderCode* code) {
                return m_context->createTessGenShader(code);
            }
            vorb::graphics::ITessEvalShader* createTessEvalShader(const IShaderCode* code) {
                return m_context->createTessEvalShader(code);
            }
            vorb::graphics::IGeometryShader* createGeometryShader(const IShaderCode* code) {
                return m_context->createGeometryShader(code);
            }
            vorb::graphics::IPixelShader* createPixelShader(const IShaderCode* code) {
                return m_context->createPixelShader(code);
            }
            vorb::graphics::IComputeShader* createComputeShader(const IShaderCode* code) {
                return m_context->createComputeShader(code);
            }
            vorb::graphics::IBufferView* makeView(IBuffer* res) {
                return m_context->makeView(res);
            }
            vorb::graphics::IConstantBlockView* makeView(IConstantBlock* res) {
                return m_context->makeView(res);
            }
            vorb::graphics::ITexture1DView* makeView(ITexture1D* res) {
                return m_context->makeView(res);
            }
            vorb::graphics::ITexture2DView* makeView(ITexture2D* res) {
                return m_context->makeView(res);
            }
            vorb::graphics::ITexture3DView* makeView(ITexture3D* res) {
                return m_context->makeView(res);
            }
            vorb::graphics::IComputeResourceView* makeComputeView(IBuffer* res) {
                return m_context->makeComputeView(res);
            }
            vorb::graphics::IComputeResourceView* makeComputeView(ITexture1D* res) {
                return m_context->makeComputeView(res);
            }
            vorb::graphics::IComputeResourceView* makeComputeView(ITexture2D* res) {
                return m_context->makeComputeView(res);
            }
            vorb::graphics::IComputeResourceView* makeComputeView(ITexture3D* res) {
                return m_context->makeComputeView(res);
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
