#pragma once
#ifndef ZH_COMPAT_DDRAW_H
#define ZH_COMPAT_DDRAW_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

typedef struct _DDCOLORKEY {
    DWORD dwColorSpaceLowValue;
    DWORD dwColorSpaceHighValue;
} DDCOLORKEY, *LPDDCOLORKEY;

typedef struct _DDPIXELFORMAT {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;
    DWORD dwRGBBitCount;
    DWORD dwRBitMask;
    DWORD dwGBitMask;
    DWORD dwBBitMask;
    DWORD dwRGBAlphaBitMask;
} DDPIXELFORMAT, *LPDDPIXELFORMAT;

typedef struct _DDSCAPS {
    DWORD dwCaps;
} DDSCAPS, *LPDDSCAPS;

typedef struct _DDSURFACEDESC {
    DWORD         dwSize;
    DWORD         dwFlags;
    DWORD         dwHeight;
    DWORD         dwWidth;
    LONG          lPitch;
    DWORD         dwBackBufferCount;
    LPVOID        lpSurface;
    DDCOLORKEY    ddckCKDestOverlay;
    DDCOLORKEY    ddckCKDestBlt;
    DDCOLORKEY    ddckCKSrcOverlay;
    DDCOLORKEY    ddckCKSrcBlt;
    DDPIXELFORMAT ddpfPixelFormat;
    DDSCAPS       ddsCaps;
} DDSURFACEDESC, *LPDDSURFACEDESC;

typedef struct _DDBLTFX {
    DWORD dwSize;
    DWORD dwDDFX;
    DWORD dwROP;
    DWORD dwDDROP;
    DWORD dwRotationAngle;
    DWORD dwZBufferOpCode;
    DWORD dwZBufferLow;
    DWORD dwZBufferHigh;
    DWORD dwZBufferBaseDest;
    DWORD dwZDestConstBitDepth;
    DWORD dwZDestConst;
    DWORD dwZSrcConstBitDepth;
    DWORD dwZSrcConst;
    DWORD dwAlphaEdgeBlendBitDepth;
    DWORD dwAlphaEdgeBlend;
    DWORD dwReserved;
    DWORD dwAlphaDestConstBitDepth;
    DWORD dwAlphaDestConst;
    DWORD dwAlphaSrcConstBitDepth;
    DWORD dwAlphaSrcConst;
    DWORD dwFillColor;
} DDBLTFX, *LPDDBLTFX;

typedef struct _DDCAPS {
    DWORD dwSize;
    DWORD dwCaps;
    DWORD dwSVBCaps;
    DWORD dwVSBCaps;
    DWORD dwSSBCaps;
    DWORD dwVidMemFree;
} DDCAPS, *LPDDCAPS;

typedef struct tagPALETTEENTRY {
    BYTE peRed;
    BYTE peGreen;
    BYTE peBlue;
    BYTE peFlags;
} PALETTEENTRY, *LPPALETTEENTRY;

