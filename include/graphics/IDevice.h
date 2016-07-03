//
// IDevice.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file IDevice.h
 * 
 * \brief The device is responsible for submitting draw calls to the graphics device.
 */

#pragma once

#ifndef Vorb_IDevice_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IDevice_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "Enums.h"
#include "IResource.h"
#include "IObject.h"
#include "Wrappers.h"

namespace vorb {
    namespace graphics {
        /* \brief
         */
        class IDevice {
        public:
            virtual void dispose() = 0;

            virtual vorb::graphics::IVertexStateBind* create(IVertexDeclaration* decl, const BufferBindings& bindings) = 0;
            virtual vorb::graphics::IRenderTarget* create(ITexture2D* res) = 0;
            virtual vorb::graphics::IBlendState* create(const BlendStateDescription& desc) = 0;
            virtual vorb::graphics::IDepthStencilState* create(const DepthStencilStateDescription& desc) = 0;
            virtual vorb::graphics::IRasterizerState* create(const RasterizerStateDescription& desc) = 0;
            virtual vorb::graphics::ISamplerState* create(const SamplerStateDescription& desc) = 0;
            virtual vorb::graphics::IQuery* create(const QueryDescription& desc) = 0;
            virtual vorb::graphics::IPredicate* create(const PredicateDescription& desc) = 0;
            virtual vorb::graphics::ISyncFence* create(const SyncFenceDescription& desc) = 0;

            virtual void generateMipmaps(IResourceView* v) = 0;

            /************************************************************************/
            /* 3D mode                                                              */
            /************************************************************************/

            virtual void use(IRenderTarget* renderTarget) = 0;
            virtual void setViewports(ui32 count, const Viewport* viewports) = 0;

            virtual void setClearColor(const f64v4& v) = 0;
            virtual void setClearDepth(f64 v) = 0;
            virtual void setClearStencil(ui32 v) = 0;
            virtual void clear(ClearBits bits) = 0;

            virtual void begin(IQuery* query) = 0;
            virtual void end(IQuery* query) = 0;

            virtual void use(IBlendState* state) = 0;
            virtual void use(IDepthStencilState* state) = 0;
            virtual void use(IRasterizerState* state) = 0;

            virtual void vertexUse(IVertexShader* shader) = 0;
            virtual void vertexUse(ui32 slot, ui32 count, IConstantBlockView** v) = 0;
            virtual void vertexUse(ui32 slot, ui32 count, ISamplerState** v) = 0;
            virtual void vertexUse(ui32 slot, ui32 count, IResourceView** v) = 0;
            virtual void tessGenUse(ITessGenShader* shader) = 0;
            virtual void tessGenUse(ui32 slot, ui32 count, IConstantBlockView** v) = 0;
            virtual void tessGenUse(ui32 slot, ui32 count, ISamplerState** v) = 0;
            virtual void tessGenUse(ui32 slot, ui32 count, IResourceView** v) = 0;
            virtual void tessEvalUse(ITessEvalShader* shader) = 0;
            virtual void tessEvalUse(ui32 slot, ui32 count, IConstantBlockView** v) = 0;
            virtual void tessEvalUse(ui32 slot, ui32 count, ISamplerState** v) = 0;
            virtual void tessEvalUse(ui32 slot, ui32 count, IResourceView** v) = 0;
            virtual void geometryUse(ITessEvalShader* shader) = 0;
            virtual void geometryUse(ui32 slot, ui32 count, IConstantBlockView** v) = 0;
            virtual void geometryUse(ui32 slot, ui32 count, ISamplerState** v) = 0;
            virtual void geometryUse(ui32 slot, ui32 count, IResourceView** v) = 0;
            virtual void pixelUse(IPixelShader* shader) = 0;
            virtual void pixelUse(ui32 slot, ui32 count, IConstantBlockView** v) = 0;
            virtual void pixelUse(ui32 slot, ui32 count, ISamplerState** v) = 0;
            virtual void pixelUse(ui32 slot, ui32 count, IResourceView** v) = 0;

            virtual void setTopology(vg::PrimitiveType type) = 0;
            virtual void use(IVertexDeclaration* decl) = 0;
            virtual void setVertexBuffers(const BufferBindings& bindings) = 0;
            virtual void setIndexBuffer(vg::IBuffer* ind, vg::MemoryFormat format, ui32 offset = 0) = 0;

            virtual void draw(size_t vertexCount, size_t vertexOff = 0) = 0;
            virtual void drawAutomatic() = 0;
            virtual void drawIndexed(size_t indexCount, size_t indexOff = 0, size_t vertexOff = 0) = 0;
            virtual void drawInstanced(size_t vertexCountPerInstance, size_t instanceCount, size_t vertexOff = 0, size_t instanceOff = 0) = 0;
            virtual void drawIndexedInstanced(size_t indexCountPerInstance, size_t instanceCount, size_t indexOff = 0, size_t vertexOff = 0, size_t instanceOff = 0) = 0;

            virtual void flush() = 0;
          
            /************************************************************************/
            /* Compute mode                                                         */
            /************************************************************************/

            virtual void clear(IComputeResourceView* res, const f32(&data)[4]) = 0;
            virtual void clear(IComputeResourceView* res, const ui32(&data)[4]) = 0;

            virtual void computeUse(IComputeShader* shader) = 0;
            virtual void computeUse(ui32 slot, ui32 count, IComputeResourceView** v) = 0;
            virtual void computeUse(ui32 slot, ui32 count, IConstantBlockView** v) = 0;
            virtual void computeUse(ui32 slot, ui32 count, ISamplerState** v) = 0;
            virtual void computeUse(ui32 slot, ui32 count, IResourceView** v) = 0;

            virtual void dispatchThreads(ui32 x, ui32 y, ui32 z) = 0;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IDevice_h__
