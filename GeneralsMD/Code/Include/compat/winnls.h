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

// ---------------------------------------------------------------------------
//  Locale IDs
// ---------------------------------------------------------------------------
#ifndef LOCALE_SYSTEM_DEFAULT
#define LOCALE_SYSTEM_DEFAULT  0x0800u
#define LOCALE_USER_DEFAULT    0x0400u
#define LOCALE_NEUTRAL         0x0000u
#define LOCALE_INVARIANT       0x007Fu
#endif

// ---------------------------------------------------------------------------
//  GetDateFormat / GetTimeFormat flags
// ---------------------------------------------------------------------------
#ifndef DATE_SHORTDATE
#define DATE_SHORTDATE          0x00000001u
#define DATE_LONGDATE           0x00000002u
#define DATE_USE_ALT_CALENDAR   0x00000004u
#define DATE_YEARMONTH          0x00000008u
#define DATE_LTRREADING         0x00000010u
#define DATE_RTLREADING         0x00000020u
#endif

#ifndef TIME_NOSECONDS
#define TIME_NOSECONDS          0x00000002u
#define TIME_NOTIMEMARKER       0x00000004u
#define TIME_FORCE24HOURFORMAT  0x00000008u
#endif

// ---------------------------------------------------------------------------
//  GetDateFormatA/W and GetTimeFormatA/W stubs
//  On POSIX, use strftime / wcsftime to produce locale-style date/time strings.
//  The game uses these only for save-game timestamps, so functional output
//  (not locale-perfect formatting) is sufficient.
// ---------------------------------------------------------------------------
#include <time.h>

static inline int GetDateFormatA(LCID lcid, DWORD dwFlags, const void *lpDate,
                                  LPCSTR lpFormat, LPSTR lpDateStr, int cchDate)
{
    (void)lcid; (void)dwFlags; (void)lpDate; (void)lpFormat;
    if (!lpDateStr || cchDate <= 0) return 0;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    return (int)strftime(lpDateStr, (size_t)cchDate, "%m/%d/%Y", tm_info);
}

static inline int GetDateFormatW(LCID lcid, DWORD dwFlags, const void *lpDate,
                                  LPCWSTR lpFormat, LPWSTR lpDateStr, int cchDate)
{
    (void)lcid; (void)dwFlags; (void)lpDate; (void)lpFormat;
    if (!lpDateStr || cchDate <= 0) return 0;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    return (int)wcsftime(lpDateStr, (size_t)cchDate, L"%m/%d/%Y", tm_info);
}

static inline int GetTimeFormatA(LCID lcid, DWORD dwFlags, const void *lpTime,
                                  LPCSTR lpFormat, LPSTR lpTimeStr, int cchTime)
{
    (void)lcid; (void)dwFlags; (void)lpTime; (void)lpFormat;
    if (!lpTimeStr || cchTime <= 0) return 0;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    // Honor TIME_NOSECONDS and TIME_FORCE24HOURFORMAT flags
    const char *fmt = (dwFlags & 0x00000008u) ? "%H:%M" : "%I:%M %p";
    if (dwFlags & 0x00000002u) fmt = (dwFlags & 0x00000008u) ? "%H:%M" : "%I:%M";
    return (int)strftime(lpTimeStr, (size_t)cchTime, fmt, tm_info);
}

static inline int GetTimeFormatW(LCID lcid, DWORD dwFlags, const void *lpTime,
                                  LPCWSTR lpFormat, LPWSTR lpTimeStr, int cchTime)
{
    (void)lcid; (void)dwFlags; (void)lpTime; (void)lpFormat;
    if (!lpTimeStr || cchTime <= 0) return 0;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    const wchar_t *fmt = (dwFlags & 0x00000008u) ? L"%H:%M" : L"%I:%M %p";
    if (dwFlags & 0x00000002u) fmt = (dwFlags & 0x00000008u) ? L"%H:%M" : L"%I:%M";
    return (int)wcsftime(lpTimeStr, (size_t)cchTime, fmt, tm_info);
}

#define GetDateFormat  GetDateFormatA
#define GetTimeFormat  GetTimeFormatA
#endif
#endif
