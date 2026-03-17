// mss.h — stub for Miles Sound System (MSS)
#pragma once

// The open-source drop doesn't ship Miles SDK headers. Stub mode compiles without audio backend.

typedef unsigned long HPROVIDER;
typedef unsigned long HSAMPLE;
typedef unsigned long HSTREAM;
typedef unsigned long HMDIDRIVER;
typedef unsigned long H3DSAMPLE;
typedef unsigned long H3DPOBJECT;
typedef unsigned int HTIMER;

// HDIGDRIVER is a pointer to a driver struct (Miles SDK uses pointer type)
typedef struct _HDIGDRIVER_STRUCT {
  int emulated_ds;
} * HDIGDRIVER;

typedef int S32;
typedef unsigned int U32;
typedef float F32;

// LPWAVEFORMAT is defined in mmeapi.h (included via windows.h) as WAVEFORMAT*
// Only define it here if windows.h hasn't been included yet
#if !defined(_MMEAPI_H_) && !defined(_WAVEFORMATEX_) && !defined(LPWAVEFORMAT)
typedef void* LPWAVEFORMAT;
#endif

#define DP_FILTER 0

static __inline void AIL_set_sample_processor(HSAMPLE, int, HPROVIDER) {}
static __inline void AIL_set_filter_sample_preference(HSAMPLE, const char*, const void*) {}

static __inline void AIL_set_3D_position(H3DSAMPLE, F32, F32, F32) {}
static __inline void AIL_set_3D_orientation(H3DSAMPLE, F32, F32, F32, F32, F32, F32) {}
static __inline void AIL_set_3D_velocity_vector(H3DSAMPLE, F32, F32, F32) {}
static __inline void AIL_set_3D_sample_distances(H3DSAMPLE, F32, F32) {}
static __inline void AIL_set_3D_sample_effects_level(H3DSAMPLE, F32) {}

#ifndef WAVE_FORMAT_IMA_ADPCM
#define WAVE_FORMAT_IMA_ADPCM 0x0011
#endif

typedef struct AILSOUNDINFO {
  S32 format;
  S32 rate;
  S32 bits;
  S32 channels;
} AILSOUNDINFO;

static __inline S32 AIL_WAV_info(void*, AILSOUNDINFO*) { return 0; }

#ifndef AILCALLBACK
  #if defined(_MSC_VER)
    #define AILCALLBACK __cdecl
  #else
    #define AILCALLBACK
  #endif
#endif

static __inline void AIL_lock(void) {}
static __inline void AIL_unlock(void) {}

// AIL startup/shutdown
static __inline void AIL_startup(void) {}
static __inline void AIL_shutdown(void) {}

// AIL preferences
#define AIL_LOCK_PROTECTION       0
#define DIG_USE_WAVEOUT           1
#define AIL_NO_ERROR              0
#define NO                        0

// AIL preference setter (returns S32 status)
static __inline S32 AIL_set_preference(U32 number, S32 value) { (void)number; (void)value; return AIL_NO_ERROR; }

// 2D driver open/close
static __inline S32 AIL_waveOutOpen(HDIGDRIVER*, void*, U32, void*) { return AIL_NO_ERROR; }
static __inline void AIL_waveOutClose(HDIGDRIVER) {}

// 3D environment
#define ENVIRONMENT_GENERIC 0

