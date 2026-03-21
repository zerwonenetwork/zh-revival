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
struct __declspec(uuid("00000000-0000-0000-0000-000000000001")) IBrowserDispatch : public IDispatch
{
	virtual HRESULT STDMETHODCALLTYPE TestMethod(int num1) = 0;
};

// Define IID_IBrowserDispatch as an inline variable (C++17) so it has
// external linkage and is defined in every translation unit that includes
// this header without causing ODR violations.
inline const IID IID_IBrowserDispatch = __uuidof(IBrowserDispatch);

#endif // EA_BROWSER_DISPATCH_H
