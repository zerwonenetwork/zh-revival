// winreg.h — compat shim for Windows Registry API
#pragma once
#ifndef ZH_COMPAT_WINREG_H
#define ZH_COMPAT_WINREG_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// Registry key type
typedef LONG LSTATUS;

// Predefined root keys — cast directly to HKEY (unsigned int) to avoid
// pointer-truncation errors on 64-bit Clang (no LONG_PTR intermediate)
#define HKEY_CLASSES_ROOT   ((HKEY)0x80000000u)
#define HKEY_CURRENT_USER   ((HKEY)0x80000001u)
#define HKEY_LOCAL_MACHINE  ((HKEY)0x80000002u)
#define HKEY_USERS          ((HKEY)0x80000003u)
#define HKEY_CURRENT_CONFIG ((HKEY)0x80000005u)

// Registry value types
#define REG_NONE        0
#define REG_SZ          1
#define REG_EXPAND_SZ   2
#define REG_BINARY      3
#define REG_DWORD       4
#define REG_DWORD_LE    4
#define REG_DWORD_BE    5
#define REG_MULTI_SZ    7
#define REG_QWORD       11

// Registry access rights
#define KEY_QUERY_VALUE    0x0001
#define KEY_SET_VALUE      0x0002
#define KEY_CREATE_SUB_KEY 0x0004
#define KEY_ENUMERATE_SUB_KEYS 0x0008
#define KEY_NOTIFY         0x0010
#define KEY_READ           0x20019
#define KEY_WRITE          0x20006
#define KEY_ALL_ACCESS     0xF003F

// Registry options
#define REG_OPTION_NON_VOLATILE  0
#define REG_OPTION_VOLATILE      1
#define REG_CREATED_NEW_KEY      1
#define REG_OPENED_EXISTING_KEY  2

// Error codes for registry (already in windows.h but repeated for clarity)
#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif
#define ERROR_NO_MORE_ITEMS 259

// Stub inline implementations — all return failure / empty results
static inline LSTATUS RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions,
                                    DWORD samDesired, HKEY* phkResult) {
    (void)hKey;(void)lpSubKey;(void)ulOptions;(void)samDesired;
    if(phkResult) *phkResult = 0;
    return (LSTATUS)ERROR_FILE_NOT_FOUND;
}
#define RegOpenKeyEx  RegOpenKeyExA
#define RegOpenKeyExW RegOpenKeyExA

static inline LSTATUS RegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved,
                                       LPSTR lpClass, DWORD dwOptions, DWORD samDesired,
                                       void* lpSecurityAttributes, HKEY* phkResult,
                                       DWORD* lpdwDisposition) {
    (void)hKey;(void)lpSubKey;(void)Reserved;(void)lpClass;(void)dwOptions;
    (void)samDesired;(void)lpSecurityAttributes;
    if(phkResult) *phkResult = 0;
    if(lpdwDisposition) *lpdwDisposition = REG_CREATED_NEW_KEY;
    return (LSTATUS)ERROR_ACCESS_DENIED;
}
#define RegCreateKeyEx  RegCreateKeyExA
#define RegCreateKeyExW RegCreateKeyExA

static inline LSTATUS RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, DWORD* lpReserved,
                                        DWORD* lpType, BYTE* lpData, DWORD* lpcbData) {
    (void)hKey;(void)lpValueName;(void)lpReserved;
    if(lpType) *lpType = REG_NONE;
    if(lpcbData) *lpcbData = 0;
    return (LSTATUS)ERROR_FILE_NOT_FOUND;
}
#define RegQueryValueEx  RegQueryValueExA
// Wide version — takes LPCWSTR, same no-op stub
static inline LSTATUS RegQueryValueExW(HKEY hKey, const wchar_t* lpValueName, DWORD* lpReserved,
                                        DWORD* lpType, BYTE* lpData, DWORD* lpcbData) {
    (void)hKey;(void)lpValueName;(void)lpReserved;
    if(lpType) *lpType = REG_NONE;
    if(lpcbData) *lpcbData = 0;
    return (LSTATUS)ERROR_FILE_NOT_FOUND;
}

static inline LSTATUS RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved,
                                      DWORD dwType, const BYTE* lpData, DWORD cbData) {
    (void)hKey;(void)lpValueName;(void)Reserved;(void)dwType;(void)lpData;(void)cbData;
    return (LSTATUS)ERROR_ACCESS_DENIED;
}
#define RegSetValueEx  RegSetValueExA
// Wide version — takes LPCWSTR
static inline LSTATUS RegSetValueExW(HKEY hKey, const wchar_t* lpValueName, DWORD Reserved,
                                      DWORD dwType, const BYTE* lpData, DWORD cbData) {
    (void)hKey;(void)lpValueName;(void)Reserved;(void)dwType;(void)lpData;(void)cbData;
    return (LSTATUS)ERROR_ACCESS_DENIED;
}

