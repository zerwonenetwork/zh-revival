// windows.h — non-Windows compatibility shim
//
// Provides a minimal subset of the Windows API surface so the game engine
// source can be compiled on Linux and macOS without modification.
//
// This file is placed on the include path BEFORE system paths on non-Windows
// builds (via CMake -I compat/) so that #include <windows.h> resolves here
// instead of failing with "file not found".
//
// Coverage: types, macros, and inline stubs used throughout the SAGE engine
// and Zero Hour source tree.  DirectX, GDI, and Win32 GUI calls that require
// actual OS support are stubbed or left undefined — they will not link on
// Linux/macOS until the SDL3+DXVK layer (P5-03) is fully connected.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.

#pragma once
#ifndef ZH_COMPAT_WINDOWS_H
#define ZH_COMPAT_WINDOWS_H

#ifndef _WIN32  // entire file is a no-op on Windows (real windows.h used instead)

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>   // memset, memcpy — backing ZeroMemory / CopyMemory
#include <stdlib.h>

// ---------------------------------------------------------------------------
//  Basic integer types
//  Match Windows SDK definitions exactly (uses unsigned long for DWORD etc.)
//  so the game's own bittype.h / wintype.h don't conflict.
// ---------------------------------------------------------------------------
typedef unsigned char    BYTE;
typedef unsigned char    UCHAR;
typedef unsigned char*   PBYTE;
typedef unsigned char*   LPBYTE;
typedef const unsigned char* LPCBYTE;
typedef char             CHAR;
typedef unsigned short   WORD;
typedef short            SHORT;
typedef unsigned long    DWORD;   // matches Windows SDK + game bittype.h
typedef long             LONG;    // matches Windows SDK + game bittype.h
typedef unsigned long    ULONG;   // matches Windows SDK + game bittype.h
typedef int              INT;
typedef unsigned int     UINT;
typedef long long        LONGLONG;
typedef unsigned long long ULONGLONG;
typedef unsigned long long DWORD64;
typedef unsigned long long QWORD;
typedef int              BOOL;
typedef float            FLOAT;

// ---------------------------------------------------------------------------
//  Pointer-size integer (matches pointer width on the target)
// ---------------------------------------------------------------------------
typedef uintptr_t ULONG_PTR;
typedef uintptr_t DWORD_PTR;
typedef intptr_t  LONG_PTR;
typedef intptr_t  INT_PTR;
typedef uintptr_t UINT_PTR;
typedef uintptr_t SIZE_T;

// ---------------------------------------------------------------------------
//  MSVC intrinsic integer types
// ---------------------------------------------------------------------------
#ifndef __int8
#define __int8    char
#endif
#ifndef __int16
#define __int16   short
#endif
#ifndef __int32
#define __int32   int
#endif
#ifndef __int64
#define __int64   long long
#endif
#ifndef __uint64
#define __uint64  unsigned long long
#endif
// _int64 — used in some SAGE headers alongside __int64
typedef long long          _int64;
typedef unsigned long long _uint64;

// ---------------------------------------------------------------------------
//  Character types
// ---------------------------------------------------------------------------
typedef char      TCHAR;
typedef wchar_t   WCHAR;    // use wchar_t so wcslen/wcscpy etc. accept WCHAR*
typedef char*     LPSTR;
typedef char*     LPTSTR;
typedef const char*    LPCSTR;
typedef const char*    LPCTSTR;
typedef wchar_t*       LPWSTR;
typedef const wchar_t* LPCWSTR;
typedef BYTE*          PBYTE;
typedef WORD*          LPWORD;

// ---------------------------------------------------------------------------
//  Void pointer aliases
// ---------------------------------------------------------------------------
typedef void*       LPVOID;
typedef const void* LPCVOID;
typedef void*       PVOID;

// ---------------------------------------------------------------------------
//  Handle types (opaque pointers)
// ---------------------------------------------------------------------------
typedef void*  HANDLE;
typedef void*  HWND;
typedef void*  HMODULE;
typedef void*  HINSTANCE;
typedef void*  HDC;
typedef void*  HBITMAP;
typedef void*  HBRUSH;
typedef void*  HPEN;
typedef void*  HFONT;
typedef void*  HGDIOBJ;
// HKEY is 32-bit on all our stub builds — the legacy registry code (registry.cpp)
// stores HKEY in an int field.  Using void* causes a pointer-truncation hard error on
// 64-bit Clang.  Since stubs never use real registry handles, unsigned int is fine.
typedef unsigned int HKEY;
typedef void*  HMENU;
typedef void*  HICON;
typedef void*  HCURSOR;
typedef void*  HPALETTE;
typedef void*  HRSRC;
typedef void*  HLOCAL;
typedef void*  HGLOBAL;
typedef void*  HTASK;
typedef void*  HFILE;
typedef void*  HGLRC;  // OpenGL rendering context
typedef void*  HACCEL;
typedef HANDLE HTHREAD;
typedef HANDLE HEVENT;
typedef HANDLE HMUTEX;

// ---------------------------------------------------------------------------
//  Return/message types
// ---------------------------------------------------------------------------
typedef LONG_PTR  LRESULT;
typedef UINT_PTR  WPARAM;
typedef LONG_PTR  LPARAM;
typedef LONG      HRESULT;

// ATOM — used as return value from RegisterClass
typedef WORD ATOM;

// ---------------------------------------------------------------------------
//  GUID — used by D3D8 and COM (must be here so d3d8.h stub compiles)
// ---------------------------------------------------------------------------
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID, IID, CLSID;
typedef GUID*       LPGUID;
typedef const GUID* LPCGUID;
#ifdef __cplusplus
typedef const GUID& REFGUID;
typedef const IID&  REFIID;
typedef const CLSID& REFCLSID;
#else
typedef const GUID* REFGUID;
typedef const IID*  REFIID;
typedef const CLSID* REFCLSID;
#endif

#ifndef IsEqualGUID
#define IsEqualGUID(a,b) (memcmp(&(a),&(b),sizeof(GUID))==0)
#endif
#ifndef IsEqualIID
#define IsEqualIID(a,b)  IsEqualGUID(a,b)
#endif

// WNDPROC — must be declared before WNDCLASS which uses it as a field
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);

// ---------------------------------------------------------------------------
//  Calling conventions (no-ops on non-Windows)
// ---------------------------------------------------------------------------
#define WINAPI
#define CALLBACK
#define APIENTRY
#define STDMETHODCALLTYPE WINAPI
#define CDECL
#define PASCAL
#define FAR
#define NEAR
#define CONST     const
#define IN
#define OUT
#define OPTIONAL
#define FORCEINLINE       __attribute__((always_inline)) inline
// MSVC-specific decorated calling conventions — empty on GCC/Clang
#ifndef __cdecl
#define __cdecl
#endif
#ifndef __stdcall
#define __stdcall
#endif
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __thiscall
#define __thiscall
#endif
#ifndef _snprintf
#define _snprintf snprintf
#endif
#ifndef _vsnprintf
#define _vsnprintf vsnprintf
#endif
#ifndef __forceinline
#define __forceinline     __attribute__((always_inline)) inline
#endif
#ifndef __declspec
#define __declspec(x)
#endif

// ---------------------------------------------------------------------------
//  Boolean constants
// ---------------------------------------------------------------------------
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  NULL
// ---------------------------------------------------------------------------
#ifndef NULL
#ifdef __cplusplus
#define NULL nullptr
#else
#define NULL ((void*)0)
#endif
#endif

// ---------------------------------------------------------------------------
//  Common constants
// ---------------------------------------------------------------------------
#define MAX_PATH        260
#define MAX_COMPUTERNAME_LENGTH 15
#define _MAX_DRIVE      3
#define _MAX_DIR        256
#define _MAX_FNAME      256
#define _MAX_EXT        256
#define INFINITE        0xFFFFFFFFu
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)(-1))
#define INVALID_FILE_SIZE    0xFFFFFFFFu
#define INVALID_SET_FILE_POINTER 0xFFFFFFFFu

// ---------------------------------------------------------------------------
//  HRESULT constants
// ---------------------------------------------------------------------------
#define S_OK                  ((HRESULT)0x00000000L)
#define S_FALSE               ((HRESULT)0x00000001L)
#define E_NOTIMPL             ((HRESULT)0x80004001L)
#define E_NOINTERFACE         ((HRESULT)0x80004002L)
#define E_POINTER             ((HRESULT)0x80004003L)
#define E_ABORT               ((HRESULT)0x80004004L)
#define E_FAIL                ((HRESULT)0x80004005L)
#define E_UNEXPECTED          ((HRESULT)0x8000FFFFL)
#define E_ACCESSDENIED        ((HRESULT)0x80070005L)
#define E_HANDLE              ((HRESULT)0x80070006L)
#define E_OUTOFMEMORY         ((HRESULT)0x8007000EL)
#define E_INVALIDARG          ((HRESULT)0x80070057L)
#define SUCCEEDED(hr)         (((HRESULT)(hr)) >= 0)
#define FAILED(hr)            (((HRESULT)(hr)) < 0)
#define MAKE_HRESULT(sev,fac,code) ((HRESULT)(((DWORD)(sev)<<31)|((DWORD)(fac)<<16)|((DWORD)(code))))

