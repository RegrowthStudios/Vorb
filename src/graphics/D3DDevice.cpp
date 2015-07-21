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
    throw std::logic_error("The method or operation is not implemented.");
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
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::use(IRenderTarget* renderTarget) {
    D3DRenderTarget* rt = static_cast<D3DRenderTarget*>(renderTarget);
    m_context->OMSetRenderTargets(1, &rt->view, nullptr);
}
void vorb::graphics::D3DDevice::setViewports(ui32 count, const Viewport* viewports) {
    throw std::logic_error("The method or operation is not implemented.");
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
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::use(IDepthStencilState* state) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::use(IRasterizerState* state) {
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::vertexUse(IVertexShader* shader) {
    auto* sh = static_cast<D3DVertexShader*>(shader);
    m_context->VSSetShader(sh->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::vertexUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::vertexUse(ui32 slot, ui32 count, ISamplerState** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::vertexUse(ui32 slot, ui32 count, IResourceView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::tessGenUse(ITessGenShader* shader) {
    auto* sh = static_cast<D3DTessGenShader*>(shader);
    m_context->HSSetShader(sh->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::tessGenUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::tessGenUse(ui32 slot, ui32 count, ISamplerState** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::tessGenUse(ui32 slot, ui32 count, IResourceView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::tessEvalUse(ITessEvalShader* shader) {
    auto* sh = static_cast<D3DTessEvalShader*>(shader);
    m_context->DSSetShader(sh->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::tessEvalUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::tessEvalUse(ui32 slot, ui32 count, ISamplerState** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::tessEvalUse(ui32 slot, ui32 count, IResourceView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::geometryUse(ITessEvalShader* shader) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::geometryUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::geometryUse(ui32 slot, ui32 count, ISamplerState** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::geometryUse(ui32 slot, ui32 count, IResourceView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::pixelUse(IPixelShader* shader) {
    auto* sh = static_cast<D3DPixelShader*>(shader);
    m_context->PSSetShader(sh->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::pixelUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::pixelUse(ui32 slot, ui32 count, ISamplerState** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::pixelUse(ui32 slot, ui32 count, IResourceView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::setTopology(vg::PrimitiveType type) {
    m_context->IASetPrimitiveTopology(vg::mapD3D::topology[(size_t)type]);
}
void vorb::graphics::D3DDevice::use(IVertexDeclaration* decl) {
    auto* v = static_cast<D3DVertexDeclaration*>(decl);
    m_context->IASetInputLayout(v->layout);
}
void vorb::graphics::D3DDevice::setVertexBuffers(vg::IBuffer** verts, ui32 startSlot, size_t numBuffers, ui32* offsets, ui32* strides) {
    D3DBuffer** v = (D3DBuffer**)verts;
    ID3D11Buffer** buffers = (ID3D11Buffer**)alloca(numBuffers);
    for (size_t i = 0; i < numBuffers; i++) {
        buffers[i] = v[i]->data;
    }
    m_context->IASetVertexBuffers(startSlot, (UINT)numBuffers, buffers, strides, offsets);
}
void vorb::graphics::D3DDevice::setVertexBuffers(const BufferBindings& bindings) {
    throw std::logic_error("The method or operation is not implemented.");
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
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::drawAutomatic() {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::drawIndexed(size_t indices, size_t indexOff, size_t vertexOff) {
    m_context->DrawIndexed((UINT)indices, (UINT)indexOff, (INT)vertexOff);
}
void vorb::graphics::D3DDevice::drawInstanced(size_t vertexCountPerInstance, size_t instanceCount, size_t vertexOff /*= 0*/, size_t instanceOff /*= 0*/) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::drawIndexedInstanced(size_t indexCountPerInstance, size_t instanceCount, size_t indexOff /*= 0*/, size_t vertexOff /*= 0*/, size_t instanceOff /*= 0*/) {
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::flush() {
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::clear(IComputeResourceView* res, const f32(&data)[4]) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::clear(IComputeResourceView* res, const ui32(&data)[4]) {
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::computeUse(IComputeShader* shader) {
    auto* s = static_cast<D3DComputeShader*>(shader);
    m_context->CSSetShader(s->shader, nullptr, 0);
}
void vorb::graphics::D3DDevice::computeUse(ui32 slot, ui32 count, IComputeResourceView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::computeUse(ui32 slot, ui32 count, IConstantBlockView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::computeUse(ui32 slot, ui32 count, ISamplerState** v) {
    throw std::logic_error("The method or operation is not implemented.");
}
void vorb::graphics::D3DDevice::computeUse(ui32 slot, ui32 count, IResourceView** v) {
    throw std::logic_error("The method or operation is not implemented.");
}

void vorb::graphics::D3DDevice::dispatchThreads(ui32 x, ui32 y, ui32 z) {
    m_context->Dispatch(x, y, z);
}
