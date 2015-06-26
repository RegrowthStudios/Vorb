#include "stdafx.h"
#include "D3DDevice.h"

#include <d3d11.h>

#include "D3DResource.h"
#include "D3DObject.h"

void vorb::graphics::D3DDevice::clear(ClearBits bits) {
    if (bits.color) {
        m_context->ClearRenderTargetView(m_target.color, &m_clearValues.color.r);
    }

    ui32 flags = 0;
    if (bits.depth) flags |= D3D11_CLEAR_DEPTH;
    if (bits.stencil) flags |= D3D11_CLEAR_STENCIL;
    m_context->ClearDepthStencilView(m_target.depthStencil, flags, m_clearValues.depth, m_clearValues.stencil);
}

void vorb::graphics::D3DDevice::use(IRenderTarget* renderTarget) {
    D3DRenderTarget* rt = static_cast<D3DRenderTarget*>(renderTarget);
    m_context->OMSetRenderTargets(1, &rt->view, nullptr);
}

void vorb::graphics::D3DDevice::computeUse(IComputeShader* shader) {
    auto* s = static_cast<D3DComputeShader*>(shader);
    m_context->CSSetShader(s->shader, nullptr, 0);
}

void vorb::graphics::D3DDevice::computeUse(ui32 slot, IComputeResourceView* v) {

}

void vorb::graphics::D3DDevice::dispatchThreads(ui32 x, ui32 y, ui32 z) {
    m_context->Dispatch(x, y, z);
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
