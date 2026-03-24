// osdep.h — OS dependency abstraction shim for Westwood Studios legacy code
//
// The original osdep.h provided platform-detection macros and primitive
// integer types for the Tiberian Sun / Red Alert 2 / Generals engine.
// Those types are now provided by the force-included compat/windows.h on
// non-Windows builds, so this stub is mostly a no-op.
//
// Included by WWLib and WW3D2 headers inside #ifdef _UNIX guards after we
// define _UNIX for native Linux/macOS builds (CMakeLists.txt).
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef ZH_COMPAT_OSDEP_H
#define ZH_COMPAT_OSDEP_H

#ifndef _WIN32

// Pull in our full compat windows.h (already force-included, but safe to repeat)
#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// Westwood osdep.h historically defined these POSIX helpers.
// On modern POSIX systems they come from <unistd.h>, which windows.h includes.
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// SR_OS_* platform flags used in some WW3D2 headers
#if defined(__linux__)
#  ifndef SR_OS_LINUX
#  define SR_OS_LINUX 1
#  endif
#elif defined(__APPLE__)
#  ifndef SR_OS_MACOS
#  define SR_OS_MACOS 1
#  endif
#endif

#endif // !_WIN32
#endif // ZH_COMPAT_OSDEP_H