// ---------------------------------------------------------------------------
//  FormatMessage flags
// ---------------------------------------------------------------------------
#define FORMAT_MESSAGE_ALLOCATE_BUFFER  0x00000100
#define FORMAT_MESSAGE_FROM_SYSTEM      0x00001000
#define FORMAT_MESSAGE_IGNORE_INSERTS   0x00000200
#define FORMAT_MESSAGE_FROM_STRING      0x00000400
#define FORMAT_MESSAGE_FROM_HMODULE     0x00000800
#define FORMAT_MESSAGE_MAX_WIDTH_MASK   0x000000FF
#define MB_OK                           0x00000000L
#define MB_ICONEXCLAMATION              0x00000030L
#define MB_ICONSTOP                     0x00000010L
#define VER_PLATFORM_WIN32s             0
#define VER_PLATFORM_WIN32_WINDOWS      1
#define VER_PLATFORM_WIN32_NT           2
#define BI_RGB                          0
#define DIB_RGB_COLORS                  0

// ---------------------------------------------------------------------------
//  File / registry / misc constants
// ---------------------------------------------------------------------------
#define GENERIC_READ                0x80000000u
#define GENERIC_WRITE               0x40000000u
#define GENERIC_EXECUTE             0x20000000u
#define GENERIC_ALL                 0x10000000u
#define FILE_SHARE_READ             0x00000001
#define FILE_SHARE_WRITE            0x00000002
#define OPEN_EXISTING               3
#define CREATE_ALWAYS               2
#define CREATE_NEW                  1
#define OPEN_ALWAYS                 4
#define FILE_ATTRIBUTE_NORMAL       0x00000080
#define FILE_ATTRIBUTE_READONLY     0x00000001
#define FILE_ATTRIBUTE_DIRECTORY    0x00000010
#define FILE_BEGIN                  0
#define FILE_CURRENT                1
#define FILE_END                    2
#define DRIVE_CDROM                 5
#define DRIVE_FIXED                 3
#define DRIVE_REMOVABLE             2
#define DRIVE_REMOTE                4
#define DRIVE_RAMDISK               6
#define DRIVE_UNKNOWN               0
#define DRIVE_NO_ROOT_DIR           1
#define PM_NOREMOVE                 0x0000
#define PM_REMOVE                   0x0001
#define WAIT_OBJECT_0               0x00000000L
#define WAIT_TIMEOUT                0x00000102L
#define WAIT_FAILED                 0xFFFFFFFFL

// WM_ message stubs (values from Windows SDK, needed by headers that define message maps)
#define WM_NULL                     0x0000
#define WM_CREATE                   0x0001
#define WM_DESTROY                  0x0002
#define WM_MOVE                     0x0003
#define WM_SIZE                     0x0005
#define WM_ACTIVATE                 0x0006
#define WM_SETFOCUS                 0x0007
#define WM_KILLFOCUS                0x0008
#define WM_PAINT                    0x000F
#define WM_CLOSE                    0x0010
#define WM_QUIT                     0x0012
#define WM_ACTIVATEAPP              0x001C
#define WM_KEYDOWN                  0x0100
#define WM_KEYUP                    0x0101
#define WM_CHAR                     0x0102
#define WM_SYSKEYDOWN               0x0104
#define WM_SYSKEYUP                 0x0105
#define WM_MOUSEMOVE                0x0200
#define WM_LBUTTONDOWN              0x0201
#define WM_LBUTTONUP                0x0202
#define WM_LBUTTONDBLCLK            0x0203
#define WM_RBUTTONDOWN              0x0204
#define WM_RBUTTONUP                0x0205
#define WM_RBUTTONDBLCLK            0x0206
#define WM_MBUTTONDOWN              0x0207
#define WM_MBUTTONUP                0x0208
#define WM_MBUTTONDBLCLK            0x0209
#define WM_MOUSEWHEEL               0x020A
#define WM_USER                     0x0400

// Virtual key codes
#define VK_BACK         0x08
#define VK_TAB          0x09
#define VK_RETURN       0x0D
#define VK_SHIFT        0x10
#define VK_CONTROL      0x11
#define VK_MENU         0x12
#define VK_CAPITAL      0x14
#define VK_ESCAPE       0x1B
#define VK_NUMLOCK      0x90
#define VK_SPACE        0x20
#define VK_LEFT         0x25
#define VK_UP           0x26
#define VK_RIGHT        0x27
#define VK_DOWN         0x28
#define VK_DELETE       0x2E
#define VK_F1           0x70

// ---------------------------------------------------------------------------
//  Win32 error codes
// ---------------------------------------------------------------------------
#define ERROR_SUCCESS             0
#define ERROR_FILE_NOT_FOUND      2
#define ERROR_PATH_NOT_FOUND      3
#define ERROR_ACCESS_DENIED       5
#define ERROR_INVALID_HANDLE      6
#define ERROR_NOT_ENOUGH_MEMORY  14
#define ERROR_ALREADY_EXISTS    183
#define ERROR_MORE_DATA         234

// ---------------------------------------------------------------------------
//  Memory macros (backed by C standard library)
// ---------------------------------------------------------------------------
#define ZeroMemory(dst, len)          memset((dst), 0, (len))
#define RtlZeroMemory(dst, len)       memset((dst), 0, (len))
#define CopyMemory(dst, src, len)     memcpy((dst), (src), (len))
#define RtlCopyMemory(dst, src, len)  memcpy((dst), (src), (len))
#define FillMemory(dst, len, val)     memset((dst), (val), (len))
#define MoveMemory(dst, src, len)     memmove((dst), (src), (len))
#define SecureZeroMemory(dst, len)    memset((dst), 0, (len))

// ---------------------------------------------------------------------------
//  NOMINMAX — suppress min/max macros to avoid conflicts with C++ stdlib
//  (GCC's <limits>/<cmath> use std::min with 3 args internally; our 2-arg
//   macros cause hard errors when those headers are included without
//   always.h having done #undef min/max first)
//
//  Game code that needs lowercase min/max gets them from:
//    - always.h (WWVegas) — defines template min/max after #undef
//    - std::min / std::max from <algorithm>
// ---------------------------------------------------------------------------
#ifndef NOMINMAX
#define NOMINMAX
#endif
// Only define min/max macros if NOMINMAX is NOT set (it is, so these are skipped)
#ifndef NOMINMAX
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#endif  // !NOMINMAX

// ---------------------------------------------------------------------------
//  MAKEWORD / MAKELONG / LOWORD / HIWORD
// ---------------------------------------------------------------------------
#define MAKEWORD(lo,hi)  ((WORD)(((BYTE)(lo))|((WORD)((BYTE)(hi)))<<8))
#define MAKELONG(lo,hi)  ((LONG)(((WORD)(lo))|((DWORD)((WORD)(hi)))<<16))
#define MAKEWPARAM(lo,hi) ((WPARAM)MAKELONG(lo,hi))
#define MAKELPARAM(lo,hi) ((LPARAM)MAKELONG(lo,hi))
#define MAKELRESULT(lo,hi) ((LRESULT)MAKELONG(lo,hi))
#define LOWORD(l)   ((WORD)(((DWORD_PTR)(l))&0xFFFF))
#define HIWORD(l)   ((WORD)((((DWORD_PTR)(l))>>16)&0xFFFF))
#define LOBYTE(w)   ((BYTE)(((DWORD_PTR)(w))&0xFF))
#define HIBYTE(w)   ((BYTE)((((DWORD_PTR)(w))>>8)&0xFF))

// ---------------------------------------------------------------------------
//  RECT, POINT, SIZE
// ---------------------------------------------------------------------------
typedef struct tagRECT  { LONG left, top, right, bottom; }  RECT,  *LPRECT,  *PRECT;
typedef struct tagPOINT { LONG x, y; }                      POINT, *LPPOINT, *PPOINT;
typedef struct tagSIZE  { LONG cx, cy; }                    SIZE,  *LPSIZE,  *PSIZE;

// ---------------------------------------------------------------------------
//  LARGE_INTEGER / ULARGE_INTEGER (used by QueryPerformanceCounter path)
// ---------------------------------------------------------------------------
typedef union _LARGE_INTEGER {
    struct { DWORD LowPart; LONG  HighPart; };
    int64_t QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    uint64_t QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

// ---------------------------------------------------------------------------
//  SYSTEMTIME
// ---------------------------------------------------------------------------
typedef struct _SYSTEMTIME {
    WORD wYear, wMonth, wDayOfWeek, wDay;
    WORD wHour, wMinute, wSecond, wMilliseconds;
} SYSTEMTIME, *LPSYSTEMTIME;

typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

typedef struct _MEMORYSTATUS {
    DWORD  dwLength;
    DWORD  dwMemoryLoad;
    SIZE_T dwTotalPhys;
    SIZE_T dwAvailPhys;
    SIZE_T dwTotalPageFile;
    SIZE_T dwAvailPageFile;
    SIZE_T dwTotalVirtual;
    SIZE_T dwAvailVirtual;
} MEMORYSTATUS, *LPMEMORYSTATUS;

typedef struct _OSVERSIONINFOA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR  szCSDVersion[128];
} OSVERSIONINFOA, OSVERSIONINFO, *LPOSVERSIONINFOA, *LPOSVERSIONINFO;

