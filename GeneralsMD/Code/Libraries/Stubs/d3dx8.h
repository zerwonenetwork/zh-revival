// d3dx8.h — minimal stub header for D3DX8 helpers
//
// Only provides what the codebase uses for logging and a few constants.
// This is NOT a functional D3DX implementation.
#pragma once

#include "d3d8.h"

#ifdef __cplusplus
extern "C" {
#endif

// Filter constants used by texture helper calls
#ifndef D3DX_FILTER_BOX
#define D3DX_FILTER_BOX 0
#endif

// The engine uses D3DXGetErrorStringA for debug output.
static __inline HRESULT D3DXGetErrorStringA(unsigned int /*res*/, char* buffer, unsigned int bufferLen)
{
  if (buffer && bufferLen) {
    buffer[0] = '\0';
  }
  return D3D_OK;
}

#ifdef __cplusplus
} // extern "C"
#endif

