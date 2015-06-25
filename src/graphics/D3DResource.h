//
// D3DResource.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DResource.h
 * @brief 
 */

#pragma once

#ifndef Vorb_D3DResource_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DResource_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <d3d11.h>

#include "graphics/IResource.h"

namespace vorb {
    namespace graphics {

        class D3DBuffer : public IBuffer {
        public:
            D3DBuffer(IContext* owner) : IBuffer(owner) {
                // Empty
            }

            virtual void disposeInternal() override {
                data->Release();
            }

            virtual size_t getMemoryUsed() const override {
                return size;
            }


            ID3D11Buffer* data = nullptr;
            size_t size = 0;
        };

        class D3DShaderCode : public IShaderCode {
        public:
            D3DShaderCode(IContext* owner) : IShaderCode(owner) {
                // Empty
            }

            virtual const void* getCode() const override {
                return data;
            }
            virtual size_t getLength() const override {
                return size;
            }

            virtual size_t getMemoryUsed() const override {
                return 0;
            }

            virtual void disposeInternal() override {
                delete(data);
            }

            void* data = nullptr;
            size_t size = 0;
        };
        class D3DShaderCodeBlob : public IShaderCode {
        public:
            D3DShaderCodeBlob(IContext* owner) : IShaderCode(owner) {
                // Empty
            }

            virtual const void* getCode() const override {
                return shaderBlob->GetBufferPointer();
            }
            virtual size_t getLength() const override {
                return shaderBlob->GetBufferSize();
            }

            virtual size_t getMemoryUsed() const override {
                return shaderBlob->GetBufferSize();
            }

            virtual void disposeInternal() override {
                shaderBlob->Release();
            }

            ID3DBlob* shaderBlob = nullptr;
        };
        class D3DVertexShader : public IVertexShader {
        public:
            D3DVertexShader(IContext* owner) : IVertexShader(owner) {
                // Empty
            }

            virtual size_t getMemoryUsed() const {
                throw std::logic_error("The method or operation is not implemented.");
            }

            virtual void disposeInternal() {
                shader->Release();
            }

            ID3D11VertexShader* shader;
        };
    }
}

#endif // !Vorb_D3DResource_h__
