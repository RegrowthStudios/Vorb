#include "stdafx.h"
#include "D3DContext.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include "D3DResource.h"
#include "D3DMap.h"

vg::IBuffer* vg::D3DContext::create(const BufferDescription& desc, OPT InitalResourceData* data) {
    // Allocate buffer instance
    D3DBuffer* buffer = new D3DBuffer(this);

    D3D11_BUFFER_DESC cdesc;
    switch (desc.type) {
    case BufferTarget::ELEMENT_ARRAY_BUFFER:
        cdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    case BufferTarget::ARRAY_BUFFER:
        cdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    default:
        // TODO(Cristian): Add other buffer types
        break;
    }

    // TODO(Cristian): Change usage member of description
    switch (desc.usage) {
    case BufferUsageHint::DYNAMIC_COPY:
    case BufferUsageHint::DYNAMIC_READ:
    case BufferUsageHint::DYNAMIC_DRAW:
        cdesc.Usage = D3D11_USAGE_DYNAMIC;
        break;
    case BufferUsageHint::STATIC_COPY:
    case BufferUsageHint::STATIC_READ:
    case BufferUsageHint::STATIC_DRAW:
        cdesc.Usage = D3D11_USAGE_IMMUTABLE;
        break;
    case BufferUsageHint::STREAM_COPY:
    case BufferUsageHint::STREAM_READ:
    case BufferUsageHint::STREAM_DRAW:
        cdesc.Usage = D3D11_USAGE_STAGING;
        break;
        // TODO(Cristian): Look for D3D11_USAGE_DEFAULT
    default:
        break;
    }

    switch (desc.usage) {
    case BufferUsageHint::DYNAMIC_COPY:
    case BufferUsageHint::STATIC_COPY:
    case BufferUsageHint::STREAM_COPY:
        cdesc.CPUAccessFlags = 0; // Sucks don't it
        break;
    case BufferUsageHint::DYNAMIC_READ:
    case BufferUsageHint::STATIC_READ:
    case BufferUsageHint::STREAM_READ:
        cdesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        break;
    case BufferUsageHint::DYNAMIC_DRAW:
    case BufferUsageHint::STATIC_DRAW:
    case BufferUsageHint::STREAM_DRAW:
        cdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
        // TODO(Cristian): Add RW capability
    default:
        break;
    }
    cdesc.ByteWidth = desc.size; // TODO(Cristian): This should work?
    cdesc.MiscFlags = 0; // TODO(Cristian): Add other options?


    if (data) {
        D3D11_SUBRESOURCE_DATA cdata {};
        cdata.pSysMem = data->data;
        m_device->CreateBuffer(&cdesc, &cdata, &buffer->data);
    } else {
        m_device->CreateBuffer(&cdesc, nullptr, &buffer->data);
    }

    // Return handle
    return buffer;
}
vorb::graphics::IConstantBlock* vorb::graphics::D3DContext::create(const ConstantBlockDescription& desc, OPT InitalResourceData* data) {
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

    if (data) {
        D3D11_SUBRESOURCE_DATA cdata {};
        cdata.pSysMem = data->data;
        m_device->CreateBuffer(&cdesc, &cdata, &buffer->data);
    } else {
        m_device->CreateBuffer(&cdesc, nullptr, &buffer->data);
    }

    // Return handle
    return buffer;
}
vorb::graphics::ITexture1D* vorb::graphics::D3DContext::create(const Texture1DDescription& desc, OPT InitalResourceData* data) {
    D3DTexture1D* texture = new D3DTexture1D(this);
    texture->m_desc = desc;

    D3D11_TEXTURE1D_DESC cdesc {};
    cdesc.Width = desc.width;
    cdesc.ArraySize = desc.atlasPages;
    cdesc.Format = vg::mapD3D::format[(size_t)desc.format];

    if (data) {
        D3D11_SUBRESOURCE_DATA cdata {};
        cdata.pSysMem = data->data;
        m_device->CreateTexture1D(&cdesc, &cdata, &texture->data);
    } else {
        m_device->CreateTexture1D(&cdesc, nullptr, &texture->data);
    }

    // TODO(Cristian): Compute texture size

    return texture;
}
vorb::graphics::ITexture2D* vorb::graphics::D3DContext::create(const Texture2DDescription& desc, OPT InitalResourceData* data) {
    D3DTexture2D* texture = new D3DTexture2D(this);
    texture->m_desc = desc;

    D3D11_TEXTURE2D_DESC cdesc {};
    cdesc.Width = desc.width;
    cdesc.Height = desc.height;
    cdesc.ArraySize = desc.atlasPages;

    if (data) {
        D3D11_SUBRESOURCE_DATA cdata {};
        cdata.pSysMem = data->data;
        cdata.SysMemPitch = data->stride.texture2DRow;
        m_device->CreateTexture2D(&cdesc, &cdata, &texture->data);
    } else {
        m_device->CreateTexture2D(&cdesc, nullptr, &texture->data);
    }

    // TODO(Cristian): Compute texture size

    return texture;
}
vorb::graphics::ITexture3D* vorb::graphics::D3DContext::create(const Texture3DDescription& desc, OPT InitalResourceData* data) {
    D3DTexture3D* texture = new D3DTexture3D(this);
    texture->m_desc = desc;

    D3D11_TEXTURE3D_DESC cdesc {};
    cdesc.Width = desc.width;
    cdesc.Height = desc.height;
    cdesc.Depth = desc.depth;
    
    if (data) {
        D3D11_SUBRESOURCE_DATA cdata {};
        cdata.pSysMem = data->data;
        cdata.SysMemPitch = data->stride.texture2DRow;
        cdata.SysMemSlicePitch = data->stride.texture3DSlice;
        m_device->CreateTexture3D(&cdesc, &cdata, &texture->data);
    } else {
        m_device->CreateTexture3D(&cdesc, nullptr, &texture->data);
    }

    // TODO(Cristian): Compute texture size

    return texture;
}

