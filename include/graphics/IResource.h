//
// IResource.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file IResource.h
 * 
 * \brief 
 */

#pragma once

#ifndef Vorb_IResource_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IResource_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "ResourceDescriptions.h"
#include "GLEnums.h"

namespace vorb {
    namespace graphics {
        class IContext;

        /*! \brief This is a GPU resource that must be manually managed
         */
        class IResource {
            friend class IContext;
        public:
            virtual ~IResource() {
                // Empty
            }

            void dispose();
        protected:
            IResource(IContext* owner);

            virtual void disposeInternal() = 0;

            IContext* m_owner; ///< The context that created this resource
        };

        /*! \brief
         */
        class IBuffer : public IResource {
        public:
            const BufferDescription& getDescription() const {
                return m_desc;
            }
        protected:
            IBuffer(IContext* owner) : IResource(owner) {
                // Empty
            }

            BufferDescription m_desc;
        };

        /*! \brief
         */
        class IConstantBlock : public IResource {
        public:
            const ConstantBlockDescription& getDescription() const {
                return m_desc;
            }
        protected:
            IConstantBlock(IContext* owner) : IResource(owner) {
                // Empty
            }

            ConstantBlockDescription m_desc;
        };

        /*! \brief
         */
        class IVertexDeclaration : public IResource {
        public:

        protected:
            IVertexDeclaration(IContext* owner) : IResource(owner) {
                // Empty
            }
        };

#pragma region Textures
        /* \brief
         */
        class ITexture : public IResource {
        public:

        protected:
            ITexture(IContext* owner) : IResource(owner) {
                // Empty
            }
        };

        /* \brief
         */
        class ITexture1D : public ITexture {
        public:
            const Texture1DDescription& getDescription() const {
                return m_desc;
            }
        protected:
            ITexture1D(IContext* owner) : ITexture(owner) {
                // Empty
            }

            Texture1DDescription m_desc;
        };

        /* \brief
         */
        class ITexture2D : public ITexture {
        public:
            const Texture2DDescription& getDescription() const {
                return m_desc;
            }
        protected:
            ITexture2D(IContext* owner) : ITexture(owner) {
                // Empty
            }

            Texture2DDescription m_desc;
        };

        /* \brief
         */
        class ITexture3D : public ITexture {
        public:
            const Texture3DDescription& getDescription() const {
                return m_desc;
            }
        protected:
            ITexture3D(IContext* owner) : ITexture(owner) {
                // Empty
            }

            Texture3DDescription m_desc;
        };
#pragma endregion

#pragma region Shaders
        /* \brief The compiled bytecode of a shader
         */
        class IShaderCode : public IResource {
        public:
            virtual vg::ShaderType getType() const = 0;
            virtual const void* getCode() const = 0;
            virtual size_t getLength() const = 0;
        protected:
            IShaderCode(IContext* owner) : IResource(owner) {
                // Empty
            }
        };

        /* \brief
         */
        class IShader : public IResource {
        public:
            virtual ShaderType getType() const = 0;
        protected:
            IShader(IContext* owner) : IResource(owner) {
                // Empty
            }
        };


        /* \brief
         */
        class IVertexShader : public IShader {
        public:
            virtual ShaderType getType() const override {
                return ShaderType::VERTEX_SHADER;
            }

        protected:
            IVertexShader(IContext* owner) : IShader(owner) {
                // Empty
            }
        };

        /* \brief
         */
        class IPixelShader : public IShader {
        public:
            virtual ShaderType getType() const override {
                return ShaderType::FRAGMENT_SHADER;
            }

        protected:
            IPixelShader(IContext* owner) : IShader(owner) {
                // Empty
            }
        };

        /* \brief
         */
        class IGeometryShader : public IShader {
        public:
            virtual ShaderType getType() const override {
                return ShaderType::GEOMETRY_SHADER;
            }

        protected:
            IGeometryShader(IContext* owner) : IShader(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class ITessGenShader : public IShader {
        public:
            virtual ShaderType getType() const override {
                return ShaderType::TESS_CONTROL_SHADER;
            }

        protected:
            ITessGenShader(IContext* owner) : IShader(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class ITessEvalShader : public IShader {
        public:
            virtual ShaderType getType() const override {
                return ShaderType::TESS_EVALUATION_SHADER;
            }

        protected:
            ITessEvalShader(IContext* owner) : IShader(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class IComputeShader : public IShader {
        public:
            virtual ShaderType getType() const override {
                return ShaderType::COMPUTE_SHADER;
            }

        protected:
            IComputeShader(IContext* owner) : IShader(owner) {
                // Empty
            }
        };
#pragma endregion

#pragma region Resource Views
        /*! \brief 
         */
        class IResourceView : public IResource {
        public:

        protected:
            IResourceView(IContext* owner) : IResource(owner) {
                // Empty
            }
        };

        class IBufferView : public IResourceView {
        public:

        protected:
            IBufferView(IContext* owner) : IResourceView(owner) {
                // Empty
            }
        };

        class IConstantBlockView : public IResourceView {
        public:

        protected:
            IConstantBlockView(IContext* owner) : IResourceView(owner) {
                // Empty
            }
        };

        class ITexture1DView : public IResourceView {
        public:

        protected:
            ITexture1DView(IContext* owner) : IResourceView(owner) {
                // Empty
            }
        };

        class ITexture2DView : public IResourceView {
        public:

        protected:
            ITexture2DView(IContext* owner) : IResourceView(owner) {
                // Empty
            }
        };

        class ITexture3DView : public IResourceView {
        public:

        protected:
            ITexture3DView(IContext* owner) : IResourceView(owner) {
                // Empty
            }
        };

        class IComputeResourceView : public IResourceView {
        public:

        protected:
            IComputeResourceView(IContext* owner) : IResourceView(owner) {
                // Empty
            }
        };
#pragma endregion
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IResource_h__
