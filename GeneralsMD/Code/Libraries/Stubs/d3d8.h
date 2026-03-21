// d3d8.h — minimal stub header for Direct3D 8
//
// This project can be built in "stub mode" without the legacy DirectX 8 SDK.
// The real D3D8 headers (d3d8.h / d3dx8.h) are not shipped with modern Windows SDKs.
// To keep compilation working, we provide a tiny subset of types/constants used by
// the WW3D2 renderer. These declarations are NOT a functional D3D8 implementation.
//
// If you have the legacy DX8 SDK, prefer using it instead (see DXSDK_LIB_DIR).
#pragma once

// Pull in Windows types first so we don't redefine HRESULT, DWORD, UINT, RECT etc.
#ifndef _WINDOWS_
#include <windows.h>
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
#ifndef D3DERR_CONFLICTINGTEXTUREFILTER
#define D3DERR_CONFLICTINGTEXTUREFILTER ((HRESULT)0x8876081EL)
#endif
#ifndef D3DERR_CONFLICTINGTEXTUREPALETTE
#define D3DERR_CONFLICTINGTEXTUREPALETTE ((HRESULT)0x8876081FL)
#endif
#ifndef D3DERR_TOOMANYOPERATIONS
#define D3DERR_TOOMANYOPERATIONS ((HRESULT)0x88760882L)
#endif
#ifndef D3DERR_UNSUPPORTEDALPHAARG
#define D3DERR_UNSUPPORTEDALPHAARG ((HRESULT)0x8876086BL)
#endif
#ifndef D3DERR_UNSUPPORTEDALPHAOPERATION
#define D3DERR_UNSUPPORTEDALPHAOPERATION ((HRESULT)0x8876086CL)
#endif
#ifndef D3DERR_UNSUPPORTEDCOLORARG
#define D3DERR_UNSUPPORTEDCOLORARG ((HRESULT)0x8876086DL)
#endif
#ifndef D3DERR_UNSUPPORTEDCOLOROPERATION
#define D3DERR_UNSUPPORTEDCOLOROPERATION ((HRESULT)0x8876086EL)
#endif
#ifndef D3DERR_UNSUPPORTEDFACTORVALUE
#define D3DERR_UNSUPPORTEDFACTORVALUE ((HRESULT)0x88760874L)
#endif
#ifndef D3DERR_UNSUPPORTEDTEXTUREFILTER
#define D3DERR_UNSUPPORTEDTEXTUREFILTER ((HRESULT)0x88760870L)
#endif
#ifndef D3DERR_WRONGTEXTUREFORMAT
#define D3DERR_WRONGTEXTUREFORMAT ((HRESULT)0x88760872L)
#endif
#ifndef D3DERR_NOTAVAILABLE
#define D3DERR_NOTAVAILABLE ((HRESULT)0x8876086AL)
#endif
#ifndef D3DERR_OUTOFVIDEOMEMORY
#define D3DERR_OUTOFVIDEOMEMORY ((HRESULT)0x8876017CL)
#endif

typedef unsigned int D3DFORMAT;
typedef int D3DPOOL;

#define D3DPOOL_DEFAULT  0
#define D3DPOOL_MANAGED  1
#define D3DPOOL_SYSTEMMEM 2

typedef struct _D3DMATRIX {
  float m[4][4];
} D3DMATRIX;

typedef struct _D3DDISPLAYMODE {
  UINT Width;
  UINT Height;
  UINT RefreshRate;
  UINT Format;
} D3DDISPLAYMODE;

typedef struct _D3DCAPS8 {
  DWORD DevCaps;
  DWORD Caps2;
  DWORD RasterCaps;
  DWORD PrimitiveMiscCaps;
  DWORD TextureCaps;
  DWORD TextureOpCaps;
  DWORD MaxTextureWidth;
  DWORD MaxTextureHeight;
  DWORD MaxTextureBlendStages;
  DWORD MaxSimultaneousTextures;
  DWORD MaxActiveLights;
  DWORD MaxUserClipPlanes;
  DWORD MaxVertexBlendMatrices;
  DWORD MaxVertexBlendMatrixIndex;
  DWORD MaxPrimitiveCount;
  DWORD MaxVertexIndex;
  DWORD MaxStreams;
  DWORD MaxStreamStride;
  DWORD VertexShaderVersion;
  DWORD MaxVertexShaderConst;
  DWORD PixelShaderVersion;
  float MaxPixelShaderValue;
  float MaxPointSize;
  // Texture filter capabilities — used by TextureFilterClass
  DWORD TextureFilterCaps;
  DWORD MaxAnisotropy;
  // Volume texture capability
  DWORD MaxVolumeExtent;
  // Adapter/device info embedded in caps (used by CheckDeviceFormat)
  UINT  AdapterOrdinal;
  UINT  DeviceType;
} D3DCAPS8;

typedef struct _D3DADAPTER_IDENTIFIER8 {
  char Driver[512];
  char Description[512];
  LARGE_INTEGER DriverVersion;  // .HighPart / .LowPart used by dx8caps.cpp
  DWORD VendorId;
  DWORD DeviceId;
  DWORD SubSysId;
  DWORD Revision;
  GUID  DeviceIdentifier;       // .Data1/.Data2/.Data3/.Data4[] used by dx8caps.cpp
} D3DADAPTER_IDENTIFIER8;

