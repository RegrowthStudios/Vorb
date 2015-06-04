//
// D3DContext.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DContext.h
 * @brief 
 */

#pragma once

#ifndef Vorb_D3DContext_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DContext_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "graphics/IContext.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGIAdapter;
struct IDXGIDevice;
struct IDXGIFactory;
struct IDXGISwapChain;

namespace vorb {
    namespace graphics {
        class D3DDevice;

        class D3DContext : public IContext {
            friend class D3DAdapter;
        public:
            virtual IBuffer* create(const BufferDescription& desc) override;
            virtual IBuffer* create(const CBufferDescription* desc) override;

            virtual void present() override;

            virtual IShaderCode* createFromPrecompiled(const void* data, size_t length);
            virtual IShaderCode* createFromCode(const cString data, size_t length);

            virtual IVertexShader* createVertexShader(const IShaderCode* code);
            virtual IGeometryShader* createGeometryShader(const IShaderCode* code);
            virtual ITessGenShader* createTessGenShader(const IShaderCode* code);
            virtual ITessEvalShader* createTessEvalShader(const IShaderCode* code);
            virtual IPixelShader* createPixelShader(const IShaderCode* code);
            virtual IComputeShader* createComputeShader(const IShaderCode* code);
        private:
            ID3D11Device* m_device;
            ID3D11DeviceContext* m_immediateContext;
            D3DDevice* m_defaultDevice;

            struct {
                IDXGIAdapter* adapter;
                IDXGIDevice* device;
                IDXGIFactory* factory;
                IDXGISwapChain* swapChain;
            } m_dxgi;
        };
    }
}

#endif // !Vorb_D3DContext_h__
