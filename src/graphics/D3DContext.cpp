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
vorb::graphics::ShaderBytecode vorb::graphics::D3DContext::compileShaderSource(const cString data, size_t length, ShaderType type, ShaderCompilerInfo headerInfo) {
    ID3DBlob* blob;
    ID3DBlob* error;
    char buf[32];
    switch (type) {
    case ShaderType::VERTEX_SHADER:
        sprintf(buf, "vs_%d_%d", headerInfo.version.major, headerInfo.version.minor);
        break;
    case ShaderType::TESS_CONTROL_SHADER:
        sprintf(buf, "hs_%d_%d", headerInfo.version.major, headerInfo.version.minor);
        break;
    case ShaderType::TESS_EVALUATION_SHADER:
        sprintf(buf, "ds_%d_%d", headerInfo.version.major, headerInfo.version.minor);
        break;
    case ShaderType::GEOMETRY_SHADER:
        sprintf(buf, "gs_%d_%d", headerInfo.version.major, headerInfo.version.minor);
        break;
    case ShaderType::FRAGMENT_SHADER:
        sprintf(buf, "ps_%d_%d", headerInfo.version.major, headerInfo.version.minor);
        break;
    case ShaderType::COMPUTE_SHADER:
        sprintf(buf, "cs_%d_%d", headerInfo.version.major, headerInfo.version.minor);
        break;
    default:
        break;
    }
    D3DCompile(data, length, NULL, NULL, NULL, NULL, buf, 0, 0, &blob, &error);
    error->Release();

    // Copy compiled code to own buffer
    ShaderBytecode code;
    code.type = type;
    code.length = blob->GetBufferSize();
    code.alloc(code.length);
    memcpy(code.code, blob->GetBufferPointer(), code.length);
    blob->Release();
}

vg::IVertexShader* vorb::graphics::D3DContext::createVertexShader(const IShaderCode* code) {
    D3DVertexShader* shader = new D3DVertexShader(this);
    m_device->CreateVertexShader(code->getCode(), code->getLength(), nullptr, &shader->shader);
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