typedef struct _D3DPRESENT_PARAMETERS {
  UINT  BackBufferWidth;
  UINT  BackBufferHeight;
  UINT  BackBufferFormat;
  UINT  BackBufferCount;
  UINT  MultiSampleType;
  UINT  SwapEffect;
  HWND  hDeviceWindow;
  BOOL  Windowed;
  BOOL  EnableAutoDepthStencil;
  UINT  AutoDepthStencilFormat;
  DWORD Flags;
  UINT  FullScreen_RefreshRateInHz;
  UINT  FullScreen_PresentationInterval;
} D3DPRESENT_PARAMETERS;

typedef struct _D3DSURFACE_DESC {
  D3DFORMAT Format;
  DWORD     Type;
  DWORD     Usage;
  D3DPOOL   Pool;
  UINT      Size;
  UINT      MultiSampleType;
  UINT      Width;
  UINT      Height;
} D3DSURFACE_DESC;

typedef struct _D3DLOCKED_RECT {
  INT   Pitch;
  void* pBits;
} D3DLOCKED_RECT;

typedef struct _D3DLOCKED_BOX {
  INT   RowPitch;
  INT   SlicePitch;
  void* pBits;
} D3DLOCKED_BOX;

typedef struct _D3DGAMMARAMP {
  WORD red[256];
  WORD green[256];
  WORD blue[256];
} D3DGAMMARAMP;

typedef struct _D3DVOLUME_DESC {
  D3DFORMAT Format;
  DWORD     Type;
  DWORD     Usage;
  D3DPOOL   Pool;
  UINT      Width;
  UINT      Height;
  UINT      Depth;
} D3DVOLUME_DESC;

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

typedef struct _D3DVECTOR {
  float x, y, z;
} D3DVECTOR;

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
  D3DVECTOR Position;
  D3DVECTOR Direction;
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
typedef int D3DTEXTUREOP;    // enum values: D3DTOP_*
typedef int D3DBLEND;        // enum values: D3DBLEND_*
typedef int D3DCUBEMAP_FACE; // enum values: D3DCUBEMAP_FACE_*
typedef D3DCUBEMAP_FACE D3DCUBEMAP_FACES;
typedef int D3DBACKBUFFER_TYPE;
typedef int D3DCMPFUNC;
typedef int D3DPRIMITIVETYPE;
typedef DWORD D3DCOLOR;

// Common transform state IDs
#define D3DTS_WORLD       256
#define D3DTS_VIEW        2
#define D3DTS_PROJECTION  3

// Minimal enums/constants used by the engine
#define D3DENUM_NO_WHQL_LEVEL               0x00000002
#define D3DCREATE_SOFTWARE_VERTEXPROCESSING 0x00000020
#define D3DCREATE_MIXED_VERTEXPROCESSING    0x00000080
#define D3DCREATE_MULTITHREADED             0x00000004
#define D3DCREATE_FPU_PRESERVE              0x00000002
#define D3DCREATE_HARDWARE_VERTEXPROCESSING 0x00000040

// SDK version sentinel (value from DX8 SDK)
#define D3D_SDK_VERSION 220

// Flexible Vertex Format (FVF) bits used by dx8fvf.h
#define D3DFVF_XYZ            0x0002
#define D3DFVF_XYZRHW         0x0004
#define D3DFVF_XYZB4         0x000E
#define D3DFVF_NORMAL        0x0010
#define D3DFVF_DIFFUSE       0x0040
#define D3DFVF_SPECULAR      0x0080
#define D3DFVF_TEX1          0x0100
#define D3DFVF_TEX2          0x0200
#define D3DFVF_TEX3          0x0300
#define D3DFVF_TEX4          0x0400
#define D3DFVF_TEX5          0x0500
#define D3DFVF_TEX6          0x0600
#define D3DFVF_TEX7          0x0700
#define D3DFVF_TEX8          0x0800
#define D3DFVF_LASTBETA_UBYTE4 0x1000

// Texcoord size macros (stubs: values only need to compile)
#define D3DFVF_TEXCOORDSIZE1(i) 0
#define D3DFVF_TEXCOORDSIZE2(i) 0
#define D3DFVF_TEXCOORDSIZE3(i) 0
#define D3DFVF_TEXCOORDSIZE4(i) 0

// Device caps / limits referenced by engine code
#define D3DDP_MAXTEXCOORD 8

#define D3DDEVCAPS_HWTRANSFORMANDLIGHT      0x00000080
#define D3DDEVCAPS_NPATCHES                 0x00020000

// Raster caps (D3DCAPS8.RasterCaps)
#define D3DPRASTERCAPS_ZBIAS                0x00004000
#define D3DPRASTERCAPS_FOGRANGE             0x00010000

// Primitive misc caps (D3DCAPS8.PrimitiveMiscCaps)
#define D3DPMISCCAPS_COLORWRITEENABLE       0x00000080

// Caps2 flags (D3DCAPS8.Caps2)
#define D3DCAPS2_FULLSCREENGAMMA            0x00020000

// Resource types (D3DRESOURCETYPE) used by IDirect3D8::CheckDeviceFormat
#define D3DRTYPE_SURFACE                    1
#define D3DRTYPE_TEXTURE                    3

// Additional usage flags
#define D3DUSAGE_RENDERTARGET               0x00000001
#define D3DUSAGE_DEPTHSTENCIL               0x00000002
#define D3DUSAGE_POINTS                     0x00000040

