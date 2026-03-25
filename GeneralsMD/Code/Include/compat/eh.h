// eh.h — MSVC C++ structured exception handling stub for non-Windows builds
//
// Provides a no-op _set_se_translator so that WinMain.cpp compiles on
// Linux / macOS.  Structured exceptions are Windows-only; on POSIX the
// hook is silently discarded.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef ZH_COMPAT_EH_H
#define ZH_COMPAT_EH_H

#ifndef _WIN32

#ifdef __cplusplus
// _set_se_translator converts Win32 SEH exceptions to C++ exceptions.
// On non-Windows it is a no-op; the handler is never installed.
typedef void (*_se_translator_function)(unsigned int, void*);
static inline _se_translator_function _set_se_translator(_se_translator_function f)
{
    (void)f;
    return nullptr;
}
#endif // __cplusplus

#endif // !_WIN32
#endif // ZH_COMPAT_EH_H