// Sample allocation
static __inline HSAMPLE AIL_allocate_sample_handle(HDIGDRIVER) { return 0; }
static __inline void AIL_release_sample_handle(HSAMPLE) {}
static __inline void AIL_init_sample(HSAMPLE) {}
static __inline void AIL_set_sample_address(HSAMPLE, const void*, U32) {}
static __inline void AIL_set_sample_type(HSAMPLE, S32, U32) {}
static __inline void AIL_set_sample_volume(HSAMPLE, S32) {}
static __inline void AIL_set_sample_pan(HSAMPLE, S32) {}
static __inline void AIL_set_sample_loop_count(HSAMPLE, S32) {}
static __inline void AIL_start_sample(HSAMPLE) {}
static __inline void AIL_stop_sample(HSAMPLE) {}
static __inline void AIL_resume_sample(HSAMPLE) {}
static __inline void AIL_end_sample(HSAMPLE) {}
static __inline S32 AIL_sample_status(HSAMPLE) { return 0; }
static __inline S32 AIL_sample_volume(HSAMPLE) { return 0; }
static __inline S32 AIL_sample_pan(HSAMPLE) { return 0; }
static __inline U32 AIL_sample_position(HSAMPLE) { return 0; }
static __inline void AIL_set_sample_position(HSAMPLE, U32) {}
static __inline void AIL_set_sample_playback_rate(HSAMPLE, S32) {}
static __inline S32 AIL_sample_playback_rate(HSAMPLE) { return 0; }

// Stream functions
static __inline HSTREAM AIL_open_stream(HDIGDRIVER, const char*, S32) { return 0; }
static __inline void AIL_close_stream(HSTREAM) {}
static __inline void AIL_start_stream(HSTREAM) {}
static __inline void AIL_pause_stream(HSTREAM, S32) {}
static __inline void AIL_set_stream_volume(HSTREAM, S32) {}
static __inline void AIL_set_stream_pan(HSTREAM, S32) {}
static __inline S32 AIL_stream_status(HSTREAM) { return 0; }
static __inline S32 AIL_stream_volume(HSTREAM) { return 0; }
static __inline S32 AIL_stream_pan(HSTREAM) { return 0; }
static __inline void AIL_set_stream_loop_count(HSTREAM, S32) {}
static __inline void AIL_set_stream_position(HSTREAM, S32) {}
static __inline S32 AIL_stream_position(HSTREAM) { return 0; }
static __inline S32 AIL_stream_ms_position(HSTREAM, S32*, S32*) { return 0; }

// 3D provider/sample
static __inline HPROVIDER AIL_open_3D_provider(U32) { return 0; }
static __inline void AIL_close_3D_provider(HPROVIDER) {}
static __inline H3DSAMPLE AIL_allocate_3D_sample_handle(HPROVIDER) { return 0; }
static __inline void AIL_release_3D_sample_handle(H3DSAMPLE) {}
static __inline void AIL_init_3D_sample_from_file(H3DSAMPLE, const void*, S32, S32, AILSOUNDINFO*) {}
static __inline void AIL_start_3D_sample(H3DSAMPLE) {}
static __inline void AIL_stop_3D_sample(H3DSAMPLE) {}
static __inline void AIL_resume_3D_sample(H3DSAMPLE) {}
static __inline void AIL_end_3D_sample(H3DSAMPLE) {}
static __inline S32 AIL_3D_sample_status(H3DSAMPLE) { return 0; }
static __inline void AIL_set_3D_sample_volume(H3DSAMPLE, S32) {}
static __inline S32 AIL_3D_sample_volume(H3DSAMPLE) { return 0; }
static __inline void AIL_set_3D_sample_loop_count(H3DSAMPLE, U32) {}
static __inline void AIL_set_3D_sample_offset(H3DSAMPLE, U32) {}
static __inline U32 AIL_3D_sample_offset(H3DSAMPLE) { return 0; }
static __inline U32 AIL_3D_sample_length(H3DSAMPLE) { return 0; }
static __inline void AIL_set_3D_room_type(HPROVIDER, S32) {}
static __inline void AIL_set_3D_speaker_type(HPROVIDER, S32) {}
static __inline void AIL_set_3D_listener_position(HPROVIDER, F32, F32, F32, F32, F32, F32, F32, F32, F32) {}
static __inline void AIL_set_3D_listener_orientation(HPROVIDER, F32, F32, F32, F32, F32, F32) {}
static __inline void AIL_set_3D_listener_velocity(HPROVIDER, F32, F32, F32, F32) {}
static __inline void AIL_update_3D_position(HPROVIDER, F32) {}

// Error string
static __inline const char* AIL_last_error(void) { return ""; }

