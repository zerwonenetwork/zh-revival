// mmsystem.h — compatibility shim for Windows Multimedia API
// Provides minimal types/constants used by the SAGE audio subsystem.
#pragma once
#ifndef ZH_COMPAT_MMSYSTEM_H
#define ZH_COMPAT_MMSYSTEM_H

#ifndef _WIN32

#include <stdint.h>
#include <stddef.h>

typedef unsigned int  MMRESULT;
typedef unsigned int  MMVERSION;
typedef void*         HWAVEOUT;
typedef void*         HWAVEIN;
typedef void*         HMIXER;
typedef void*         HMIXEROBJ;
typedef void*         HWAVE;
typedef void*         HMMIO;
typedef unsigned int  UINT_PTR_MM;

// MMRESULT error codes
#define MMSYSERR_BASE        0
#define MMSYSERR_NOERROR     0
#define MMSYSERR_ERROR       1
#define MMSYSERR_BADDEVICEID 2
#define MMSYSERR_NOTENABLED  3
#define MMSYSERR_ALLOCATED   4
#define MMSYSERR_INVALHANDLE 5
#define MMSYSERR_NODRIVER    6
#define MMSYSERR_NOMEM       7
#define MMSYSERR_NOTSUPPORTED 8
#define MMSYSERR_BADERRNUM   9
#define MMSYSERR_INVALFLAG   10
#define MMSYSERR_INVALPARAM  11
#define MMSYSERR_HANDLEBUSY  12
#define MMSYSERR_LASTERROR   13
#define WAVERR_BASE          32
#define WAVERR_BADFORMAT     (WAVERR_BASE + 0)
#define WAVERR_STILLPLAYING  (WAVERR_BASE + 1)
#define WAVERR_UNPREPARED    (WAVERR_BASE + 2)
#define WAVERR_SYNC          (WAVERR_BASE + 3)

// Wave format types
#define WAVE_FORMAT_PCM      1
#define WAVE_FORMAT_ADPCM    2
#define WAVE_FORMAT_IEEE_FLOAT 3
#define WAVE_MAPPER          ((UINT)-1)
#define WAVE_ALLOWSYNC       0x0002

// Callback flags
#define CALLBACK_NULL        0x00000000
#define CALLBACK_WINDOW      0x00010000
#define CALLBACK_TASK        0x00020000
#define CALLBACK_FUNCTION    0x00030000
#define CALLBACK_THREAD      0x00020000
#define CALLBACK_EVENT       0x00050000

// WAVEFORMATEX
#pragma pack(push, 1)
typedef struct tWAVEFORMATEX {
    unsigned short wFormatTag;
    unsigned short nChannels;
    unsigned long  nSamplesPerSec;
    unsigned long  nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short wBitsPerSample;
    unsigned short cbSize;
} WAVEFORMATEX, *PWAVEFORMATEX, *LPWAVEFORMATEX;
typedef const WAVEFORMATEX* LPCWAVEFORMATEX;

// WAVEHDR
typedef struct wavehdr_tag {
    char*          lpData;
    unsigned long  dwBufferLength;
    unsigned long  dwBytesRecorded;
    unsigned long* dwUser;
    unsigned long  dwFlags;
    unsigned long  dwLoops;
    struct wavehdr_tag* lpNext;
    unsigned long* reserved;
} WAVEHDR, *PWAVEHDR, *LPWAVEHDR;
#pragma pack(pop)

#define WHDR_DONE        0x00000001
#define WHDR_PREPARED    0x00000002
#define WHDR_BEGINLOOP   0x00000004
#define WHDR_ENDLOOP     0x00000008
#define WHDR_INQUEUE     0x00000010

// WAVEOUTCAPS
typedef struct tagWAVEOUTCAPSA {
    unsigned short wMid, wPid;
    unsigned int   vDriverVersion;
    char           szPname[32];
    unsigned long  dwFormats, dwChannels, dwSupport;
} WAVEOUTCAPSA, *LPWAVEOUTCAPSA;
#define WAVEOUTCAPS  WAVEOUTCAPSA

