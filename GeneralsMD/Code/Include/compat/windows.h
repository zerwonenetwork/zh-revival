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
typedef void*  HKEY;
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
//  Min / Max (some game headers use these without including algorithm)
// ---------------------------------------------------------------------------
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

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
static inline BOOL    SetWindowPos(HWND a,HWND b,int c,int d,int e,int f,UINT g) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g; return FALSE; }
static inline BOOL    GetClientRect(HWND h, LPRECT r)     { (void)h; if(r){r->left=r->top=r->right=r->bottom=0;} return FALSE; }
static inline BOOL    GetWindowRect(HWND h, LPRECT r)     { (void)h; if(r){r->left=r->top=r->right=r->bottom=0;} return FALSE; }
static inline BOOL    ClientToScreen(HWND h, LPPOINT p)   { (void)h; (void)p; return FALSE; }
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
static inline DWORD   GetWindowsDirectoryA(LPSTR path, DWORD size)     {
    if (path && size > 0) { path[0] = '.'; if (size > 1) path[1] = '\0'; }
    return 1;
}
#define GetWindowsDirectory GetWindowsDirectoryA
static inline UINT    GetTempFileNameA(LPCSTR path, LPCSTR prefix, UINT unique, LPSTR buffer) {
    (void)path; (void)prefix; (void)unique;
    if (buffer) {
        strcpy(buffer, "zh_compat.tmp");
        return 1;
    }
    return 0;
}
#define GetTempFileName GetTempFileNameA
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
#define REALTIME_PRIORITY_CLASS 0x00000100
#endif

#ifndef THREAD_PRIORITY_TIME_CRITICAL
#define THREAD_PRIORITY_TIME_CRITICAL 15
#endif

#endif  // !_WIN32
#endif  // ZH_COMPAT_WINDOWS_H