typedef struct _TIME_ZONE_INFORMATION {
    LONG       Bias;
    WCHAR      StandardName[32];
    SYSTEMTIME StandardDate;
    LONG       StandardBias;
    WCHAR      DaylightName[32];
    SYSTEMTIME DaylightDate;
    LONG       DaylightBias;
} TIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;

// ---------------------------------------------------------------------------
//  OVERLAPPED (used by async file I/O)
// ---------------------------------------------------------------------------
typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union { struct { DWORD Offset, OffsetHigh; }; PVOID Pointer; };
    HANDLE    hEvent;
} OVERLAPPED, *LPOVERLAPPED;

// ---------------------------------------------------------------------------
//  CRITICAL_SECTION stub (not real — linking to actual threading needs work)
// ---------------------------------------------------------------------------
typedef struct _CRITICAL_SECTION {
    void*  DebugInfo;
    LONG   LockCount;
    LONG   RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} CRITICAL_SECTION, *LPCRITICAL_SECTION, *PCRITICAL_SECTION;

// _WINDOWS_ — tell d3d8.h and similar stubs that windows.h is already included
// so they don't try to re-include it via #ifndef _WINDOWS_ guards
#ifndef _WINDOWS_
#define _WINDOWS_
#endif

// ---------------------------------------------------------------------------
//  Standard C includes needed by many game files
// ---------------------------------------------------------------------------
#ifdef __cplusplus
#  include <cstdio>
#  include <cstring>
#  include <cctype>
#  include <cwchar>
#  include <cstdlib>
#else
#  include <stdio.h>
#  include <string.h>
#  include <ctype.h>
#  include <wchar.h>
#  include <stdlib.h>
#endif

// ---------------------------------------------------------------------------
//  Case-insensitive string comparison (POSIX — not in <string.h> on some platforms)
// ---------------------------------------------------------------------------
#include <strings.h>   // provides strcasecmp / strncasecmp on Linux & macOS
#ifndef stricmp
#define stricmp  strcasecmp
#endif
#ifndef strnicmp
#define strnicmp strncasecmp
#endif
#ifndef _stricmp
#define _stricmp  strcasecmp
#endif
#ifndef _strnicmp
#define _strnicmp strncasecmp
#endif
#ifndef _wcsicmp
#define _wcsicmp  wcscasecmp
#endif
// strcmpi / _strcmpi — non-POSIX variants used by some WW code
#ifndef strcmpi
#define strcmpi  strcasecmp
#endif
#ifndef _strcmpi
#define _strcmpi strcasecmp
#endif

// Win32 lstr* string functions — global namespace (called as ::lstrcmpi etc.)
static inline int lstrcmpiA(LPCSTR s1, LPCSTR s2) { return strcasecmp(s1?s1:"", s2?s2:""); }
static inline int lstrcmpA(LPCSTR s1, LPCSTR s2)  { return strcmp(s1?s1:"", s2?s2:""); }
static inline LPSTR lstrcpyA(LPSTR dst, LPCSTR src) { if(dst&&src) strcpy(dst,src); return dst; }
static inline LPSTR lstrcpynA(LPSTR dst, LPCSTR src, int n) {
    if(dst&&src&&n>0){strncpy(dst,src,n-1);dst[n-1]='\0';} return dst;
}
static inline LPSTR lstrcatA(LPSTR dst, LPCSTR src) { if(dst&&src) strcat(dst,src); return dst; }
static inline int lstrlenA(LPCSTR s) { return s?(int)strlen(s):0; }
#define lstrcmpi  lstrcmpiA
#define lstrcmp   lstrcmpA
#define lstrcpy   lstrcpyA
#define lstrcpyn  lstrcpynA
#define lstrcat   lstrcatA
#define lstrlen   lstrlenA

// _strdup — MSVC alias for POSIX strdup
#ifndef _strdup
#define _strdup strdup
#endif
#ifndef _wcsdup
#define _wcsdup wcsdup
#endif

// _strlwr / _strupr — MSVC in-place case-convert functions
#include <ctype.h>
static inline char* _strlwr(char* s) {
    if (s) { for (char* p = s; *p; ++p) *p = (char)tolower((unsigned char)*p); }
    return s;
}
static inline char* _strupr(char* s) {
    if (s) { for (char* p = s; *p; ++p) *p = (char)toupper((unsigned char)*p); }
    return s;
}
// Wide-char versions — stub (return as-is)
static inline wchar_t* _wcslwr(wchar_t* s) { return s; }
static inline wchar_t* _wcsupr(wchar_t* s) { return s; }

// ---------------------------------------------------------------------------
//  Stub inline implementations — all static inline, no extern "C" needed
//  (static inline has internal linkage; no name mangling issues)
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  WNDCLASS / WNDCLASSEX (needed by any header referencing window registration)
// ---------------------------------------------------------------------------
typedef struct tagWNDCLASSA {
    UINT      style;
    WNDPROC   lpfnWndProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCSTR    lpszMenuName;
    LPCSTR    lpszClassName;
} WNDCLASSA, WNDCLASS, *LPWNDCLASSA, *LPWNDCLASS;

typedef struct tagWNDCLASSEXA {
    UINT      cbSize;
    UINT      style;
    WNDPROC   lpfnWndProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCSTR    lpszMenuName;
    LPCSTR    lpszClassName;
    HICON     hIconSm;
} WNDCLASSEXA, WNDCLASSEX, *LPWNDCLASSEXA, *LPWNDCLASSEX;

typedef struct tagMSG {
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD  time;
    POINT  pt;
} MSG, *LPMSG, *PMSG;

// ---------------------------------------------------------------------------
//  Stub inline implementations — all static inline, no extern "C" needed
// ---------------------------------------------------------------------------
#ifdef __cplusplus
#include <cstdio>
#include <cstring>
#else
#include <stdio.h>
#include <string.h>
#endif

static inline DWORD FormatMessageA(DWORD fl, LPCVOID src, DWORD id, DWORD lang, LPSTR buf, DWORD sz, va_list* args) {
    (void)fl;(void)src;(void)id;(void)lang;(void)args;
    if(buf && sz > 0) buf[0] = '\0'; return 0;
}
#define FormatMessage  FormatMessageA
#define FormatMessageW FormatMessageA

