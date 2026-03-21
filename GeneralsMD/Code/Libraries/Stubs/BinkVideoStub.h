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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <cstdint>

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
//  BINK — core video state structure.
//  Only the fields actually read by BinkVideoPlayer.cpp are declared here.
//  Real Bink SDK has ~60 fields; we provide the minimum needed to compile.
// ---------------------------------------------------------------------------
typedef struct BINK_tag
{
    DWORD Width;       // frame width in pixels
    DWORD Height;      // frame height in pixels
    DWORD Frames;      // total number of frames in the clip
    DWORD FrameNum;    // current frame number (1-based)
    DWORD LastFrameNum;// last valid frame number
    DWORD FrameRate;   // numerator of frame rate (frames per second * FrameRateDiv)
    DWORD FrameRateDiv;// denominator of frame rate (usually 1)
} BINK;

// Handle is a pointer to the BINK structure (NULL = no video open)
typedef BINK* HBINK;

// ---------------------------------------------------------------------------
//  Bink copy-to-buffer surface format flags
// ---------------------------------------------------------------------------
#define BINKSURFACE32     3   // 32-bit RGBX
#define BINKSURFACE24     4   // 24-bit RGB
#define BINKSURFACE16     5   // 16-bit RGB565
#define BINKSURFACE565    BINKSURFACE16
#define BINKSURFACE555    6

// ---------------------------------------------------------------------------
//  Stub function implementations — all are inline no-ops.
//  Video features degrade to a blank / skipped screen.
// ---------------------------------------------------------------------------

// Open a .bik file.  Returns NULL — caller must handle null handle gracefully.
inline HBINK BinkOpen(char const* /*path*/, DWORD /*flags*/)
{
    return nullptr;
}

// Close a previously opened Bink handle.
inline void BinkClose(HBINK bink)
{
    // nothing to free in stub — real SDK frees internal buffers here
    (void)bink;
}

// Poll whether the next frame is ready to decode.
// Returns 0 = ready, non-zero = still waiting.
// Stub always returns 0 (immediately ready) so the caller's wait-loop exits.
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
// dest        — pointer to destination pixel buffer
// destpitch   — bytes per row in the destination buffer
// destheight  — height of the destination buffer in pixels
// destx/desty — top-left position within destination to write to
// flags       — surface format flags (BINKSURFACE*)
inline void BinkCopyToBuffer(HBINK bink,
                              void*  dest,
                              S32    destpitch,
                              U32    destheight,
                              U32    destx,
                              U32    desty,
                              U32    flags)
{
    (void)bink; (void)dest; (void)destpitch;
    (void)destheight; (void)destx; (void)desty; (void)flags;
}

// Advance to the next frame.
inline void BinkNextFrame(HBINK bink)
{
    (void)bink;
}

// Go to a specific frame index (0-based).
inline void BinkGoto(HBINK bink, DWORD frame, int /*flags*/)
{
    (void)bink; (void)frame;
}

// Get the current frame number (1-based).
inline DWORD BinkGetFrameNum(HBINK bink)
{
    (void)bink;
    return 0;
}

// Pause or unpause playback.
inline void BinkPause(HBINK bink, int pause)
{
    (void)bink; (void)pause;
}

// Set Miles audio driver for Bink audio sync.
inline void BinkSetSoundSystem(void* /*open_func*/, DWORD /*driver*/)
{
}

inline void BinkSetVolume(HBINK /*bink*/, U32 /*track*/, S32 /*volume*/)
{
}

inline void BinkSetSoundTrack(U32 /*count*/, U32* /*tracks*/)
{
}

inline int BinkSoundUseDirectSound(void* /*driver*/)
{
    return 1;
}

#endif // STUB_IMPL
#endif // ZH_BINK_VIDEO_STUB_H
