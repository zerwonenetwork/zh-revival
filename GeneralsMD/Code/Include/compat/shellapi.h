// shellapi.h — compatibility shim for Windows Shell API
// Provides minimal stubs so the game engine compiles on Linux/macOS.
#pragma once
#ifndef ZH_COMPAT_SHELLAPI_H
#define ZH_COMPAT_SHELLAPI_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// SW_ show-window constants used by ShellExecute
#ifndef SW_SHOW
#define SW_SHOW     5
#define SW_HIDE     0
#define SW_NORMAL   1
#define SW_MINIMIZE 6
#define SW_MAXIMIZE 3
#endif

// SHELLEXECUTEINFO — used by ShellExecuteEx
typedef struct _SHELLEXECUTEINFOA {
    DWORD     cbSize;
    ULONG     fMask;
    HWND      hwnd;
    LPCSTR    lpVerb;
    LPCSTR    lpFile;
    LPCSTR    lpParameters;
    LPCSTR    lpDirectory;
    int       nShow;
    HINSTANCE hInstApp;
    void*     lpIDList;
    LPCSTR    lpClass;
    HKEY      hkeyClass;
    DWORD     dwHotKey;
    HANDLE    hIcon;
    HANDLE    hProcess;
} SHELLEXECUTEINFOA, *LPSHELLEXECUTEINFOA;
#define SHELLEXECUTEINFO  SHELLEXECUTEINFOA
#define LPSHELLEXECUTEINFO LPSHELLEXECUTEINFOA

// Stub inline implementations
static inline HINSTANCE ShellExecuteA(HWND hwnd, LPCSTR op, LPCSTR file, LPCSTR params, LPCSTR dir, int show) {
    (void)hwnd;(void)op;(void)file;(void)params;(void)dir;(void)show;
    return (HINSTANCE)0;
}
#define ShellExecute  ShellExecuteA
#define ShellExecuteW ShellExecuteA

static inline BOOL ShellExecuteExA(LPSHELLEXECUTEINFOA info) {
    (void)info; return FALSE;
}
#define ShellExecuteEx  ShellExecuteExA
#define ShellExecuteExW ShellExecuteExA

static inline HINSTANCE FindExecutableA(LPCSTR file, LPCSTR dir, LPSTR result) {
    (void)file;(void)dir; if(result) result[0]='\0'; return (HINSTANCE)0;
}
#define FindExecutable  FindExecutableA
#define FindExecutableW FindExecutableA

#endif // !_WIN32
#endif // ZH_COMPAT_SHELLAPI_H