static inline ATOM    RegisterClassA(const WNDCLASSA*)    { return (ATOM)1; }
static inline ATOM    RegisterClassExA(const WNDCLASSEXA*){ return (ATOM)1; }
#define RegisterClass    RegisterClassA
#define RegisterClassEx  RegisterClassExA
static inline BOOL    UnregisterClassA(LPCSTR, HINSTANCE) { return TRUE; }
#define UnregisterClass  UnregisterClassA
static inline HWND    CreateWindowExA(DWORD,LPCSTR,LPCSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,LPVOID) { return NULL; }
#define CreateWindow(cls,wnd,sty,x,y,w,h,par,men,ins,par2) CreateWindowExA(0,cls,wnd,sty,x,y,w,h,par,men,ins,par2)
#define CreateWindowEx CreateWindowExA
static inline int     MessageBoxA(HWND,LPCSTR,LPCSTR,UINT) { return 0; }
#define MessageBox MessageBoxA
static inline BOOL    DestroyWindow(HWND h)               { (void)h; return FALSE; }
static inline BOOL    ShowWindow(HWND h, int cmd)         { (void)h;(void)cmd; return FALSE; }
static inline BOOL    UpdateWindow(HWND h)                { (void)h; return FALSE; }
// SetWindowPos flags
#define SWP_NOSIZE        0x0001
#define SWP_NOMOVE        0x0002
#define SWP_NOZORDER      0x0004
#define SWP_NOREDRAW      0x0008
#define SWP_NOACTIVATE    0x0010
#define SWP_FRAMECHANGED  0x0020
#define SWP_SHOWWINDOW    0x0040
#define SWP_HIDEWINDOW    0x0080
#define SWP_NOCOPYBITS    0x0100
#define SWP_NOOWNERZORDER 0x0200
#define SWP_NOSENDCHANGING 0x0400
#define SWP_DRAWFRAME     SWP_FRAMECHANGED
#define SWP_NOREPOSITION  SWP_NOOWNERZORDER
// HWND Z-order constants
#define HWND_TOP          ((HWND)0)
#define HWND_BOTTOM       ((HWND)1)
#define HWND_TOPMOST      ((HWND)(-1))
#define HWND_NOTOPMOST    ((HWND)(-2))
static inline BOOL    SetWindowPos(HWND a,HWND b,int c,int d,int e,int f,UINT g) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g; return FALSE; }
// Window long constants (GetWindowLong / SetWindowLong)
#define GWL_WNDPROC       (-4)
#define GWL_HINSTANCE     (-6)
#define GWL_HWNDPARENT    (-8)
#define GWL_STYLE         (-16)
#define GWL_EXSTYLE       (-20)
#define GWL_USERDATA      (-21)
#define GWL_ID            (-12)
// 64-bit GWLP variants
#define GWLP_WNDPROC      (-4)
#define GWLP_HINSTANCE    (-6)
#define GWLP_HWNDPARENT   (-8)
#define GWLP_USERDATA     (-21)
#define GWLP_ID           (-12)
// Extended window styles
#define WS_EX_TOPMOST     0x00000008L
#define WS_EX_TOOLWINDOW  0x00000080L
#define WS_EX_LAYERED     0x00080000L
// GetWindowLong / SetWindowLong stubs
static inline LONG GetWindowLongA(HWND h, int n) { (void)h;(void)n; return 0; }
static inline LONG SetWindowLongA(HWND h, int n, LONG v) { (void)h;(void)n;(void)v; return 0; }
#define GetWindowLong  GetWindowLongA
#define SetWindowLong  SetWindowLongA
static inline LONG_PTR GetWindowLongPtrA(HWND h, int n) { (void)h;(void)n; return 0; }
static inline LONG_PTR SetWindowLongPtrA(HWND h, int n, LONG_PTR v) { (void)h;(void)n;(void)v; return 0; }
#define GetWindowLongPtr  GetWindowLongPtrA
#define SetWindowLongPtr  SetWindowLongPtrA
// AdjustWindowRect stub
static inline BOOL AdjustWindowRect(LPRECT r, DWORD style, BOOL menu) { (void)style;(void)menu; return r!=NULL; }
static inline BOOL AdjustWindowRectEx(LPRECT r, DWORD style, BOOL menu, DWORD exStyle) { (void)style;(void)menu;(void)exStyle; return r!=NULL; }
// GetDesktopWindow — returns NULL stub handle
static inline HWND GetDesktopWindow(void) { return NULL; }
static inline HWND GetForegroundWindow(void) { return NULL; }
static inline HWND GetParent(HWND h) { (void)h; return NULL; }
static inline HWND SetParent(HWND h, HWND p) { (void)h;(void)p; return NULL; }
// GetDlgCtrlID
static inline int GetDlgCtrlID(HWND h) { (void)h; return 0; }
static inline HWND GetDlgItem(HWND h, int id) { (void)h;(void)id; return NULL; }
static inline BOOL EnableWindow(HWND h, BOOL e) { (void)h;(void)e; return FALSE; }
static inline BOOL IsWindowEnabled(HWND h) { (void)h; return TRUE; }
static inline BOOL IsWindowVisible(HWND h) { (void)h; return FALSE; }
static inline BOOL IsWindow(HWND h) { (void)h; return FALSE; }
static inline BOOL MoveWindow(HWND h,int x,int y,int w,int ht,BOOL rep) { (void)h;(void)x;(void)y;(void)w;(void)ht;(void)rep; return FALSE; }
static inline int  GetWindowTextA(HWND h, LPSTR b, int n) { (void)h; if(b&&n>0)b[0]=0; return 0; }
static inline int  GetWindowTextLengthA(HWND h) { (void)h; return 0; }
static inline BOOL SetWindowTextA(HWND h, LPCSTR t) { (void)h;(void)t; return FALSE; }
#define GetWindowText       GetWindowTextA
#define GetWindowTextLength GetWindowTextLengthA
#define SetWindowText       SetWindowTextA
static inline BOOL InvalidateRect(HWND h, const LPRECT r, BOOL e) { (void)h;(void)r;(void)e; return FALSE; }
static inline BOOL ValidateRect(HWND h, const LPRECT r) { (void)h;(void)r; return FALSE; }
// Display gamma ramp (needs D3D/display device, stub no-op)
typedef WORD RAMP3[256];
typedef struct tagRAMPDATA { RAMP3 red, green, blue; } RAMPDATA;
static inline BOOL SetDeviceGammaRamp(HDC hdc, LPVOID ramp) { (void)hdc;(void)ramp; return FALSE; }
static inline BOOL GetDeviceGammaRamp(HDC hdc, LPVOID ramp) { (void)hdc;(void)ramp; return FALSE; }
static inline BOOL    GetClientRect(HWND h, LPRECT r)     { (void)h; if(r){r->left=r->top=r->right=r->bottom=0;} return FALSE; }
static inline BOOL    GetWindowRect(HWND h, LPRECT r)     { (void)h; if(r){r->left=r->top=r->right=r->bottom=0;} return FALSE; }
static inline BOOL    ClientToScreen(HWND h, LPPOINT p)   { (void)h; (void)p; return FALSE; }
static inline BOOL    ScreenToClient(HWND h, LPPOINT p)   { (void)h; (void)p; return FALSE; }
static inline int     ShowCursor(BOOL bShow)              { (void)bShow; return 0; }
static inline BOOL    GetCursorPos(LPPOINT p)             { if(p){p->x=0;p->y=0;} return TRUE; }
static inline BOOL    SetCursorPos(int x, int y)          { (void)x;(void)y; return TRUE; }
static inline HCURSOR SetCursor(HCURSOR h)                { (void)h; return NULL; }
static inline HCURSOR GetCursor(void)                     { return NULL; }
static inline HCURSOR LoadCursorA(HINSTANCE h, LPCSTR n)  { (void)h;(void)n; return NULL; }
#define LoadCursor LoadCursorA
static inline BOOL    PeekMessageA(LPMSG m,HWND h,UINT a,UINT b,UINT c) { (void)m;(void)h;(void)a;(void)b;(void)c; return FALSE; }
static inline BOOL    GetMessageA(LPMSG m,HWND h,UINT a,UINT b)         { (void)m;(void)h;(void)a;(void)b; return FALSE; }
static inline BOOL    TranslateMessage(const MSG* m)      { (void)m; return FALSE; }
static inline int     TranslateAcceleratorA(HWND h, HACCEL accel, LPMSG msg) { (void)h; (void)accel; (void)msg; return 0; }
static inline BOOL    IsDialogMessageA(HWND h, LPMSG msg) { (void)h; (void)msg; return FALSE; }
static inline LRESULT DispatchMessageA(const MSG* m)      { (void)m; return 0; }
static inline LRESULT DefWindowProcA(HWND h,UINT msg,WPARAM w,LPARAM l){ (void)h;(void)msg;(void)w;(void)l; return 0; }
#define DefWindowProc DefWindowProcA
#define PeekMessage   PeekMessageA
#define GetMessage    GetMessageA
#define TranslateAccelerator TranslateAcceleratorA
#define IsDialogMessage IsDialogMessageA
#define DispatchMessage DispatchMessageA
static inline void    PostQuitMessage(int code)           { (void)code; }
static inline HMODULE GetModuleHandleA(LPCSTR n)          { (void)n; return NULL; }
#define GetModuleHandle GetModuleHandleA
typedef INT_PTR (WINAPI *FARPROC)(void);
static inline HMODULE LoadLibraryA(LPCSTR n)              { (void)n; return NULL; }
static inline FARPROC GetProcAddress(HMODULE h, LPCSTR n) { (void)h; (void)n; return NULL; }
static inline BOOL    FreeLibrary(HMODULE h)              { (void)h; return FALSE; }
#define LoadLibrary  LoadLibraryA
static inline int     LoadStringA(HINSTANCE, UINT, LPSTR buffer, int cchBufferMax) {
    if (buffer && cchBufferMax > 0) buffer[0] = '\0';
    return 0;
}
#define LoadString LoadStringA
static inline HRSRC   FindResourceA(HINSTANCE, LPCSTR, LPCSTR)        { return NULL; }
#define FindResource FindResourceA
static inline HGLOBAL LoadResource(HINSTANCE, HRSRC)                  { return NULL; }
static inline LPVOID  LockResource(HGLOBAL)                           { return NULL; }
static inline DWORD   SizeofResource(HINSTANCE, HRSRC)                { return 0; }
static inline BOOL    SetForegroundWindow(HWND h)         { (void)h; return FALSE; }
static inline HWND    GetActiveWindow(void)               { return NULL; }
static inline HWND    SetFocus_w(HWND h)                  { return h; }
#define SetFocus SetFocus_w
static inline SHORT   GetKeyState(int)                    { return 0; }
static inline SHORT   GetAsyncKeyState(int)               { return 0; }
static inline UINT    MapVirtualKeyA(UINT code, UINT maptype) { (void)maptype; return code; }
#define MapVirtualKey MapVirtualKeyA
static inline int     ToAscii(UINT virt_key, UINT scan_code, const BYTE* key_state, LPWORD translated, UINT flags) {
    (void)scan_code; (void)key_state; (void)flags;
    if (translated) {
        *translated = (WORD)(virt_key & 0xFF);
        return 1;
    }
    return 0;
}
static inline BOOL    IsWindow(HWND h)                    { (void)h; return FALSE; }
static inline BOOL    IsWindowVisible(HWND h)             { (void)h; return FALSE; }
static inline LRESULT SendMessageA(HWND h,UINT m,WPARAM w,LPARAM l){ (void)h;(void)m;(void)w;(void)l; return 0; }
#define SendMessage SendMessageA
static inline BOOL    PostMessageA(HWND h,UINT m,WPARAM w,LPARAM l){ (void)h;(void)m;(void)w;(void)l; return FALSE; }
#define PostMessage PostMessageA
static inline void    InitializeCriticalSection(LPCRITICAL_SECTION p)  { (void)p; }
static inline void    DeleteCriticalSection(LPCRITICAL_SECTION p)      { (void)p; }
static inline void    EnterCriticalSection(LPCRITICAL_SECTION p)       { (void)p; }
static inline void    LeaveCriticalSection(LPCRITICAL_SECTION p)       { (void)p; }
static inline BOOL    TryEnterCriticalSection(LPCRITICAL_SECTION p)    { (void)p; return TRUE; }
static inline void    OutputDebugStringA(LPCSTR s)                     { if(s) fprintf(stderr, "%s", s); }
static inline void    OutputDebugStringW(LPCWSTR s)                    { (void)s; }
#define OutputDebugString OutputDebugStringA
static inline DWORD   GetLastError(void)                               { return 0; }
static inline void    SetLastError(DWORD e)                            { (void)e; }
static inline DWORD   GetTickCount(void)                               { return 0; }
static inline unsigned long _lrotl(unsigned long value, int shift)     {
    unsigned bits = (unsigned)(sizeof(unsigned long) * 8);
    unsigned amount = ((unsigned)shift) & (bits - 1);
    return amount ? ((value << amount) | (value >> (bits - amount))) : value;
}
static inline void    Sleep(DWORD ms)                                  { (void)ms; }
static inline BOOL    QueryPerformanceCounter(LARGE_INTEGER* p)        { if(p) p->QuadPart=0; return TRUE; }
static inline BOOL    QueryPerformanceFrequency(LARGE_INTEGER* p)      { if(p) p->QuadPart=1000000; return TRUE; }
static inline void    GetSystemTime(SYSTEMTIME* st)                    { if(st) memset(st,0,sizeof(*st)); }
static inline void    GetLocalTime(SYSTEMTIME* st)                     { if(st) memset(st,0,sizeof(*st)); }
static inline HDC     GetDC(HWND)                                      { return NULL; }
static inline int     ReleaseDC(HWND, HDC)                             { return 0; }
static inline BOOL    DeleteObject(HGDIOBJ)                            { return TRUE; }
static inline HBITMAP CreateDIBSection(HDC, const BITMAPINFO*, UINT, void** bits, HANDLE, DWORD) {
    if (bits) *bits = NULL;
    return NULL;
}
static inline void    GlobalMemoryStatus(LPMEMORYSTATUS ms)            { if(ms) memset(ms, 0, sizeof(*ms)); }
static inline BOOL    GetVersionExA(LPOSVERSIONINFOA info)             {
    if (info) {
        memset(info, 0, sizeof(*info));
        info->dwOSVersionInfoSize = sizeof(*info);
        info->dwPlatformId = VER_PLATFORM_WIN32_NT;
    }
    return TRUE;
}
#define GetVersionEx GetVersionExA
static inline DWORD   GetTimeZoneInformation(LPTIME_ZONE_INFORMATION tz) {
    if (tz) memset(tz, 0, sizeof(*tz));
    return 0;
}
static inline int     wsprintfA(LPSTR buffer, LPCSTR format, ...) {
    int result = 0;
    if (buffer && format) {
        va_list args;
        va_start(args, format);
        result = vsprintf(buffer, format, args);
        va_end(args);
    }
    return result;
}
#define wsprintf wsprintfA
// GetWindowsDirectoryA and GetTempFileNameA defined in sysinfo stubs block below
static inline HANDLE  CreateMutexA(void* sa, BOOL initial_owner, LPCSTR name) { (void)sa; (void)initial_owner; (void)name; return (HANDLE)1; }
#define CreateMutex CreateMutexA
static inline HANDLE  CreateEventA(void*,BOOL,BOOL,LPCSTR)            { return NULL; }
#define CreateEvent CreateEventA
typedef struct _STARTUPINFOA {
    DWORD  cb;
    LPSTR  lpReserved;
    LPSTR  lpDesktop;
    LPSTR  lpTitle;
    DWORD  dwX;
    DWORD  dwY;
    DWORD  dwXSize;
    DWORD  dwYSize;
    DWORD  dwXCountChars;
    DWORD  dwYCountChars;
    DWORD  dwFillAttribute;
    DWORD  dwFlags;
    WORD   wShowWindow;
    WORD   cbReserved2;
    BYTE*  lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFOA, STARTUPINFO, *LPSTARTUPINFOA, *LPSTARTUPINFO;
typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD  dwProcessId;
    DWORD  dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
typedef struct _WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    CHAR  cFileName[MAX_PATH];
    CHAR  cAlternateFileName[14];
} WIN32_FIND_DATAA, WIN32_FIND_DATA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;
static inline HANDLE  CreateFileA(LPCSTR path, DWORD access, DWORD share, void* sa,
                                  DWORD creation, DWORD flags, HANDLE template_file) {
    (void)path; (void)access; (void)share; (void)sa; (void)creation; (void)flags; (void)template_file;
    return INVALID_HANDLE_VALUE;
}
#define CreateFile CreateFileA
static inline BOOL    DeviceIoControl(HANDLE device, DWORD code, void* in_buffer, DWORD in_size,
                                      void* out_buffer, DWORD out_size, DWORD* bytes_returned,
                                      LPOVERLAPPED overlapped) {
    (void)device; (void)code; (void)in_buffer; (void)in_size;
    (void)out_buffer; (void)out_size; (void)overlapped;
    if (bytes_returned) *bytes_returned = 0;
    return FALSE;
}
static inline BOOL    WriteFile(HANDLE h, LPCVOID buffer, DWORD bytes, DWORD* written, LPOVERLAPPED overlapped) {
    (void)h; (void)buffer; (void)bytes; (void)overlapped;
    if (written) *written = 0;
    return FALSE;
}
static inline DWORD   GetFileAttributesA(LPCSTR path)                 { (void)path; return 0xFFFFFFFFu; }
#define GetFileAttributes GetFileAttributesA
static inline BOOL    DeleteFileA(LPCSTR path)                        { (void)path; return FALSE; }
#define DeleteFile DeleteFileA
static inline BOOL    MoveFileA(LPCSTR existing_name, LPCSTR new_name){ (void)existing_name; (void)new_name; return FALSE; }
#define MoveFile MoveFileA
static inline HANDLE  FindFirstFileA(LPCSTR path, LPWIN32_FIND_DATAA info) {
    (void)path;
    if (info) memset(info, 0, sizeof(*info));
    return INVALID_HANDLE_VALUE;
}
#define FindFirstFile FindFirstFileA
static inline BOOL    FindNextFileA(HANDLE handle, LPWIN32_FIND_DATAA info) {
    (void)handle;
    if (info) memset(info, 0, sizeof(*info));
    return FALSE;
}
#define FindNextFile FindNextFileA
static inline BOOL    FindClose(HANDLE handle)                        { (void)handle; return TRUE; }
static inline void    _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext) {
    (void)path;
    if (drive) drive[0] = '\0';
    if (dir) dir[0] = '\0';
    if (fname) fname[0] = '\0';
    if (ext) ext[0] = '\0';
}
static inline BOOL    CreateProcessA(LPCSTR app, LPSTR cmd, void* proc_attr, void* thread_attr,
                                     BOOL inherit_handles, DWORD creation_flags, void* env,
                                     LPCSTR current_dir, LPSTARTUPINFOA startup, LPPROCESS_INFORMATION process) {
    (void)app; (void)cmd; (void)proc_attr; (void)thread_attr; (void)inherit_handles;
    (void)creation_flags; (void)env; (void)current_dir; (void)startup;
    if (process) memset(process, 0, sizeof(*process));
    return FALSE;
}
#define CreateProcess CreateProcessA
static inline BOOL    SetEvent(HANDLE h)                               { (void)h; return FALSE; }
static inline BOOL    ResetEvent(HANDLE h)                             { (void)h; return FALSE; }
static inline BOOL    ReleaseMutex(HANDLE h)                           { (void)h; return TRUE; }
static inline DWORD   WaitForSingleObject(HANDLE h, DWORD ms)         { (void)h;(void)ms; return 0; }
static inline BOOL    CloseHandle(HANDLE h)                            { (void)h; return TRUE; }
static inline HANDLE  GetCurrentProcess(void)                          { return NULL; }
static inline HANDLE  GetCurrentThread(void)                           { return NULL; }
static inline DWORD   GetPriorityClass(HANDLE h)                       { (void)h; return 0; }
static inline BOOL    SetPriorityClass(HANDLE h, DWORD priority)       { (void)h; (void)priority; return TRUE; }
static inline int     GetThreadPriority(HANDLE h)                      { (void)h; return 0; }
static inline BOOL    SetThreadPriority(HANDLE h, int priority)        { (void)h; (void)priority; return TRUE; }
static inline DWORD   GetCurrentThreadId(void)                        { return 1; }
static inline DWORD   GetCurrentProcessId(void)                       { return 1; }
static inline LONG    InterlockedIncrement(LONG* p)                   { return ++(*p); }
static inline LONG    InterlockedDecrement(LONG* p)                   { return --(*p); }
static inline LONG    InterlockedExchange(LONG* p, LONG v)            { LONG old=*p; *p=v; return old; }
static inline HANDLE  GetStdHandle(DWORD n)                           { (void)n; return NULL; }
static inline BOOL    WriteConsoleA(HANDLE,const void*,DWORD,DWORD*,void*) { return FALSE; }
static inline UINT    GetDriveTypeA(LPCSTR p)                         { (void)p; return 0; }
#define GetDriveType GetDriveTypeA