// Texture operation capability flags (D3DCAPS8.TextureOpCaps)
#define D3DTEXOPCAPS_SELECTARG1             0x00000002
#define D3DTEXOPCAPS_SELECTARG2             0x00000004
#define D3DTEXOPCAPS_MODULATE               0x00000008
#define D3DTEXOPCAPS_MODULATE2X             0x00000010
#define D3DTEXOPCAPS_MODULATE4X             0x00000020
#define D3DTEXOPCAPS_ADD                    0x00000040
#define D3DTEXOPCAPS_ADDSIGNED              0x00000080
#define D3DTEXOPCAPS_ADDSIGNED2X            0x00000100
#define D3DTEXOPCAPS_SUBTRACT               0x00000200
#define D3DTEXOPCAPS_ADDSMOOTH              0x00000200
#define D3DTEXOPCAPS_BLENDDIFFUSEALPHA      0x00000400
#define D3DTEXOPCAPS_BLENDTEXTUREALPHA      0x00000800
#define D3DTEXOPCAPS_BLENDFACTORALPHA       0x00001000
#define D3DTEXOPCAPS_BLENDTEXTUREALPHAPM    0x00002000
#define D3DTEXOPCAPS_BLENDCURRENTALPHA      0x00002000
#define D3DTEXOPCAPS_PREMODULATE            0x00004000
#define D3DTEXOPCAPS_MODULATEALPHA_ADDCOLOR 0x00040000
#define D3DTEXOPCAPS_DOTPRODUCT3            0x00200000
#define D3DTEXOPCAPS_BUMPENVMAP             0x00800000
#define D3DTEXOPCAPS_BUMPENVMAPLUMINANCE    0x01000000

// Texture caps (D3DCAPS8.TextureCaps)
#define D3DPTEXTURECAPS_CUBEMAP             0x00000800

#define D3DSWAPEFFECT_DISCARD               1
#define D3DSWAPEFFECT_COPY_VSYNC            3
#define D3DMULTISAMPLE_NONE                 0
#define D3DPRESENT_INTERVAL_DEFAULT         0
#define D3DPRESENT_INTERVAL_IMMEDIATE       0x80000000u
#define D3DPRESENT_INTERVAL_ONE             0x00000001u
#define D3DPRESENT_INTERVAL_TWO             0x00000002u
#define D3DPRESENT_INTERVAL_THREE           0x00000004u
#define D3DPRESENT_RATE_DEFAULT             0

#define D3DSGR_NO_CALIBRATION               0
#define D3DSGR_CALIBRATE                    1

#define D3DSTENCILOP_KEEP                   1
#define D3DSTENCILOP_ZERO                   2
#define D3DSTENCILOP_REPLACE                3
#define D3DSTENCILOP_INCRSAT                4
#define D3DSTENCILOP_DECRSAT                5
#define D3DSTENCILOP_INVERT                 6
#define D3DSTENCILOP_INCR                   7
#define D3DSTENCILOP_DECR                   8

#define D3DRS_LINEPATTERN                   10
#define D3DRS_LASTPIXEL                     16
#define D3DRS_DITHERENABLE                  26
#define D3DRS_ZVISIBLE                      30
#define D3DRS_EDGEANTIALIAS                 40
#define D3DRS_WRAP0                         128
#define D3DRS_WRAP1                         129
#define D3DRS_WRAP2                         130
#define D3DRS_WRAP3                         131
#define D3DRS_WRAP4                         132
#define D3DRS_WRAP5                         133
#define D3DRS_WRAP6                         134
#define D3DRS_WRAP7                         135
#define D3DRS_LOCALVIEWER                   142
#define D3DRS_CLIPPLANEENABLE               152

// Formats referenced in code
#define D3DFMT_UNKNOWN  0
#define D3DFMT_R5G6B5   23
#define D3DFMT_X1R5G5B5 24
#define D3DFMT_A1R5G5B5 25
#define D3DFMT_D16      80
#define D3DFMT_D24X8    77
#define D3DFMT_D24S8    75
#define D3DFMT_D32      71
#define D3DFMT_A8R8G8B8 21
#define D3DFMT_X8R8G8B8 22
#define D3DFMT_R8G8B8   20
#define D3DFMT_A4R4G4B4 26
#define D3DFMT_P8       41
#define D3DFMT_L8       50
#define D3DFMT_V8U8     60
#define D3DFMT_L6V5U5   61
#define D3DFMT_X8L8V8U8 62
#define D3DFMT_DXT1     0x31545844
#define D3DFMT_DXT2     0x32545844
#define D3DFMT_DXT3     0x33545844
#define D3DFMT_DXT4     0x34545844
#define D3DFMT_DXT5     0x35545844
// Additional format enums used by assetmgr.cpp / ww3d.cpp
#define D3DFMT_A8         28
#define D3DFMT_R3G3B2     27
#define D3DFMT_A8R3G3B2   29
#define D3DFMT_X4R4G4B4   30
#define D3DFMT_A8P8       40
#define D3DFMT_A8L8       51
#define D3DFMT_A4L4       52
#define D3DFMT_Q8W8V8U8   63
#define D3DFMT_V16U16     64
#define D3DFMT_W11V11U10  65
#define D3DFMT_UYVY       0x59565955
#define D3DFMT_YUY2       0x32595559
#define D3DFMT_D16_LOCKABLE 70
#define D3DFMT_D15S1        73
#define D3DFMT_D24X4S4      79

