// d3d8.h â€” minimal stub header for Direct3D 8
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
  // Texture filter capabilities â€” used by TextureFilterClass
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

// D3DTS_TEXTURE0..7 â€” texture transform state indices
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

// COM calling convention: D3D8 interfaces use __stdcall on Windows.
// MinGW GCC also supports __stdcall.  On native Linux/macOS builds
// (where D3D8 is never actually called at runtime) define it away.
#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#  define D3D8CC __stdcall
#else
#  ifndef __stdcall
#    define __stdcall   // keep any code that uses __stdcall explicitly from breaking
#  endif
#  define D3D8CC
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

// -----------------------------------------------------------------------
// COM vtable layout: every D3D8 interface is a COM interface.
// The vtable MUST start with QueryInterface / AddRef / Release (IUnknown),
// followed by the interface's own methods in exactly the order the real
// D3D8.DLL (and any wrapper such as dxwrapper/d3d8to9/DXVK) exposes them.
// Wrong ordering â†’ every call dispatches to the wrong vtable slot â†’ crash.
// -----------------------------------------------------------------------

// IUnknown_D3D8: minimal base so we don't pull in objbase.h
struct IUnknown_D3D8 {
  virtual HRESULT D3D8CC QueryInterface(const GUID& riid, void** ppvObject) = 0;
  virtual ULONG D3D8CC   AddRef()   = 0;
  virtual ULONG D3D8CC   Release()  = 0;
};

// IDirect3DResource8 (common base for textures and buffers)
// Vtable slots 0-2: IUnknown; 3-10: resource methods
struct IDirect3DResource8 : public IUnknown_D3D8 {
  virtual HRESULT D3D8CC GetDevice(IDirect3DDevice8** ppDevice) = 0;                                         // 3
  virtual HRESULT D3D8CC SetPrivateData(const GUID&, const void*, DWORD, DWORD) = 0;                         // 4
  virtual HRESULT D3D8CC GetPrivateData(const GUID&, void*, DWORD*) = 0;                                     // 5
  virtual HRESULT D3D8CC FreePrivateData(const GUID&) = 0;                                                   // 6
  virtual DWORD D3D8CC   SetPriority(DWORD PriorityNew) = 0;                                                 // 7
  virtual DWORD D3D8CC   GetPriority() = 0;                                                                  // 8
  virtual void D3D8CC    PreLoad() = 0;                                                                       // 9
  virtual DWORD D3D8CC   GetType() = 0;  // returns D3DRESOURCETYPE                                          // 10
};

// IDirect3DBaseTexture8
// Vtable slots 0-10: IDirect3DResource8; 11-13: base-texture methods
struct IDirect3DBaseTexture8 : public IDirect3DResource8 {
  virtual DWORD D3D8CC SetLOD(DWORD LODNew) = 0;                                                             // 11
  virtual DWORD D3D8CC GetLOD() = 0;                                                                         // 12
  virtual DWORD D3D8CC GetLevelCount() = 0;                                                                  // 13
};

// IDirect3DTexture8
// Vtable slots 0-13: IDirect3DBaseTexture8; 14-18: texture-specific methods
struct IDirect3DTexture8 : public IDirect3DBaseTexture8 {
  virtual HRESULT D3D8CC GetLevelDesc(UINT Level, D3DSURFACE_DESC* pDesc) = 0;                              // 14
  virtual HRESULT D3D8CC GetSurfaceLevel(UINT Level, IDirect3DSurface8** ppSurfaceLevel) = 0;               // 15
  virtual HRESULT D3D8CC LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags) = 0; // 16
  virtual HRESULT D3D8CC UnlockRect(UINT Level) = 0;                                                        // 17
  virtual HRESULT D3D8CC AddDirtyRect(const RECT* pDirtyRect) = 0;                                         // 18
};

