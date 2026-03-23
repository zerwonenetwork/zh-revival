// winsock2.h — compat shim for Windows Sockets 2.0
// Delegates to our winsock.h shim which provides the POSIX socket bridge.
#pragma once
#ifndef ZH_COMPAT_WINSOCK2_H
#define ZH_COMPAT_WINSOCK2_H

#ifndef _WIN32
#include "winsock.h"
#endif

#endif // ZH_COMPAT_WINSOCK2_H
