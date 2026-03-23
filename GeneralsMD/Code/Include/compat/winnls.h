// winnls.h — compat shim for Windows NLS (National Language Support)
#pragma once
#ifndef ZH_COMPAT_WINNLS_H
#define ZH_COMPAT_WINNLS_H
#ifndef _WIN32
#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif
// Types must come before functions that return them
typedef DWORD LCID;
typedef WORD  LANGID;

#define CP_ACP      0
#define CP_UTF8     65001
#define CP_UTF7     65000
#define CP_OEMCP    1
#define MB_PRECOMPOSED    0x00000001
#define MB_COMPOSITE      0x00000002
#define MB_ERR_INVALID_CHARS 0x00000008
#define WC_COMPOSITECHECK 0x00000200
#define WC_NO_BEST_FIT_CHARS 0x00000400

#include <string.h>
#include <wchar.h>

static inline int MultiByteToWideChar(UINT cp,DWORD flags,LPCSTR mb,int mbc,LPWSTR wc,int wcc){
    (void)cp;(void)flags;
    if(!mb)return 0;
    int len=(mbc<0)?(int)strlen(mb)+1:mbc;
    if(!wc||wcc==0)return len;
    int n=len<wcc?len:wcc-1;
    for(int i=0;i<n;i++)wc[i]=(wchar_t)(unsigned char)mb[i];
    if(wcc>n)wc[n]=0;
    return n;
}
static inline int WideCharToMultiByte(UINT cp,DWORD flags,LPCWSTR wc,int wcc,LPSTR mb,int mbc,LPCSTR defchar,BOOL* used){
    (void)cp;(void)flags;(void)defchar;(void)used;
    if(!wc)return 0;
    int len=(wcc<0)?(int)wcslen(wc)+1:wcc;
    if(!mb||mbc==0)return len;
    int n=len<mbc?len:mbc-1;
    for(int i=0;i<n;i++)mb[i]=(char)(wc[i]&0x7F);
    if(mbc>n)mb[n]=0;
    return n;
}
static inline LCID GetSystemDefaultLCID(void){return 0x0409;}
static inline LCID GetUserDefaultLCID(void){return 0x0409;}
static inline LANGID GetSystemDefaultLangID(void){return 0x0409;}
static inline LANGID GetUserDefaultLangID(void){return 0x0409;}
#endif
#endif
