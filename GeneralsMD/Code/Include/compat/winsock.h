// winsock.h — compat shim for Windows Sockets 1.1
#pragma once
#ifndef ZH_COMPAT_WINSOCK_H
#define ZH_COMPAT_WINSOCK_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

// Map SOCKET type — on POSIX it's an int
#ifndef ZH_COMPAT_SOCKET_DEFINED
#define ZH_COMPAT_SOCKET_DEFINED
typedef int SOCKET;
#endif
typedef struct hostent HOSTENT;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  ((SOCKET)(~0))
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR    (-1)
#endif

// WSA error codes mapped to errno equivalents
#define WSAEWOULDBLOCK    EAGAIN
#define WSAEINPROGRESS    EINPROGRESS
#define WSAEALREADY       EALREADY
#define WSAENOTSOCK       ENOTSOCK
#define WSAECONNREFUSED   ECONNREFUSED
#define WSAETIMEDOUT      ETIMEDOUT
#define WSAECONNRESET     ECONNRESET
#define WSAENETUNREACH    ENETUNREACH
#define WSAEADDRINUSE     EADDRINUSE
#define WSAEHOSTUNREACH   EHOSTUNREACH
#define WSAENOTCONN       ENOTCONN

// sockaddr re-exported (already in sys/socket.h)
// WSADATA stub
typedef struct WSAData {
    unsigned short wVersion;
    unsigned short wHighVersion;
    char           szDescription[257];
    char           szSystemStatus[129];
    unsigned short iMaxSockets;
    unsigned short iMaxUdpDg;
    char*          lpVendorInfo;
} WSADATA, *LPWSADATA;

static inline int WSAStartup(unsigned short wVersionRequested, LPWSADATA lpWSAData) {
    (void)wVersionRequested;
    if(lpWSAData) {
        memset(lpWSAData, 0, sizeof(*lpWSAData));
        lpWSAData->wVersion = wVersionRequested;
        lpWSAData->wHighVersion = wVersionRequested;
    }
    return 0;
}
static inline int  WSACleanup(void)       { return 0; }
static inline int  WSAGetLastError(void)  { return errno; }
static inline void WSASetLastError(int e) { errno = e; }

// closesocket → close on POSIX
#ifndef closesocket
#define closesocket close
#endif
// ioctlsocket → ioctl on POSIX
#ifndef ioctlsocket
#include <sys/ioctl.h>
static inline int ioctlsocket(SOCKET s, long cmd, unsigned long* argp) {
    return ioctl(s, cmd, argp);
}
#endif

#endif // !_WIN32
#endif // ZH_COMPAT_WINSOCK_H
