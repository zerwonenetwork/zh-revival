// BinkVideoStub.h — stub for RAD Game Tools Bink Video 1.x
//
// Provides interface-compatible type definitions and no-op function stubs
// so the project compiles without the proprietary Bink SDK (binkw32.dll).
//
// Guard: wrap all content in #ifdef STUB_IMPL so real SDK headers can be
// swapped back in simply by removing -DSTUB_IMPL from the build and
// supplying the real bink.h on the include path.
//
// With the stub active, video playback degrades gracefully:
//   BinkOpen  → returns NULL
//   BinkWait  → returns 0  (frame ready immediately — nothing to wait for)
//   All frame operations → no-ops; the video surface is never written
//
// Replacement plan: Task P5-02 — replace with FFmpeg / WebM pipeline.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.

#pragma once
#ifndef ZH_BINK_VIDEO_STUB_H
#define ZH_BINK_VIDEO_STUB_H

#ifdef STUB_IMPL

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#else
#  include <stdint.h>
#  include <stddef.h>
   typedef int            BOOL;
   typedef unsigned long  DWORD;
   typedef unsigned short WORD;
   typedef unsigned int   UINT;
   typedef void*          HANDLE;
   typedef void*          HWND;
   typedef char*          LPSTR;
   typedef const char*    LPCSTR;
   typedef void*          LPVOID;
#  ifndef TRUE
#  define TRUE  1
#  define FALSE 0
#  endif
#endif
#include <cstdint>
#include <cstdio>

// RAD's public headers typically define these basic types; define them here
// so stub consumers don't need the proprietary type headers.
#ifndef ZH_STUB_S32_DEFINED
#define ZH_STUB_S32_DEFINED
typedef int32_t  S32;
#endif
#ifndef ZH_STUB_U32_DEFINED
#define ZH_STUB_U32_DEFINED
typedef uint32_t U32;
#endif

// ---------------------------------------------------------------------------
//  BINK open flags (used by callers: BinkOpen(path, BINKPRELOADALL))
// ---------------------------------------------------------------------------
#define BINKPRELOADALL      0x00000000U  // stub: any flag value is accepted
#define BINKFILEHANDLE      0x00080000U
#define BINKNOFRAMEBUFFERS  0x00000400U

// ---------------------------------------------------------------------------
//  Bink copy-to-buffer surface format flags
// ---------------------------------------------------------------------------
#define BINKSURFACE32     3   // 32-bit RGBX
#define BINKSURFACE24     4   // 24-bit RGB
#define BINKSURFACE16     5   // 16-bit RGB565
#define BINKSURFACE565    BINKSURFACE16
#define BINKSURFACE555    6

// ---------------------------------------------------------------------------
//  Function implementations.
//
//  When ZH_FFMPEG_VIDEO is defined: extern declarations only.
//  Real FFmpeg-backed bodies are in FFmpegBinkShim.cpp.
//
//  Otherwise: inline no-ops that log skipped videos to Logs/video_skip.log.
// ---------------------------------------------------------------------------

#ifdef ZH_FFMPEG_VIDEO
// ---- FFmpeg-backed mode: declarations only ---------------------------------
// The BINK struct is extended in this mode to hold FFmpeg private state.
// Forward-declare the FFmpeg types so this header stays self-contained.
struct AVFormatContext;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;
struct SwsContext;
#endif // ZH_FFMPEG_VIDEO

#ifdef ZH_FFMPEG_VIDEO
// Redefine BINK to include FFmpeg state appended after the public fields.
// Public fields stay at identical offsets, so BinkVideoPlayer.cpp is unaffected.
#undef ZH_BINK_STRUCT_DEFINED
#endif

