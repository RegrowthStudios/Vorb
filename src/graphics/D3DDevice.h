//
// D3DDevice.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file D3DDevice.h
 * 
 * \brief 
 */

#pragma once

#ifndef Vorb_D3DDevice_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DDevice_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "graphics/IDevice.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace vorb {
    namespace graphics {
        class D3DDevice : public IDevice {
            friend class D3DAdapter;
        public:
            virtual void dispose();
      
            virtual IRenderTarget* create(ITexture2D* res) override;
            virtual IVertexStateBind* create(IVertexDeclaration* decl, const BufferBindings& bindings);
            virtual vorb::graphics::IBlendState* create(const BlendStateDescription& desc);
            virtual vorb::graphics::IDepthStencilState* create(const DepthStencilStateDescription& desc);
            virtual vorb::graphics::IRasterizerState* create(const RasterizerStateDescription& desc);
            virtual vorb::graphics::ISamplerState* create(const SamplerStateDescription& desc);
            virtual vorb::graphics::IQuery* create(const QueryDescription& desc);
            virtual vorb::graphics::IPredicate* create(const PredicateDescription& desc);
            virtual vorb::graphics::ISyncFence* create(const SyncFenceDescription& desc);

            virtual void generateMipmaps(IResourceView* v);

            virtual void use(IRenderTarget* renderTarget) override;
            virtual void setViewports(ui32 count, const Viewport* viewports);

            virtual void clear(ClearBits bits) override;
            virtual void setClearColor(const f64v4& v) override {
                m_clearValues.color.r = (f32)v.r;
                m_clearValues.color.g = (f32)v.g;
                m_clearValues.color.b = (f32)v.b;
                m_clearValues.color.a = (f32)v.a;
            }
            virtual void setClearDepth(f64 v) override {
                m_clearValues.depth = static_cast<f32>(v);
            }
            virtual void setClearStencil(ui32 v) override {
                m_clearValues.depth = static_cast<ui8>(v & 0x000000ffu);
            }

            virtual void begin(IQuery* query);
            virtual void end(IQuery* query);
           
            virtual void use(IBlendState* state);
            virtual void use(IDepthStencilState* state);
            virtual void use(IRasterizerState* state);

            virtual void vertexUse(IVertexShader* shader);
            virtual void vertexUse(ui32 slot, ui32 count, IConstantBlockView** v);
            virtual void vertexUse(ui32 slot, ui32 count, ISamplerState** v);
            virtual void vertexUse(ui32 slot, ui32 count, IResourceView** v);
            virtual void tessGenUse(ITessGenShader* shader);
            virtual void tessGenUse(ui32 slot, ui32 count, IConstantBlockView** v);
            virtual void tessGenUse(ui32 slot, ui32 count, ISamplerState** v);
            virtual void tessGenUse(ui32 slot, ui32 count, IResourceView** v);
            virtual void tessEvalUse(ITessEvalShader* shader);
            virtual void tessEvalUse(ui32 slot, ui32 count, IConstantBlockView** v);
            virtual void tessEvalUse(ui32 slot, ui32 count, ISamplerState** v);
            virtual void tessEvalUse(ui32 slot, ui32 count, IResourceView** v);
            virtual void geometryUse(ITessEvalShader* shader);
            virtual void geometryUse(ui32 slot, ui32 count, IConstantBlockView** v);
            virtual void geometryUse(ui32 slot, ui32 count, ISamplerState** v);
            virtual void geometryUse(ui32 slot, ui32 count, IResourceView** v);
            virtual void pixelUse(IPixelShader* shader);
            virtual void pixelUse(ui32 slot, ui32 count, IConstantBlockView** v);
            virtual void pixelUse(ui32 slot, ui32 count, ISamplerState** v);
            virtual void pixelUse(ui32 slot, ui32 count, IResourceView** v);

            virtual void setTopology(vg::PrimitiveType type);
            virtual void use(IVertexDeclaration* decl);
            void setVertexBuffers(vg::IBuffer** verts, ui32 startSlot, size_t numBuffers, ui32* offsets, ui32* strides);
            virtual void setVertexBuffers(const BufferBindings& bindings);
            virtual void setIndexBuffer(vg::IBuffer* ind, vg::MemoryFormat format, ui32 offset = 0);

            virtual void draw(size_t vertexCount, size_t vertexOff = 0);
            virtual void drawAutomatic();
            virtual void drawIndexed(size_t indices, size_t indexOff = 0, size_t vertexOff = 0);
            virtual void drawInstanced(size_t vertexCountPerInstance, size_t instanceCount, size_t vertexOff = 0, size_t instanceOff = 0);
            virtual void drawIndexedInstanced(size_t indexCountPerInstance, size_t instanceCount, size_t indexOff = 0, size_t vertexOff = 0, size_t instanceOff = 0);

            virtual void flush();

            virtual void clear(IComputeResourceView* res, const f32(&data)[4]);
            virtual void clear(IComputeResourceView* res, const ui32(&data)[4]);

            virtual void computeUse(IComputeShader* shader) override;
            virtual void computeUse(ui32 slot, ui32 count, IComputeResourceView** v);
            virtual void computeUse(ui32 slot, ui32 count, IConstantBlockView** v);
            virtual void computeUse(ui32 slot, ui32 count, ISamplerState** v);
            virtual void computeUse(ui32 slot, ui32 count, IResourceView** v);

            virtual void dispatchThreads(ui32 x, ui32 y, ui32 z) override;
        private:
            ID3D11Device* m_device;
            ID3D11DeviceContext* m_context;

            struct {
                ID3D11RenderTargetView* color;
                ID3D11DepthStencilView* depthStencil;
            } m_target;

            struct {
                f32v4 color;
                f32 depth;
                ui8 stencil;
            } m_clearValues;
        };
    }
}

#endif // !Vorb_D3DDevice_h__
