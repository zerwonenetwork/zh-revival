// MilesSoundStub.h — stub for RAD Game Tools Miles Sound System 6.x
//
// Provides interface-compatible type definitions and no-op function stubs
// so the project compiles without the proprietary MSS SDK.
//
// Guard: wrap all content in #ifdef STUB_IMPL so real SDK headers can be
// swapped back in simply by removing -DSTUB_IMPL from the build and
// supplying the real MSS/MSS.h on the include path.
//
// Replacement plan: Task P5-01 — replace with openal-soft.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.

#pragma once
#ifndef ZH_MILES_SOUND_STUB_H
#define ZH_MILES_SOUND_STUB_H

#ifdef STUB_IMPL

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>  // DWORD, HWND etc. already pulled in by game headers

// ---------------------------------------------------------------------------
//  Primitive types — match Miles 6.x MSS.H typedefs
// ---------------------------------------------------------------------------
typedef signed char    S8;
typedef unsigned char  U8;
typedef signed short   S16;
typedef unsigned short U16;
#ifndef ZH_STUB_S32_DEFINED
#define ZH_STUB_S32_DEFINED
typedef signed long    S32;
#endif
#ifndef ZH_STUB_U32_DEFINED
#define ZH_STUB_U32_DEFINED
typedef unsigned long  U32;
#endif
typedef float          F32;
typedef double         F64;

// ---------------------------------------------------------------------------
//  Calling convention for callbacks
// ---------------------------------------------------------------------------
#ifndef AILCALLBACK
#define AILCALLBACK __cdecl
#endif

// ---------------------------------------------------------------------------
//  Opaque handle types
// ---------------------------------------------------------------------------
typedef void* HSAMPLE;      // 2-D (flat) sample handle
typedef void* H3DSAMPLE;    // 3-D sample handle
typedef void* HSTREAM;      // streaming audio handle
typedef void* HDIGDRIVER;   // digital audio driver
typedef void* HMDIDRIVER;   // MIDI driver handle
typedef void* HAUDIO;       // quick-play audio handle
typedef void* H3DPOBJECT;   // 3-D listener / emitter object
typedef U32   HPROVIDER;    // 3-D provider handle (index, not a pointer)
typedef U32   HPROENUM;
typedef void* AILLPDIRECTSOUND;

#ifndef WAVE_FORMAT_IMA_ADPCM
#define WAVE_FORMAT_IMA_ADPCM 0x0011
#endif

#define HPROENUM_FIRST 0
#define DP_FILTER 0

// ---------------------------------------------------------------------------
//  AILSOUNDINFO — describes a PCM audio buffer
// ---------------------------------------------------------------------------
typedef struct
{
    DWORD  format;       // WAVE_FORMAT_PCM = 1
    void*  data_ptr;     // pointer to raw sample data
    DWORD  data_len;     // byte length of sample data
    DWORD  rate;         // sample rate in Hz (e.g. 22050, 44100)
    DWORD  bits;         // bits per sample (8 or 16)
    DWORD  channels;     // 1 = mono, 2 = stereo
    DWORD  samples;      // total number of PCM samples
    DWORD  block_size;   // bytes per compressed block (PCM = 0)
    void*  initial_ptr;  // start-of-decode pointer (may equal data_ptr)
} AILSOUNDINFO;

// ---------------------------------------------------------------------------
//  3-D speaker type constants
// ---------------------------------------------------------------------------
#define AIL_3D_2_SPEAKER   0
#define AIL_3D_HEADPHONE   1
#define AIL_3D_4_SPEAKER   2
#define AIL_3D_SURROUND    3
#define AIL_3D_51_SPEAKER  4
#define AIL_3D_71_SPEAKER  5

