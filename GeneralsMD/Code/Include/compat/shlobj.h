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

static inline void CoTaskMemFree(void* p) { free(p); }

#endif // !_WIN32
#endif // ZH_COMPAT_SHLOBJ_H