// Device type
#define D3DDEVTYPE_HAL  1
#define D3DDEVTYPE_REF  2

// Adapter
#define D3DADAPTER_DEFAULT 0

// Render states
#define D3DRS_ZENABLE                   7
#define D3DRS_FILLMODE                  8
#define D3DRS_SHADEMODE                 9
#define D3DRS_ZWRITEENABLE              14
#define D3DRS_ALPHATESTENABLE           15
#define D3DRS_SRCBLEND                  19
#define D3DRS_DESTBLEND                 20
#define D3DRS_CULLMODE                  22
#define D3DRS_ZFUNC                     23
#define D3DRS_ALPHAREF                  24
#define D3DRS_ALPHAFUNC                 25
#define D3DRS_ALPHABLENDENABLE          27
#define D3DRS_FOGENABLE                 28
#define D3DRS_SPECULARENABLE            29
#define D3DRS_FOGCOLOR                  34
#define D3DRS_FOGTABLEMODE              35
#define D3DRS_FOGSTART                  36
#define D3DRS_FOGEND                    37
#define D3DRS_FOGDENSITY                38
#define D3DRS_RANGEFOGENABLE            48
#define D3DRS_STENCILENABLE             52
#define D3DRS_STENCILFAIL               53
#define D3DRS_STENCILZFAIL              54
#define D3DRS_STENCILPASS               55
#define D3DRS_STENCILFUNC               56
#define D3DRS_STENCILREF                57
#define D3DRS_STENCILMASK               58
#define D3DRS_STENCILWRITEMASK          59
#define D3DRS_CLIPPING                  136
#define D3DRS_LIGHTING                  137
#define D3DRS_AMBIENT                   139
#define D3DRS_DIFFUSEMATERIALSOURCE     145
#define D3DRS_SPECULARMATERIALSOURCE    146
#define D3DRS_AMBIENTMATERIALSOURCE     147
#define D3DRS_EMISSIVEMATERIALSOURCE    148
#define D3DRS_VERTEXBLEND               151
#define D3DRS_SOFTWAREVERTEXPROCESSING  153
#define D3DRS_POINTSIZE                 154
#define D3DRS_POINTSIZE_MIN             155
#define D3DRS_POINTSPRITEENABLE         156
#define D3DRS_POINTSCALEENABLE          157
#define D3DRS_POINTSCALE_A              158
#define D3DRS_POINTSCALE_B              159
#define D3DRS_POINTSCALE_C              160
#define D3DRS_MULTISAMPLEANTIALIAS      161
#define D3DRS_MULTISAMPLEMASK           162
#define D3DRS_PATCHEDGESTYLE            163
#define D3DRS_PATCHSEGMENTS             164
#define D3DRS_DEBUGMONITORTOKEN         165
#define D3DRS_POINTSIZE_MAX             166
#define D3DRS_INDEXEDVERTEXBLENDENABLE  167
#define D3DRS_COLORWRITEENABLE          168
#define D3DRS_TWEENFACTOR               170
#define D3DRS_BLENDOP                   171
#define D3DRS_ZBIAS                     47
#define D3DRS_TEXTUREFACTOR             60
#define D3DRS_NORMALIZENORMALS          143
#define D3DRS_FOGVERTEXMODE             140
#define D3DRS_COLORVERTEX               141
// Material color sources
#define D3DMCS_MATERIAL  0
#define D3DMCS_COLOR1    1
#define D3DMCS_COLOR2    2

// Texture stage states
#define D3DTSS_COLOROP              1
#define D3DTSS_COLORARG1            2
#define D3DTSS_COLORARG2            3
#define D3DTSS_ALPHAOP              4
#define D3DTSS_ALPHAARG1            5
#define D3DTSS_ALPHAARG2            6
#define D3DTSS_BUMPENVMAT00         7
#define D3DTSS_BUMPENVMAT01         8
#define D3DTSS_BUMPENVMAT10         9
#define D3DTSS_BUMPENVMAT11         10
#define D3DTSS_TEXCOORDINDEX        11
#define D3DTSS_ADDRESSU             13
#define D3DTSS_ADDRESSV             14
#define D3DTSS_BORDERCOLOR          15
#define D3DTSS_MAGFILTER            16
#define D3DTSS_MINFILTER            17
#define D3DTSS_MIPFILTER            18
#define D3DTSS_MIPMAPLODBIAS        19
#define D3DTSS_MAXMIPLEVEL          20
#define D3DTSS_MAXANISOTROPY        21
#define D3DTSS_BUMPENVLSCALE        22
#define D3DTSS_BUMPENVLOFFSET       23
#define D3DTSS_TEXTURETRANSFORMFLAGS 24
#define D3DTSS_ADDRESSW             25
#define D3DTSS_TCI_PASSTHRU                     0
#define D3DTSS_TCI_CAMERASPACENORMAL            0x00010000
#define D3DTSS_TCI_CAMERASPACEPOSITION          0x00020000
#define D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR  0x00030000
// COLORARG0 is a third argument for the LERP texture op
#define D3DTSS_COLORARG0            26
#define D3DTSS_ALPHAARG0            27
#define D3DTSS_RESULTARG            28

