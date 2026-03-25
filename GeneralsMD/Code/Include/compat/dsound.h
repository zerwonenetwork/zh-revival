// dsound.h — non-Windows compatibility stub
//
// Provides the minimal DirectSound API surface used by MilesAudioManager.cpp
// on non-Windows builds (Linux / macOS).  No actual audio is produced;
// DirectSound is a Windows-only API and is replaced by OpenAL in P5-01.
//
// Symbols provided:
//   LPDIRECTSOUND / IDirectSound  — forward declaration + typedef only
//   DSSPEAKER_*                   — speaker geometry constants
//   DSSPEAKER_CONFIG()            — macro that extracts config from cookie
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.

#pragma once
#ifndef ZH_COMPAT_DSOUND_H
#define ZH_COMPAT_DSOUND_H

#ifndef _WIN32

#include "windows.h"   // DWORD, HWND etc.

// ---------------------------------------------------------------------------
//  Forward-declare IDirectSound so that pointer types compile.
//  No actual implementation — all code paths that call DS methods are
//  guarded by platform checks or never reached on non-Windows builds.
// ---------------------------------------------------------------------------
struct IDirectSound;
typedef struct IDirectSound  *LPDIRECTSOUND;

// ---------------------------------------------------------------------------
//  Speaker geometry constants (dsound.h / dsconf.h on Windows SDK)
// ---------------------------------------------------------------------------
#ifndef DSSPEAKER_DIRECTOUT
#define DSSPEAKER_DIRECTOUT   0
#define DSSPEAKER_HEADPHONE   1
#define DSSPEAKER_MONO        2
#define DSSPEAKER_QUAD        3
#define DSSPEAKER_STEREO      4
#define DSSPEAKER_SURROUND    5
#define DSSPEAKER_5POINT1     6
#define DSSPEAKER_7POINT1     7
#endif

// DSSPEAKER_CONFIG extracts the speaker-geometry word from the cookie
// returned by IDirectSound::GetSpeakerConfig().
#ifndef DSSPEAKER_CONFIG
#define DSSPEAKER_CONFIG(x)   ((DWORD)(x) & 0x000000FFu)
#endif

#endif // !_WIN32
#endif // ZH_COMPAT_DSOUND_H
