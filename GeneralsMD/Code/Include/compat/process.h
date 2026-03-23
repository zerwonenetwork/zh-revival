// process.h — compat shim for MSVC <process.h>
// Provides _getpid, _spawnl, _execl etc. via POSIX equivalents.
#pragma once
#ifndef ZH_COMPAT_PROCESS_H
#define ZH_COMPAT_PROCESS_H

#ifndef _WIN32

#include <unistd.h>
#include <stdlib.h>

// getpid / _getpid
#ifndef _getpid
#define _getpid getpid
#endif

// _getppid
#ifndef _getppid
#define _getppid getppid
#endif

// _exit
#ifndef _exit
/* _exit already a POSIX function, nothing to do */
#endif

// _spawnl / _spawnlp stubs (no-op on non-Windows)
static inline int _spawnl(int mode, const char* path, const char* arg0, ...) {
    (void)mode;(void)path;(void)arg0; return -1;
}
static inline int _spawnlp(int mode, const char* file, const char* arg0, ...) {
    (void)mode;(void)file;(void)arg0; return -1;
}

// _execl stubs
static inline int _execl(const char* path, const char* arg0, ...) {
    (void)path;(void)arg0; return -1;
}

// _beginthread / _endthread stubs (no-op; real threading not supported in stub mode)
#include <pthread.h>
static inline unsigned long _beginthread(void (*start)(void*), unsigned stacksize, void* arg) {
    (void)stacksize;
    pthread_t t;
    return pthread_create(&t, NULL, (void*(*)(void*))start, arg) == 0 ? (unsigned long)t : 0;
}
static inline void _endthread(void) {}

// P_* constants used with _spawn
#define _P_WAIT    0
#define _P_NOWAIT  1
#define _P_OVERLAY 2
#define _P_DETACH  4

#endif // !_WIN32
#endif // ZH_COMPAT_PROCESS_H