// ---------------------------------------------------------------------------
//  Callback function-pointer typedefs
// ---------------------------------------------------------------------------
typedef void  (AILCALLBACK* AILSAMPLECB)   (HSAMPLE   s);
typedef void  (AILCALLBACK* AIL3DSAMPLECB) (H3DSAMPLE s);
typedef void  (AILCALLBACK* AILSTREAMCB)   (HSTREAM   s);
typedef U32   (AILCALLBACK* AILFOPEN)      (char const* filename, U32* file_handle);
typedef void  (AILCALLBACK* AILFCLOSE)     (U32 file_handle);
typedef S32   (AILCALLBACK* AILFSEEK)      (U32 file_handle, S32 offset, U32 type);
typedef U32   (AILCALLBACK* AILFREAD)      (U32 file_handle, void* buffer, U32 bytes);

// ---------------------------------------------------------------------------
//  Function implementations.
//
//  When ZH_OPENAL_AUDIO is defined: extern declarations only.
//  Real bodies are provided by OpenALMilesShim.cpp and linked in.
//
//  Otherwise: inline no-op stubs — audio degrades gracefully to silence.
// ---------------------------------------------------------------------------

#ifdef ZH_OPENAL_AUDIO
// ---- OpenAL-backed mode: declarations only ---------------------------------

char const* AIL_MSS_version            (void);
S32         AIL_startup                (void);
void        AIL_shutdown               (void);
S32         AIL_quick_startup          (void);
S32         AIL_quick_startup          (S32, S32, U32, S32, S32);
void        AIL_quick_handles          (HDIGDRIVER* pdig, HMDIDRIVER* pmdi);
void        AIL_quick_handles          (HDIGDRIVER* pdig, HMDIDRIVER* pmdi, HDIGDRIVER* pdig2);
void        AIL_set_redist_directory   (char const*);
U32         AIL_get_timer_highest_delay(void);

void AIL_set_file_callbacks(AILFOPEN open_cb, AILFCLOSE close_cb,
                             AILFSEEK seek_cb,  AILFREAD  read_cb);

void AIL_get_DirectSound_info(HDIGDRIVER dig, void** ppds, void** ppbuf);

HSAMPLE AIL_allocate_sample_handle  (HDIGDRIVER dig);
void    AIL_release_sample_handle   (HSAMPLE s);
void    AIL_init_sample             (HSAMPLE s);
void    AIL_set_sample_file         (HSAMPLE s, void const* data, AILSOUNDINFO* info);
void    AIL_start_sample            (HSAMPLE s);
void    AIL_stop_sample             (HSAMPLE s);
void    AIL_resume_sample           (HSAMPLE s);
void    AIL_set_sample_volume_pan   (HSAMPLE s, F32 vol, F32 pan);
void    AIL_sample_volume_pan       (HSAMPLE s, F32* vol, F32* pan);
void    AIL_set_sample_playback_rate(HSAMPLE s, S32 rate);
S32     AIL_sample_playback_rate    (HSAMPLE s);
void    AIL_set_sample_processor    (HSAMPLE s, U32 which, HPROVIDER prov);
void    AIL_set_filter_sample_preference(HSAMPLE s, char const* name, void const* val);
void*   AIL_sample_user_data        (HSAMPLE s, U32 index);
void    AIL_set_sample_user_data    (HSAMPLE s, U32 index, void* val);
void    AIL_set_sample_user_data    (HSAMPLE s, U32 index, int val);
void    AIL_register_EOS_callback   (HSAMPLE s, AILSAMPLECB cb);

