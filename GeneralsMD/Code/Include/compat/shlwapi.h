// shlwapi.h — compat shim for Shell Lightweight API
#pragma once
#ifndef ZH_COMPAT_SHLWAPI_H
#define ZH_COMPAT_SHLWAPI_H
#ifndef _WIN32
#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif
#include <string.h>
static inline BOOL PathFileExistsA(LPCSTR p){return p&&(access(p,F_OK)==0)?TRUE:FALSE;}
#define PathFileExists PathFileExistsA
static inline LPCSTR PathFindExtensionA(LPCSTR p){if(!p)return p;const char*d=strrchr(p,'.');return d?d:p+strlen(p);}
#define PathFindExtension PathFindExtensionA
static inline LPCSTR PathFindFileNameA(LPCSTR p){if(!p)return p;const char*s=strrchr(p,'/');const char*b=strrchr(p,'\\');const char*m=s>b?s:b;return m?m+1:p;}
#define PathFindFileName PathFindFileNameA
static inline BOOL PathRemoveFileSpecA(LPSTR p){if(!p)return FALSE;char*s=strrchr(p,'/');char*b=strrchr(p,'\\');char*m=s>b?s:b;if(m){*m='\0';return TRUE;}return FALSE;}
#define PathRemoveFileSpec PathRemoveFileSpecA
static inline LPSTR PathAddBackslashA(LPSTR p){if(!p)return p;size_t n=strlen(p);if(n>0&&p[n-1]!='/'&&p[n-1]!='\\'){p[n]='/';p[n+1]='\0';}return p;}
#define PathAddBackslash PathAddBackslashA
static inline BOOL PathIsRelativeA(LPCSTR p){return(!p||p[0]!='/')?TRUE:FALSE;}
#define PathIsRelative PathIsRelativeA
static inline int StrCmpIA(LPCSTR a,LPCSTR b){return strcasecmp(a?a:"",b?b:"");}
#define StrCmpI StrCmpIA
static inline int StrCmpNIA(LPCSTR a,LPCSTR b,int n){return strncasecmp(a?a:"",b?b:"",(size_t)n);}
#define StrCmpNI StrCmpNIA
#ifndef unistd_h_included_for_shlwapi
#include <unistd.h>
#define unistd_h_included_for_shlwapi
#endif
#endif
#endif
