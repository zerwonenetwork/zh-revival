// d3d8.h — minimal stub header for Direct3D 8
//
// This project can be built in "stub mode" without the legacy DirectX 8 SDK.
// The real D3D8 headers (d3d8.h / d3dx8.h) are not shipped with modern Windows SDKs.
// To keep compilation working, we provide a tiny subset of types/constants used by
// the WW3D2 renderer. These declarations are NOT a functional D3D8 implementation.
//
// If you have the legacy DX8 SDK, prefer using it instead (see DXSDK_LIB_DIR).
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _HRESULT_DEFINED
typedef long HRESULT;
#define _HRESULT_DEFINED
#endif

#ifndef _ULONG_DEFINED
typedef unsigned long ULONG;
#define _ULONG_DEFINED
#endif

#ifndef _DWORD_DEFINED
typedef unsigned long DWORD;
#define _DWORD_DEFINED
#endif

#ifndef D3D_OK
#define D3D_OK ((HRESULT)0L)
#endif

#ifndef E_FAIL
#define E_FAIL ((HRESULT)0x80004005L)
#endif

// Common D3D8 error codes used for device loss handling
#ifndef D3DERR_DEVICELOST
#define D3DERR_DEVICELOST ((HRESULT)0x88760868L)
#endif
#ifndef D3DERR_DEVICENOTRESET
#define D3DERR_DEVICENOTRESET ((HRESULT)0x88760869L)
#endif

// Forward declarations for COM-style interfaces
typedef struct IDirect3D8 IDirect3D8;
typedef struct IDirect3DDevice8 IDirect3DDevice8;
typedef struct IDirect3DSurface8 IDirect3DSurface8;
typedef struct IDirect3DTexture8 IDirect3DTexture8;
typedef struct IDirect3DBaseTexture8 IDirect3DBaseTexture8;
typedef struct IDirect3DVertexBuffer8 IDirect3DVertexBuffer8;
typedef struct IDirect3DIndexBuffer8 IDirect3DIndexBuffer8;
typedef struct IDirect3DSwapChain8 IDirect3DSwapChain8;

typedef unsigned int D3DFORMAT;
typedef int D3DPOOL;

#define D3DPOOL_DEFAULT 0
#define D3DPOOL_MANAGED 1

typedef struct _D3DMATRIX {
  float m[4][4];
} D3DMATRIX;

typedef struct _D3DDISPLAYMODE {
  unsigned int Width;
  unsigned int Height;
  unsigned int RefreshRate;
  unsigned int Format;
} D3DDISPLAYMODE;

typedef struct _D3DCAPS8 {
  unsigned int DevCaps;
  unsigned int MaxTextureWidth;
  unsigned int MaxTextureHeight;
} D3DCAPS8;

typedef struct _D3DADAPTER_IDENTIFIER8 {
  char Driver[512];
  char Description[512];
  unsigned int VendorId;
  unsigned int DeviceId;
  unsigned int SubSysId;
  unsigned int Revision;
} D3DADAPTER_IDENTIFIER8;

typedef struct _D3DPRESENT_PARAMETERS {
  unsigned int BackBufferWidth;
  unsigned int BackBufferHeight;
  unsigned int BackBufferFormat;
  unsigned int BackBufferCount;
  unsigned int MultiSampleType;
  unsigned int SwapEffect;
  void*        hDeviceWindow;
  int          Windowed;
  int          EnableAutoDepthStencil;
  unsigned int AutoDepthStencilFormat;
  unsigned int Flags;
  unsigned int FullScreen_RefreshRateInHz;
  unsigned int FullScreen_PresentationInterval;
} D3DPRESENT_PARAMETERS;

typedef struct _D3DVIEWPORT8 {
  DWORD X;
  DWORD Y;
  DWORD Width;
  DWORD Height;
  float MinZ;
  float MaxZ;
} D3DVIEWPORT8;

typedef struct _D3DCOLORVALUE {
  float r, g, b, a;
} D3DCOLORVALUE;

typedef struct _D3DMATERIAL8 {
  D3DCOLORVALUE Diffuse;
  D3DCOLORVALUE Ambient;
  D3DCOLORVALUE Specular;
  D3DCOLORVALUE Emissive;
  float Power;
} D3DMATERIAL8;

typedef struct _D3DLIGHT8 {
  DWORD Type;
  D3DCOLORVALUE Diffuse;
  D3DCOLORVALUE Specular;
  D3DCOLORVALUE Ambient;
  float Position[3];
  float Direction[3];
  float Range;
  float Falloff;
  float Attenuation0;
  float Attenuation1;
  float Attenuation2;
  float Theta;
  float Phi;
} D3DLIGHT8;

typedef int D3DTRANSFORMSTATETYPE;
typedef int D3DRENDERSTATETYPE;
typedef int D3DTEXTURESTAGESTATETYPE;

// Common transform state IDs
#define D3DTS_WORLD       256
#define D3DTS_VIEW        2
#define D3DTS_PROJECTION  3