#define D3DVBF_DISABLE              0
#define D3DVBF_1WEIGHTS             1
#define D3DVBF_2WEIGHTS             2
#define D3DVBF_3WEIGHTS             3
#define D3DVBF_TWEENING             255
#define D3DVBF_0WEIGHTS             256

#define D3DPATCHEDGE_DISCRETE       0
#define D3DPATCHEDGE_CONTINUOUS     1

#define D3DDMT_ENABLE               0
#define D3DDMT_DISABLE              1

// D3DTS_TEXTURE0..7 — texture transform state indices
#define D3DTS_TEXTURE0   16
#define D3DTS_TEXTURE1   17
#define D3DTS_TEXTURE2   18
#define D3DTS_TEXTURE3   19
#define D3DTS_TEXTURE4   20
#define D3DTS_TEXTURE5   21
#define D3DTS_TEXTURE6   22
#define D3DTS_TEXTURE7   23

// Texture address modes
#define D3DTADDRESS_WRAP           1
#define D3DTADDRESS_MIRROR         2
#define D3DTADDRESS_CLAMP          3
#define D3DTADDRESS_BORDER         4
#define D3DTADDRESS_MIRRORONCE     5

// Texture filter types (D3DTEXTUREFILTERTYPE)
#define D3DTEXF_NONE               0
#define D3DTEXF_POINT              1
#define D3DTEXF_LINEAR             2
#define D3DTEXF_ANISOTROPIC        3
#define D3DTEXF_FLATCUBIC          4
#define D3DTEXF_GAUSSIANCUBIC      5

// Texture filter capability flags (D3DCAPS8.TextureFilterCaps)
#define D3DPTFILTERCAPS_MINFPOINT           0x00000100
#define D3DPTFILTERCAPS_MINFLINEAR          0x00000200
#define D3DPTFILTERCAPS_MINFANISOTROPIC     0x00000400
#define D3DPTFILTERCAPS_MIPFPOINT           0x00010000
#define D3DPTFILTERCAPS_MIPFLINEAR          0x00020000
#define D3DPTFILTERCAPS_MAGFPOINT           0x01000000
#define D3DPTFILTERCAPS_MAGFLINEAR          0x02000000
#define D3DPTFILTERCAPS_MAGFANISOTROPIC     0x04000000

// Texture transform flags
#define D3DTTFF_DISABLE             0
#define D3DTTFF_COUNT1              1
#define D3DTTFF_COUNT2              2
#define D3DTTFF_COUNT3              3
#define D3DTTFF_COUNT4              4
#define D3DTTFF_PROJECTED           256

// Texture ops
#define D3DTOP_DISABLE              1
#define D3DTOP_SELECTARG1           2
#define D3DTOP_SELECTARG2           3
#define D3DTOP_MODULATE             4
#define D3DTOP_MODULATE2X           5
#define D3DTOP_MODULATE4X           6
#define D3DTOP_ADD                  7
#define D3DTOP_ADDSIGNED            8
#define D3DTOP_ADDSIGNED2X          9
#define D3DTOP_SUBTRACT             10
#define D3DTOP_ADDSMOOTH            11
#define D3DTOP_BLENDDIFFUSEALPHA    12
#define D3DTOP_BLENDTEXTUREALPHA    13
#define D3DTOP_BLENDFACTORALPHA     14
#define D3DTOP_BLENDTEXTUREALPHAPM  15
#define D3DTOP_BLENDCURRENTALPHA    16
#define D3DTOP_PREMODULATE          17
#define D3DTOP_MODULATEALPHA_ADDCOLOR 18
#define D3DTOP_MODULATECOLOR_ADDALPHA 19
#define D3DTOP_MODULATEINVALPHA_ADDCOLOR 20
#define D3DTOP_MODULATEINVCOLOR_ADDALPHA 21
#define D3DTOP_BUMPENVMAP           22
#define D3DTOP_BUMPENVMAPLUMINANCE  23
#define D3DTOP_DOTPRODUCT3          24
#define D3DTOP_MULTIPLYADD          25
#define D3DTOP_LERP                 26

// Texture args
#define D3DTA_DIFFUSE               0
#define D3DTA_CURRENT               1
#define D3DTA_TEXTURE               2
#define D3DTA_TFACTOR               3
#define D3DTA_SPECULAR              4
#define D3DTA_TEMP                  5
#define D3DTA_SELECTMASK            0x0000000f
#define D3DTA_COMPLEMENT            16
#define D3DTA_ALPHAREPLICATE        32

#define D3DWRAP_U                   0x00000001
#define D3DWRAP_V                   0x00000002
#define D3DWRAP_W                   0x00000004

#define D3DZB_FALSE                 0
#define D3DZB_TRUE                  1
#define D3DZB_USEW                  2

// Blend modes
#define D3DBLEND_ZERO               1
#define D3DBLEND_ONE                2
#define D3DBLEND_SRCCOLOR           3
#define D3DBLEND_INVSRCCOLOR        4
#define D3DBLEND_SRCALPHA           5
#define D3DBLEND_INVSRCALPHA        6
#define D3DBLEND_DESTALPHA          7
#define D3DBLEND_INVDESTALPHA       8
#define D3DBLEND_DESTCOLOR          9
#define D3DBLEND_INVDESTCOLOR       10
#define D3DBLEND_SRCALPHASAT        11
#define D3DBLEND_BOTHSRCALPHA       12
#define D3DBLEND_BOTHINVSRCALPHA    13

