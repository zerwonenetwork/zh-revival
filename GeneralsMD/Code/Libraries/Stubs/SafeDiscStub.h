// SafeDiscStub.h — stub for Macrovision SafeDisc CD copy-protection
//
// SafeDisc was disabled by Microsoft in Windows 10 (KB3086255) and is
// entirely non-functional on modern systems.  This stub replaces the
// proprietary SafeDisc header (Common/SafeDisc/CdaPfn.h) so the project
// compiles without any SafeDisc SDK present.
//
// Stub behaviour:
//   - All validation/presence checks return TRUE (CD present / valid)
//   - No actual CD or DRM interaction occurs
//   - Task P1-11 will remove the CD check call-sites entirely
//
// Guard: wrap all content in #ifdef STUB_IMPL so callers that supply a
// real CdaPfn.h can compile without picking up these definitions.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.

#pragma once
#ifndef ZH_SAFEDISC_STUB_H
#define ZH_SAFEDISC_STUB_H

#ifdef STUB_IMPL

// On Windows: real windows.h. On Linux/macOS: compat/windows.h is
// injected via CMake force-include (-include) before this header is parsed.
#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#endif

// ---------------------------------------------------------------------------
//  CdaPfn — "CD Audio PFN" (function-pointer table) used by SafeDisc to
//  call back into the game for CD-audio operations.  The real header
//  declares this as a struct of function pointers; the stub declares a
//  compatible empty struct so sizeof / offsetof on it compiles cleanly.
// ---------------------------------------------------------------------------
typedef struct CDA_PFN_tag
{
    void* pfnCdaOpen;
    void* pfnCdaClose;
    void* pfnCdaPlay;
    void* pfnCdaStop;
    void* pfnCdaGetStatus;
} CDA_PFN;

// Pointer to the table (used in some SafeDisc call sites)
typedef CDA_PFN* PCDA_PFN;

// SafeDisc instrumentation macros become no-ops in stub builds.
#ifndef CDAPFN_OVERHEAD_L5
#define CDAPFN_OVERHEAD_L5 0
#endif
#ifndef CDAPFN_CONSTRAINT_NONE
#define CDAPFN_CONSTRAINT_NONE 0
#endif
#ifndef CDAPFN_DECLARE_GLOBAL
#define CDAPFN_DECLARE_GLOBAL(func, overhead, constraint)
#endif
#ifndef CDAPFN_ENDMARK
#define CDAPFN_ENDMARK(func) ((void)0)
#endif

// ---------------------------------------------------------------------------
//  SafeDisc validation entry points
//  All return TRUE so the game boots as if a valid CD is present.
//  Task P1-11 will remove these call-sites and this stub becomes moot.
// ---------------------------------------------------------------------------

// Called at startup to verify the disc is present and valid.
// Returns TRUE = disc valid (stub always passes).
inline BOOL SafeDisc_Validate(void)
{
    return TRUE;
}

// Called to check whether the SafeDisc launcher / wrapper is running.
// Returns TRUE = launcher running (stub always passes).
inline BOOL SafeDisc_IsLauncherRunning(void)
{
    return TRUE;
}

// Called to notify the SafeDisc launcher that the game has started.
// Returns TRUE = notification sent successfully.
inline BOOL SafeDisc_NotifyLauncher(void)
{
    return TRUE;
}

// Called at shutdown to release SafeDisc resources.
inline void SafeDisc_Shutdown(void)
{
}

// ---------------------------------------------------------------------------
//  Stub constant — used in some call-sites to gate the entire check
// ---------------------------------------------------------------------------
#define SAFEDISC_STUB  1   // defined when using stubs; absent with real SDK

#endif // STUB_IMPL
#endif // ZH_SAFEDISC_STUB_H