static inline LSTATUS RegDeleteKeyA(HKEY hKey, LPCSTR lpSubKey) {
    (void)hKey;(void)lpSubKey; return (LSTATUS)ERROR_ACCESS_DENIED;
}
#define RegDeleteKey  RegDeleteKeyA
#define RegDeleteKeyW RegDeleteKeyA

static inline LSTATUS RegDeleteValueA(HKEY hKey, LPCSTR lpValueName) {
    (void)hKey;(void)lpValueName; return (LSTATUS)ERROR_ACCESS_DENIED;
}
#define RegDeleteValue  RegDeleteValueA
#define RegDeleteValueW RegDeleteValueA

static inline LSTATUS RegCloseKey(HKEY hKey) { (void)hKey; return (LSTATUS)ERROR_SUCCESS; }

static inline LSTATUS RegEnumKeyExA(HKEY hKey, DWORD dwIndex, LPSTR lpName, DWORD* lpcchName,
                                     DWORD* lpReserved, LPSTR lpClass, DWORD* lpcchClass,
                                     void* lpftLastWriteTime) {
    (void)hKey;(void)dwIndex;(void)lpName;(void)lpcchName;(void)lpReserved;
    (void)lpClass;(void)lpcchClass;(void)lpftLastWriteTime;
    return (LSTATUS)ERROR_NO_MORE_ITEMS;
}
#define RegEnumKeyEx  RegEnumKeyExA
#define RegEnumKeyExW RegEnumKeyExA

static inline LSTATUS RegEnumValueA(HKEY hKey, DWORD dwIndex, LPSTR lpValueName,
                                     DWORD* lpcchValueName, DWORD* lpReserved, DWORD* lpType,
                                     BYTE* lpData, DWORD* lpcbData) {
    (void)hKey;(void)dwIndex;(void)lpValueName;(void)lpcchValueName;(void)lpReserved;
    (void)lpType;(void)lpData;(void)lpcbData;
    return (LSTATUS)ERROR_NO_MORE_ITEMS;
}
#define RegEnumValue  RegEnumValueA
#define RegEnumValueW RegEnumValueA

static inline LSTATUS RegQueryInfoKeyA(HKEY hKey, LPSTR lpClass, DWORD* lpcchClass,
                                        DWORD* lpReserved, DWORD* lpcSubKeys,
                                        DWORD* lpcbMaxSubKeyLen, DWORD* lpcbMaxClassLen,
                                        DWORD* lpcValues, DWORD* lpcbMaxValueNameLen,
                                        DWORD* lpcbMaxValueLen, DWORD* lpcbSecurityDescriptor,
                                        void* lpftLastWriteTime) {
    (void)hKey;(void)lpClass;(void)lpcchClass;(void)lpReserved;
    if(lpcSubKeys) *lpcSubKeys = 0;
    if(lpcbMaxSubKeyLen) *lpcbMaxSubKeyLen = 0;
    if(lpcbMaxClassLen) *lpcbMaxClassLen = 0;
    if(lpcValues) *lpcValues = 0;
    if(lpcbMaxValueNameLen) *lpcbMaxValueNameLen = 0;
    if(lpcbMaxValueLen) *lpcbMaxValueLen = 0;
    if(lpcbSecurityDescriptor) *lpcbSecurityDescriptor = 0;
    if(lpftLastWriteTime) memset(lpftLastWriteTime, 0, 8); // sizeof(FILETIME)
    return (LSTATUS)ERROR_SUCCESS;
}
#define RegQueryInfoKey  RegQueryInfoKeyA
#define RegQueryInfoKeyW RegQueryInfoKeyA

static inline LSTATUS RegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, HKEY* phkResult) {
    (void)hKey;(void)lpSubKey;
    if(phkResult) *phkResult = 0;
    return (LSTATUS)ERROR_FILE_NOT_FOUND;
}
#define RegOpenKey  RegOpenKeyA
#define RegOpenKeyW RegOpenKeyA

static inline LSTATUS RegCreateKeyA(HKEY hKey, LPCSTR lpSubKey, HKEY* phkResult) {
    (void)hKey;(void)lpSubKey;
    if(phkResult) *phkResult = 0;
    return (LSTATUS)ERROR_ACCESS_DENIED;
}
#define RegCreateKey  RegCreateKeyA
#define RegCreateKeyW RegCreateKeyA

#endif // !_WIN32
#endif // ZH_COMPAT_WINREG_H
