#include "stdafx.h"
#include "D3DContext.h"

#include <d3d11.h>

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
    device->CreateBuffer(desc, nullptr, &buffer->data);

    // Return handle
    return buffer;
}