#ifndef REALTIME_PRIORITY_CLASS
#define REALTIME_PRIORITY_CLASS  0x00000100
#define HIGH_PRIORITY_CLASS      0x00000080
#define ABOVE_NORMAL_PRIORITY_CLASS 0x00008000
#define NORMAL_PRIORITY_CLASS    0x00000020
#define BELOW_NORMAL_PRIORITY_CLASS 0x00004000
#define IDLE_PRIORITY_CLASS      0x00000040
#endif

#ifndef THREAD_PRIORITY_TIME_CRITICAL
#define THREAD_PRIORITY_TIME_CRITICAL  15
#define THREAD_PRIORITY_HIGHEST         2
#define THREAD_PRIORITY_ABOVE_NORMAL    1
#define THREAD_PRIORITY_NORMAL          0
#define THREAD_PRIORITY_BELOW_NORMAL   (-1)
#define THREAD_PRIORITY_LOWEST         (-2)
#define THREAD_PRIORITY_IDLE           (-15)
#define THREAD_PRIORITY_ERROR_RETURN    0x7fffffff
#endif

// Thread management stubs
static inline BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode) { (void)hThread;(void)dwExitCode; return FALSE; }
static inline BOOL GetExitCodeThread(HANDLE hThread, DWORD* lpExitCode) { (void)hThread; if(lpExitCode)*lpExitCode=0; return TRUE; }
static inline DWORD SuspendThread(HANDLE hThread) { (void)hThread; return 0; }
static inline DWORD ResumeThread(HANDLE hThread)  { (void)hThread; return 0; }
static inline HANDLE CreateThread(void* lpSA, SIZE_T dwStackSize, void* lpStartAddress,
                                   void* lpParameter, DWORD dwCreationFlags, DWORD* lpThreadId) {
    (void)lpSA;(void)dwStackSize;(void)lpStartAddress;(void)lpParameter;
    (void)dwCreationFlags;(void)lpThreadId; return NULL;
}
static inline BOOL SetThreadPriorityBoost(HANDLE hThread, BOOL bDisablePriorityBoost) { (void)hThread;(void)bDisablePriorityBoost; return TRUE; }

