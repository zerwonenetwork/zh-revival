// mss.h — stub for Miles Sound System (MSS)
#pragma once

// The open-source drop doesn't ship Miles SDK headers. Stub mode compiles without audio backend.

typedef void* HPROVIDER;
typedef void* HSAMPLE;
typedef void* HSTREAM;
typedef void* HDIGDRIVER;
typedef void* HMDIDRIVER;
typedef void* H3DSAMPLE;
typedef void* H3DPOBJECT;
typedef unsigned int HTIMER;

typedef int S32;
typedef unsigned int U32;

typedef void* LPWAVEFORMAT;

#ifndef AILCALLBACK
  #if defined(_MSC_VER)
    #define AILCALLBACK __cdecl
  #else
    #define AILCALLBACK
  #endif
#endif

static __inline void AIL_lock(void) {}
static __inline void AIL_unlock(void) {}