#ifndef ZH_BINK_STRUCT_DEFINED
#define ZH_BINK_STRUCT_DEFINED
typedef struct BINK_tag
{
    DWORD Width;
    DWORD Height;
    DWORD Frames;
    DWORD FrameNum;
    DWORD LastFrameNum;
    DWORD FrameRate;
    DWORD FrameRateDiv;
#ifdef ZH_FFMPEG_VIDEO
    // Private FFmpeg state — not touched by BinkVideoPlayer.cpp
    AVFormatContext* _fmtCtx;
    AVCodecContext*  _codecCtx;
    AVFrame*         _frame;
    AVPacket*        _packet;
    int              _videoStream;
    SwsContext*      _swsCtx;
    bool             _frameReady;
#endif
} BINK;
typedef BINK* HBINK;
#endif // ZH_BINK_STRUCT_DEFINED

#ifdef ZH_FFMPEG_VIDEO
// ---- FFmpeg-backed mode: extern declarations only --------------------------

HBINK BinkOpen           (char const* path, DWORD flags);
void  BinkClose          (HBINK bink);
int   BinkWait           (HBINK bink);
void  BinkDoFrame        (HBINK bink);
void  BinkCopyToBuffer   (HBINK bink, void* dest, S32 destpitch,
                          U32 destheight, U32 destx, U32 desty, U32 flags);
void  BinkNextFrame      (HBINK bink);
void  BinkGoto           (HBINK bink, DWORD frame, int flags);
DWORD BinkGetFrameNum    (HBINK bink);
void  BinkPause          (HBINK bink, int pause);
void  BinkSetSoundSystem (void* open_func, DWORD driver);
void  BinkSetVolume      (HBINK bink, U32 track, S32 volume);
void  BinkSetSoundTrack  (U32 count, U32* tracks);
int   BinkSoundUseDirectSound(void* driver);

#else // ZH_FFMPEG_VIDEO not defined — original no-op inline stubs

// Helper: append one line to Logs/video_skip.log (best-effort, no crash on failure)
inline void zh_bink_log_skip(char const* path)
{
    FILE* f = fopen("Logs/video_skip.log", "a");
    if (f) { fprintf(f, "P5-02: video skipped (no Bink/FFmpeg): %s\n", path ? path : "<null>"); fclose(f); }
}

// Open a .bik file.  Returns NULL — caller must handle null handle gracefully.
inline HBINK BinkOpen(char const* path, DWORD /*flags*/)
{
    zh_bink_log_skip(path);
    return nullptr;
}

// Close a previously opened Bink handle.
inline void BinkClose(HBINK bink)
{
    (void)bink;
}

// Poll whether the next frame is ready to decode.
// Returns 0 = ready, non-zero = still waiting.
// Stub always returns 0 so the caller's wait-loop exits immediately.
inline int BinkWait(HBINK bink)
{
    (void)bink;
    return 0;
}

// Decompress the current frame into Bink's internal buffer.
inline void BinkDoFrame(HBINK bink)
{
    (void)bink;
}

// Copy the current decompressed frame into a destination surface buffer.
inline void BinkCopyToBuffer(HBINK bink, void* dest, S32 destpitch,
                              U32 destheight, U32 destx, U32 desty, U32 flags)
{
    (void)bink; (void)dest; (void)destpitch;
    (void)destheight; (void)destx; (void)desty; (void)flags;
}

// Advance to the next frame.
inline void BinkNextFrame(HBINK bink) { (void)bink; }

// Go to a specific frame index (0-based).
inline void BinkGoto(HBINK bink, DWORD frame, int /*flags*/)
    { (void)bink; (void)frame; }

// Get the current frame number (1-based).
inline DWORD BinkGetFrameNum(HBINK bink) { (void)bink; return 0; }

// Pause or unpause playback.
inline void BinkPause(HBINK bink, int pause) { (void)bink; (void)pause; }

// Set Miles audio driver for Bink audio sync.
inline void BinkSetSoundSystem(void* /*open_func*/, DWORD /*driver*/) {}
inline void BinkSetVolume(HBINK /*bink*/, U32 /*track*/, S32 /*volume*/) {}
inline void BinkSetSoundTrack(U32 /*count*/, U32* /*tracks*/) {}
inline int  BinkSoundUseDirectSound(void* /*driver*/) { return 1; }

#endif // ZH_FFMPEG_VIDEO

#endif // STUB_IMPL
#endif // ZH_BINK_VIDEO_STUB_H
