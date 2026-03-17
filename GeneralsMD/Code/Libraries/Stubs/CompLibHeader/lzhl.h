#ifndef LZHL_H
#define LZHL_H

#include "Lib/basetype.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* LZHL_DHANDLE;
typedef void* LZHL_CHANDLE;

inline LZHL_DHANDLE LZHLCreateDecompressor(void) { return NULL; }
inline void LZHLDestroyDecompressor(LZHL_DHANDLE h) { (void)h; }
inline int LZHLDecompress(LZHL_DHANDLE h, char* out, UnsignedInt* dstSz, char* in, UnsignedInt* srcSz) { return 0; }

inline LZHL_CHANDLE LZHLCreateCompressor(void) { return NULL; }
inline void LZHLDestroyCompressor(LZHL_CHANDLE h) { (void)h; }
inline int LZHLCompress(LZHL_CHANDLE h, char* out, UnsignedInt* dstSz, char* in, UnsignedInt* srcSz) { return 0; }

inline UnsignedInt LZHLGetMaxCompressedSize(UnsignedInt rawSize) { return rawSize + (rawSize / 8) + 16; }

#ifdef __cplusplus
}
#endif

#endif