// Mutex/semaphore stubs
static inline HANDLE OpenMutexA(DWORD desiredAccess, BOOL inheritHandle, LPCSTR name) {
    (void)desiredAccess;(void)inheritHandle;(void)name; return NULL;
}
#define OpenMutex OpenMutexA
static inline HANDLE CreateSemaphoreA(void* sa, LONG init, LONG max, LPCSTR name) {
    (void)sa;(void)init;(void)max;(void)name; return NULL;
}
#define CreateSemaphore CreateSemaphoreA
static inline BOOL ReleaseSemaphore(HANDLE h, LONG count, LONG* prev) { (void)h;(void)count;(void)prev; return FALSE; }

// ---------------------------------------------------------------------------
//  FILETIME and file-information structs (used by rawfile.cpp and others)
// ---------------------------------------------------------------------------
#ifndef _FILETIME_
#define _FILETIME_
typedef struct _FILETIME { DWORD dwLowDateTime; DWORD dwHighDateTime; } FILETIME, *PFILETIME, *LPFILETIME;
#endif

#ifndef _BY_HANDLE_FILE_INFORMATION_
#define _BY_HANDLE_FILE_INFORMATION_
typedef struct _BY_HANDLE_FILE_INFORMATION {
    DWORD    dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD    dwVolumeSerialNumber;
    DWORD    nFileSizeHigh;
    DWORD    nFileSizeLow;
    DWORD    nNumberOfLinks;
    DWORD    nFileIndexHigh;
    DWORD    nFileIndexLow;
} BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;
#endif

// ---------------------------------------------------------------------------
//  File I/O functions missing from the stubs above (need POSIX headers)
// ---------------------------------------------------------------------------
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

// ReadFile — backed by POSIX read(2)
static inline BOOL ReadFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead,
                             DWORD* lpNumberOfBytesRead, void* lpOverlapped) {
    (void)lpOverlapped;
    // hFile is stored as (fd+1) cast to pointer by our CreateFileA stub — but that stub
    // returns INVALID_HANDLE_VALUE, so on real runs this is a no-op stub.
    if (lpNumberOfBytesRead) *lpNumberOfBytesRead = 0;
    return FALSE;
}

// GetFileSize — stub (file handle is not a real fd via our CreateFileA)
static inline DWORD GetFileSize(HANDLE hFile, DWORD* lpFileSizeHigh) {
    (void)hFile;
    if (lpFileSizeHigh) *lpFileSizeHigh = 0;
    return INVALID_FILE_SIZE;
}

// SetFilePointer — stub
static inline DWORD SetFilePointer(HANDLE hFile, LONG lDistanceToMove,
                                    LONG* lpDistanceToMoveHigh, DWORD dwMoveMethod) {
    (void)hFile;(void)lDistanceToMove;(void)lpDistanceToMoveHigh;(void)dwMoveMethod;
    return INVALID_SET_FILE_POINTER;
}

// GetFileInformationByHandle — stub
static inline BOOL GetFileInformationByHandle(HANDLE hFile,
                                               BY_HANDLE_FILE_INFORMATION* lpFileInformation) {
    (void)hFile;
    if (lpFileInformation) memset(lpFileInformation, 0, sizeof(*lpFileInformation));
    return FALSE;
}

// FILETIME <-> DOS date/time conversion stubs
static inline BOOL FileTimeToDosDateTime(const FILETIME* lpFileTime,
                                          WORD* lpFatDate, WORD* lpFatTime) {
    (void)lpFileTime;
    if (lpFatDate) *lpFatDate = 0;
    if (lpFatTime) *lpFatTime = 0;
    return TRUE;
}

static inline BOOL DosDateTimeToFileTime(WORD wFatDate, WORD wFatTime,
                                          FILETIME* lpFileTime) {
    (void)wFatDate;(void)wFatTime;
    if (lpFileTime) { lpFileTime->dwLowDateTime = 0; lpFileTime->dwHighDateTime = 0; }
    return TRUE;
}

// SetFileTime — stub
static inline BOOL SetFileTime(HANDLE hFile, const FILETIME* lpCreationTime,
                                const FILETIME* lpLastAccessTime,
                                const FILETIME* lpLastWriteTime) {
    (void)hFile;(void)lpCreationTime;(void)lpLastAccessTime;(void)lpLastWriteTime;
    return FALSE;
}

// GetFileTime — stub
static inline BOOL GetFileTime(HANDLE hFile, FILETIME* lpCreationTime,
                                FILETIME* lpLastAccessTime, FILETIME* lpLastWriteTime) {
    (void)hFile;
    if (lpCreationTime)   memset(lpCreationTime,   0, sizeof(FILETIME));
    if (lpLastAccessTime) memset(lpLastAccessTime, 0, sizeof(FILETIME));
    if (lpLastWriteTime)  memset(lpLastWriteTime,  0, sizeof(FILETIME));
    return FALSE;
}

// FileTimeToSystemTime / SystemTimeToFileTime stubs
static inline BOOL FileTimeToSystemTime(const FILETIME* ft, SYSTEMTIME* st) {
    (void)ft; if (st) memset(st, 0, sizeof(*st)); return TRUE;
}
static inline BOOL SystemTimeToFileTime(const SYSTEMTIME* st, FILETIME* ft) {
    (void)st; if (ft) memset(ft, 0, sizeof(*ft)); return TRUE;
}
static inline BOOL FileTimeToLocalFileTime(const FILETIME* utc, FILETIME* local) {
    if (local && utc) *local = *utc; return TRUE;
}
static inline BOOL LocalFileTimeToFileTime(const FILETIME* local, FILETIME* utc) {
    if (utc && local) *utc = *local; return TRUE;
}

// CompareFileTime — stub
static inline LONG CompareFileTime(const FILETIME* ft1, const FILETIME* ft2) {
    if (!ft1 || !ft2) return 0;
    if (ft1->dwHighDateTime != ft2->dwHighDateTime)
        return (ft1->dwHighDateTime < ft2->dwHighDateTime) ? -1 : 1;
    if (ft1->dwLowDateTime != ft2->dwLowDateTime)
        return (ft1->dwLowDateTime < ft2->dwLowDateTime) ? -1 : 1;
    return 0;
}

