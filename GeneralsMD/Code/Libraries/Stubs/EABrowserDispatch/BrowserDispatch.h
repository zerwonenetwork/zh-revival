// EABrowserDispatch/BrowserDispatch.h — ZH-REVIVAL STUB
// Provides a minimal stub for the EA Browser Dispatch COM interface.
// The embedded browser subsystem (ENABLE_EMBEDDED_BROWSER) is disabled in
// dx8webbrowser.h; this stub exists only to satisfy the #include in
// WebBrowser.h so that the file parses without error.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef EA_BROWSER_DISPATCH_H
#define EA_BROWSER_DISPATCH_H

#include <oaidl.h>

// Only a single dispatch method is referenced by the browser wrapper.
// MSVC: use __declspec(uuid) + __uuidof for ATL compatibility.
// MinGW/GCC: skip __declspec(uuid) and define IID as a literal GUID struct.
#if defined(_MSC_VER) && !defined(__GNUC__)
struct __declspec(uuid("00000000-0000-0000-0000-000000000001")) IBrowserDispatch : public IDispatch
{
	virtual HRESULT STDMETHODCALLTYPE TestMethod(int num1) = 0;
};
inline const IID IID_IBrowserDispatch = __uuidof(IBrowserDispatch);
#else
struct IBrowserDispatch : public IDispatch
{
	virtual HRESULT STDMETHODCALLTYPE TestMethod(int num1) = 0;
};
// GUID: 00000000-0000-0000-0000-000000000001
inline const IID IID_IBrowserDispatch = {0x00000000, 0x0000, 0x0000, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}};
#endif

#endif // EA_BROWSER_DISPATCH_H