// Stub inline functions
#ifdef __cplusplus
#include <cstring>
#else
#include <string.h>
#endif
static inline MMRESULT waveOutOpen(HWAVEOUT* h, unsigned int id, LPCWAVEFORMATEX fmt, unsigned long cb, unsigned long inst, unsigned long flags) {
    (void)h;(void)id;(void)fmt;(void)cb;(void)inst;(void)flags; return MMSYSERR_NOTSUPPORTED;
}
static inline MMRESULT waveOutClose(HWAVEOUT h)          { (void)h; return MMSYSERR_NOERROR; }
static inline MMRESULT waveOutPrepareHeader(HWAVEOUT h, LPWAVEHDR hdr, unsigned int sz) { (void)h;(void)hdr;(void)sz; return MMSYSERR_NOERROR; }
static inline MMRESULT waveOutUnprepareHeader(HWAVEOUT h, LPWAVEHDR hdr, unsigned int sz) { (void)h;(void)hdr;(void)sz; return MMSYSERR_NOERROR; }
static inline MMRESULT waveOutWrite(HWAVEOUT h, LPWAVEHDR hdr, unsigned int sz) { (void)h;(void)hdr;(void)sz; return MMSYSERR_NOTSUPPORTED; }
static inline MMRESULT waveOutReset(HWAVEOUT h)          { (void)h; return MMSYSERR_NOERROR; }
static inline MMRESULT waveOutPause(HWAVEOUT h)          { (void)h; return MMSYSERR_NOERROR; }
static inline MMRESULT waveOutRestart(HWAVEOUT h)        { (void)h; return MMSYSERR_NOERROR; }
static inline MMRESULT waveOutGetDevCapsA(unsigned int id, LPWAVEOUTCAPSA caps, unsigned int sz) {
    (void)id;(void)sz; if(caps) memset(caps,0,sizeof(*caps)); return MMSYSERR_NOERROR;
}
static inline unsigned int waveOutGetNumDevs(void)       { return 0; }

// ---------------------------------------------------------------------------
//  Multimedia Timer (timeGetTime used by systimer.h)
// ---------------------------------------------------------------------------
#ifdef __cplusplus
#  include <ctime>
#else
#  include <time.h>
#endif
static inline unsigned long timeGetTime(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long)(ts.tv_sec * 1000UL + ts.tv_nsec / 1000000UL);
}
static inline unsigned long timeBeginPeriod(unsigned int u) { (void)u; return 0; }
static inline unsigned long timeEndPeriod(unsigned int u)   { (void)u; return 0; }

// ---------------------------------------------------------------------------
//  Multimedia timer (timeSetEvent / timeKillEvent) — used by wwmouse.cpp
// ---------------------------------------------------------------------------
#define TIME_ONESHOT    0x0000
#define TIME_PERIODIC   0x0001
#define TIME_CALLBACK_FUNCTION    0x0000
#define TIME_CALLBACK_EVENT_SET   0x0010
#define TIME_CALLBACK_EVENT_PULSE 0x0020

#ifndef ZH_COMPAT_WINDOWS_H
#  include "windows.h"  // for DWORD_PTR, UINT
#endif
typedef void (*TIMECALLBACK)(unsigned int uTimerID, unsigned int uMsg,
                              unsigned long dwUser,
                              unsigned long dw1, unsigned long dw2);
typedef TIMECALLBACK *LPTIMECALLBACK;

static inline unsigned int timeSetEvent(unsigned int uDelay, unsigned int uResolution,
                                         TIMECALLBACK fptc, unsigned long dwUser,
                                         unsigned int fuEvent) {
    (void)uDelay;(void)uResolution;(void)fptc;(void)dwUser;(void)fuEvent; return 0;
}
static inline unsigned int timeKillEvent(unsigned int uTimerID) { (void)uTimerID; return 0; }

#endif // !_WIN32
#endif // ZH_COMPAT_MMSYSTEM_H
