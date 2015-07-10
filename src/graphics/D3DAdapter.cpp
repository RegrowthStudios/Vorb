#include "stdafx.h"
#include "D3DAdapter.h"

#include <d3d11.h>

#include "D3DContext.h"
#include "D3DDevice.h"

vg::IContext* vg::D3DAdapter::createContext(OUT OPT IDevice** defaultDevice) {
    D3DContext* context = nullptr;// new D3DContext({});

    D3D_FEATURE_LEVEL versions[] = {
        requestedFeatureLevel,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    D3D_FEATURE_LEVEL versionObtained;

    // TODO(Cristian): D3D11_CREATE_DEVICE_SINGLETHREADED
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, versions, 5, D3D11_SDK_VERSION, &context->m_device, &versionObtained, &context->m_immediateContext);
    if (hr != S_OK) {
        printf("Device context creation failed: %d\n", hr);
        delete context;
        return nullptr;
    }
    if (versionObtained != requestedFeatureLevel) {
        puts("D3D version obtained was not the preferred version");
    }

    // Output default rendering device
    //context->m_defaultDevice = new D3DDevice({});
    context->m_defaultDevice->m_device = context->m_device;
    context->m_defaultDevice->m_context = context->m_immediateContext;
    if (defaultDevice) {
        *defaultDevice = context->m_defaultDevice;
    }

    // Context tracks its API
    APIVersion v {};
    v.api = API::DIRECT_3D;
    switch (versionObtained) {
    case D3D_FEATURE_LEVEL_10_0:
        v.version.major = 10;
        v.version.minor = 0;
        break;
    case D3D_FEATURE_LEVEL_10_1:
        v.version.major = 10;
        v.version.minor = 1;
        break;
    case D3D_FEATURE_LEVEL_11_0:
        v.version.major = 11;
        v.version.minor = 0;
        break;
    case D3D_FEATURE_LEVEL_11_1:
        v.version.major = 11;
        v.version.minor = 1;
        break;
    default:
        break;
    }
    context->m_api = v;

    // Query DXGI interfaces
    context->m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&context->m_dxgi.device);
    context->m_dxgi.device->GetParent(__uuidof(IDXGIAdapter), (void**)&context->m_dxgi.adapter);
    context->m_dxgi.adapter->GetParent(__uuidof(IDXGIFactory), (void**)&context->m_dxgi.factory);

    return context;
}

vg::IDevice* vg::D3DAdapter::createDevice(IContext* c) {
    D3DDevice* device = nullptr;// new D3DDevice({});
    D3DContext* context = reinterpret_cast<D3DContext*>(c);

    HRESULT hr = context->m_device->CreateDeferredContext(0, &device->m_context);
    if (hr != S_OK) {
        delete device;
        return nullptr;
    }

    device->m_device = context->m_device;
    return device;
}

void vorb::graphics::D3DAdapter::attachToWindow(IContext* c, void* h) {
    HWND hWnd = (HWND)h;
    D3DContext* context = (D3DContext*)c;

    RECT wRect;
    GetWindowRect(hWnd, &wRect);
    UINT width = wRect.right - wRect.left;
    UINT height = wRect.bottom - wRect.top;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    context->m_dxgi.factory->CreateSwapChain(context->m_device, &sd, &context->m_dxgi.swapChain);

    // Get a pointer to the back buffer and set it
    ID3D11Resource* backTexture;
    context->m_dxgi.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backTexture);
    context->m_device->CreateRenderTargetView(backTexture, NULL, &context->m_defaultDevice->m_target.color);
    context->m_device->CreateDepthStencilView(backTexture, NULL, &context->m_defaultDevice->m_target.depthStencil);
    context->m_immediateContext->OMSetRenderTargets(1, &context->m_defaultDevice->m_target.color, context->m_defaultDevice->m_target.depthStencil);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (f32)width;
    vp.Height = (f32)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    context->m_immediateContext->RSSetViewports(1, &vp);
}
