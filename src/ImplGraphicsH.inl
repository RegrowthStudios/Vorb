#if defined(VORB_IMPL_GRAPHICS_OPENGL)
#include <GL/gl.h>
#elif defined(VORB_IMPL_GRAPHICS_D3D)
// Make sure we get correct DirectX version
#if defined(VORB_DX_11)
#include <d3d11.h>
#define VORB_DX_COM void
#define VORB_DX_DEVICE ID3D10Device
#elif defined(VORB_DX_9)
#include <d3d9.h>
#define VORB_DX_COM IDirect3D9
#define VORB_DX_DEVICE IDirect3DDevice9
#endif

typedef struct {
    VORB_DX_COM* d3d;
    VORB_DX_DEVICE* device;
} D3DContext;
#define VUI_COM(V) ((D3DContext*)V)->d3d
#define VUI_CONTEXT(V) ((D3DContext*)V)->device
#define VG_DX_DEVICE(V) ((VORB_DX_DEVICE*)V)
#endif