H3DSAMPLE AIL_allocate_3D_sample_handle  (HPROVIDER prov);
void      AIL_release_3D_sample_handle   (H3DSAMPLE s);
void      AIL_set_3D_sample_file         (H3DSAMPLE s, void const* data, AILSOUNDINFO* info);
void      AIL_set_3D_sample_file         (H3DSAMPLE s, void const* data);
void      AIL_start_3D_sample            (H3DSAMPLE s);
void      AIL_stop_3D_sample             (H3DSAMPLE s);
void      AIL_resume_3D_sample           (H3DSAMPLE s);
void      AIL_set_3D_sample_volume       (H3DSAMPLE s, F32 vol);
void      AIL_set_3D_sample_playback_rate(H3DSAMPLE s, S32 rate);
S32       AIL_3D_sample_playback_rate    (H3DSAMPLE s);
void      AIL_set_3D_sample_occlusion    (H3DSAMPLE s, F32 occ);
void      AIL_set_3D_sample_distances    (H3DSAMPLE s, F32 near_d, F32 far_d);
void      AIL_set_3D_position            (H3DPOBJECT obj, F32 x, F32 y, F32 z);
void*     AIL_3D_user_data               (H3DSAMPLE s, U32 index);
void      AIL_set_3D_user_data           (H3DSAMPLE s, U32 index, void* val);
void      AIL_set_3D_user_data           (H3DSAMPLE s, U32 index, int val);
void      AIL_register_3D_EOS_callback   (H3DSAMPLE s, AIL3DSAMPLECB cb);

HSTREAM AIL_open_stream             (HDIGDRIVER dig, void const* data, S32 block_size);
void    AIL_close_stream            (HSTREAM s);
void    AIL_start_stream            (HSTREAM s);
void    AIL_pause_stream            (HSTREAM s, S32 on_off);
void    AIL_set_stream_volume_pan   (HSTREAM s, F32 vol, F32 pan);
void    AIL_stream_volume_pan       (HSTREAM s, F32* vol, F32* pan);
S32     AIL_stream_loop_count       (HSTREAM s);
void    AIL_set_stream_loop_count   (HSTREAM s, S32 count);
S32     AIL_stream_ms_position      (HSTREAM s);
void    AIL_stream_ms_position      (HSTREAM s, long* cur, long* total);
void    AIL_register_stream_callback(HSTREAM s, AILSTREAMCB cb);

S32       AIL_open_3D_provider     (HPROVIDER prov);
void      AIL_close_3D_provider    (HPROVIDER prov);
H3DPOBJECT AIL_open_3D_listener   (HPROVIDER prov);
void      AIL_close_3D_listener   (H3DPOBJECT obj);
void      AIL_set_3D_speaker_type  (HPROVIDER prov, S32 type);
void      AIL_set_3D_orientation   (H3DPOBJECT obj,
                                    F32 fx, F32 fy, F32 fz,
                                    F32 ux, F32 uy, F32 uz);
S32       AIL_enumerate_3D_providers(HPROVIDER* last, char const** name);
S32       AIL_enumerate_3D_providers(HPROENUM* next, HPROVIDER* prov, char** name);

S32  AIL_enumerate_filters(void* last, HPROVIDER* prov, char const** name);
S32  AIL_enumerate_filters(HPROENUM* next, HPROVIDER* prov, char** name);

HAUDIO AIL_quick_load_and_play(char const* path, U32 loop, S32 reserve);
void   AIL_quick_unload        (void* handle);
void   AIL_quick_set_volume    (HAUDIO handle, F32 vol, F32 pan);
void   AIL_WAV_info            (void const* data, AILSOUNDINFO* info);
void   AIL_decompress_ADPCM    (AILSOUNDINFO* info, void** out, U32* size);
void   AIL_mem_free_lock       (void* p);

#else // ZH_OPENAL_AUDIO not defined — original no-op inline stubs

// --- Init / shutdown --------------------------------------------------------
inline char const* AIL_MSS_version            (void)                              { return "STUB 0.0"; }
inline S32         AIL_startup                (void)                              { return 1; }
inline void        AIL_shutdown               (void)                              {}
inline S32         AIL_quick_startup          (void)                              { return 1; }
inline S32         AIL_quick_startup          (S32, S32, U32, S32, S32)          { return 1; }
inline void        AIL_quick_handles          (HDIGDRIVER* pdig, HMDIDRIVER* pmdi) { if(pdig) *pdig=nullptr; if(pmdi) *pmdi=nullptr; }
inline void        AIL_quick_handles          (HDIGDRIVER* pdig, HMDIDRIVER* pmdi, HDIGDRIVER* pdig2) { if(pdig) *pdig=nullptr; if(pmdi) *pmdi=nullptr; if(pdig2) *pdig2=nullptr; }
inline void        AIL_set_redist_directory   (char const*)                       {}
inline U32         AIL_get_timer_highest_delay(void)                              { return 0; }