// Minimal enums/constants used by the engine
#define D3DENUM_NO_WHQL_LEVEL 0x00000002
#define D3DCREATE_SOFTWARE_VERTEXPROCESSING 0x00000020
#define D3DCREATE_MIXED_VERTEXPROCESSING    0x00000080
#define D3DCREATE_MULTITHREADED             0x00000004
#define D3DCREATE_FPU_PRESERVE              0x00000002

// Flexible Vertex Format (FVF) bits used by dx8fvf.h
#define D3DFVF_XYZ       0x0002
#define D3DFVF_NORMAL    0x0010
#define D3DFVF_DIFFUSE   0x0040
#define D3DFVF_TEX1      0x0100
#define D3DFVF_TEX2      0x0200
#define D3DFVF_TEX3      0x0300
#define D3DFVF_TEX4      0x0400

// Texcoord size macros (stubs: values only need to compile)
#define D3DFVF_TEXCOORDSIZE1(i) 0
#define D3DFVF_TEXCOORDSIZE2(i) 0
#define D3DFVF_TEXCOORDSIZE3(i) 0
#define D3DFVF_TEXCOORDSIZE4(i) 0

// Device caps / limits referenced by engine code
#define D3DDP_MAXTEXCOORD 8

#define D3DDEVCAPS_HWTRANSFORMANDLIGHT      0x00000080

#define D3DSWAPEFFECT_DISCARD               1
#define D3DPRESENT_INTERVAL_DEFAULT         0
#define D3DPRESENT_RATE_DEFAULT             0

// Formats referenced in code
#define D3DFMT_UNKNOWN 0
#define D3DFMT_R5G6B5   23
#define D3DFMT_X1R5G5B5 24
#define D3DFMT_A1R5G5B5 25
#define D3DFMT_D16      80
#define D3DFMT_D24X8    77
#define D3DFMT_D24S8    75
#define D3DFMT_D32      71

// Device type
#define D3DDEVTYPE_HAL  1

// Adapter
#define D3DADAPTER_DEFAULT 0

// Dummy interfaces (only signatures used by compilation)
#ifdef __cplusplus
struct IDirect3D8 {
  virtual HRESULT GetDeviceCaps(unsigned int, unsigned int, D3DCAPS8*) = 0;
  virtual HRESULT GetAdapterIdentifier(unsigned int, unsigned int, D3DADAPTER_IDENTIFIER8*) = 0;
  virtual unsigned int GetAdapterCount() = 0;
  virtual unsigned int GetAdapterModeCount(unsigned int) = 0;
  virtual HRESULT EnumAdapterModes(unsigned int, unsigned int, D3DDISPLAYMODE*) = 0;
  virtual HRESULT GetAdapterDisplayMode(unsigned int, D3DDISPLAYMODE*) = 0;
  virtual HRESULT CheckDeviceFormat(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int) = 0;
  virtual HRESULT CheckDepthStencilMatch(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int) = 0;
  virtual HRESULT CreateDevice(unsigned int, unsigned int, void*, unsigned long, D3DPRESENT_PARAMETERS*, IDirect3DDevice8**) = 0;
  virtual ULONG Release() = 0;
};

typedef int BOOL;
typedef DWORD D3DCOLOR;

// Minimal render state ids used by wrapper code
#define D3DRS_FOGSTART 0
#define D3DRS_FOGEND   0
#define D3DRS_AMBIENT  0
#define D3DRS_ZBIAS    0
#define D3DRS_SOFTWAREVERTEXPROCESSING 0

// Texture stage state IDs used by shader code
#define D3DTSS_TEXCOORDINDEX 0
#define D3DTSS_TCI_PASSTHRU  0

struct IDirect3DDevice8 {
  virtual HRESULT TestCooperativeLevel() = 0;
  virtual HRESULT Reset(D3DPRESENT_PARAMETERS*) = 0;
  virtual HRESULT Present(const void*, const void*, void*, const void*) = 0;
  virtual HRESULT SetVertexShader(DWORD) = 0;
  virtual HRESULT SetPixelShader(DWORD) = 0;
  virtual HRESULT SetVertexShaderConstant(DWORD, const void*, DWORD) = 0;
  virtual HRESULT SetPixelShaderConstant(DWORD, const void*, DWORD) = 0;
  virtual HRESULT SetTransform(D3DTRANSFORMSTATETYPE, const D3DMATRIX*) = 0;
  virtual HRESULT GetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) = 0;
  virtual HRESULT SetMaterial(const D3DMATERIAL8*) = 0;
  virtual HRESULT SetLight(DWORD, const D3DLIGHT8*) = 0;
  virtual HRESULT LightEnable(DWORD, BOOL) = 0;
  virtual HRESULT SetRenderState(D3DRENDERSTATETYPE, DWORD) = 0;
  virtual HRESULT SetClipPlane(DWORD, const float*) = 0;
  virtual HRESULT SetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD) = 0;
  virtual HRESULT SetTexture(DWORD, IDirect3DBaseTexture8*) = 0;
};
#endif

#ifdef __cplusplus
} // extern "C"
#endif