// IDirect3DCubeTexture8
// Vtable slots 0-13: IDirect3DBaseTexture8; 14-18: cube-texture-specific methods
struct IDirect3DCubeTexture8 : public IDirect3DBaseTexture8 {
  virtual HRESULT D3D8CC GetLevelDesc(UINT Level, D3DSURFACE_DESC* pDesc) = 0;                              // 14
  virtual HRESULT D3D8CC GetCubeMapSurface(D3DCUBEMAP_FACE FaceType, UINT Level, IDirect3DSurface8** ppSurface) = 0; // 15
  virtual HRESULT D3D8CC LockRect(D3DCUBEMAP_FACE FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags) = 0; // 16
  virtual HRESULT D3D8CC UnlockRect(D3DCUBEMAP_FACE FaceType, UINT Level) = 0;                             // 17
  virtual HRESULT D3D8CC AddDirtyRect(D3DCUBEMAP_FACE FaceType, const RECT* pDirtyRect) = 0;               // 18
};

// IDirect3DVolumeTexture8
// Vtable slots 0-13: IDirect3DBaseTexture8; 14-18: volume-texture-specific methods
struct IDirect3DVolumeTexture8 : public IDirect3DBaseTexture8 {
  virtual HRESULT D3D8CC GetLevelDesc(UINT Level, D3DVOLUME_DESC* pDesc) = 0;                              // 14
  virtual HRESULT D3D8CC GetVolumeLevel(UINT Level, void** ppVolumeLevel) = 0;                             // 15
  virtual HRESULT D3D8CC LockBox(UINT Level, D3DLOCKED_BOX* pLockedVolume, const void* pBox, DWORD Flags) = 0; // 16
  virtual HRESULT D3D8CC UnlockBox(UINT Level) = 0;                                                        // 17
  virtual HRESULT D3D8CC AddDirtyBox(const void* pDirtyBox) = 0;                                          // 18
};

// IDirect3DSurface8
// Vtable slots 0-2: IUnknown; 3-10: surface methods
struct IDirect3DSurface8 : public IUnknown_D3D8 {
  virtual HRESULT D3D8CC GetDevice(IDirect3DDevice8** ppDevice) = 0;                                        // 3
  virtual HRESULT D3D8CC SetPrivateData(const GUID&, const void*, DWORD, DWORD) = 0;                        // 4
  virtual HRESULT D3D8CC GetPrivateData(const GUID&, void*, DWORD*) = 0;                                    // 5
  virtual HRESULT D3D8CC FreePrivateData(const GUID&) = 0;                                                  // 6
  virtual HRESULT D3D8CC GetContainer(const GUID& riid, void** ppContainer) = 0;                            // 7
  virtual HRESULT D3D8CC GetDesc(D3DSURFACE_DESC* pDesc) = 0;                                              // 8
  virtual HRESULT D3D8CC LockRect(D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags) = 0;        // 9
  virtual HRESULT D3D8CC UnlockRect() = 0;                                                                  // 10
};

// IDirect3DVertexBuffer8
// Vtable slots 0-10: IDirect3DResource8; 11-13: buffer methods
struct IDirect3DVertexBuffer8 : public IDirect3DResource8 {
  virtual HRESULT D3D8CC Lock(UINT OffsetToLock, UINT SizeToLock, BYTE** ppbData, DWORD Flags) = 0;       // 11
  virtual HRESULT D3D8CC Unlock() = 0;                                                                      // 12
  virtual HRESULT D3D8CC GetDesc(void* pDesc) = 0;  // D3DVERTEXBUFFER_DESC*                               // 13
};

// IDirect3DIndexBuffer8
// Vtable slots 0-10: IDirect3DResource8; 11-13: buffer methods
struct IDirect3DIndexBuffer8 : public IDirect3DResource8 {
  virtual HRESULT D3D8CC Lock(UINT OffsetToLock, UINT SizeToLock, BYTE** ppbData, DWORD Flags) = 0;       // 11
  virtual HRESULT D3D8CC Unlock() = 0;                                                                      // 12
  virtual HRESULT D3D8CC GetDesc(void* pDesc) = 0;  // D3DINDEXBUFFER_DESC*                                // 13
};

// IDirect3DSwapChain8
// Vtable slots 0-2: IUnknown; 3-4: swap chain methods
struct IDirect3DSwapChain8 : public IUnknown_D3D8 {
  virtual HRESULT D3D8CC Present(const RECT*, const RECT*, HWND, const void*) = 0;                        // 3
  virtual HRESULT D3D8CC GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer) = 0; // 4
};

