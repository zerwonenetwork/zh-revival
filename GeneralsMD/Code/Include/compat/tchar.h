// tchar.h — compatibility shim for MSVC <tchar.h>
// Maps TCHAR / _T() / _tcs* to narrow (char) variants on non-Windows.
#pragma once
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#ifndef _TCHAR_DEFINED
#define _TCHAR_DEFINED
typedef char TCHAR;
typedef char _TCHAR;
#endif

#ifndef _T
#define _T(x)   x
#define TEXT(x) x
#endif

#define _tcslen     strlen
#define _tcsclen    strlen  // MBCS "char count" — same as strlen in ANSI mode
#define _tcscpy     strcpy
#define _tcsncpy    strncpy
#define _tcscat     strcat
#define _tcscmp     strcmp
#define _tcsncmp    strncmp
#define _tcsicmp    strcasecmp
#define _tcsnicmp   strncasecmp
#define _tcschr     strchr
#define _tcsrchr    strrchr
#define _tcsstr     strstr
#define _tcstol     strtol
#define _tcstoul    strtoul
#define _tcstod     strtod
#define _stprintf   sprintf
#define _sntprintf  snprintf
#define _tprintf    printf
#define _ftprintf   fprintf
#define _vstprintf  vsprintf
#define _vsntprintf vsnprintf
#define _tscanf     scanf
#define _stscanf    sscanf
#define _tfopen     fopen
#define _topen      open
#define _taccess    access
