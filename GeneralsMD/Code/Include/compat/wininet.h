// wininet.h — compat shim for Windows Internet API
#pragma once
#ifndef ZH_COMPAT_WININET_H
#define ZH_COMPAT_WININET_H
#ifndef _WIN32
#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif
typedef void* HINTERNET;
#define INTERNET_OPEN_TYPE_DIRECT     1
#define INTERNET_OPEN_TYPE_PRECONFIG  0
#define INTERNET_FLAG_RELOAD          0x80000000
#define INTERNET_FLAG_NO_CACHE_WRITE  0x04000000
#define INTERNET_SERVICE_HTTP         3
#define HTTP_QUERY_STATUS_CODE        19
#define HTTP_QUERY_FLAG_NUMBER        0x20000000
static inline HINTERNET InternetOpenA(LPCSTR a,DWORD b,LPCSTR c,LPCSTR d,DWORD e){(void)a;(void)b;(void)c;(void)d;(void)e;return NULL;}
#define InternetOpen InternetOpenA
static inline HINTERNET InternetConnectA(HINTERNET a,LPCSTR b,WORD c,LPCSTR d,LPCSTR e,DWORD f,DWORD g,DWORD_PTR h){(void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;return NULL;}
#define InternetConnect InternetConnectA
static inline BOOL InternetCloseHandle(HINTERNET h){(void)h;return FALSE;}
static inline HINTERNET HttpOpenRequestA(HINTERNET a,LPCSTR b,LPCSTR c,LPCSTR d,LPCSTR e,LPCSTR* f,DWORD g,DWORD_PTR h){(void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g;(void)h;return NULL;}
#define HttpOpenRequest HttpOpenRequestA
static inline BOOL HttpSendRequestA(HINTERNET a,LPCSTR b,DWORD c,void* d,DWORD e){(void)a;(void)b;(void)c;(void)d;(void)e;return FALSE;}
#define HttpSendRequest HttpSendRequestA
static inline BOOL HttpQueryInfoA(HINTERNET a,DWORD b,void* c,DWORD* d,DWORD* e){(void)a;(void)b;(void)c;(void)d;(void)e;return FALSE;}
#define HttpQueryInfo HttpQueryInfoA
static inline BOOL InternetReadFile(HINTERNET a,void* b,DWORD c,DWORD* d){(void)a;(void)b;(void)c;if(d)*d=0;return FALSE;}
#endif
#endif