vorb::graphics::IVertexDeclaration* vorb::graphics::D3DContext::create(VertexElementDescription* desc, IShaderCode* vertexShaderCode, size_t numElements) {
    D3DShaderCode* code = (D3DShaderCode*)vertexShaderCode;
    D3DVertexDeclaration* decl = new D3DVertexDeclaration(this);

    D3D11_INPUT_ELEMENT_DESC* cdesc = (D3D11_INPUT_ELEMENT_DESC*)alloca(numElements * sizeof(D3D11_INPUT_ELEMENT_DESC));
    for (size_t i = 0; i < numElements; i++) {
        cdesc[i].AlignedByteOffset = desc[i].offset;
        cdesc[i].Format = mapD3D::format[(size_t)desc[i].type];
        cdesc[i].InputSlot = desc[i].slot;
        cdesc[i].SemanticIndex = desc[i].semanticIndex;
        cdesc[i].SemanticName = mapD3D::semanticName[(size_t)desc[i].semantic];
        cdesc[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
        cdesc[i].InstanceDataStepRate = 0;
    }
    m_device->CreateInputLayout(cdesc, numElements, code->data, code->size, &decl->layout);
    return decl;
}

void vg::D3DContext::present() {
    m_dxgi.swapChain->Present(1, 0);
}

// TODO(Cristian): Add a move method
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
    D3DConstantBlockView* srv = new D3DConstantBlockView(this);
    srv->buffer = cres->data;
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

vorb::graphics::IComputeResourceView* vorb::graphics::D3DContext::makeComputeView(IBuffer* res) {
    D3DBuffer* cres = static_cast<D3DBuffer*>(res);
    D3DComputeResourceView* crv = new D3DComputeResourceView(this);

    D3D11_UNORDERED_ACCESS_VIEW_DESC desc {};
    desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    desc.Buffer.FirstElement = 0;
    desc.Buffer.NumElements = 0; // TODO(Cristian): Properly fill in
    desc.Buffer.Flags = 0; // TODO(Cristian): Properly fill in
    desc.Format = vg::mapD3D::format[(size_t)cres->getDescription().format];

    m_device->CreateUnorderedAccessView(cres->data, &desc, &crv->view);
    return crv;
}
vorb::graphics::IComputeResourceView* vorb::graphics::D3DContext::makeComputeView(ITexture1D* res) {
    D3DTexture1D* cres = static_cast<D3DTexture1D*>(res);
    D3DComputeResourceView* crv = new D3DComputeResourceView(this);

    D3D11_UNORDERED_ACCESS_VIEW_DESC desc {};
    if (cres->arraySlices > 0) {
        desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
        desc.Texture1DArray.FirstArraySlice = 0;
        desc.Texture1DArray.ArraySize = cres->arraySlices;
    } else {
        desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
    }
    desc.Texture1D.MipSlice = 0;
    desc.Format = vg::mapD3D::format[(size_t)cres->getDescription().format];

    m_device->CreateUnorderedAccessView(cres->data, &desc, &crv->view);
    return crv;
}
vorb::graphics::IComputeResourceView* vorb::graphics::D3DContext::makeComputeView(ITexture2D* res) {
    D3DTexture2D* cres = static_cast<D3DTexture2D*>(res);
    D3DComputeResourceView* crv = new D3DComputeResourceView(this);

    D3D11_UNORDERED_ACCESS_VIEW_DESC desc {};
    if (cres->arraySlices > 0) {
        desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
        desc.Texture2DArray.FirstArraySlice = 0;
        desc.Texture2DArray.ArraySize = cres->arraySlices;
    } else {
        desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    }
    desc.Texture2D.MipSlice = 0;
    desc.Format = vg::mapD3D::format[(size_t)cres->getDescription().format];

    m_device->CreateUnorderedAccessView(cres->data, &desc, &crv->view);
    return crv;
}
vorb::graphics::IComputeResourceView* vorb::graphics::D3DContext::makeComputeView(ITexture3D* res) {
    D3DTexture3D* cres = static_cast<D3DTexture3D*>(res);
    D3DComputeResourceView* crv = new D3DComputeResourceView(this);

    D3D11_UNORDERED_ACCESS_VIEW_DESC desc {};
    desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
    desc.Texture3D.MipSlice = 0;
    desc.Texture3D.FirstWSlice = 0;
    desc.Texture3D.WSize = 0; // TODO(Cristian): Fill in with the correct size
    desc.Format = vg::mapD3D::format[(size_t)cres->getDescription().format];

    m_device->CreateUnorderedAccessView(cres->data, &desc, &crv->view);
    return crv;
}

void vorb::graphics::D3DContext::dispose() {
    m_device->Release();
}

