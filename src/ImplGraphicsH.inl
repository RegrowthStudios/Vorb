#if defined(VORB_IMPL_GRAPHICS_D3D)
// Make sure we get correct DirectX version
#if defined(VORB_DX_11)
#include <d3d11.h>
#define VORB_DX_COM void
#ifndef VORB_DX_DEVICE
#define VORB_DX_DEVICE ID3D11Device
#endif // !VORB_DX_DEVICE

#elif defined(VORB_DX_9)
#include <d3d9.h>
#define VORB_DX_COM IDirect3D9
#ifndef VORB_DX_DEVICE
#define VORB_DX_DEVICE IDirect3DDevice9
#endif // !VORB_DX_DEVICE
#endif

typedef struct {
    VORB_DX_COM* d3d;
    VORB_DX_DEVICE* device;
#if defined(VORB_DX_11)
    IDXGISwapChain* dxgi;
    ID3D11DeviceContext* immediateContext;
    ID3D11Texture2D* backBufferTexture;
    ID3D11RenderTargetView* backBufferRenderTargetView;
#endif
} D3DContext;
#define VUI_COM(V) ((D3DContext*)V)->d3d
#define VUI_CONTEXT(V) ((D3DContext*)V)->device

#ifndef VG_DX_DEVICE
#define VG_DX_DEVICE(V) ((VORB_DX_DEVICE*)V)
#endif // !VG_DX_DEVICE
#endif