// Compare functions
#define D3DCMP_NEVER                1
#define D3DCMP_LESS                 2
#define D3DCMP_EQUAL                3
#define D3DCMP_LESSEQUAL            4
#define D3DCMP_GREATER              5
#define D3DCMP_NOTEQUAL             6
#define D3DCMP_GREATEREQUAL         7
#define D3DCMP_ALWAYS               8

// Cull modes
#define D3DCULL_NONE                1
#define D3DCULL_CW                  2
#define D3DCULL_CCW                 3

// Primitive types
#define D3DPT_POINTLIST             1
#define D3DPT_LINELIST              2
#define D3DPT_LINESTRIP             3
#define D3DPT_TRIANGLELIST          4
#define D3DPT_TRIANGLESTRIP         5
#define D3DPT_TRIANGLEFAN           6
#define D3DLIGHT_POINT              1
#define D3DLIGHT_SPOT               2
#define D3DLIGHT_DIRECTIONAL        3

// Fill modes
#define D3DFILL_POINT               1
#define D3DFILL_WIREFRAME           2
#define D3DFILL_SOLID               3

// Shade modes
#define D3DSHADE_FLAT               1
#define D3DSHADE_GOURAUD            2
#define D3DSHADE_PHONG              3

// Clear flags
#define D3DCLEAR_TARGET             0x00000001L
#define D3DCLEAR_ZBUFFER            0x00000002L
#define D3DCLEAR_STENCIL            0x00000004L

// Fog modes
#define D3DFOG_NONE                 0
#define D3DFOG_EXP                  1
#define D3DFOG_EXP2                 2
#define D3DFOG_LINEAR               3

// Primitive types
#define D3DPT_POINTLIST             1
#define D3DPT_LINELIST              2
#define D3DPT_LINESTRIP             3
#define D3DPT_TRIANGLELIST          4
#define D3DPT_TRIANGLESTRIP         5
#define D3DPT_TRIANGLEFAN           6

// Lock flags
#define D3DLOCK_READONLY            0x00000010
#define D3DLOCK_NOSYSLOCK           0x00000800
#define D3DLOCK_NOOVERWRITE         0x00001000
#define D3DLOCK_DISCARD             0x00002000
#define D3DLOCK_NO_DIRTY_UPDATE     0x00008000

// Usage flags
#define D3DUSAGE_WRITEONLY          0x00000008
#define D3DUSAGE_DYNAMIC            0x00000200
#define D3DUSAGE_NPATCHES           0x00000100
#define D3DUSAGE_SOFTWAREPROCESSING 0x00000010

// Index buffer format used with D3DFMT_INDEX16
#define D3DFMT_INDEX16 101
#define D3DFMT_INDEX32 102

// Cubemap face count
#define D3DCUBEMAP_FACE_COUNT 6

// Cubemap face enum values (used with IDirect3DCubeTexture8)
#define D3DCUBEMAP_FACE_POSITIVE_X 0
#define D3DCUBEMAP_FACE_NEGATIVE_X 1
#define D3DCUBEMAP_FACE_POSITIVE_Y 2
#define D3DCUBEMAP_FACE_NEGATIVE_Y 3
#define D3DCUBEMAP_FACE_POSITIVE_Z 4
#define D3DCUBEMAP_FACE_NEGATIVE_Z 5

// Backbuffer types
#define D3DBACKBUFFER_TYPE_MONO     0
#define D3DCURSOR_IMMEDIATE_UPDATE  0x00000001L

// Color write enable flags
#define D3DCOLORWRITEENABLE_RED     1
#define D3DCOLORWRITEENABLE_GREEN   2
#define D3DCOLORWRITEENABLE_BLUE    4
#define D3DCOLORWRITEENABLE_ALPHA   8

// Blend ops
#define D3DBLENDOP_ADD              1
#define D3DBLENDOP_SUBTRACT         2
#define D3DBLENDOP_REVSUBTRACT      3
#define D3DBLENDOP_MIN              4
#define D3DBLENDOP_MAX              5

// Vertex blend
#define D3DVBF_DISABLE              0

// Vertex shader declaration tokens
#define D3DVSDT_FLOAT1              0x00
#define D3DVSDT_FLOAT2              0x01
#define D3DVSDT_FLOAT3              0x02
#define D3DVSDT_FLOAT4              0x03
#define D3DVSDT_D3DCOLOR            0x04
#define D3DVSDT_UBYTE4              0x05
#define D3DVSDT_SHORT2              0x06
#define D3DVSDT_SHORT4              0x07

#define D3DVSD_STREAM(_StreamNumber) ((DWORD)(0x10000000u | ((_StreamNumber) & 0xF)))
#define D3DVSD_REG(_VertexRegister, _Type) ((DWORD)((((_Type) & 0xF) << 16) | ((_VertexRegister) & 0xFF)))
#define D3DVSD_END()                 0xFFFFFFFFu

// C++ interface declarations (virtual method tables)
#ifdef __cplusplus

#ifndef D3DXMATH_H_ZH_STUB
#include "D3DXMath.h"
#endif