// --- File I/O callbacks -----------------------------------------------------
inline void AIL_set_file_callbacks(AILFOPEN open_cb, AILFCLOSE close_cb,
                                   AILFSEEK seek_cb,  AILFREAD  read_cb)          {}

// --- Driver info ------------------------------------------------------------
inline void AIL_get_DirectSound_info(HDIGDRIVER dig, void** ppds, void** ppbuf)  { if(ppds) *ppds=nullptr; if(ppbuf) *ppbuf=nullptr; }

// --- 2-D sample management --------------------------------------------------
inline HSAMPLE AIL_allocate_sample_handle  (HDIGDRIVER dig)                      { return nullptr; }
inline void    AIL_release_sample_handle   (HSAMPLE s)                           {}
inline void    AIL_init_sample             (HSAMPLE s)                           {}
inline void    AIL_set_sample_file         (HSAMPLE s, void const* data, AILSOUNDINFO* info) {}
inline void    AIL_start_sample            (HSAMPLE s)                           {}
inline void    AIL_stop_sample             (HSAMPLE s)                           {}
inline void    AIL_resume_sample           (HSAMPLE s)                           {}
inline void    AIL_set_sample_volume_pan   (HSAMPLE s, F32 vol, F32 pan)         {}
inline void    AIL_sample_volume_pan       (HSAMPLE s, F32* vol, F32* pan)       { if(vol) *vol=1.f; if(pan) *pan=0.f; }
inline void    AIL_set_sample_playback_rate(HSAMPLE s, S32 rate)                 {}
inline S32     AIL_sample_playback_rate    (HSAMPLE s)                           { return 22050; }
inline void    AIL_set_sample_processor    (HSAMPLE s, U32 which, HPROVIDER prov) {}
inline void    AIL_set_filter_sample_preference(HSAMPLE s, char const* name, void const* val) {}
inline void*   AIL_sample_user_data        (HSAMPLE s, U32 index)                { return nullptr; }
inline void    AIL_set_sample_user_data    (HSAMPLE s, U32 index, void* val)     {}
inline void    AIL_set_sample_user_data    (HSAMPLE s, U32 index, int val)       {}
inline void    AIL_register_EOS_callback   (HSAMPLE s, AILSAMPLECB cb)           {}

// --- 3-D sample management --------------------------------------------------
inline H3DSAMPLE AIL_allocate_3D_sample_handle  (HPROVIDER prov)                 { return nullptr; }
inline void      AIL_release_3D_sample_handle   (H3DSAMPLE s)                   {}
inline void      AIL_set_3D_sample_file         (H3DSAMPLE s, void const* data, AILSOUNDINFO* info) {}
inline void      AIL_set_3D_sample_file         (H3DSAMPLE s, void const* data) {}
inline void      AIL_start_3D_sample            (H3DSAMPLE s)                   {}
inline void      AIL_stop_3D_sample             (H3DSAMPLE s)                   {}
inline void      AIL_resume_3D_sample           (H3DSAMPLE s)                   {}
inline void      AIL_set_3D_sample_volume       (H3DSAMPLE s, F32 vol)          {}
inline void      AIL_set_3D_sample_playback_rate(H3DSAMPLE s, S32 rate)         {}
inline S32       AIL_3D_sample_playback_rate    (H3DSAMPLE s)                   { return 22050; }
inline void      AIL_set_3D_sample_occlusion    (H3DSAMPLE s, F32 occ)          {}
inline void      AIL_set_3D_sample_distances    (H3DSAMPLE s, F32 near_d, F32 far_d) {}
inline void      AIL_set_3D_position            (H3DPOBJECT obj, F32 x, F32 y, F32 z) {}
inline void*     AIL_3D_user_data               (H3DSAMPLE s, U32 index)        { return nullptr; }
inline void      AIL_set_3D_user_data           (H3DSAMPLE s, U32 index, void* val) {}
inline void      AIL_set_3D_user_data           (H3DSAMPLE s, U32 index, int val) {}
inline void      AIL_register_3D_EOS_callback   (H3DSAMPLE s, AIL3DSAMPLECB cb) {}