// IDirect3DDevice8 â€” 97 vtable slots (0-2 IUnknown, 3-96 device methods)
// Every slot must appear in exactly this order to match the real COM vtable.
struct IDirect3DDevice8 : public IUnknown_D3D8 {
  virtual HRESULT D3D8CC TestCooperativeLevel() = 0;                                                        // 3
  virtual UINT D3D8CC    GetAvailableTextureMem() = 0;                                                      // 4
  virtual HRESULT D3D8CC ResourceManagerDiscardBytes(DWORD Bytes) = 0;                                      // 5
  virtual HRESULT D3D8CC GetDirect3D(IDirect3D8** ppD3D8) = 0;                                             // 6
  virtual HRESULT D3D8CC GetDeviceCaps(D3DCAPS8* pCaps) = 0;                                               // 7
  virtual HRESULT D3D8CC GetDisplayMode(D3DDISPLAYMODE* pMode) = 0;                                        // 8
  virtual HRESULT D3D8CC GetCreationParameters(void* pParameters) = 0;                                     // 9
  virtual HRESULT D3D8CC SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap) = 0; // 10
  virtual void D3D8CC    SetCursorPosition(int X, int Y, DWORD Flags) = 0;                                 // 11
  virtual BOOL D3D8CC    ShowCursor(BOOL bShow) = 0;                                                        // 12
  virtual HRESULT D3D8CC CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPP, IDirect3DSwapChain8** ppSwapChain) = 0; // 13
  virtual HRESULT D3D8CC Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) = 0;                        // 14
  virtual HRESULT D3D8CC Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const void* pDirtyRegion) = 0; // 15
  virtual HRESULT D3D8CC GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer) = 0; // 16
  virtual HRESULT D3D8CC GetRasterStatus(void* pRasterStatus) = 0;                                         // 17
  virtual void D3D8CC    SetGammaRamp(DWORD Flags, const D3DGAMMARAMP* pRamp) = 0;                         // 18
  virtual void D3D8CC    GetGammaRamp(D3DGAMMARAMP* pRamp) = 0;                                            // 19
  virtual HRESULT D3D8CC CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture) = 0; // 20
  virtual HRESULT D3D8CC CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture) = 0; // 21
  virtual HRESULT D3D8CC CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture) = 0; // 22
  virtual HRESULT D3D8CC CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer) = 0; // 23
  virtual HRESULT D3D8CC CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer) = 0; // 24
  virtual HRESULT D3D8CC CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, DWORD MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface) = 0; // 25
  virtual HRESULT D3D8CC CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, DWORD MultiSample, IDirect3DSurface8** ppSurface) = 0; // 26
  virtual HRESULT D3D8CC CreateImageSurface(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface) = 0; // 27
  virtual HRESULT D3D8CC CopyRects(IDirect3DSurface8* pSourceSurface, const RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestSurface, const POINT* pDestPointsArray) = 0; // 28
  virtual HRESULT D3D8CC UpdateTexture(IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture) = 0; // 29
  virtual HRESULT D3D8CC GetFrontBuffer(IDirect3DSurface8* pDestSurface) = 0;                              // 30
  virtual HRESULT D3D8CC SetRenderTarget(IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil) = 0; // 31
  virtual HRESULT D3D8CC GetRenderTarget(IDirect3DSurface8** ppRenderTarget) = 0;                          // 32
  virtual HRESULT D3D8CC GetDepthStencilSurface(IDirect3DSurface8** ppZStencilSurface) = 0;               // 33
  virtual HRESULT D3D8CC BeginScene() = 0;                                                                  // 34
  virtual HRESULT D3D8CC EndScene() = 0;                                                                    // 35
  virtual HRESULT D3D8CC Clear(DWORD Count, const void* pRects, DWORD Flags, DWORD Color, float Z, DWORD Stencil) = 0; // 36
  virtual HRESULT D3D8CC SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) = 0;          // 37
  virtual HRESULT D3D8CC GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) = 0;                // 38
  virtual HRESULT D3D8CC MultiplyTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) = 0;     // 39
  virtual HRESULT D3D8CC SetViewport(const D3DVIEWPORT8* pViewport) = 0;                                   // 40
  virtual HRESULT D3D8CC GetViewport(D3DVIEWPORT8* pViewport) = 0;                                        // 41
  virtual HRESULT D3D8CC SetMaterial(const D3DMATERIAL8* pMaterial) = 0;                                  // 42
  virtual HRESULT D3D8CC GetMaterial(D3DMATERIAL8* pMaterial) = 0;                                        // 43
  virtual HRESULT D3D8CC SetLight(DWORD Index, const D3DLIGHT8* pLight) = 0;                              // 44
  virtual HRESULT D3D8CC GetLight(DWORD Index, D3DLIGHT8* pLight) = 0;                                    // 45
  virtual HRESULT D3D8CC LightEnable(DWORD Index, BOOL Enable) = 0;                                       // 46
  virtual HRESULT D3D8CC GetLightEnable(DWORD Index, BOOL* pEnable) = 0;                                  // 47
  virtual HRESULT D3D8CC SetClipPlane(DWORD Index, const float* pPlane) = 0;                              // 48
  virtual HRESULT D3D8CC GetClipPlane(DWORD Index, float* pPlane) = 0;                                    // 49
  virtual HRESULT D3D8CC SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) = 0;                       // 50
  virtual HRESULT D3D8CC GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) = 0;                     // 51
  virtual HRESULT D3D8CC BeginStateBlock() = 0;                                                            // 52
  virtual HRESULT D3D8CC EndStateBlock(DWORD* pToken) = 0;                                                 // 53
  virtual HRESULT D3D8CC ApplyStateBlock(DWORD Token) = 0;                                                 // 54
  virtual HRESULT D3D8CC CaptureStateBlock(DWORD Token) = 0;                                              // 55
  virtual HRESULT D3D8CC DeleteStateBlock(DWORD Token) = 0;                                               // 56
  virtual HRESULT D3D8CC CreateStateBlock(DWORD Type, DWORD* pToken) = 0;                                 // 57
  virtual HRESULT D3D8CC SetClipStatus(const void* pClipStatus) = 0;                                      // 58
  virtual HRESULT D3D8CC GetClipStatus(void* pClipStatus) = 0;                                            // 59
  virtual HRESULT D3D8CC GetTexture(DWORD Stage, IDirect3DBaseTexture8** ppTexture) = 0;                  // 60
  virtual HRESULT D3D8CC SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture) = 0;                    // 61
  virtual HRESULT D3D8CC GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) = 0; // 62
  virtual HRESULT D3D8CC SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) = 0;   // 63
  virtual HRESULT D3D8CC ValidateDevice(DWORD* pNumPasses) = 0;                                            // 64
  virtual HRESULT D3D8CC GetInfo(DWORD DevInfoID, void* pDevInfoStruct, DWORD DevInfoStructSize) = 0;     // 65
  virtual HRESULT D3D8CC SetPaletteEntries(UINT PaletteNumber, const void* pEntries) = 0;                 // 66
  virtual HRESULT D3D8CC GetPaletteEntries(UINT PaletteNumber, void* pEntries) = 0;                       // 67
  virtual HRESULT D3D8CC SetCurrentTexturePalette(UINT PaletteNumber) = 0;                                // 68
  virtual HRESULT D3D8CC GetCurrentTexturePalette(UINT* pPaletteNumber) = 0;                              // 69
  virtual HRESULT D3D8CC DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) = 0; // 70
  virtual HRESULT D3D8CC DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) = 0; // 71
  virtual HRESULT D3D8CC DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) = 0; // 72
  virtual HRESULT D3D8CC DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) = 0; // 73
  virtual HRESULT D3D8CC ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags) = 0; // 74
  virtual HRESULT D3D8CC CreateVertexShader(const DWORD* pDeclaration, const DWORD* pFunction, DWORD* pHandle, DWORD Usage) = 0; // 75
  virtual HRESULT D3D8CC SetVertexShader(DWORD Handle) = 0;                                               // 76
  virtual HRESULT D3D8CC GetVertexShader(DWORD* pHandle) = 0;                                             // 77
  virtual HRESULT D3D8CC DeleteVertexShader(DWORD Handle) = 0;                                            // 78
  virtual HRESULT D3D8CC SetVertexShaderConstant(DWORD Register, const void* pConstantData, DWORD ConstantCount) = 0; // 79
  virtual HRESULT D3D8CC GetVertexShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) = 0; // 80
  virtual HRESULT D3D8CC GetVertexShaderDeclaration(DWORD Handle, void* pData, DWORD* pSizeOfData) = 0;  // 81
  virtual HRESULT D3D8CC GetVertexShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData) = 0;     // 82
  virtual HRESULT D3D8CC SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride) = 0; // 83
  virtual HRESULT D3D8CC GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride) = 0; // 84
  virtual HRESULT D3D8CC SetIndices(IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex) = 0;         // 85
  virtual HRESULT D3D8CC GetIndices(IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex) = 0;     // 86
  virtual HRESULT D3D8CC CreatePixelShader(const DWORD* pFunction, DWORD* pHandle) = 0;                  // 87
  virtual HRESULT D3D8CC SetPixelShader(DWORD Handle) = 0;                                               // 88
  virtual HRESULT D3D8CC GetPixelShader(DWORD* pHandle) = 0;                                             // 89
  virtual HRESULT D3D8CC DeletePixelShader(DWORD Handle) = 0;                                            // 90
  virtual HRESULT D3D8CC SetPixelShaderConstant(DWORD Register, const void* pConstantData, DWORD ConstantCount) = 0; // 91
  virtual HRESULT D3D8CC GetPixelShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) = 0; // 92
  virtual HRESULT D3D8CC GetPixelShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData) = 0;      // 93
  virtual HRESULT D3D8CC DrawRectPatch(UINT Handle, const float* pNumSegs, const void* pRectPatchInfo) = 0; // 94
  virtual HRESULT D3D8CC DrawTriPatch(UINT Handle, const float* pNumSegs, const void* pTriPatchInfo) = 0;  // 95
  virtual HRESULT D3D8CC DeletePatch(UINT Handle) = 0;                                                    // 96

  // Non-virtual convenience overloads (do not add vtable slots)
  HRESULT SetVertexShaderConstant(DWORD Register, const D3DXVECTOR4& Value, DWORD Count) { return SetVertexShaderConstant(Register, &Value, Count); }
  HRESULT SetPixelShaderConstant(DWORD Register, const D3DXVECTOR4& Value, DWORD Count) { return SetPixelShaderConstant(Register, &Value, Count); }
};