// Forward declarations needed for cross-references
struct D3DXVECTOR4;
struct IDirect3DSurface8;
struct IDirect3DTexture8;
struct IDirect3DBaseTexture8;
struct IDirect3DCubeTexture8;
struct IDirect3DVolumeTexture8;
struct IDirect3DVertexBuffer8;
struct IDirect3DIndexBuffer8;
struct IDirect3DSwapChain8;
struct IDirect3DDevice8;
struct IDirect3D8;
typedef IDirect3DSurface8* LPDIRECT3DSURFACE8;
typedef IDirect3DTexture8* LPDIRECT3DTEXTURE8;
typedef IDirect3DBaseTexture8* LPDIRECT3DBASETEXTURE8;
typedef IDirect3DCubeTexture8* LPDIRECT3DCUBETEXTURE8;
typedef IDirect3DVolumeTexture8* LPDIRECT3DVOLUMETEXTURE8;
typedef IDirect3DVertexBuffer8* LPDIRECT3DVERTEXBUFFER8;
typedef IDirect3DIndexBuffer8* LPDIRECT3DINDEXBUFFER8;
typedef IDirect3DSwapChain8* LPDIRECT3DSWAPCHAIN8;
typedef IDirect3DDevice8* LPDIRECT3DDEVICE8;
typedef IDirect3D8* LPDIRECT3D8;

struct IDirect3DBaseTexture8 {
  virtual ULONG AddRef() = 0;
  virtual ULONG Release() = 0;
  virtual DWORD SetLOD(DWORD LODNew) = 0;
  virtual DWORD GetLevelCount() = 0;
  virtual DWORD GetPriority() = 0;
  virtual DWORD SetPriority(DWORD Priority) = 0;
};

struct IDirect3DTexture8 : public IDirect3DBaseTexture8 {
  virtual HRESULT GetSurfaceLevel(UINT, IDirect3DSurface8**) = 0;
  virtual HRESULT GetLevelDesc(UINT, D3DSURFACE_DESC*) = 0;
  virtual HRESULT LockRect(UINT, D3DLOCKED_RECT*, const RECT*, DWORD) = 0;
  virtual HRESULT UnlockRect(UINT) = 0;
};

struct IDirect3DCubeTexture8 : public IDirect3DBaseTexture8 {
  virtual HRESULT GetLevelDesc(UINT Level, D3DSURFACE_DESC* pDesc) = 0;
  virtual HRESULT LockRect(D3DCUBEMAP_FACE Face, UINT Level, D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags) = 0;
  virtual HRESULT UnlockRect(D3DCUBEMAP_FACE Face, UINT Level) = 0;
};

struct IDirect3DVolumeTexture8 : public IDirect3DBaseTexture8 {
  virtual HRESULT GetLevelDesc(UINT Level, D3DVOLUME_DESC* pDesc) = 0;
  virtual HRESULT LockBox(UINT Level, D3DLOCKED_BOX* pLockedVolume, const void* pBox, DWORD Flags) = 0;
  virtual HRESULT UnlockBox(UINT Level) = 0;
};

struct IDirect3DSurface8 {
  virtual ULONG AddRef() = 0;
  virtual ULONG Release() = 0;
  virtual HRESULT GetDesc(void*) = 0;
  virtual HRESULT LockRect(void*, const RECT*, DWORD) = 0;
  virtual HRESULT UnlockRect() = 0;
};

struct IDirect3DVertexBuffer8 {
  virtual ULONG AddRef() = 0;
  virtual ULONG Release() = 0;
  virtual HRESULT Lock(UINT, UINT, BYTE**, DWORD) = 0;
  virtual HRESULT Unlock() = 0;
};

struct IDirect3DIndexBuffer8 {
  virtual ULONG AddRef() = 0;
  virtual ULONG Release() = 0;
  virtual HRESULT Lock(UINT, UINT, BYTE**, DWORD) = 0;
  virtual HRESULT Unlock() = 0;
};

struct IDirect3DSwapChain8 {
  virtual ULONG AddRef() = 0;
  virtual ULONG Release() = 0;
  virtual HRESULT GetBackBuffer(UINT, D3DBACKBUFFER_TYPE, IDirect3DSurface8**) = 0;
};