// --- Streaming audio --------------------------------------------------------
inline HSTREAM AIL_open_stream             (HDIGDRIVER dig, void const* data, S32 block_size) { return nullptr; }
inline void    AIL_close_stream            (HSTREAM s)                           {}
inline void    AIL_start_stream            (HSTREAM s)                           {}
inline void    AIL_pause_stream            (HSTREAM s, S32 on_off)               {}
inline void    AIL_set_stream_volume_pan   (HSTREAM s, F32 vol, F32 pan)         {}
inline void    AIL_stream_volume_pan       (HSTREAM s, F32* vol, F32* pan)       { if(vol) *vol=1.f; if(pan) *pan=0.f; }
inline S32     AIL_stream_loop_count       (HSTREAM s)                           { return 0; }
inline void    AIL_set_stream_loop_count   (HSTREAM s, S32 count)                {}
inline S32     AIL_stream_ms_position      (HSTREAM s)                           { return 0; }
inline void    AIL_stream_ms_position      (HSTREAM s, long* cur, long* total)   { if(cur) *cur=0; if(total) *total=0; }
inline void    AIL_register_stream_callback(HSTREAM s, AILSTREAMCB cb)           {}

// --- 3-D provider / listener ------------------------------------------------
inline S32       AIL_open_3D_provider     (HPROVIDER prov)                       { return 1; /* non-zero = failure */ }
inline void      AIL_close_3D_provider    (HPROVIDER prov)                       {}
inline H3DPOBJECT AIL_open_3D_listener   (HPROVIDER prov)                        { return nullptr; }
inline void      AIL_close_3D_listener   (H3DPOBJECT obj)                        {}
inline void      AIL_set_3D_speaker_type  (HPROVIDER prov, S32 type)             {}
inline void      AIL_set_3D_orientation   (H3DPOBJECT obj,
                                           F32 fx, F32 fy, F32 fz,
                                           F32 ux, F32 uy, F32 uz)               {}
inline S32       AIL_enumerate_3D_providers(HPROVIDER* last, char const** name)  { if(last) *last=0; if(name) *name=nullptr; return 0; }
inline S32       AIL_enumerate_3D_providers(HPROENUM* next, HPROVIDER* prov, char** name)  { if(next) *next=0; if(prov) *prov=0; if(name) *name=nullptr; return 0; }

// --- Filter enumeration -----------------------------------------------------
inline S32  AIL_enumerate_filters(void* last, HPROVIDER* prov, char const** name) { if(prov) *prov=0; if(name) *name=nullptr; return 0; }
inline S32  AIL_enumerate_filters(HPROENUM* next, HPROVIDER* prov, char** name) { if(next) *next=0; if(prov) *prov=0; if(name) *name=nullptr; return 0; }

// --- Quick-play helpers -----------------------------------------------------
inline HAUDIO AIL_quick_load_and_play(char const* path, U32 loop, S32 reserve)  { return nullptr; }
inline void   AIL_quick_unload        (void* handle)                             {}
inline void   AIL_quick_set_volume    (HAUDIO handle, F32 vol, F32 pan)          {}
inline void   AIL_WAV_info            (void const* data, AILSOUNDINFO* info)     { if(info) ZeroMemory(info, sizeof(*info)); }
inline void   AIL_decompress_ADPCM    (AILSOUNDINFO* info, void** out, U32* size) { if(out) *out=nullptr; if(size) *size=0; }
inline void   AIL_mem_free_lock       (void* p)                                   {}

#endif // ZH_OPENAL_AUDIO

#endif // STUB_IMPL
#endif // ZH_MILES_SOUND_STUB_H
