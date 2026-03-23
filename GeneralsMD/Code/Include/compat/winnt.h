// winnt.h — compat shim for Windows NT base types and PE structures
#pragma once
#ifndef ZH_COMPAT_WINNT_H
#define ZH_COMPAT_WINNT_H

#ifndef _WIN32

#ifndef ZH_COMPAT_WINDOWS_H
#include "windows.h"
#endif

// ---------------------------------------------------------------------------
//  PE (Portable Executable) image structures
//  Used by verchk.cpp to inspect EXE timestamps.
// ---------------------------------------------------------------------------
#ifndef _IMAGE_DOS_HEADER_
#define _IMAGE_DOS_HEADER_
typedef struct _IMAGE_DOS_HEADER {
    WORD  e_magic;        // Magic number (0x5A4D = "MZ")
    WORD  e_cblp;
    WORD  e_cp;
    WORD  e_crlc;
    WORD  e_cparhdr;
    WORD  e_minalloc;
    WORD  e_maxalloc;
    WORD  e_ss;
    WORD  e_sp;
    WORD  e_csum;
    WORD  e_ip;
    WORD  e_cs;
    WORD  e_lfarlc;
    WORD  e_ovno;
    WORD  e_res[4];
    WORD  e_oemid;
    WORD  e_oeminfo;
    WORD  e_res2[10];
    LONG  e_lfanew;       // Offset to PE header (IMAGE_NT_HEADERS)
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
#endif

#ifndef _IMAGE_FILE_HEADER_
#define _IMAGE_FILE_HEADER_
typedef struct _IMAGE_FILE_HEADER {
    WORD  Machine;
    WORD  NumberOfSections;
    DWORD TimeDateStamp;
    DWORD PointerToSymbolTable;
    DWORD NumberOfSymbols;
    WORD  SizeOfOptionalHeader;
    WORD  Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;
#endif

#ifndef _IMAGE_OPTIONAL_HEADER_
#define _IMAGE_OPTIONAL_HEADER_
typedef struct _IMAGE_OPTIONAL_HEADER {
    WORD  Magic;
    BYTE  MajorLinkerVersion;
    BYTE  MinorLinkerVersion;
    DWORD SizeOfCode;
    DWORD SizeOfInitializedData;
    DWORD SizeOfUninitializedData;
    DWORD AddressOfEntryPoint;
    DWORD BaseOfCode;
    DWORD BaseOfData;
    DWORD ImageBase;
    DWORD SectionAlignment;
    DWORD FileAlignment;
    WORD  MajorOperatingSystemVersion;
    WORD  MinorOperatingSystemVersion;
    WORD  MajorImageVersion;
    WORD  MinorImageVersion;
    WORD  MajorSubsystemVersion;
    WORD  MinorSubsystemVersion;
    DWORD Win32VersionValue;
    DWORD SizeOfImage;
    DWORD SizeOfHeaders;
    DWORD CheckSum;
    WORD  Subsystem;
    WORD  DllCharacteristics;
    DWORD SizeOfStackReserve;
    DWORD SizeOfStackCommit;
    DWORD SizeOfHeapReserve;
    DWORD SizeOfHeapCommit;
    DWORD LoaderFlags;
    DWORD NumberOfRvaAndSizes;
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;
#endif

#ifndef _IMAGE_NT_HEADERS_
#define _IMAGE_NT_HEADERS_
typedef struct _IMAGE_NT_HEADERS {
    DWORD                 Signature;
    IMAGE_FILE_HEADER     FileHeader;
    IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;
#endif

// Machine types
#define IMAGE_FILE_MACHINE_UNKNOWN  0x0000
#define IMAGE_FILE_MACHINE_I386     0x014C
#define IMAGE_FILE_MACHINE_AMD64    0x8664
#define IMAGE_FILE_MACHINE_ARM      0x01C0
#define IMAGE_FILE_MACHINE_ARM64    0xAA64

#endif // !_WIN32
#endif // ZH_COMPAT_WINNT_H