#define DD_OK                          S_OK
#define DDERR_ALREADYINITIALIZED       E_FAIL
#define DDERR_BLTFASTCANTCLIP          E_FAIL
#define DDERR_CANNOTATTACHSURFACE      E_FAIL
#define DDERR_CANNOTDETACHSURFACE      E_FAIL
#define DDERR_CANTCREATEDC             E_FAIL
#define DDERR_CANTDUPLICATE            E_FAIL
#define DDERR_CANTLOCKSURFACE          E_FAIL
#define DDERR_CLIPPERISUSINGHWND       E_FAIL
#define DDERR_COLORKEYNOTSET           E_FAIL
#define DDERR_CURRENTLYNOTAVAIL        E_FAIL
#define DDERR_DIRECTDRAWALREADYCREATED E_FAIL
#define DDERR_EXCEPTION                E_FAIL
#define DDERR_EXCLUSIVEMODEALREADYSET  E_FAIL
#define DDERR_GENERIC                  E_FAIL
#define DDERR_HEIGHTALIGN              E_FAIL
#define DDERR_HWNDALREADYSET           E_FAIL
#define DDERR_HWNDSUBCLASSED           E_FAIL
#define DDERR_IMPLICITLYCREATED        E_FAIL
#define DDERR_INCOMPATIBLEPRIMARY      E_FAIL
#define DDERR_INVALIDCAPS              E_FAIL
#define DDERR_INVALIDCLIPLIST          E_FAIL
#define DDERR_INVALIDDIRECTDRAWGUID    E_FAIL
#define DDERR_INVALIDMODE              E_FAIL
#define DDERR_INVALIDOBJECT            E_FAIL
#define DDERR_INVALIDPARAMS            E_INVALIDARG
#define DDERR_INVALIDPIXELFORMAT       E_FAIL
#define DDERR_INVALIDPOSITION          E_FAIL
#define DDERR_INVALIDRECT              E_FAIL
#define DDERR_INVALIDSURFACETYPE       E_FAIL
#define DDERR_LOCKEDSURFACES           E_FAIL
#define DDERR_NO3D                     E_FAIL
#define DDERR_NOALPHAHW                E_FAIL
#define DDERR_NOBLTHW                  E_FAIL
#define DDERR_NOCLIPLIST               E_FAIL
#define DDERR_NOCLIPPERATTACHED        E_FAIL
#define DDERR_NOCOLORCONVHW            E_FAIL
#define DDERR_NOCOLORKEY               E_FAIL
#define DDERR_NOCOLORKEYHW             E_FAIL
#define DDERR_NOCOOPERATIVELEVELSET    E_FAIL
#define DDERR_NODC                     E_FAIL
#define DDERR_NODDROPSHW               E_FAIL
#define DDERR_NODIRECTDRAWHW           E_FAIL
#define DDERR_NODIRECTDRAWSUPPORT      E_FAIL
#define DDERR_NOEMULATION              E_FAIL
#define DDERR_NOEXCLUSIVEMODE          E_FAIL
#define DDERR_NOFLIPHW                 E_FAIL
#define DDERR_NOGDI                    E_FAIL
#define DDERR_NOHWND                   E_FAIL
#define DDERR_NOMIRRORHW               E_FAIL
#define DDERR_NOOVERLAYDEST            E_FAIL
#define DDERR_NOOVERLAYHW              E_FAIL
#define DDERR_NOPALETTEATTACHED        E_FAIL
#define DDERR_NOPALETTEHW              E_FAIL
#define DDERR_NORASTEROPHW             E_FAIL
#define DDERR_NOROTATIONHW             E_FAIL
#define DDERR_NOSTRETCHHW              E_FAIL
#define DDERR_NOT4BITCOLOR             E_FAIL
#define DDERR_NOT4BITCOLORINDEX        E_FAIL
#define DDERR_NOT8BITCOLOR             E_FAIL
#define DDERR_NOTAOVERLAYSURFACE       E_FAIL
#define DDERR_NOTEXTUREHW              E_FAIL
#define DDERR_NOTFLIPPABLE             E_FAIL
#define DDERR_NOTFOUND                 E_FAIL
#define DDERR_NOTLOCKED                E_FAIL
#define DDERR_NOTPALETTIZED            E_FAIL
#define DDERR_NOVSYNCHW                E_FAIL
#define DDERR_NOZBUFFERHW              E_FAIL
#define DDERR_NOZOVERLAYHW             E_FAIL
#define DDERR_OUTOFCAPS                E_FAIL
#define DDERR_OUTOFMEMORY              E_OUTOFMEMORY
#define DDERR_OUTOFVIDEOMEMORY         E_OUTOFMEMORY
#define DDERR_OVERLAYCANTCLIP          E_FAIL
#define DDERR_OVERLAYCOLORKEYONLYONEACTIVE E_FAIL
#define DDERR_OVERLAYNOTVISIBLE        E_FAIL
#define DDERR_PALETTEBUSY              E_FAIL
#define DDERR_PRIMARYSURFACEALREADYEXISTS E_FAIL
#define DDERR_REGIONTOOSMALL           E_FAIL
#define DDERR_SURFACEALREADYATTACHED   E_FAIL
#define DDERR_SURFACEALREADYDEPENDENT  E_FAIL
#define DDERR_SURFACEBUSY              E_FAIL
#define DDERR_SURFACEISOBSCURED        E_FAIL
#define DDERR_SURFACELOST              E_FAIL
#define DDERR_SURFACENOTATTACHED       E_FAIL
#define DDERR_TOOBIGHEIGHT             E_FAIL
#define DDERR_TOOBIGSIZE               E_FAIL
#define DDERR_TOOBIGWIDTH              E_FAIL
#define DDERR_UNSUPPORTED              E_NOTIMPL
#define DDERR_UNSUPPORTEDFORMAT        E_NOTIMPL
#define DDERR_UNSUPPORTEDMASK          E_NOTIMPL
#define DDERR_VERTICALBLANKINPROGRESS  E_FAIL
#define DDERR_WASSTILLDRAWING          E_FAIL
#define DDERR_WRONGMODE                E_FAIL
#define DDERR_XALIGN                   E_FAIL

#define DDSD_CAPS               0x00000001
#define DDSD_HEIGHT             0x00000002
#define DDSD_WIDTH              0x00000004
#define DDSD_BACKBUFFERCOUNT    0x00000020
#define DDSD_PIXELFORMAT        0x00001000

#define DDSCAPS_PRIMARYSURFACE  0x00000200
#define DDSCAPS_OFFSCREENPLAIN  0x00000040
#define DDSCAPS_SYSTEMMEMORY    0x00000800
#define DDSCAPS_VIDEOMEMORY     0x00004000
#define DDSCAPS_COMPLEX         0x00000008
#define DDSCAPS_FLIP            0x00000010
#define DDSCAPS_BACKBUFFER      0x00000004

#define DDSCAPS2_CUBEMAP        0x00000200
#define DDSCAPS2_VOLUME         0x00200000

