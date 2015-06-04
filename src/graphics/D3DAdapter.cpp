#include "stdafx.h"
#include "D3DAdapter.h"

#include <d3d11.h>

#include "D3DContext.h"
#include "D3DDevice.h"

vg::IContext* vg::D3DAdapter::createContext(OUT OPT IDevice** defaultDevice) {
    D3DContext* context = new D3DContext({});

    D3D_FEATURE_LEVEL versions[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    D3D_FEATURE_LEVEL versionObtained;

    // TODO(Cristian): D3D11_CREATE_DEVICE_SINGLETHREADED
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, versions, 4, D3D11_SDK_VERSION, &context->m_device, &versionObtained, &context->m_immediateContext);
    if (hr != S_OK) {
        printf("Device context creation failed: %d\n", hr);
        delete context;
        return nullptr;
    }
    if (versionObtained != D3D_FEATURE_LEVEL_11_1) {
        puts("D3D version obtained was not the preferred version");
    }

    // Output default rendering device
    context->m_defaultDevice = new D3DDevice({});
    context->m_defaultDevice->m_device = context->m_device;
    context->m_defaultDevice->m_context = context->m_immediateContext;
    if (defaultDevice) {
        *defaultDevice = context->m_defaultDevice;
    }

    // Query DXGI interfaces
    context->m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&context->m_dxgi.device);
    context->m_dxgi.device->GetParent(__uuidof(IDXGIAdapter), (void**)&context->m_dxgi.adapter);
    context->m_dxgi.adapter->GetParent(__uuidof(IDXGIFactory), (void**)&context->m_dxgi.factory);

    return context;
}

vg::IDevice* vg::D3DAdapter::createDevice(IContext* c) {
    D3DDevice* device = new D3DDevice({});
    D3DContext* context = reinterpret_cast<D3DContext*>(c);

    HRESULT hr = context->m_device->CreateDeferredContext(0, &device->m_context);
    if (hr != S_OK) {
        delete device;
        return nullptr;
    }

    device->m_device = context->m_device;
    return device;
}

namespace {
    LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (message == WM_CLOSE) {
            PostQuitMessage(0);
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
}

vg::D3DAdapter::WindowHandle vg::D3DAdapter::createWindow(IContext* c) {
    D3DContext* context = (D3DContext*)c;

    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        0,
        WindowProcedure,
        0L, 0L,
        NULL, NULL, NULL, NULL, NULL,
        "VAMPIRE", NULL
    };
    wc.hInstance = GetModuleHandle(NULL);
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow("VAMPIRE", "Vorb Test DX",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );
    if (!hWnd) {
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError();

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        puts((const cString)lpMsgBuf);

        LocalFree(lpMsgBuf);
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
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
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    context->m_immediateContext->RSSetViewports(1, &vp);

    return hWnd;
}

namespace {
    vg::D3DAdapter adapter = vg::D3DAdapter({});
}
vg::IAdapter* vg::getD3DAdapter() {
    return &adapter;
}