struct IDirect3DDevice8 {
  virtual HRESULT TestCooperativeLevel() = 0;
  virtual HRESULT Reset(D3DPRESENT_PARAMETERS*) = 0;
  virtual HRESULT Present(const void*, const void*, void*, const void*) = 0;
  virtual HRESULT SetVertexShader(DWORD) = 0;
  virtual HRESULT SetPixelShader(DWORD) = 0;
  virtual HRESULT SetVertexShaderConstant(DWORD, const void*, DWORD) = 0;
  virtual HRESULT SetPixelShaderConstant(DWORD, const void*, DWORD) = 0;
  HRESULT SetVertexShaderConstant(DWORD Register, const D3DXVECTOR4& Value, DWORD Count) { return SetVertexShaderConstant(Register, &Value, Count); }
  HRESULT SetPixelShaderConstant(DWORD Register, const D3DXVECTOR4& Value, DWORD Count) { return SetPixelShaderConstant(Register, &Value, Count); }
  virtual HRESULT SetTransform(D3DTRANSFORMSTATETYPE, const D3DMATRIX*) = 0;
  virtual HRESULT GetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*) = 0;
  virtual HRESULT SetMaterial(const D3DMATERIAL8*) = 0;
  virtual HRESULT SetLight(DWORD, const D3DLIGHT8*) = 0;
  virtual HRESULT LightEnable(DWORD, BOOL) = 0;
  virtual HRESULT SetRenderState(D3DRENDERSTATETYPE, DWORD) = 0;
  virtual HRESULT GetRenderState(D3DRENDERSTATETYPE, DWORD*) = 0;
  virtual HRESULT SetClipPlane(DWORD, const float*) = 0;
  virtual HRESULT SetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD) = 0;
  virtual HRESULT GetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD*) = 0;
  virtual HRESULT SetTexture(DWORD, IDirect3DBaseTexture8*) = 0;
  virtual HRESULT GetTexture(DWORD, IDirect3DBaseTexture8**) = 0;
  virtual HRESULT CopyRects(IDirect3DSurface8*, const RECT*, UINT, IDirect3DSurface8*, const POINT*) = 0;
  virtual HRESULT CreateTexture(UINT, UINT, UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DTexture8**) = 0;
  virtual HRESULT CreateVertexBuffer(UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer8**) = 0;
  virtual HRESULT CreateIndexBuffer(UINT, DWORD, D3DFORMAT, D3DPOOL, IDirect3DIndexBuffer8**) = 0;
  virtual HRESULT SetStreamSource(UINT, IDirect3DVertexBuffer8*, UINT) = 0;
  virtual HRESULT SetIndices(IDirect3DIndexBuffer8*, UINT) = 0;
  virtual HRESULT DrawPrimitive(DWORD, UINT, UINT) = 0;
  virtual HRESULT DrawIndexedPrimitive(DWORD, UINT, UINT, UINT, UINT) = 0;
  virtual HRESULT DrawPrimitiveUP(DWORD, UINT, const void*, UINT) = 0;
  virtual HRESULT DrawIndexedPrimitiveUP(DWORD, UINT, UINT, UINT, const void*, D3DFORMAT, const void*, UINT) = 0;
  virtual HRESULT BeginScene() = 0;
  virtual HRESULT EndScene() = 0;
  virtual HRESULT Clear(DWORD, const void*, DWORD, DWORD, float, DWORD) = 0;
  virtual HRESULT SetViewport(const D3DVIEWPORT8*) = 0;
  virtual HRESULT GetViewport(D3DVIEWPORT8*) = 0;
  virtual HRESULT GetDisplayMode(D3DDISPLAYMODE*) = 0;
  virtual HRESULT GetDeviceCaps(D3DCAPS8*) = 0;
  virtual HRESULT GetBackBuffer(UINT, DWORD, IDirect3DSurface8**) = 0;
  virtual HRESULT GetFrontBuffer(IDirect3DSurface8*) = 0;
  virtual HRESULT GetDepthStencilSurface(IDirect3DSurface8**) = 0;
  virtual HRESULT CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS*, IDirect3DSwapChain8**) = 0;
  virtual HRESULT CreateDepthStencilSurface(UINT, UINT, D3DFORMAT, DWORD, IDirect3DSurface8**) = 0;
  virtual HRESULT CreateImageSurface(UINT, UINT, D3DFORMAT, IDirect3DSurface8**) = 0;
  virtual HRESULT SetRenderTarget(IDirect3DSurface8*, IDirect3DSurface8*) = 0;
  virtual HRESULT GetRenderTarget(IDirect3DSurface8**) = 0;
  virtual HRESULT UpdateTexture(IDirect3DBaseTexture8*, IDirect3DBaseTexture8*) = 0;
  virtual HRESULT ValidateDevice(DWORD*) = 0;
  virtual HRESULT CreateVertexShader(const DWORD*, const DWORD*, DWORD*, DWORD) = 0;
  virtual HRESULT CreatePixelShader(const DWORD*, DWORD*) = 0;
  virtual HRESULT DeleteVertexShader(DWORD) = 0;
  virtual HRESULT DeletePixelShader(DWORD) = 0;
  virtual HRESULT SetCursorProperties(UINT, UINT, IDirect3DSurface8*) = 0;
  virtual void SetCursorPosition(int, int, DWORD) = 0;
  virtual BOOL ShowCursor(BOOL) = 0;
  virtual UINT GetAvailableTextureMem() = 0;
  virtual void SetGammaRamp(DWORD, const D3DGAMMARAMP*) = 0;
  virtual HRESULT ResourceManagerDiscardBytes(DWORD Bytes) = 0;
  virtual ULONG Release() = 0;
};

struct IDirect3D8 {
  virtual HRESULT GetDeviceCaps(UINT, UINT, D3DCAPS8*) = 0;
  virtual HRESULT GetAdapterIdentifier(UINT, DWORD, D3DADAPTER_IDENTIFIER8*) = 0;
  virtual UINT GetAdapterCount() = 0;
  virtual UINT GetAdapterModeCount(UINT) = 0;
  virtual HRESULT EnumAdapterModes(UINT, UINT, D3DDISPLAYMODE*) = 0;
  virtual HRESULT GetAdapterDisplayMode(UINT, D3DDISPLAYMODE*) = 0;
  virtual HRESULT CheckDeviceType(UINT, UINT, UINT, UINT, BOOL) = 0;
  virtual HRESULT CheckDeviceFormat(UINT, UINT, UINT, UINT, UINT, UINT) = 0;
  virtual HRESULT CheckDepthStencilMatch(UINT, UINT, UINT, UINT, UINT) = 0;
  virtual HRESULT CreateDevice(UINT, UINT, HWND, DWORD, D3DPRESENT_PARAMETERS*, IDirect3DDevice8**) = 0;
  virtual ULONG Release() = 0;
};

#endif // __cplusplus