#define DDBLT_WAIT              0x00000010
#define DDBLT_COLORFILL         0x00000400
#define DDLOCK_SURFACEMEMORYPTR 0x00000000
#define DDLOCK_WAIT             0x00000001
#define DDGBS_ISBLTDONE         0x00000001
#define DDPCAPS_8BIT            0x00000020
#define DDPCAPS_ALLOW256        0x00000040
#define DDSCL_NORMAL            0x00000008
#define DDSCL_EXCLUSIVE         0x00000010
#define DDSCL_FULLSCREEN        0x00000020
#define DDWAITVB_BLOCKBEGIN     0x00000001

#define DDCAPS_BLT              0x00000040
#define DDCAPS_BLTQUEUE         0x00000080
#define DDCAPS_PALETTEVSYNC     0x00000080
#define DDCAPS_BANKSWITCHED     0x08000000
#define DDCAPS_BLTCOLORFILL     0x40000000
#define DDCAPS_NOHARDWARE       0x00000002
#define DDCAPS_CANBLTSYSMEM     0x00000010

struct IDirectDrawSurface;
struct IDirectDrawPalette;
struct IDirectDrawClipper;

struct IDirectDraw {
    virtual HRESULT CreateSurface(LPDDSURFACEDESC, IDirectDrawSurface**, void*) { return DDERR_UNSUPPORTED; }
    virtual HRESULT SetCooperativeLevel(HWND, DWORD) { return DDERR_UNSUPPORTED; }
    virtual HRESULT SetDisplayMode(DWORD, DWORD, DWORD) { return DDERR_UNSUPPORTED; }
    virtual HRESULT CreatePalette(DWORD, PALETTEENTRY*, IDirectDrawPalette**, void*) { return DDERR_UNSUPPORTED; }
    virtual HRESULT QueryInterface(REFIID, LPVOID*) { return DDERR_UNSUPPORTED; }
    virtual HRESULT GetCaps(LPDDCAPS, LPDDCAPS) { return DDERR_UNSUPPORTED; }
    virtual HRESULT RestoreDisplayMode() { return DDERR_UNSUPPORTED; }
    virtual HRESULT WaitForVerticalBlank(DWORD, HANDLE) { return DDERR_UNSUPPORTED; }
    virtual HRESULT CreateClipper(DWORD, IDirectDrawClipper**, void*) { return DDERR_UNSUPPORTED; }
    virtual ULONG Release() { return 0; }
    virtual ~IDirectDraw() {}
};

struct IDirectDraw2 : public IDirectDraw {};

struct IDirectDrawPalette {
    virtual HRESULT SetEntries(DWORD, DWORD, DWORD, PALETTEENTRY*) { return DDERR_UNSUPPORTED; }
    virtual ULONG Release() { return 0; }
    virtual ~IDirectDrawPalette() {}
};

struct IDirectDrawClipper {
    virtual HRESULT SetHWnd(DWORD, HWND) { return DDERR_UNSUPPORTED; }
    virtual ULONG Release() { return 0; }
    virtual ~IDirectDrawClipper() {}
};

struct IDirectDrawSurface {
    virtual HRESULT GetSurfaceDesc(LPDDSURFACEDESC) { return DDERR_UNSUPPORTED; }
    virtual HRESULT SetClipper(IDirectDrawClipper*) { return DDERR_UNSUPPORTED; }
    virtual ULONG Release() { return 0; }
    virtual HRESULT GetDC(HDC*) { return DDERR_UNSUPPORTED; }
    virtual HRESULT ReleaseDC(HDC) { return DDERR_UNSUPPORTED; }
    virtual HRESULT GetAttachedSurface(LPDDSCAPS, IDirectDrawSurface**) { return DDERR_UNSUPPORTED; }
    virtual HRESULT Lock(LPRECT, LPDDSURFACEDESC, DWORD, HANDLE) { return DDERR_UNSUPPORTED; }
    virtual HRESULT Unlock(LPVOID) { return DDERR_UNSUPPORTED; }
    virtual HRESULT IsLost() { return DDERR_SURFACELOST; }
    virtual HRESULT Restore() { return DDERR_UNSUPPORTED; }
    virtual HRESULT Blt(LPRECT, IDirectDrawSurface*, LPRECT, DWORD, LPDDBLTFX) { return DDERR_UNSUPPORTED; }
    virtual HRESULT SetPalette(IDirectDrawPalette*) { return DDERR_UNSUPPORTED; }
    virtual HRESULT GetBltStatus(DWORD) { return DDERR_UNSUPPORTED; }
    virtual ~IDirectDrawSurface() {}
};

typedef IDirectDraw*         LPDIRECTDRAW;
typedef IDirectDraw2*        LPDIRECTDRAW2;
typedef IDirectDrawSurface*  LPDIRECTDRAWSURFACE;
typedef IDirectDrawPalette*  LPDIRECTDRAWPALETTE;
typedef IDirectDrawClipper*  LPDIRECTDRAWCLIPPER;

static const IID IID_IDirectDraw2 = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static inline HRESULT DirectDrawCreate(LPGUID, LPDIRECTDRAW* dd, void*) {
    if (dd) *dd = NULL;
    return DDERR_UNSUPPORTED;
}

#endif

#endif
