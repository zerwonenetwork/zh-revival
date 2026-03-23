// objbase.h — compat shim for COM base interfaces
#pragma once
#ifndef ZH_COMPAT_OBJBASE_H
#define ZH_COMPAT_OBJBASE_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// IUnknown — minimal COM base interface stub
#ifdef __cplusplus
struct IUnknown {
    virtual HRESULT QueryInterface(REFIID riid, void** ppvObject) = 0;
    virtual unsigned long AddRef(void) = 0;
    virtual unsigned long Release(void) = 0;
    virtual ~IUnknown() {}
};
#endif

// COM memory allocation stubs
static inline void* CoTaskMemAlloc(size_t cb)     { return malloc(cb); }
static inline void  CoTaskMemFree_base(void* p)   { free(p); }
#ifndef CoTaskMemFree
#define CoTaskMemFree CoTaskMemFree_base
#endif
static inline void* CoTaskMemRealloc(void* p, size_t cb) { return realloc(p, cb); }

// COM init/uninit stubs
static inline HRESULT CoInitialize(void* reserved)   { (void)reserved; return S_OK; }
static inline HRESULT CoInitializeEx(void* r, DWORD d){ (void)r;(void)d; return S_OK; }
static inline void    CoUninitialize(void)            {}
static inline HRESULT CoCreateInstance(REFCLSID rclsid, void* pUnkOuter, DWORD dwClsContext,
                                       REFIID riid, void** ppv) {
    (void)rclsid;(void)pUnkOuter;(void)dwClsContext;(void)riid;
    if(ppv) *ppv = NULL;
    return E_NOINTERFACE;
}

// CLSCTX flags
#define CLSCTX_INPROC_SERVER  0x1
#define CLSCTX_INPROC_HANDLER 0x2
#define CLSCTX_LOCAL_SERVER   0x4
#define CLSCTX_ALL            0x17

// IDispatch — minimal COM automation interface
#ifdef __cplusplus
struct IDispatch : public IUnknown {
    virtual HRESULT GetTypeInfoCount(UINT* pctinfo) = 0;
    virtual HRESULT GetTypeInfo(UINT iTInfo, DWORD lcid, void** ppTInfo) = 0;
    virtual HRESULT GetIDsOfNames(REFIID riid, void** rgszNames, UINT cNames,
                                   DWORD lcid, LONG* rgDispId) = 0;
    virtual HRESULT Invoke(LONG dispIdMember, REFIID riid, DWORD lcid, WORD wFlags,
                           void* pDispParams, void* pVarResult,
                           void* pExcepInfo, UINT* puArgErr) = 0;
};
typedef IDispatch* LPDISPATCH;
#endif

#endif // !_WIN32
#endif // ZH_COMPAT_OBJBASE_H
