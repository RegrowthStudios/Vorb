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
vorb::graphics::IConstantBlock* vorb::graphics::D3DContext::create(const ConstantBlockDescription& desc) {
    // Allocate buffer instance
    D3DConstantBlock* buffer = new D3DConstantBlock(this);

    // Fill the D3D description
    D3D11_BUFFER_DESC cdesc;
    cdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    if (desc.size > (D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT << 4)) {
        cdesc.ByteWidth = D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT << 4;
    } else if (desc.size & 0xfu) {
        cdesc.ByteWidth = desc.size & ~0xfu;
        cdesc.ByteWidth += 0x10u;
    } else {
        cdesc.ByteWidth = desc.size;
    }
    cdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    cdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    buffer->size = cdesc.ByteWidth;

    // Create the buffer on the GPU
    m_device->CreateBuffer(&cdesc, nullptr, &buffer->data);

    // Return handle
    return buffer;
}
vorb::graphics::ITexture1D* vorb::graphics::D3DContext::create(const Texture1DDescription& desc) {
    D3DTexture1D* texture = new D3DTexture1D(this);

    D3D11_TEXTURE1D_DESC cdesc {};
    cdesc.Width = desc.width;
    cdesc.ArraySize = desc.atlasPages;

    m_device->CreateTexture1D(&cdesc, nullptr, &texture->data);

    // TODO(Cristian): Compute texture size

    return texture;
}
vorb::graphics::ITexture2D* vorb::graphics::D3DContext::create(const Texture2DDescription& desc) {
    D3DTexture2D* texture = new D3DTexture2D(this);

    D3D11_TEXTURE2D_DESC cdesc {};
    cdesc.Width = desc.width;
    cdesc.Height = desc.height;
    cdesc.ArraySize = desc.atlasPages;

    m_device->CreateTexture2D(&cdesc, nullptr, &texture->data);

    // TODO(Cristian): Compute texture size

    return texture;
}
vorb::graphics::ITexture3D* vorb::graphics::D3DContext::create(const Texture3DDescription& desc) {
    D3DTexture3D* texture = new D3DTexture3D(this);

    D3D11_TEXTURE3D_DESC cdesc {};
    cdesc.Width = desc.width;
    cdesc.Height = desc.height;
    cdesc.Depth = desc.depth;
    
    m_device->CreateTexture3D(&cdesc, nullptr, &texture->data);

    // TODO(Cristian): Compute texture size

    return texture;
}

void vg::D3DContext::present() {
    m_dxgi.swapChain->Present(1, 0);
}

// TODO(Cristian): Implement
vorb::graphics::ShaderBytecode vorb::graphics::D3DContext::compileShaderSource(const cString data, size_t length, ShaderType type, ShaderCompilerInfo headerInfo) {
    ID3DBlob* blob = nullptr;
    ID3DBlob* error = nullptr;
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
    D3DCompile(data, length, nullptr, nullptr, nullptr, "main", buf, 0, 0, &blob, &error);
    if (error) {
        puts((const cString)error->GetBufferPointer());
        error->Release();
        return ShaderBytecode {};
    }

    // Copy compiled code to own buffer
    ShaderBytecode code;
    code.type = type;
    code.length = blob->GetBufferSize();
    code.alloc(code.length + sizeof(vg::ShaderType));
    memcpy(code.code, blob->GetBufferPointer(), code.length);
    blob->Release();

    return code;
}
vorb::graphics::IShaderCode* vorb::graphics::D3DContext::loadCompiledShader(ShaderBytecode code) {
    D3DShaderCode* shaderCode = new D3DShaderCode(this);
    shaderCode->data = malloc(code.length);
    shaderCode->size = code.length;
    memcpy(shaderCode->data, code.code, shaderCode->size);
    return shaderCode;
}

