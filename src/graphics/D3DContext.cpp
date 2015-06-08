#include "stdafx.h"
#include "D3DContext.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include "D3DDescCompile.h"
#include "D3DResource.h"

vg::IBuffer* vg::D3DContext::create(const BufferDescription& desc) {
    // Create the D3D buffer description on the stack
    CBufferDescription cDesc = {};
    fill(cDesc, desc);

    // Generate buffer
    return create(&cDesc);
}
vg::IBuffer* vg::D3DContext::create(const CBufferDescription* desc) {
    // Allocate buffer instance
    D3DBuffer* buffer = new D3DBuffer(this);
    buffer->size = desc->ByteWidth;

    // Create the buffer on the GPU
    m_device->CreateBuffer(desc, nullptr, &buffer->data);

    // Return handle
    return buffer;
}

void vg::D3DContext::present() {
    m_dxgi.swapChain->Present(1, 0);
}

// TODO(Cristian): Implement
vg::IShaderCode* vorb::graphics::D3DContext::createFromPrecompiled(const void* data, size_t length) {
    D3DShaderCode* code = new D3DShaderCode(this);
    code->data = operator new(length);
    memcpy(code->data, data, length);
    return code;
}
vg::IShaderCode* vorb::graphics::D3DContext::createFromCode(const cString data, size_t length) {
    D3DShaderCodeBlob* blob = new D3DShaderCodeBlob(this);
    ID3DBlob* error;
    D3DCompile(data, length, NULL, NULL, NULL, NULL, "vs_4_0", 0, 0,&blob->shaderBlob, &error);
    error->Release();
    return blob;
}

vg::IVertexShader* vorb::graphics::D3DContext::createVertexShader(const IShaderCode* code) {
    //D3DVertexShader* shader = new D3DVertexShader(this);
    //m_device->CreateVertexShader();
    return nullptr;
}
vg::IGeometryShader* vorb::graphics::D3DContext::createGeometryShader(const IShaderCode* code) {
    return nullptr;
}
vg::ITessGenShader* vorb::graphics::D3DContext::createTessGenShader(const IShaderCode* code) {
    return nullptr;
}
vg::ITessEvalShader* vorb::graphics::D3DContext::createTessEvalShader(const IShaderCode* code) {
    return nullptr;
}
vg::IPixelShader* vorb::graphics::D3DContext::createPixelShader(const IShaderCode* code) {
    return nullptr;
}
vg::IComputeShader* vorb::graphics::D3DContext::createComputeShader(const IShaderCode* code) {
    return nullptr;
}
