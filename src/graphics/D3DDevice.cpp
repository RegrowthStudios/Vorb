#include "stdafx.h"
#include "D3DDevice.h"

#include <d3d11.h>

#include "D3DResource.h"
#include "D3DObject.h"
#include "D3DMap.h"

void vorb::graphics::D3DDevice::dispose() {
    m_context->Release();
}

vorb::graphics::IRenderTarget* vorb::graphics::D3DDevice::create(ITexture2D* res) {
    D3DTexture2D* texture = static_cast<D3DTexture2D*>(res);
    D3DRenderTarget* rt = new D3DRenderTarget(this);

    D3D11_RENDER_TARGET_VIEW_DESC desc;
    if (texture->arraySlices) {
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        desc.Texture2DArray.FirstArraySlice = 0;
        desc.Texture2DArray.ArraySize = texture->arraySlices;
        desc.Texture2DArray.MipSlice = 0;
    } else {
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;
    }

    return rt;
}
vorb::graphics::IVertexStateBind* vorb::graphics::D3DDevice::create(IVertexDeclaration* decl, const BufferBindings& bindings) {
    throw std::logic_error("The method or operation is not implemented.");
}
vorb::graphics::IBlendState* vorb::graphics::D3DDevice::create(const BlendStateDescription& desc) {
    throw std::logic_error("The method or operation is not implemented.");
}
vorb::graphics::IDepthStencilState* vorb::graphics::D3DDevice::create(const DepthStencilStateDescription& desc) {
    throw std::logic_error("The method or operation is not implemented.");
}
vorb::graphics::IRasterizerState* vorb::graphics::D3DDevice::create(const RasterizerStateDescription& desc) {
    throw std::logic_error("The method or operation is not implemented.");
}
vorb::graphics::ISamplerState* vorb::graphics::D3DDevice::create(const SamplerStateDescription& desc) {
    D3DSamplerState* state = new D3DSamplerState(this);

    D3D11_SAMPLER_DESC cdesc {};
    cdesc.AddressU = vg::mapD3D::addressMode[(size_t)desc.addressMode.u];
    cdesc.AddressV = vg::mapD3D::addressMode[(size_t)desc.addressMode.v];
    cdesc.AddressW = vg::mapD3D::addressMode[(size_t)desc.addressMode.w];
    memcpy(cdesc.BorderColor, &desc.borderColor, sizeof(f32v4));
    cdesc.ComparisonFunc = vg::mapD3D::comparisonMode[(size_t)desc.comparison];
    cdesc.MaxAnisotropy = desc.maxAnisotropySamples;
    cdesc.MaxLOD = desc.maxLOD;
    cdesc.MinLOD = desc.minLOD;
    if (desc.maxAnisotropySamples > 0) {
        // We are doing anisotropic filtering
        switch (desc.samplingMode) {
        case TextureSampleMode::NORMAL:
            cdesc.Filter = D3D11_FILTER_ANISOTROPIC;
            break;
        case TextureSampleMode::COMPARISON:
            cdesc.Filter = D3D11_FILTER_COMPARISON_ANISOTROPIC;
            break;
        case TextureSampleMode::MINIMUM:
            cdesc.Filter = D3D11_FILTER_MINIMUM_ANISOTROPIC;
            break;
        case TextureSampleMode::MAXIMUM:
            cdesc.Filter = D3D11_FILTER_MAXIMUM_ANISOTROPIC;
            break;
        default:
            break;
        }
    } else {
        // No anisotropy
        switch (desc.minificationFilter) {
        case TextureFilterMode::POINT:
            switch (desc.magnificationFilter) {
            case TextureFilterMode::POINT:
                switch (desc.samplingMode) {
                case TextureSampleMode::NORMAL:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
                    break;
                case TextureSampleMode::COMPARISON:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
                    break;
                case TextureSampleMode::MINIMUM:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR : D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
                    break;
                case TextureSampleMode::MAXIMUM:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR : D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
                    break;
                default:
                    break;
                }
                break;
            case TextureFilterMode::LINEAR:
                switch (desc.samplingMode) {
                case TextureSampleMode::NORMAL:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR : D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
                    break;
                case TextureSampleMode::COMPARISON:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
                    break;
                case TextureSampleMode::MINIMUM:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR : D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
                    break;
                case TextureSampleMode::MAXIMUM:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR : D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            break;
        case TextureFilterMode::LINEAR:
            switch (desc.magnificationFilter) {
            case TextureFilterMode::POINT:
                switch (desc.samplingMode) {
                case TextureSampleMode::NORMAL:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR : D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
                    break;
                case TextureSampleMode::COMPARISON:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
                    break;
                case TextureSampleMode::MINIMUM:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR : D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
                    break;
                case TextureSampleMode::MAXIMUM:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR : D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
                    break;
                default:
                    break;
                }
                break;
            case TextureFilterMode::LINEAR:
                switch (desc.samplingMode) {
                case TextureSampleMode::NORMAL:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
                    break;
                case TextureSampleMode::COMPARISON:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
                    break;
                case TextureSampleMode::MINIMUM:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
                    break;
                case TextureSampleMode::MAXIMUM:
                    cdesc.Filter = desc.shouldFilterMipmaps ? D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    cdesc.MipLODBias = 0.0f; // TODO(Cristian): Add a parameter to desc?

    m_device->CreateSamplerState(&cdesc, &state->state);
    return state;
}
vorb::graphics::IQuery* vorb::graphics::D3DDevice::create(const QueryDescription& desc) {
    throw std::logic_error("The method or operation is not implemented.");
}
vorb::graphics::IPredicate* vorb::graphics::D3DDevice::create(const PredicateDescription& desc) {
    throw std::logic_error("The method or operation is not implemented.");
}
vorb::graphics::ISyncFence* vorb::graphics::D3DDevice::create(const SyncFenceDescription& desc) {
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::generateMipmaps(IResourceView* v) {
    D3DShaderResourceView* view = (D3DShaderResourceView*)v;
    m_context->GenerateMips(view->view);
}

void vorb::graphics::D3DDevice::use(IRenderTarget* renderTarget) {
    D3DRenderTarget* rt = static_cast<D3DRenderTarget*>(renderTarget);
    m_context->OMSetRenderTargets(1, &rt->view, nullptr);
}
void vorb::graphics::D3DDevice::setViewports(ui32 count, const Viewport* viewports) {
    D3D11_VIEWPORT* ports = (D3D11_VIEWPORT*)alloca(count * sizeof(D3D11_VIEWPORT));
    for (size_t i = 0; i < count; i++) {
        ports[i].TopLeftX = viewports[i].min.x;
        ports[i].TopLeftY = viewports[i].min.y;
        ports[i].Width = viewports[i].size.x;
        ports[i].Height = viewports[i].size.y;
        ports[i].MinDepth = viewports[i].depth.min;
        ports[i].MaxDepth = viewports[i].depth.max;
    }
    m_context->RSSetViewports(count, ports);
}

void vorb::graphics::D3DDevice::clear(ClearBits bits) {
    if (bits.color) {
        m_context->ClearRenderTargetView(m_target.color, &m_clearValues.color.r);
    }

    ui32 flags = 0;
    if (bits.depth) flags |= D3D11_CLEAR_DEPTH;
    if (bits.stencil) flags |= D3D11_CLEAR_STENCIL;
    m_context->ClearDepthStencilView(m_target.depthStencil, flags, m_clearValues.depth, m_clearValues.stencil);
}

void vorb::graphics::D3DDevice::use(IBlendState* state) {
    D3DBlendState* v = (D3DBlendState*)state;
    //f32 factors[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; TODO(Cristian): This is used for special blend states
    m_context->OMSetBlendState(v->state, nullptr, ~0u);
}
void vorb::graphics::D3DDevice::use(IDepthStencilState* state) {
    D3DDepthStencilState* v = (D3DDepthStencilState*)state;
    m_context->OMSetDepthStencilState(v->state, 0); // TODO(Cristian): Add ref stencil somewhere to depth stencil state
}
void vorb::graphics::D3DDevice::use(IRasterizerState* state) {
    D3DRasterizerState* v = (D3DRasterizerState*)state;
    m_context->RSSetState(v->state);
}

#define REDIRECT_ARRAY(TYPEOUT, VOUT, TRUETYPEIN, VIN, N, OP) \
    TRUETYPEIN* tmpTrueArray = (TRUETYPEIN*)VIN; \
    TYPEOUT* VOUT = (TYPEOUT*)alloca(N * sizeof(TYPEOUT)); \
    for (size_t i = 0; i < N; i++) VOUT[i] = (TYPEOUT)(((TRUETYPEIN)(tmpTrueArray[i]))##OP)

void vorb::graphics::D3DDevice::vertexUse(IVertexShader* shader) {
    auto* sh = static_cast<D3DVertexShader*>(shader);
    m_context->VSSetShader(sh->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::vertexUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    REDIRECT_ARRAY(ID3D11Buffer*, d3dViews, D3DConstantBlockView*, v, count, ->buffer);
    m_context->VSSetConstantBuffers(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::vertexUse(ui32 slot, ui32 count, ISamplerState** v) {
    REDIRECT_ARRAY(ID3D11SamplerState*, d3dStates, D3DSamplerState*, v, count, ->state);
    m_context->VSSetSamplers(slot, count, d3dStates);
}
void vorb::graphics::D3DDevice::vertexUse(ui32 slot, ui32 count, IResourceView** v) {
    REDIRECT_ARRAY(ID3D11ShaderResourceView*, d3dViews, D3DShaderResourceView*, v, count, ->view);
    m_context->VSSetShaderResources(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::tessGenUse(ITessGenShader* shader) {
    auto* sh = static_cast<D3DTessGenShader*>(shader);
    m_context->HSSetShader(sh->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::tessGenUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    REDIRECT_ARRAY(ID3D11Buffer*, d3dViews, D3DConstantBlockView*, v, count, ->buffer);
    m_context->HSSetConstantBuffers(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::tessGenUse(ui32 slot, ui32 count, ISamplerState** v) {
    REDIRECT_ARRAY(ID3D11SamplerState*, d3dStates, D3DSamplerState*, v, count, ->state);
    m_context->HSSetSamplers(slot, count, d3dStates);
}
void vorb::graphics::D3DDevice::tessGenUse(ui32 slot, ui32 count, IResourceView** v) {
    REDIRECT_ARRAY(ID3D11ShaderResourceView*, d3dViews, D3DShaderResourceView*, v, count, ->view);
    m_context->HSSetShaderResources(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::tessEvalUse(ITessEvalShader* shader) {
    auto* sh = static_cast<D3DTessEvalShader*>(shader);
    m_context->DSSetShader(sh->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::tessEvalUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    REDIRECT_ARRAY(ID3D11Buffer*, d3dViews, D3DConstantBlockView*, v, count, ->buffer);
    m_context->DSSetConstantBuffers(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::tessEvalUse(ui32 slot, ui32 count, ISamplerState** v) {
    REDIRECT_ARRAY(ID3D11SamplerState*, d3dStates, D3DSamplerState*, v, count, ->state);
    m_context->DSSetSamplers(slot, count, d3dStates);
}
void vorb::graphics::D3DDevice::tessEvalUse(ui32 slot, ui32 count, IResourceView** v) {
    REDIRECT_ARRAY(ID3D11ShaderResourceView*, d3dViews, D3DShaderResourceView*, v, count, ->view);
    m_context->DSSetShaderResources(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::geometryUse(ITessEvalShader* shader) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::geometryUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    REDIRECT_ARRAY(ID3D11Buffer*, d3dViews, D3DConstantBlockView*, v, count, ->buffer);
    m_context->GSSetConstantBuffers(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::geometryUse(ui32 slot, ui32 count, ISamplerState** v) {
    REDIRECT_ARRAY(ID3D11SamplerState*, d3dStates, D3DSamplerState*, v, count, ->state);
    m_context->GSSetSamplers(slot, count, d3dStates);
}
void vorb::graphics::D3DDevice::geometryUse(ui32 slot, ui32 count, IResourceView** v) {
    REDIRECT_ARRAY(ID3D11ShaderResourceView*, d3dViews, D3DShaderResourceView*, v, count, ->view);
    m_context->GSSetShaderResources(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::pixelUse(IPixelShader* shader) {
    auto* sh = static_cast<D3DPixelShader*>(shader);
    m_context->PSSetShader(sh->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::pixelUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    REDIRECT_ARRAY(ID3D11Buffer*, d3dViews, D3DConstantBlockView*, v, count, ->buffer);
    m_context->PSSetConstantBuffers(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::pixelUse(ui32 slot, ui32 count, ISamplerState** v) {
    REDIRECT_ARRAY(ID3D11SamplerState*, d3dStates, D3DSamplerState*, v, count, ->state);
    m_context->PSSetSamplers(slot, count, d3dStates);
}
void vorb::graphics::D3DDevice::pixelUse(ui32 slot, ui32 count, IResourceView** v) {
    REDIRECT_ARRAY(ID3D11ShaderResourceView*, d3dViews, D3DShaderResourceView*, v, count, ->view);
    m_context->PSSetShaderResources(slot, count, d3dViews);
}

void vorb::graphics::D3DDevice::setTopology(vg::PrimitiveType type) {
    m_context->IASetPrimitiveTopology(vg::mapD3D::topology[(size_t)type]);
}
void vorb::graphics::D3DDevice::use(IVertexDeclaration* decl) {
    auto* v = static_cast<D3DVertexDeclaration*>(decl);
    m_context->IASetInputLayout(v->layout);
}
void vorb::graphics::D3DDevice::setVertexBuffers(vg::IBuffer** verts, ui32 startSlot, size_t numBuffers, ui32* offsets, ui32* strides) {
    REDIRECT_ARRAY(ID3D11Buffer*, buffers, D3DBuffer*, verts, numBuffers, ->data);
    m_context->IASetVertexBuffers(startSlot, (UINT)numBuffers, buffers, strides, offsets);
}
void vorb::graphics::D3DDevice::setVertexBuffers(const BufferBindings& bindings) {
    setVertexBuffers(bindings.buffers, bindings.startSlot, bindings.numBuffers, bindings.offsets, bindings.strides);
}
void vorb::graphics::D3DDevice::setIndexBuffer(vg::IBuffer* ind, vg::MemoryFormat format, ui32 offset /*= 0*/) {
    auto* v = static_cast<D3DBuffer*>(ind);
    m_context->IASetIndexBuffer(v->data, vg::mapD3D::format[(size_t)format], offset);
}

void vorb::graphics::D3DDevice::begin(IQuery* query) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::end(IQuery* query) {
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::draw(size_t vertexCount, size_t vertexOff /*= 0*/) {
    m_context->Draw((UINT)vertexCount, (UINT)vertexOff);
}
void vorb::graphics::D3DDevice::drawAutomatic() {
    m_context->DrawAuto();
}
void vorb::graphics::D3DDevice::drawIndexed(size_t indices, size_t indexOff, size_t vertexOff) {
    m_context->DrawIndexed((UINT)indices, (UINT)indexOff, (INT)vertexOff);
}
void vorb::graphics::D3DDevice::drawInstanced(size_t vertexCountPerInstance, size_t instanceCount, size_t vertexOff /*= 0*/, size_t instanceOff /*= 0*/) {
    m_context->DrawInstanced((UINT)vertexCountPerInstance, (UINT)instanceCount, (UINT)vertexOff, (UINT)instanceOff);
}
void vorb::graphics::D3DDevice::drawIndexedInstanced(size_t indexCountPerInstance, size_t instanceCount, size_t indexOff /*= 0*/, size_t vertexOff /*= 0*/, size_t instanceOff /*= 0*/) {
    m_context->DrawIndexedInstanced((UINT)indexCountPerInstance, (UINT)instanceCount, (UINT)indexOff, (INT)vertexOff, (UINT)instanceOff);
}

void vorb::graphics::D3DDevice::flush() {
    m_context->Flush();
}

void vorb::graphics::D3DDevice::clear(IComputeResourceView* res, const f32(&data)[4]) {
    D3DComputeResourceView* v = (D3DComputeResourceView*)res;
    m_context->ClearUnorderedAccessViewFloat(v->view, data);
}
void vorb::graphics::D3DDevice::clear(IComputeResourceView* res, const ui32(&data)[4]) {
    D3DComputeResourceView* v = (D3DComputeResourceView*)res;
    m_context->ClearUnorderedAccessViewUint(v->view, data);
}

void vorb::graphics::D3DDevice::computeUse(IComputeShader* shader) {
    auto* s = static_cast<D3DComputeShader*>(shader);
    m_context->CSSetShader(s->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::computeUse(ui32 slot, ui32 count, IComputeResourceView** v) {
    REDIRECT_ARRAY(ID3D11UnorderedAccessView*, d3dViews, D3DComputeResourceView*, v, count, ->view);
    m_context->CSSetUnorderedAccessViews(slot, count, d3dViews, nullptr);
}
void vorb::graphics::D3DDevice::computeUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    REDIRECT_ARRAY(ID3D11Buffer*, d3dViews, D3DConstantBlockView*, v, count, ->buffer);
    m_context->CSSetConstantBuffers(slot, count, d3dViews);
}
void vorb::graphics::D3DDevice::computeUse(ui32 slot, ui32 count, ISamplerState** v) {
    REDIRECT_ARRAY(ID3D11SamplerState*, d3dStates, D3DSamplerState*, v, count, ->state);
    m_context->CSSetSamplers(slot, count, d3dStates);
}
void vorb::graphics::D3DDevice::computeUse(ui32 slot, ui32 count, IResourceView** v) {
    REDIRECT_ARRAY(ID3D11ShaderResourceView*, d3dViews, D3DShaderResourceView*, v, count, ->view);
    m_context->CSSetShaderResources(slot, count, d3dViews);
}

void vorb::graphics::D3DDevice::dispatchThreads(ui32 x, ui32 y, ui32 z) {
    m_context->Dispatch(x, y, z);
}