// FlushFileBuffers — stub
static inline BOOL FlushFileBuffers(HANDLE hFile) { (void)hFile; return TRUE; }

// GetFileSize (64-bit) — stub
static inline DWORD GetFileSizeEx(HANDLE hFile, LARGE_INTEGER* lpFileSize) {
    (void)hFile; if (lpFileSize) lpFileSize->QuadPart = 0; return FALSE;
}

// ---------------------------------------------------------------------------
//  Pull in sub-headers that Windows code expects windows.h to implicitly cover
// ---------------------------------------------------------------------------
// winreg.h - registry API (many files include only <windows.h> but use RegOpenKeyEx etc.)
#include "winreg.h"
// winnls.h - NLS / codepage functions (MultiByteToWideChar, CP_ACP, etc.)
#include "winnls.h"
// mmsystem.h - multimedia timer (timeSetEvent/Kill, timeGetTime) — wwmouse.cpp etc.
#include "mmsystem.h"
// winnt.h - PE image structures (IMAGE_DOS_HEADER, IMAGE_FILE_HEADER, etc.)
#include "winnt.h"
// objbase.h - COM base interfaces (IUnknown, IDispatch, LPDISPATCH, CoInitialize etc.)
#include "objbase.h"

// ---------------------------------------------------------------------------
//  Control window messages (used by windowsx.h macros and direct SendMessage)
// ---------------------------------------------------------------------------
// Edit control messages
#define EM_GETSEL           0x00B0
#define EM_SETSEL           0x00B1
#define EM_GETRECT          0x00B2
#define EM_SETRECT          0x00B3
#define EM_SETLIMITTEXT     0x00C5
#define EM_LIMITTEXT        EM_SETLIMITTEXT
// Static control messages
#define STM_SETICON         0x0170
#define STM_GETICON         0x0171
#define STM_SETIMAGE        0x0172
#define STM_GETIMAGE        0x0173
// Button messages
#define BM_GETCHECK         0x00F0
#define BM_SETCHECK         0x00F1
#define BM_GETSTATE         0x00F2
#define BM_SETSTATE         0x00F3
// ListBox messages
#define LB_ADDSTRING        0x0180
#define LB_INSERTSTRING     0x0181
#define LB_DELETESTRING     0x0182
#define LB_RESETCONTENT     0x0184
#define LB_SETSEL           0x0185
#define LB_SETCURSEL        0x0186
#define LB_GETSEL           0x0187
#define LB_GETCURSEL        0x0188
#define LB_GETTEXT          0x0189
#define LB_GETTEXTLEN       0x018A
#define LB_GETCOUNT         0x018B
#define LB_SETITEMDATA      0x019A
#define LB_GETITEMDATA      0x0199
// ComboBox messages
#define CB_ADDSTRING        0x0143
#define CB_DELETESTRING     0x0144
#define CB_GETCOUNT         0x0146
#define CB_GETCURSEL        0x0147
#define CB_GETLBTEXT        0x0148
#define CB_GETLBTEXTLEN     0x0149
#define CB_INSERTSTRING     0x014A
#define CB_RESETCONTENT     0x014B
#define CB_SETCURSEL        0x014E
#define CB_SETITEMDATA      0x0151
#define CB_GETITEMDATA      0x0150

// ---------------------------------------------------------------------------
//  Undef POSIX constants that conflict with game engine enum member names.
//  These come from system <limits.h> (pulled in transitively via <ctime>).
//  The game engine uses these as enum identifiers, not as POSIX constants.
// ---------------------------------------------------------------------------
#ifdef PASS_MAX
#undef PASS_MAX   // POSIX: max passes for sysconf — conflicts with ShaderClass::DepthCompareType
#endif
#ifdef PASS_NEVER
#undef PASS_NEVER
#endif
#ifdef CHAR_MAX
// CHAR_MAX is OK — don't undef it; just undef names that clash with game enums
#endif

// ---------------------------------------------------------------------------
//  Non-underscore MSVC string helpers (used via ::strupr / ::strlwr)
//  Some WW code calls strupr/strlwr without the leading underscore.
// ---------------------------------------------------------------------------
#ifndef _zh_strupr_defined_
#define _zh_strupr_defined_
static inline char* strupr(char* s) {
    if (s) { for (char* p = s; *p; ++p) *p = (char)toupper((unsigned char)*p); }
    return s;
}
static inline char* strlwr(char* s) {
    if (s) { for (char* p = s; *p; ++p) *p = (char)tolower((unsigned char)*p); }
    return s;
}
#endif

// ---------------------------------------------------------------------------
//  System information stubs (GetComputerName, GetUserName, GetDiskFreeSpace)
// ---------------------------------------------------------------------------
#ifndef _zh_sysinfo_stubs_
#define _zh_sysinfo_stubs_
static inline BOOL GetComputerNameA(LPSTR lpBuffer, DWORD* lpnSize) {
    const char* name = "localhost";
    DWORD len = (DWORD)strlen(name);
    if (!lpBuffer || !lpnSize || *lpnSize <= len) { if(lpnSize) *lpnSize = len+1; return FALSE; }
    strcpy(lpBuffer, name); *lpnSize = len; return TRUE;
}
#define GetComputerName  GetComputerNameA
#define GetComputerNameW GetComputerNameA
static inline BOOL GetUserNameA(LPSTR lpBuffer, DWORD* lpnSize) {
    const char* name = "user";
    DWORD len = (DWORD)strlen(name);
    if (!lpBuffer || !lpnSize || *lpnSize <= len) { if(lpnSize) *lpnSize = len+1; return FALSE; }
    strcpy(lpBuffer, name); *lpnSize = len; return TRUE;
}
#define GetUserName  GetUserNameA
#define GetUserNameW GetUserNameA
// GetDiskFreeSpace — return large fake values so code thinks disk has space
static inline BOOL GetDiskFreeSpaceA(LPCSTR lpRootPathName,
                                      DWORD* lpSectorsPerCluster, DWORD* lpBytesPerSector,
                                      DWORD* lpNumberOfFreeClusters, DWORD* lpTotalNumberOfClusters) {
    (void)lpRootPathName;
    if(lpSectorsPerCluster)     *lpSectorsPerCluster     = 8;
    if(lpBytesPerSector)        *lpBytesPerSector        = 512;
    if(lpNumberOfFreeClusters)  *lpNumberOfFreeClusters  = 0x00FFFFFF;
    if(lpTotalNumberOfClusters) *lpTotalNumberOfClusters = 0x00FFFFFF;
    return TRUE;
}
#define GetDiskFreeSpace  GetDiskFreeSpaceA
#define GetDiskFreeSpaceW GetDiskFreeSpaceA
static inline BOOL GetDiskFreeSpaceExA(LPCSTR lpDirName,
                                        ULARGE_INTEGER* lpFreeBytesAvailableToCaller,
                                        ULARGE_INTEGER* lpTotalNumberOfBytes,
                                        ULARGE_INTEGER* lpTotalNumberOfFreeBytes) {
    (void)lpDirName;
    if(lpFreeBytesAvailableToCaller) lpFreeBytesAvailableToCaller->QuadPart = (uint64_t)4096*1024*1024;
    if(lpTotalNumberOfBytes)         lpTotalNumberOfBytes->QuadPart         = (uint64_t)4096*1024*1024;
    if(lpTotalNumberOfFreeBytes)     lpTotalNumberOfFreeBytes->QuadPart     = (uint64_t)4096*1024*1024;
    return TRUE;
}
#define GetDiskFreeSpaceEx  GetDiskFreeSpaceExA
#define GetDiskFreeSpaceExW GetDiskFreeSpaceExA
// GetVolumeInformation stub
static inline BOOL GetVolumeInformationA(LPCSTR lpRootPathName,
                                          LPSTR lpVolumeNameBuffer, DWORD nVolumeNameSize,
                                          DWORD* lpVolumeSerialNumber, DWORD* lpMaximumComponentLength,
                                          DWORD* lpFileSystemFlags, LPSTR lpFileSystemNameBuffer,
                                          DWORD nFileSystemNameSize) {
    (void)lpRootPathName;
    if(lpVolumeNameBuffer && nVolumeNameSize > 0) lpVolumeNameBuffer[0] = '\0';
    if(lpVolumeSerialNumber) *lpVolumeSerialNumber = 0;
    if(lpMaximumComponentLength) *lpMaximumComponentLength = 255;
    if(lpFileSystemFlags) *lpFileSystemFlags = 0;
    if(lpFileSystemNameBuffer && nFileSystemNameSize > 0) { strncpy(lpFileSystemNameBuffer,"ext4",nFileSystemNameSize-1); lpFileSystemNameBuffer[nFileSystemNameSize-1]='\0'; }
    return TRUE;
}
#define GetVolumeInformation  GetVolumeInformationA
#define GetVolumeInformationW GetVolumeInformationA
// GetModuleFileName stub
static inline DWORD GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize) {
    (void)hModule; if(lpFilename && nSize > 0) lpFilename[0] = '\0'; return 0;
}
#define GetModuleFileName  GetModuleFileNameA
#define GetModuleFileNameW GetModuleFileNameA
// WinExec stub
static inline UINT WinExec(LPCSTR lpCmdLine, UINT uCmdShow) { (void)lpCmdLine;(void)uCmdShow; return 0; }
// GetWindowsDirectory stub
static inline UINT GetWindowsDirectoryA(LPSTR lpBuffer, UINT uSize) {
    if(lpBuffer && uSize > 1) { lpBuffer[0]='/'; lpBuffer[1]='\0'; } return 1;
}
#define GetWindowsDirectory  GetWindowsDirectoryA
#define GetWindowsDirectoryW GetWindowsDirectoryA
// GetTempFileName stub
static inline UINT GetTempFileNameA(LPCSTR lpPathName, LPCSTR lpPrefixString, UINT uUnique, LPSTR lpTempFileName) {
    if(!lpTempFileName) return 0;
    snprintf(lpTempFileName, MAX_PATH, "%s/%sXXXXXX",
             lpPathName ? lpPathName : "/tmp",
             lpPrefixString ? lpPrefixString : "tmp");
    return uUnique ? uUnique : 1;
}
#define GetTempFileName  GetTempFileNameA
#define GetTempFileNameW GetTempFileNameA
// GetSystemDirectory stub
static inline UINT GetSystemDirectoryA(LPSTR lpBuffer, UINT uSize) {
    if(lpBuffer && uSize > 4) { strncpy(lpBuffer,"/usr",uSize-1); lpBuffer[uSize-1]='\0'; } return 4;
}
#define GetSystemDirectory  GetSystemDirectoryA
#define GetSystemDirectoryW GetSystemDirectoryA
// GetCurrentDirectory stub — returns process CWD
static inline DWORD GetCurrentDirectoryA(DWORD nBuf, LPSTR lpBuf) {
    if(!lpBuf||nBuf==0) return 0;
    if(getcwd(lpBuf,(size_t)nBuf)) return (DWORD)strlen(lpBuf);
    lpBuf[0]='\0'; return 0;
}
#define GetCurrentDirectory  GetCurrentDirectoryA
#define GetCurrentDirectoryW GetCurrentDirectoryA
static inline BOOL SetCurrentDirectoryA(LPCSTR lpPathName) {
    return (lpPathName && chdir(lpPathName)==0) ? TRUE : FALSE;
}
#define SetCurrentDirectory  SetCurrentDirectoryA
#define SetCurrentDirectoryW SetCurrentDirectoryA
// GetTempPath stub (if not already defined above)
#ifndef _zh_gettemppath_defined_
#define _zh_gettemppath_defined_
static inline DWORD GetTempPathA(DWORD nBuf, LPSTR lpBuf) {
    const char* t = "/tmp"; DWORD len=(DWORD)strlen(t);
    if(lpBuf && nBuf > len) { strcpy(lpBuf,t); } return len;
}
#define GetTempPath  GetTempPathA
#define GetTempPathW GetTempPathA
#endif
// GetFullPathName stub (if not already defined)
#ifndef _zh_getfullpathname_defined_
#define _zh_getfullpathname_defined_
#include <limits.h>
static inline DWORD GetFullPathNameA(LPCSTR lp, DWORD nBuf, LPSTR buf, LPSTR* part) {
    if(!buf||nBuf==0) return 0;
    char resolved[PATH_MAX];
    if(!realpath(lp,resolved)) strncpy(resolved,lp,PATH_MAX-1);
    strncpy(buf,resolved,nBuf-1); buf[nBuf-1]='\0';
    if(part){char*s=strrchr(buf,'/');*part=s?s+1:buf;}
    return (DWORD)strlen(buf);
}
#define GetFullPathName  GetFullPathNameA
#define GetFullPathNameW GetFullPathNameA
#endif
// LoadLibrary stubs (if not already defined)
#ifndef _zh_loadlibrary_defined_
#define _zh_loadlibrary_defined_
static inline HMODULE LoadLibraryExA(LPCSTR n, HANDLE h, DWORD f) { (void)n;(void)h;(void)f; return NULL; }
#define LoadLibraryEx  LoadLibraryExA
#define LoadLibraryExW LoadLibraryExA
#define LoadLibraryW   LoadLibraryA
#endif
#endif // _zh_sysinfo_stubs_

