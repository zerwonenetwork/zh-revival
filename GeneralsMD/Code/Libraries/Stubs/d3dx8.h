// d3dx8.h — minimal stub header for D3DX8 helpers
//
// Provides stubs for all D3DX8 functions used by the codebase.
// This is NOT a functional D3DX implementation — all texture loads return failure.
#pragma once

#include "d3d8.h"

// Minimal D3DX buffer typedef used by shader build path.
#ifndef D3DXBUFFER_DEFINED
#define D3DXBUFFER_DEFINED
typedef struct ID3DXBuffer ID3DXBuffer;
typedef ID3DXBuffer* LPD3DXBUFFER;
#endif

// D3DX filter / default constants
#ifndef D3DX_DEFAULT
#define D3DX_DEFAULT        ((UINT)-1)
#endif
#ifndef D3DX_FILTER_NONE
#define D3DX_FILTER_NONE    0x00000001
#define D3DX_FILTER_POINT   0x00000002
#define D3DX_FILTER_LINEAR  0x00000003
#define D3DX_FILTER_TRIANGLE 0x00000004
#define D3DX_FILTER_BOX     0x00000005
#endif

// Minimal D3DXIMAGE_INFO stub
#ifndef D3DXIMAGE_INFO_DEFINED
#define D3DXIMAGE_INFO_DEFINED
struct D3DXIMAGE_INFO {
    UINT Width, Height, Depth, MipLevels;
    D3DFORMAT Format;
};
#endif

// The engine uses D3DXGetErrorStringA for debug output.
inline HRESULT D3DXGetErrorStringA(unsigned int /*res*/, char* buffer, unsigned int bufferLen)
{
    if (buffer && bufferLen) { buffer[0] = '\0'; }
    return D3D_OK;
}

// D3DXCreateTexture — returns failure so caller falls back
inline HRESULT D3DXCreateTexture(
    IDirect3DDevice8* pDevice, UINT Width, UINT Height,
    UINT MipLevels, DWORD Usage, D3DFORMAT Format,
    D3DPOOL Pool, IDirect3DTexture8** ppTexture)
{
    (void)pDevice; (void)Width; (void)Height; (void)MipLevels;
    (void)Usage; (void)Format; (void)Pool;
    if (ppTexture) *ppTexture = nullptr;
    return (HRESULT)0x887A0001L; // D3DERR_NOTAVAILABLE (safe failure)
}

// D3DXCreateCubeTexture — cube texture creation stub
inline HRESULT D3DXCreateCubeTexture(
    IDirect3DDevice8* pDevice, UINT Size, UINT MipLevels,
    DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
    IDirect3DCubeTexture8** ppCubeTexture)
{
    (void)pDevice; (void)Size; (void)MipLevels;
    (void)Usage; (void)Format; (void)Pool;
    if (ppCubeTexture) *ppCubeTexture = nullptr;
    return (HRESULT)0x887A0001L;
}

// D3DXCreateTextureFromFileExA — returns failure so caller uses MissingTexture
inline HRESULT D3DXCreateTextureFromFileExA(
    IDirect3DDevice8* pDevice, const char* pSrcFile,
    UINT Width, UINT Height, UINT MipLevels, DWORD Usage,
    D3DFORMAT Format, D3DPOOL Pool,
    DWORD Filter, DWORD MipFilter, DWORD ColorKey,
    D3DXIMAGE_INFO* pSrcInfo, void* pPalette,
    IDirect3DTexture8** ppTexture)
{
    (void)pDevice; (void)pSrcFile; (void)Width; (void)Height;
    (void)MipLevels; (void)Usage; (void)Format; (void)Pool;
    (void)Filter; (void)MipFilter; (void)ColorKey;
    (void)pSrcInfo; (void)pPalette;
    if (ppTexture) *ppTexture = nullptr;
    return (HRESULT)0x887A0002L; // D3DERR_INVALIDCALL
}
#define D3DXCreateTextureFromFileEx D3DXCreateTextureFromFileExA

// D3DXFilterTexture — generate mip maps, stub no-op returns S_OK
inline HRESULT D3DXFilterTexture(
    IDirect3DBaseTexture8* pBaseTexture,
    const void* pPalette, UINT SrcLevel, DWORD Filter)
{
    (void)pBaseTexture; (void)pPalette; (void)SrcLevel; (void)Filter;
    return D3D_OK;
}

// D3DXLoadSurfaceFromSurface — blit surface to surface, stub no-op
inline HRESULT D3DXLoadSurfaceFromSurface(
    IDirect3DSurface8* pDestSurface, const void* pDestPalette,
    const RECT* pDestRect, IDirect3DSurface8* pSrcSurface,
    const void* pSrcPalette, const RECT* pSrcRect,
    DWORD Filter, DWORD ColorKey)
{
    (void)pDestSurface; (void)pDestPalette; (void)pDestRect;
    (void)pSrcSurface; (void)pSrcPalette; (void)pSrcRect;
    (void)Filter; (void)ColorKey;
    return D3D_OK;
}

