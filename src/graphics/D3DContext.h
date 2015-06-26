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
            virtual IContext* getResourceBuilderContext() override {
                return this;
            }

            // TODO: This needs to be expanded
            void getFeatureSupport();

            virtual IBuffer* create(const BufferDescription& desc) override;
            virtual IBuffer* create(const CBufferDescription* desc) override;
            virtual IConstantBlock* create(const ConstantBlockDescription& desc) override;
            virtual ITexture1D* create(const Texture1DDescription& desc) override;
            virtual ITexture2D* create(const Texture2DDescription& desc) override;
            virtual ITexture3D* create(const Texture3DDescription& desc) override;
            virtual IBufferView* makeView(IBuffer* res) override;
            virtual IConstantBlockView* makeView(IConstantBlock* res) override;
            virtual ITexture1DView* makeView(ITexture1D* res) override;
            virtual ITexture2DView* makeView(ITexture2D* res) override;
            virtual ITexture3DView* makeView(ITexture3D* res) override;
            virtual IComputeResourceView* makeComputeView(IBuffer* res) override;
            virtual IComputeResourceView* makeComputeView(ITexture1D* res) override;
            virtual IComputeResourceView* makeComputeView(ITexture2D* res) override;
            virtual IComputeResourceView* makeComputeView(ITexture3D* res) override;

            virtual void present() override;

            virtual ShaderBytecode compileShaderSource(const cString data, size_t length, ShaderType type, ShaderCompilerInfo headerInfo);
            virtual IShaderCode* loadCompiledShader(ShaderBytecode code);

            virtual IVertexShader* createVertexShader(const IShaderCode* code);
            virtual ITessGenShader* createTessGenShader(const IShaderCode* code);
            virtual ITessEvalShader* createTessEvalShader(const IShaderCode* code);
            virtual IGeometryShader* createGeometryShader(const IShaderCode* code);
            virtual IPixelShader* createPixelShader(const IShaderCode* code);
            virtual IComputeShader* createComputeShader(const IShaderCode* code);
        protected:
            virtual void add(IResource* resource);
            virtual void remove(IResource* resource);
            virtual void free(IResource* resource);


        private:
            ID3D11Device* m_device;
            ID3D11DeviceContext* m_immediateContext;
            D3DDevice* m_defaultDevice;

            std::unordered_set<IResource*> m_resources;

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
