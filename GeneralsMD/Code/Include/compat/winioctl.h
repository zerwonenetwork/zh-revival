#pragma once
#ifndef ZH_COMPAT_WINIOCTL_H
#define ZH_COMPAT_WINIOCTL_H

#ifndef _WIN32

#ifndef FILE_DEVICE_UNKNOWN
#define FILE_DEVICE_UNKNOWN 0x00000022
#endif

#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED 0
#endif

#ifndef FILE_ANY_ACCESS
#define FILE_ANY_ACCESS 0
#endif

#ifndef FILE_WRITE_DATA
#define FILE_WRITE_DATA 0x0002
#endif

#ifndef CTL_CODE
#define CTL_CODE(DeviceType, Function, Method, Access) \
    (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#endif

#endif

#endif