// ---------------------------------------------------------------------------
//  Wide-char printf helpers — map MSVC-specific names to POSIX equivalents
// ---------------------------------------------------------------------------
#ifndef _vsnwprintf
#define _vsnwprintf vswprintf
#endif
#ifndef _snwprintf
#define _snwprintf  swprintf
#endif
#ifndef _wcsnicmp
#define _wcsnicmp   wcsncasecmp
#endif
#ifndef _wcsicmp
#define _wcsicmp    wcscasecmp
#endif
// wcsncasecmp / wcscasecmp may need explicit declaration on some POSIX targets
#include <wchar.h>

// ---------------------------------------------------------------------------
//  Global memory functions (used by verchk.cpp and other Win32 code)
// ---------------------------------------------------------------------------
#ifndef GMEM_MOVEABLE
#define GMEM_MOVEABLE   0x0002
#define GMEM_ZEROINIT   0x0040
#define GHND            (GMEM_MOVEABLE | GMEM_ZEROINIT)
#define GPTR            0x0040
#endif
static inline HGLOBAL GlobalAlloc(UINT uFlags, SIZE_T dwBytes) { (void)uFlags; return malloc(dwBytes); }
static inline LPVOID  GlobalLock(HGLOBAL hMem)                 { return hMem; }
static inline BOOL    GlobalUnlock(HGLOBAL hMem)               { (void)hMem; return TRUE; }
static inline HGLOBAL GlobalFree(HGLOBAL hMem)                 { free(hMem); return NULL; }
static inline SIZE_T  GlobalSize(HGLOBAL hMem)                 { (void)hMem; return 0; }

static inline HLOCAL LocalAlloc(UINT uFlags, SIZE_T uBytes) { (void)uFlags; return malloc(uBytes); }
static inline LPVOID LocalLock(HLOCAL hMem)                 { return hMem; }
static inline BOOL   LocalUnlock(HLOCAL hMem)               { (void)hMem; return TRUE; }
static inline HLOCAL LocalFree(HLOCAL hMem)                 { free(hMem); return NULL; }

// ---------------------------------------------------------------------------
//  Version-info structures and stubs (verchk.cpp, versioncheck.cpp)
// ---------------------------------------------------------------------------
#ifndef _VS_FIXEDFILEINFO_
#define _VS_FIXEDFILEINFO_
typedef struct tagVS_FIXEDFILEINFO {
    DWORD dwSignature;
    DWORD dwStrucVersion;
    DWORD dwFileVersionMS;
    DWORD dwFileVersionLS;
    DWORD dwProductVersionMS;
    DWORD dwProductVersionLS;
    DWORD dwFileFlagsMask;
    DWORD dwFileFlags;
    DWORD dwFileOS;
    DWORD dwFileType;
    DWORD dwFileSubtype;
    DWORD dwFileDateMS;
    DWORD dwFileDateLS;
} VS_FIXEDFILEINFO;
#endif

static inline DWORD GetFileVersionInfoSizeA(LPCSTR lptstrFilename, DWORD* lpdwHandle) {
    (void)lptstrFilename; if(lpdwHandle) *lpdwHandle = 0; return 0;
}
#define GetFileVersionInfoSize  GetFileVersionInfoSizeA
#define GetFileVersionInfoSizeW GetFileVersionInfoSizeA

static inline BOOL GetFileVersionInfoA(LPCSTR lptstrFilename, DWORD dwHandle,
                                        DWORD dwLen, LPVOID lpData) {
    (void)lptstrFilename;(void)dwHandle;(void)dwLen;(void)lpData; return FALSE;
}
#define GetFileVersionInfo  GetFileVersionInfoA
#define GetFileVersionInfoW GetFileVersionInfoA

static inline BOOL VerQueryValueA(LPCVOID pBlock, LPCSTR lpSubBlock,
                                   LPVOID* lplpBuffer, UINT* puLen) {
    (void)pBlock;(void)lpSubBlock;
    if(lplpBuffer) *lplpBuffer = NULL;
    if(puLen) *puLen = 0;
    return FALSE;
}
#define VerQueryValue  VerQueryValueA
#define VerQueryValueW VerQueryValueA

// ---------------------------------------------------------------------------
//  Clipboard stubs (some UI code touches these)
// ---------------------------------------------------------------------------
#ifndef CF_TEXT
#define CF_TEXT         1
#define CF_BITMAP       2
#define CF_UNICODETEXT  13
#endif
static inline BOOL OpenClipboard(HWND hWnd)        { (void)hWnd; return FALSE; }
static inline BOOL CloseClipboard(void)            { return FALSE; }
static inline BOOL EmptyClipboard(void)            { return FALSE; }
static inline HANDLE GetClipboardData(UINT uFmt)   { (void)uFmt; return NULL; }
static inline HANDLE SetClipboardData(UINT uFmt, HANDLE hMem) { (void)uFmt;(void)hMem; return NULL; }

#endif  // !_WIN32
#endif  // ZH_COMPAT_WINDOWS_H

