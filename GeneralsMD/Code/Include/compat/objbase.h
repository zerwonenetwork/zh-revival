// objbase.h — compat shim for COM base interfaces
#pragma once
#ifndef ZH_COMPAT_OBJBASE_H
#define ZH_COMPAT_OBJBASE_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// ── COM method declaration macros ──────────────────────────────────────────
// STDMETHOD / STDMETHOD_ are used in COM interface declarations to declare
// virtual methods with the correct calling convention.
#ifndef STDMETHOD
#define STDMETHOD(method)        virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method)  virtual type    STDMETHODCALLTYPE method
#define STDMETHODIMP             HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)      type    STDMETHODCALLTYPE
#define PURE                     = 0
#define THIS_
#define THIS                     void
#define DECLARE_INTERFACE(iface)      struct iface
#define DECLARE_INTERFACE_(iface, base) struct iface : public base
#endif

// ── IUnknown — minimal COM base interface stub ─────────────────────────────
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
static inline HRESULT OleInitialize(void* reserved)   { return CoInitialize(reserved); }
static inline void    OleUninitialize(void)           { CoUninitialize(); }
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

// IDispatch is defined in oaidl.h (included from windows.h after objbase.h).
// Only forward-declare it here so code that includes only objbase.h can use LPDISPATCH.
#ifdef __cplusplus
struct IDispatch;
typedef IDispatch* LPDISPATCH;
#endif

#endif // !_WIN32
#endif // ZH_COMPAT_OBJBASE_H
