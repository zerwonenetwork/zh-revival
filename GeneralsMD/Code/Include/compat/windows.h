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
#include <string.h>   // memset, memcpy — backing ZeroMemory / CopyMemory
#include <stdlib.h>

// ---------------------------------------------------------------------------
//  Basic integer types
// ---------------------------------------------------------------------------
typedef uint8_t   BYTE;
typedef uint8_t   UCHAR;
typedef int8_t    CHAR;
typedef uint16_t  WORD;
typedef int16_t   SHORT;
typedef uint32_t  DWORD;
typedef int32_t   LONG;
typedef uint32_t  ULONG;
typedef int32_t   INT;
typedef uint32_t  UINT;
typedef uint64_t  LONGLONG;
typedef uint64_t  ULONGLONG;
typedef uint64_t  DWORD64;
typedef uint64_t  QWORD;
typedef int       BOOL;
typedef float     FLOAT;

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
//  Character types
// ---------------------------------------------------------------------------
typedef char      TCHAR;
typedef uint16_t  WCHAR;
typedef char*     LPSTR;
typedef char*     LPTSTR;
typedef const char*    LPCSTR;
typedef const char*    LPCTSTR;
typedef uint16_t* LPWSTR;
typedef const uint16_t* LPCWSTR;

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

// WNDPROC — must be declared before WNDCLASS which uses it as a field
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);

// ---------------------------------------------------------------------------
//  Calling conventions (no-ops on non-Windows)
// ---------------------------------------------------------------------------
#define WINAPI
#define CALLBACK
#define APIENTRY
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
#define WM_RBUTTONDOWN              0x0204
#define WM_RBUTTONUP                0x0205
#define WM_MBUTTONDOWN              0x0207
#define WM_MBUTTONUP                0x0208
#define WM_MOUSEWHEEL               0x020A
#define WM_USER                     0x0400

// Virtual key codes
#define VK_BACK         0x08
#define VK_TAB          0x09
#define VK_RETURN       0x0D
#define VK_SHIFT        0x10
#define VK_CONTROL      0x11
#define VK_MENU         0x12
#define VK_ESCAPE       0x1B
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
    struct { DWORD LowPart; LONG  HighPart; } s;
    int64_t QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; } s;
    uint64_t QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

// ---------------------------------------------------------------------------
//  SYSTEMTIME
// ---------------------------------------------------------------------------
typedef struct _SYSTEMTIME {
    WORD wYear, wMonth, wDayOfWeek, wDay;
    WORD wHour, wMinute, wSecond, wMilliseconds;
} SYSTEMTIME, *LPSYSTEMTIME;

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

// ---------------------------------------------------------------------------
//  Stub inline implementations — all static inline, no extern "C" needed
//  (static inline has internal linkage; no name mangling issues)
// ---------------------------------------------------------------------------
#ifdef __cplusplus
#  include <cstdio>
#  include <cstring>
#else
#  include <stdio.h>
#  include <string.h>
#endif

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
static inline BOOL    DestroyWindow(HWND h)               { (void)h; return FALSE; }
static inline BOOL    ShowWindow(HWND h, int cmd)         { (void)h;(void)cmd; return FALSE; }
static inline BOOL    UpdateWindow(HWND h)                { (void)h; return FALSE; }
static inline BOOL    SetWindowPos(HWND a,HWND b,int c,int d,int e,int f,UINT g) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g; return FALSE; }
static inline BOOL    GetClientRect(HWND h, LPRECT r)     { (void)h; if(r){r->left=r->top=r->right=r->bottom=0;} return FALSE; }
static inline BOOL    GetWindowRect(HWND h, LPRECT r)     { (void)h; if(r){r->left=r->top=r->right=r->bottom=0;} return FALSE; }
static inline BOOL    PeekMessageA(LPMSG m,HWND h,UINT a,UINT b,UINT c) { (void)m;(void)h;(void)a;(void)b;(void)c; return FALSE; }
static inline BOOL    GetMessageA(LPMSG m,HWND h,UINT a,UINT b)         { (void)m;(void)h;(void)a;(void)b; return FALSE; }
static inline BOOL    TranslateMessage(const MSG* m)      { (void)m; return FALSE; }
static inline LRESULT DispatchMessageA(const MSG* m)      { (void)m; return 0; }
static inline LRESULT DefWindowProcA(HWND h,UINT msg,WPARAM w,LPARAM l){ (void)h;(void)msg;(void)w;(void)l; return 0; }
#define DefWindowProc DefWindowProcA
#define PeekMessage   PeekMessageA
#define GetMessage    GetMessageA
#define DispatchMessage DispatchMessageA
static inline void    PostQuitMessage(int code)           { (void)code; }
static inline HMODULE GetModuleHandleA(LPCSTR n)          { (void)n; return NULL; }
#define GetModuleHandle GetModuleHandleA
static inline BOOL    SetForegroundWindow(HWND h)         { (void)h; return FALSE; }
static inline HWND    SetFocus_w(HWND h)                  { return h; }
#define SetFocus SetFocus_w
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
static inline void    Sleep(DWORD ms)                                  { (void)ms; }
static inline BOOL    QueryPerformanceCounter(LARGE_INTEGER* p)        { if(p) p->QuadPart=0; return TRUE; }
static inline BOOL    QueryPerformanceFrequency(LARGE_INTEGER* p)      { if(p) p->QuadPart=1000000; return TRUE; }
static inline void    GetSystemTime(SYSTEMTIME* st)                    { if(st) memset(st,0,sizeof(*st)); }
static inline void    GetLocalTime(SYSTEMTIME* st)                     { if(st) memset(st,0,sizeof(*st)); }
static inline HANDLE  CreateEventA(void*,BOOL,BOOL,LPCSTR)            { return NULL; }
#define CreateEvent CreateEventA
static inline BOOL    SetEvent(HANDLE h)                               { (void)h; return FALSE; }
static inline BOOL    ResetEvent(HANDLE h)                             { (void)h; return FALSE; }
static inline DWORD   WaitForSingleObject(HANDLE h, DWORD ms)         { (void)h;(void)ms; return 0; }
static inline BOOL    CloseHandle(HANDLE h)                            { (void)h; return TRUE; }
static inline DWORD   GetCurrentThreadId(void)                        { return 1; }
static inline DWORD   GetCurrentProcessId(void)                       { return 1; }
static inline LONG    InterlockedIncrement(LONG* p)                   { return ++(*p); }
static inline LONG    InterlockedDecrement(LONG* p)                   { return --(*p); }
static inline LONG    InterlockedExchange(LONG* p, LONG v)            { LONG old=*p; *p=v; return old; }
static inline HANDLE  GetStdHandle(DWORD n)                           { (void)n; return NULL; }
static inline BOOL    WriteConsoleA(HANDLE,const void*,DWORD,DWORD*,void*) { return FALSE; }
static inline UINT    GetDriveTypeA(LPCSTR p)                         { (void)p; return 0; }
#define GetDriveType GetDriveTypeA

#endif  // !_WIN32
#endif  // ZH_COMPAT_WINDOWS_H
