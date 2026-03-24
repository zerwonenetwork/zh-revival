// io.h — non-Windows compatibility shim
//
// Maps MSVC low-level I/O API (_open, _read, _write, _close, _lseek, _access,
// _stat, _O_xxx, _S_xxx) to POSIX equivalents so that Windows source files
// that include <io.h> compile on Linux and macOS without modification.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.

#pragma once
#ifndef ZH_COMPAT_IO_H
#define ZH_COMPAT_IO_H

#ifndef _WIN32

#include <stdarg.h>   // va_list, va_start, va_arg, va_end — needed by _open variadic stub
#include <stddef.h>   // size_t
#include <unistd.h>   // read, write, close, lseek, access
#include <fcntl.h>    // open, O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_TRUNC, O_APPEND
#include <sys/stat.h> // stat, S_IRUSR, S_IWUSR, S_IFREG, S_IFDIR

// ---------------------------------------------------------------------------
//  Open flags — map _O_xxx to POSIX O_xxx
//  _O_BINARY and _O_TEXT are no-ops on POSIX (no text-mode newline translation)
// ---------------------------------------------------------------------------
#ifndef _O_RDONLY
#define _O_RDONLY   O_RDONLY
#endif
#ifndef _O_WRONLY
#define _O_WRONLY   O_WRONLY
#endif
#ifndef _O_RDWR
#define _O_RDWR     O_RDWR
#endif
#ifndef _O_CREAT
#define _O_CREAT    O_CREAT
#endif
#ifndef _O_TRUNC
#define _O_TRUNC    O_TRUNC
#endif
#ifndef _O_APPEND
#define _O_APPEND   O_APPEND
#endif
#ifndef _O_BINARY
#define _O_BINARY   0     // no-op on POSIX
#endif
#ifndef _O_TEXT
#define _O_TEXT     0     // no-op on POSIX
#endif
#ifndef _O_EXCL
#define _O_EXCL     O_EXCL
#endif
#ifndef _O_NOINHERIT
#define _O_NOINHERIT 0    // no equivalent — ignored
#endif

// ---------------------------------------------------------------------------
//  Permission bits — map _S_Ixxx to POSIX S_Ixxx
// ---------------------------------------------------------------------------
#ifndef _S_IREAD
#define _S_IREAD    S_IRUSR   // 0400
#endif
#ifndef _S_IWRITE
#define _S_IWRITE   S_IWUSR   // 0200
#endif
#ifndef _S_IEXEC
#define _S_IEXEC    S_IXUSR   // 0100
#endif
#ifndef _S_IFREG
#define _S_IFREG    S_IFREG
#endif
#ifndef _S_IFDIR
#define _S_IFDIR    S_IFDIR
#endif
#ifndef _S_IFMT
#define _S_IFMT     S_IFMT
#endif

// ---------------------------------------------------------------------------
//  File stat — _stat struct is the same as struct stat on POSIX
// ---------------------------------------------------------------------------
#ifndef _stat
#define _stat       stat
#endif

// ---------------------------------------------------------------------------
//  Low-level I/O functions — map MSVC underscore names to POSIX
// ---------------------------------------------------------------------------
static inline int _open(const char *path, int flags, ...)
{
    // Extract optional mode argument (used when O_CREAT is set)
    mode_t mode = 0666;
    if (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = (mode_t)va_arg(ap, int);
        va_end(ap);
    }
    return open(path, flags, mode);
}

static inline int _close(int fd)
{
    return close(fd);
}

static inline long _read(int fd, void *buf, unsigned int count)
{
    return (long)read(fd, buf, (size_t)count);
}

static inline long _write(int fd, const void *buf, unsigned int count)
{
    return (long)write(fd, buf, (size_t)count);
}

static inline long _lseek(int fd, long offset, int whence)
{
    return (long)lseek(fd, (off_t)offset, whence);
}

#ifndef ZH_COMPAT_ACCESS_DEFINED
#define ZH_COMPAT_ACCESS_DEFINED
static inline int _access(const char *path, int mode)
{
    return access(path, mode);
}
#endif

static inline long _filelength(int fd)
{
    long cur = _lseek(fd, 0, SEEK_CUR);
    long end = _lseek(fd, 0, SEEK_END);
    _lseek(fd, cur, SEEK_SET);
    return end;
}

static inline int _eof(int fd)
{
    long cur = _lseek(fd, 0, SEEK_CUR);
    long end = _lseek(fd, 0, SEEK_END);
    _lseek(fd, cur, SEEK_SET);
    return (cur >= end) ? 1 : 0;
}

static inline long _tell(int fd)
{
    return _lseek(fd, 0, SEEK_CUR);
}

#endif // !_WIN32
#endif // ZH_COMPAT_IO_H
