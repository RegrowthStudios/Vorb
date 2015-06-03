#include "stdafx.h"
#include "D3DDescCompile.h"

#include "graphics/ResourceDescriptions.h"

void vg::fill(CBufferDescription& v, const BufferDescription& desc) {
    switch (desc.type) {
    case BufferTarget::ELEMENT_ARRAY_BUFFER:
        v.BindFlags = D3D11_BIND_INDEX_BUFFER;
    case BufferTarget::ARRAY_BUFFER:
        v.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    default:
        // TODO(Cristian): Add other buffer types
        break;
    }

    // TODO(Cristian): Change usage member of description
    switch (desc.usage) {
    case BufferUsageHint::DYNAMIC_COPY:
    case BufferUsageHint::DYNAMIC_READ:
    case BufferUsageHint::DYNAMIC_DRAW:
        v.Usage = D3D11_USAGE_DYNAMIC;
        break;
    case BufferUsageHint::STATIC_COPY:
    case BufferUsageHint::STATIC_READ:
    case BufferUsageHint::STATIC_DRAW:
        v.Usage = D3D11_USAGE_IMMUTABLE;
        break;
    case BufferUsageHint::STREAM_COPY:
    case BufferUsageHint::STREAM_READ:
    case BufferUsageHint::STREAM_DRAW:
        v.Usage = D3D11_USAGE_STAGING;
        break;
        // TODO(Cristian): Look for D3D11_USAGE_DEFAULT
    default:
        break;
    }

    switch (desc.usage) {
    case BufferUsageHint::DYNAMIC_COPY:
    case BufferUsageHint::STATIC_COPY:
    case BufferUsageHint::STREAM_COPY:
        v.CPUAccessFlags = 0; // Sucks don't it
        break;
    case BufferUsageHint::DYNAMIC_READ:
    case BufferUsageHint::STATIC_READ:
    case BufferUsageHint::STREAM_READ:
        v.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        break;
    case BufferUsageHint::DYNAMIC_DRAW:
    case BufferUsageHint::STATIC_DRAW:
    case BufferUsageHint::STREAM_DRAW:
        v.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
        // TODO(Cristian): Add RW capability
    default:
        break;
    }

    v.ByteWidth = desc.size; // TODO(Cristian): This should work?
    v.MiscFlags = 0; // TODO(Cristian): Add other options?
}

template<typename T, typename D>
T* genericCompile(const D& desc) {
    T* ptr = new T();
    memset(ptr, 0, sizeof(T));
    fill(*ptr, desc);
    return ptr;
}

vg::CBufferDescription* vg::compile(const BufferDescription& desc) {
    return genericCompile<vg::CBufferDescription>(desc);
}
