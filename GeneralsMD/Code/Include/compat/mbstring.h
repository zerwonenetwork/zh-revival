#pragma once
#ifndef ZH_COMPAT_MBSTRING_H
#define ZH_COMPAT_MBSTRING_H

#ifndef _WIN32

#include <stddef.h>

static inline size_t _mbsnccnt(const unsigned char* string, size_t bytes)
{
    size_t count = 0;
    if (!string) {
        return 0;
    }
    while (count < bytes && string[count] != '\0') {
        ++count;
    }
    return count;
}

#endif

#endif
