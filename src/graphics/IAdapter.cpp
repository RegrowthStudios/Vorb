#include "stdafx.h"
#include "graphics/IAdapter.h"

#include <d3d11.h>
#include <GL/wglext.h>

#include "ui/OSWindow.h"
#include "D3DAdapter.h"

#define COUNT_DX_VERSIONS 4

namespace std {
    template<>
    struct hash<vorb::graphics::APIVersion> : public _Bitwise_hash<vorb::graphics::APIVersion> {

    };
}

int ChoosePixelFormatEx(HDC hdc, int bpp, int depth) {
    int numModes = DescribePixelFormat(hdc, 1, 0, nullptr);
    int mode = 1;
    for (int i = 0; i <= numModes; i++) {
        PIXELFORMATDESCRIPTOR pfd {};
        DescribePixelFormat(hdc, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
        if ((pfd.dwFlags & PFD_SUPPORT_OPENGL)) {
            bool icd = (pfd.dwFlags & (PFD_GENERIC_ACCELERATED | PFD_GENERIC_FORMAT)) == 0;
            if(icd && pfd.cColorBits == bpp && pfd.cDepthBits == depth) mode = i;
        }
    }
    return mode;
}

vorb::graphics::IAdapter::IAdapter(APIVersion version) :
    m_requestedVersion(version) {
    // Empty
}

void vorb::graphics::IAdapter::listAdapterTypes(std::vector<APIVersion>& apis) {
    std::set<APIVersion> versions;

    /************************************************************************/
    /* D3D                                                                  */
    /************************************************************************/
    D3D_FEATURE_LEVEL dxVersions[COUNT_DX_VERSIONS] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    APIVersion dxAPIs[COUNT_DX_VERSIONS] = {
        { API::DIRECT_3D, 11, 1 },
        { API::DIRECT_3D, 11, 0 },
        { API::DIRECT_3D, 10, 1 },
        { API::DIRECT_3D, 10, 0 }
    };
    D3D_FEATURE_LEVEL dxVersionObtained;
    for (size_t i = 0; i < COUNT_DX_VERSIONS; i++) {
        ID3D11Device* device;
        ID3D11DeviceContext* context;
        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &dxVersions[i], 1, D3D11_SDK_VERSION, &device, &dxVersionObtained, &context);
        if (device && dxVersionObtained == dxVersions[i]) {
            versions.insert(dxAPIs[i]);
            context->Release();
            device->Release();
        }
    }

    /************************************************************************/
    /* OpenGL                                                               */
    /************************************************************************/
    // Create a dummy window
    WNDCLASSEX wndClass {};
    wndClass.cbSize             = sizeof(WNDCLASSEX);
    wndClass.style              = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wndClass.lpfnWndProc        = DefWindowProc;
    wndClass.cbClsExtra         = 0;
    wndClass.cbWndExtra         = 0;
    wndClass.hInstance          = 0;
    wndClass.hIcon              = 0;
    wndClass.hCursor            = LoadCursor(0, IDC_ARROW);
    wndClass.hbrBackground      = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName       = 0;
    wndClass.lpszClassName      = "DummyWndClass";
    wndClass.hIconSm            = 0;
    RegisterClassEx(&wndClass);

    HWND wnd = CreateWindowEx(0,
        "DummyWndClass",
        "",
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        0, 0, 0, 0);
    HDC hDC = GetDC(wnd);

    // Setup OpenGL
    PIXELFORMATDESCRIPTOR pfd {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormatEx(hDC, 32, 24);
    SetPixelFormat(hDC, pixelFormat, &pfd);
    auto glContext = wglCreateContext(hDC);
    wglMakeCurrent(hDC, glContext);
    auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (wglCreateContextAttribsARB) {
        for (size_t i = 1; i < 10; i++) {
            for (size_t j = 1; j < 10; j++) {
                GLint attribs[] = 
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, i,
                    WGL_CONTEXT_MINOR_VERSION_ARB, j,
                    // WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                    // WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                    0
                };

                HGLRC glrc = wglCreateContextAttribsARB(hDC, glContext, attribs);
                if (glrc) {
                    APIVersion version = { API::OPENGL, i, j };
                    versions.insert(version);
                    wglDeleteContext(glrc);
                }
            }
        }
    } else {
        const ui8* s = glGetString(GL_VERSION);
        APIVersion version = { API::OPENGL, s[0] - '0', s[2] - '0' };
        versions.insert(version);
    }
    wglDeleteContext(glContext);

    // Destroy the dummy window
    DestroyWindow(wnd);

    // Push all the API versions
    for (auto it : versions) {
        apis.push_back(it);
    }
}

vorb::graphics::IAdapter* vorb::graphics::getAdapter(API api, ui32 majorVersion, ui32 minorVersion) {
    APIVersion version = { api, majorVersion, minorVersion };
    switch (api) {
    case vorb::graphics::API::DIRECT_3D:
        switch (majorVersion) {
        case 10:
            switch (minorVersion) {
            case 0:
                return new D3DAdapter(version, D3D_FEATURE_LEVEL_10_0);
            case 1:
                return new D3DAdapter(version, D3D_FEATURE_LEVEL_10_1);
            default:
                break;
            }
        case 11:
            switch (minorVersion) {
            case 0:
                return new D3DAdapter(version, D3D_FEATURE_LEVEL_11_0);
            case 1:
                return new D3DAdapter(version, D3D_FEATURE_LEVEL_11_1);
            default:
                break;
            }
        }
        break;
    case vorb::graphics::API::OPENGL:
        break;
    }
    return nullptr;
}