// IDirect3D8 â€” 16 vtable slots (0-2 IUnknown, 3-15 D3D8 methods)
struct IDirect3D8 : public IUnknown_D3D8 {
  virtual HRESULT D3D8CC RegisterSoftwareDevice(void* pInitializeFunction) = 0;                           // 3
  virtual UINT D3D8CC    GetAdapterCount() = 0;                                                            // 4
  virtual HRESULT D3D8CC GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8* pIdentifier) = 0; // 5
  virtual UINT D3D8CC    GetAdapterModeCount(UINT Adapter) = 0;                                           // 6
  virtual HRESULT D3D8CC EnumAdapterModes(UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode) = 0;            // 7
  virtual HRESULT D3D8CC GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode) = 0;                  // 8
  virtual HRESULT D3D8CC CheckDeviceType(UINT Adapter, DWORD CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) = 0; // 9
  virtual HRESULT D3D8CC CheckDeviceFormat(UINT Adapter, DWORD DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, DWORD RType, D3DFORMAT CheckFormat) = 0; // 10
  virtual HRESULT D3D8CC CheckDeviceMultiSampleType(UINT Adapter, DWORD DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, DWORD MultiSampleType) = 0; // 11
  virtual HRESULT D3D8CC CheckDepthStencilMatch(UINT Adapter, DWORD DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) = 0; // 12
  virtual HRESULT D3D8CC GetDeviceCaps(UINT Adapter, DWORD DeviceType, D3DCAPS8* pCaps) = 0;              // 13
  virtual void*   GetAdapterMonitor(UINT Adapter) = 0;  // returns HMONITOR                        // 14
  virtual HRESULT D3D8CC CreateDevice(UINT Adapter, DWORD DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface) = 0; // 15
};

#endif // __cplusplus
