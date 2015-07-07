//
// D3DDevice.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DDevice.h
 * @brief 
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

            virtual IRenderTarget* create(ITexture2D* res) override;

            virtual IVertexStateBind* create(IVertexDeclaration* decl, const BufferBindings& bindings);

            virtual void use(IRenderTarget* renderTarget) override;

            virtual void use(IVertexDeclaration* decl);

            virtual void computeUse(IComputeShader* shader) override;
            virtual void computeUse(ui32 slot, IComputeResourceView* v) override;

            virtual void dispatchThreads(ui32 x, ui32 y, ui32 z) override;

            virtual void vertexUse(IVertexShader* shader);

            virtual void tessGenUse(ITessGenShader* shader);

            virtual void tessEvalUse(ITessEvalShader* shader);

            virtual void pixelUse(IPixelShader* shader);

            virtual void setTopology(vg::PrimitiveType type);

            virtual void drawIndexed(size_t indices, size_t indexOff = 0, size_t vertexOff = 0);

            virtual void setVertexBuffers(vg::IBuffer** verts, ui32 startSlot, size_t numBuffers, ui32* offsets, ui32* strides);

            virtual void setVertexBuffers(const BufferBindings& bindings);

            virtual void setIndexBuffer(vg::IBuffer* ind, vg::MemoryFormat format, ui32 offset = 0);

            virtual void dispose();


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
