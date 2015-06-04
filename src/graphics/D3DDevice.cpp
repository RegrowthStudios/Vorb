#include "stdafx.h"
#include "D3DDevice.h"

#include <d3d11.h>

void vorb::graphics::D3DDevice::clear(ClearBits bits) {
    if (bits.color) {
        m_context->ClearRenderTargetView(m_target.color, &m_clearValues.color.r);
    }

    ui32 flags = 0;
    if (bits.depth) flags |= D3D11_CLEAR_DEPTH;
    if (bits.stencil) flags |= D3D11_CLEAR_STENCIL;
    m_context->ClearDepthStencilView(m_target.depthStencil, flags, m_clearValues.depth, m_clearValues.stencil);
}
