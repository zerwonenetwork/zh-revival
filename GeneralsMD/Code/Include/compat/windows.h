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
#define FORCEINLINE  __attribute__((always_inline)) inline

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
//  Stub inline implementations for critical-section and basic Win32 calls
//  that are used in headers (e.g. inline functions in game headers)
// ---------------------------------------------------------------------------
#ifdef __cplusplus
#include <cstdio>
extern "C" {
#endif

static inline void  InitializeCriticalSection(LPCRITICAL_SECTION)     {}
static inline void  DeleteCriticalSection(LPCRITICAL_SECTION)         {}
static inline void  EnterCriticalSection(LPCRITICAL_SECTION)          {}
static inline void  LeaveCriticalSection(LPCRITICAL_SECTION)          {}
static inline BOOL  TryEnterCriticalSection(LPCRITICAL_SECTION)       { return TRUE; }
static inline void  OutputDebugStringA(LPCSTR)                        {}
static inline void  OutputDebugStringW(LPCWSTR)                       {}
static inline DWORD GetLastError(void)                                { return 0; }
static inline void  SetLastError(DWORD)                               {}
static inline DWORD GetTickCount(void)                                { return 0; }
static inline void  Sleep(DWORD)                                      {}
static inline BOOL  QueryPerformanceCounter(LARGE_INTEGER* p)         { if(p) p->QuadPart=0; return TRUE; }
static inline BOOL  QueryPerformanceFrequency(LARGE_INTEGER* p)       { if(p) p->QuadPart=1000000; return TRUE; }

#ifdef __cplusplus
}
#endif

// ---------------------------------------------------------------------------
//  OutputDebugString alias
// ---------------------------------------------------------------------------
#define OutputDebugString OutputDebugStringA

// ---------------------------------------------------------------------------
//  WNDPROC type (not functional on non-Windows, just satisfies the type system)
// ---------------------------------------------------------------------------
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);

#endif  // !_WIN32
#endif  // ZH_COMPAT_WINDOWS_H