vg::IVertexShader* vorb::graphics::D3DContext::createVertexShader(const IShaderCode* code) {
    D3DVertexShader* shader = new D3DVertexShader(this);
    HRESULT hr = m_device->CreateVertexShader(code->getCode(), code->getLength(), nullptr, &shader->shader);
    if (hr != S_OK) {
        shader->dispose();
        return nullptr;
    } else {
        return shader;
    }
}
vg::ITessGenShader* vorb::graphics::D3DContext::createTessGenShader(const IShaderCode* code) {
    auto* shader = new D3DTessGenShader(this);
    HRESULT hr = m_device->CreateHullShader(code->getCode(), code->getLength(), nullptr, &shader->shader);
    if (hr != S_OK) {
        shader->dispose();
        return nullptr;
    } else {
        return shader;
    }
}
vg::ITessEvalShader* vorb::graphics::D3DContext::createTessEvalShader(const IShaderCode* code) {
    auto* shader = new D3DTessEvalShader(this);
    HRESULT hr = m_device->CreateDomainShader(code->getCode(), code->getLength(), nullptr, &shader->shader);
    if (hr != S_OK) {
        shader->dispose();
        return nullptr;
    } else {
        return shader;
    }
}
vg::IGeometryShader* vorb::graphics::D3DContext::createGeometryShader(const IShaderCode* code) {
    D3DGeometryShader* shader = new D3DGeometryShader(this);
    HRESULT hr = m_device->CreateGeometryShader(code->getCode(), code->getLength(), nullptr, &shader->shader);
    if (hr != S_OK) {
        shader->dispose();
        return nullptr;
    } else {
        return shader;
    }
}
vg::IPixelShader* vorb::graphics::D3DContext::createPixelShader(const IShaderCode* code) {
    D3DPixelShader* shader = new D3DPixelShader(this);
    HRESULT hr = m_device->CreatePixelShader(code->getCode(), code->getLength(), nullptr, &shader->shader);
    if (hr != S_OK) {
        shader->dispose();
        return nullptr;
    } else {
        return shader;
    }
}
vg::IComputeShader* vorb::graphics::D3DContext::createComputeShader(const IShaderCode* code) {
    auto* shader = new D3DComputeShader(this);
    HRESULT hr = m_device->CreateComputeShader(code->getCode(), code->getLength(), nullptr, &shader->shader);
    if (hr != S_OK) {
        shader->dispose();
        return nullptr;
    } else {
        return shader;
    }
}

void vorb::graphics::D3DContext::add(IResource* resource) {
    m_resources.insert(resource);
}
void vorb::graphics::D3DContext::remove(IResource* resource) {
    // m_resources.erase(resource);
}
void vorb::graphics::D3DContext::free(IResource* resource) {
    delete resource;
}

void vorb::graphics::D3DContext::getFeatureSupport() {
    D3D11_FEATURE_DATA_THREADING threadingInfo;
    m_device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadingInfo, sizeof(D3D11_FEATURE_DATA_THREADING));
}

vorb::graphics::IBufferView* vorb::graphics::D3DContext::makeView(IBuffer* res) {
    D3DBuffer* cres = static_cast<D3DBuffer*>(res);
    D3DShaderResourceView* srv = new D3DShaderResourceView(this);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;


    m_device->CreateShaderResourceView(cres->data, &desc, &srv->view);
    return srv;
}
vorb::graphics::IConstantBlockView* vorb::graphics::D3DContext::makeView(IConstantBlock* res) {
    D3DConstantBlock* cres = static_cast<D3DConstantBlock*>(res);
    D3DShaderResourceView* srv = new D3DShaderResourceView(this);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    
    
    m_device->CreateShaderResourceView(cres->data, &desc, &srv->view);
    return srv;
}
vorb::graphics::ITexture1DView* vorb::graphics::D3DContext::makeView(ITexture1D* res) {
    D3DTexture1D* cres = static_cast<D3DTexture1D*>(res);
    D3DShaderResourceView* srv = new D3DShaderResourceView(this);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc {};
    if (cres->arraySlices != 0) {
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
        desc.Texture1DArray.FirstArraySlice = 0;
        desc.Texture1DArray.ArraySize = cres->arraySlices;
        desc.Texture1DArray.MipLevels = 1;
        desc.Texture1DArray.MostDetailedMip = 0;
    } else {
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
        desc.Texture1D.MipLevels = 1;
        desc.Texture1D.MostDetailedMip = 0;
    }

    m_device->CreateShaderResourceView(cres->data, &desc, &srv->view);
    return srv;
}
vorb::graphics::ITexture2DView* vorb::graphics::D3DContext::makeView(ITexture2D* res) {
    D3DTexture2D* cres = static_cast<D3DTexture2D*>(res);
    D3DShaderResourceView* srv = new D3DShaderResourceView(this);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;


    m_device->CreateShaderResourceView(cres->data, &desc, &srv->view);
    return srv;
}
vorb::graphics::ITexture3DView* vorb::graphics::D3DContext::makeView(ITexture3D* res) {
    D3DTexture3D* cres = static_cast<D3DTexture3D*>(res);
    D3DShaderResourceView* srv = new D3DShaderResourceView(this);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    
    
    m_device->CreateShaderResourceView(cres->data, &desc, &srv->view);
    return srv;
}

