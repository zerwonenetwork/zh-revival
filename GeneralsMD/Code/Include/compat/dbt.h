// dbt.h — Device Broadcast Messages stub for non-Windows builds
//
// WinMain.cpp includes <dbt.h> for WM_DEVICECHANGE handling.
// On Linux / macOS there is no Win32 device-broadcast API; stubs are
// provided so the file parses without error.
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.
#pragma once
#ifndef ZH_COMPAT_DBT_H
#define ZH_COMPAT_DBT_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// WM_DEVICECHANGE notification codes
#define DBT_DEVICEARRIVAL         0x8000
#define DBT_DEVICEQUERYREMOVE     0x8001
#define DBT_DEVICEQUERYREMOVEFAILED 0x8002
#define DBT_DEVICEREMOVEPENDING   0x8003
#define DBT_DEVICEREMOVECOMPLETE  0x8004
#define DBT_DEVTYP_OEM            0x00000000
#define DBT_DEVTYP_DEVNODE        0x00000001
#define DBT_DEVTYP_VOLUME         0x00000002
#define DBT_DEVTYP_PORT           0x00000003
#define DBT_DEVTYP_NET            0x00000004
#define DBT_DEVTYP_DEVICEINTERFACE 0x00000005
#define DBT_DEVTYP_HANDLE         0x00000006

typedef struct _DEV_BROADCAST_HDR {
    DWORD dbch_size;
    DWORD dbch_devicetype;
    DWORD dbch_reserved;
} DEV_BROADCAST_HDR, *PDEV_BROADCAST_HDR;

typedef struct _DEV_BROADCAST_VOLUME {
    DEV_BROADCAST_HDR dbcv_hdr;
    DWORD dbcv_unitmask;
    WORD  dbcv_flags;
} DEV_BROADCAST_VOLUME, *PDEV_BROADCAST_VOLUME;

#define DBTF_MEDIA   0x0001
#define DBTF_NET     0x0002

#endif // !_WIN32
#endif // ZH_COMPAT_DBT_H
