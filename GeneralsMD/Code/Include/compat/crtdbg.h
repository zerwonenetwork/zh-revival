// crtdbg.h — MSVC CRT debug library stub for non-Windows builds
//
// Provides no-op versions of _CrtSetDbgFlag and related macros so that
// WinMain.cpp and other debug-path code compiles on Linux / macOS.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef ZH_COMPAT_CRTDBG_H
#define ZH_COMPAT_CRTDBG_H

#ifndef _WIN32

// _CRTDBG_* bit-flags used by _CrtSetDbgFlag
#ifndef _CRTDBG_REPORT_FLAG
#define _CRTDBG_REPORT_FLAG     (-1)
#define _CRTDBG_ALLOC_MEM_DF    0x00000001
#define _CRTDBG_DELAY_FREE_MEM_DF 0x00000002
#define _CRTDBG_CHECK_ALWAYS_DF 0x00000004
#define _CRTDBG_CHECK_CRT_DF    0x00000008
#define _CRTDBG_LEAK_CHECK_DF   0x00000020
#endif

static inline int  _CrtSetDbgFlag(int newFlag)           { (void)newFlag; return 0; }
static inline void _CrtDumpMemoryLeaks(void)             {}
static inline void _CrtSetReportMode(int type, int mode) { (void)type; (void)mode; }
static inline void _CrtSetReportFile(int type, void* f)  { (void)type; (void)f; }
static inline void _CrtCheckMemory(void)                 {}

#endif // !_WIN32
#endif // ZH_COMPAT_CRTDBG_H
