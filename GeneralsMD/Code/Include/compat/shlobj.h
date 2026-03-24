// shlobj.h — compatibility shim for Windows Shell Object API
// Provides minimal stubs for SHBrowseForFolder, SHGetPathFromIDList etc.
#pragma once
#ifndef ZH_COMPAT_SHLOBJ_H
#define ZH_COMPAT_SHLOBJ_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// ITEMIDLIST — opaque shell ID list
typedef struct _ITEMIDLIST { struct { unsigned short cb; } mkid; } ITEMIDLIST;
typedef ITEMIDLIST* LPITEMIDLIST;
typedef const ITEMIDLIST* LPCITEMIDLIST;

// BROWSEINFO — used by SHBrowseForFolder
typedef int (CALLBACK* BFFCALLBACK)(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
typedef struct _browseinfoa {
    HWND          hwndOwner;
    LPCITEMIDLIST pidlRoot;
    LPSTR         pszDisplayName;
    LPCSTR        lpszTitle;
    UINT          ulFlags;
    BFFCALLBACK   lpfn;
    LPARAM        lParam;
    int           iImage;
} BROWSEINFOA, *PBROWSEINFOA, *LPBROWSEINFOA;
#define BROWSEINFO  BROWSEINFOA
#define LPBROWSEINFO LPBROWSEINFOA

// BIF_ flags
#define BIF_RETURNONLYFSDIRS  0x0001
#define BIF_DONTGOBELOWDOMAIN 0x0002
#define BIF_STATUSTEXT        0x0004
#define BIF_RETURNFSANCESTORS 0x0008
#define BIF_EDITBOX           0x0010
#define BIF_NEWDIALOGSTYLE    0x0040
#define BIF_BROWSEINCLUDEFILES 0x4000

// Stubs
static inline LPITEMIDLIST SHBrowseForFolderA(LPBROWSEINFOA bi) { (void)bi; return NULL; }
#define SHBrowseForFolder  SHBrowseForFolderA
#define SHBrowseForFolderW SHBrowseForFolderA

static inline BOOL SHGetPathFromIDListA(LPCITEMIDLIST pidl, LPSTR path) {
    (void)pidl; if(path) path[0]='\0'; return FALSE;
}
#define SHGetPathFromIDList  SHGetPathFromIDListA
#define SHGetPathFromIDListW SHGetPathFromIDListA

// CoTaskMemFree is already defined in objbase.h (via #define CoTaskMemFree CoTaskMemFree_base).
// Only define it here if objbase.h hasn't been included yet.
#ifndef CoTaskMemFree
static inline void CoTaskMemFree_shlobj_(void* p) { free(p); }
#define CoTaskMemFree CoTaskMemFree_shlobj_
#endif

// ── CSIDL constants (special folder identifiers) ───────────────────────────
#ifndef CSIDL_DESKTOP
#define CSIDL_DESKTOP                 0x0000
#define CSIDL_INTERNET                0x0001
#define CSIDL_PROGRAMS                0x0002
#define CSIDL_CONTROLS                0x0003
#define CSIDL_PRINTERS                0x0004
#define CSIDL_PERSONAL                0x0005   // "My Documents"
#define CSIDL_FAVORITES               0x0006
#define CSIDL_STARTUP                 0x0007
#define CSIDL_RECENT                  0x0008
#define CSIDL_SENDTO                  0x0009
#define CSIDL_BITBUCKET               0x000A
#define CSIDL_STARTMENU               0x000B
#define CSIDL_MYDOCUMENTS             CSIDL_PERSONAL
#define CSIDL_MYMUSIC                 0x000D
#define CSIDL_MYVIDEO                 0x000E
#define CSIDL_DESKTOPDIRECTORY        0x0010
#define CSIDL_DRIVES                  0x0011
#define CSIDL_NETWORK                 0x0012
#define CSIDL_NETHOOD                 0x0013
#define CSIDL_FONTS                   0x0014
#define CSIDL_TEMPLATES               0x0015
#define CSIDL_COMMON_STARTMENU        0x0016
#define CSIDL_COMMON_PROGRAMS         0x0017
#define CSIDL_COMMON_STARTUP          0x0018
#define CSIDL_COMMON_DESKTOPDIRECTORY 0x0019
#define CSIDL_APPDATA                 0x001A
#define CSIDL_PRINTHOOD               0x001B
#define CSIDL_LOCAL_APPDATA           0x001C
#define CSIDL_ALTSTARTUP              0x001D
#define CSIDL_COMMON_ALTSTARTUP       0x001E
#define CSIDL_COMMON_FAVORITES        0x001F
#define CSIDL_INTERNET_CACHE          0x0020
#define CSIDL_COOKIES                 0x0021
#define CSIDL_HISTORY                 0x0022
#define CSIDL_COMMON_APPDATA          0x0023
#define CSIDL_WINDOWS                 0x0024
#define CSIDL_SYSTEM                  0x0025
#define CSIDL_PROGRAM_FILES           0x0026
#define CSIDL_MYPICTURES              0x0027
#define CSIDL_PROFILE                 0x0028
#define CSIDL_PROGRAM_FILES_COMMON    0x002B
#define CSIDL_COMMON_TEMPLATES        0x002D
#define CSIDL_COMMON_DOCUMENTS        0x002E
#define CSIDL_FLAG_CREATE             0x8000
#endif // CSIDL_DESKTOP

// ── SHGetSpecialFolderPath stub ─────────────────────────────────────────────
// On non-Windows, return the user's home directory for CSIDL_PERSONAL,
// and /tmp for everything else.
#ifndef SHGetSpecialFolderPath
#include <stdlib.h>
#include <string.h>
static inline BOOL SHGetSpecialFolderPathA(HWND hwnd, LPSTR pszPath, int nFolder, BOOL fCreate) {
    (void)hwnd; (void)fCreate;
    if (!pszPath) return FALSE;
    const char* home = getenv("HOME");
    if (!home) home = "/tmp";
    // For CSIDL_PERSONAL (My Documents) and similar, use $HOME
    strncpy(pszPath, home, MAX_PATH - 1);
    pszPath[MAX_PATH - 1] = '\0';
    (void)nFolder; // treat all folders as home for stub purposes
    return TRUE;
}
#define SHGetSpecialFolderPath  SHGetSpecialFolderPathA
#define SHGetSpecialFolderPathW SHGetSpecialFolderPathA
#endif // SHGetSpecialFolderPath

#endif // !_WIN32
#endif // ZH_COMPAT_SHLOBJ_H
