// ole2.h — OLE2 compatibility stub for non-Windows builds
//
// On Windows, ole2.h pulls in OLE2 compound-document and drag/drop APIs.
// For zh-revival on Linux / macOS we only need the COM base (IUnknown,
// CoInitialize, etc.) which is already provided by compat/objbase.h.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef ZH_COMPAT_OLE2_H
#define ZH_COMPAT_OLE2_H

#ifndef _WIN32

#ifndef ZH_COMPAT_OBJBASE_H
#include "objbase.h"
#endif

// Stub types for any OLE2 symbols referenced outside of #ifdef _WIN32 guards
#ifndef CLIPFORMAT
typedef unsigned short CLIPFORMAT;
#endif

#endif // !_WIN32
#endif // ZH_COMPAT_OLE2_H
