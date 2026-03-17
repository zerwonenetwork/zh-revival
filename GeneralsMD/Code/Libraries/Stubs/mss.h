// mss.h — stub for Miles Sound System (MSS)
#pragma once

// The open-source drop doesn't ship Miles SDK headers. Stub mode compiles without audio backend.

typedef unsigned long HPROVIDER;
typedef unsigned long HSAMPLE;
typedef unsigned long HSTREAM;
typedef unsigned long HDIGDRIVER;
typedef unsigned long HMDIDRIVER;
typedef unsigned long H3DSAMPLE;
typedef unsigned long H3DPOBJECT;
typedef unsigned int HTIMER;

typedef int S32;
typedef unsigned int U32;
typedef float F32;

typedef void* LPWAVEFORMAT;

#define DP_FILTER 0

static __inline void AIL_set_sample_processor(HSAMPLE, int, HPROVIDER) {}
static __inline void AIL_set_filter_sample_preference(HSAMPLE, const char*, const void*) {}

#ifndef AILCALLBACK
  #if defined(_MSC_VER)
    #define AILCALLBACK __cdecl
  #else
    #define AILCALLBACK
  #endif
#endif

static __inline void AIL_lock(void) {}
static __inline void AIL_unlock(void) {}

