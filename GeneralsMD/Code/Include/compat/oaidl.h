#pragma once
#ifndef ZH_COMPAT_OAIDL_H
#define ZH_COMPAT_OAIDL_H

#ifndef _WIN32

#ifndef ZH_COMPAT_OBJBASE_H
#include "objbase.h"
#endif

typedef WCHAR OLECHAR;
typedef OLECHAR* LPOLESTR;
typedef const OLECHAR* LPCOLESTR;
typedef OLECHAR* BSTR;
typedef DWORD LCID;
typedef LONG DISPID;
typedef unsigned short VARTYPE;

#ifndef STDMETHODCALLTYPE
#define STDMETHODCALLTYPE WINAPI
#endif

#ifndef STDMETHODIMP
#define STDMETHODIMP HRESULT STDMETHODCALLTYPE
#endif

#define LOCALE_SYSTEM_DEFAULT 0x0800
#define VT_EMPTY              0
#define DISPATCH_METHOD       0x0001
#define DISPATCH_PROPERTYGET  0x0002
#define DISPATCH_PROPERTYPUT  0x0004

typedef struct tagVARIANT {
    VARTYPE vt;
    WORD    wReserved1;
    WORD    wReserved2;
    WORD    wReserved3;
    union {
        LONG lVal;
        LONG llVal;
        void* byref;
    };
} VARIANT;

typedef struct tagDISPPARAMS {
    VARIANT* rgvarg;
    DISPID*  rgdispidNamedArgs;
    UINT     cArgs;
    UINT     cNamedArgs;
} DISPPARAMS;

typedef struct tagEXCEPINFO {
    WORD    wCode;
    WORD    wReserved;
    BSTR    bstrSource;
    BSTR    bstrDescription;
    BSTR    bstrHelpFile;
    DWORD   dwHelpContext;
    void*   pvReserved;
    HRESULT (STDMETHODCALLTYPE *pfnDeferredFillIn)(struct tagEXCEPINFO*);
    HRESULT scode;
} EXCEPINFO;

struct ITypeInfo;

struct IDispatch : public IUnknown {
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, OLECHAR** rgszNames,
                                                    UINT cNames, LCID lcid, DISPID* rgDispId) = 0;
    virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                                             DISPPARAMS* pDispParams, VARIANT* pVarResult,
                                             EXCEPINFO* pExcepInfo, UINT* puArgErr) = 0;
};

static const IID IID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

#endif

#endif
