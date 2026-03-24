/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// This file contains all the header files that shouldn't change frequently.
// Be careful what you stick in here, because putting files that change often in here will 
// tend to cheese people's goats.

#ifndef __PRERTS_H__
#define __PRERTS_H__

//-----------------------------------------------------------------------------
// srj sez: this must come first, first, first.
#define _STLP_USE_NEWALLOC					1
//#define _STLP_USE_CUSTOM_NEWALLOC		STLSpecialAlloc
class STLSpecialAlloc;


// We actually don't use Windows for much other than timeGetTime, but it was included in 40 
// different .cpp files, so I bit the bullet and included it here.
// PLEASE DO NOT ABUSE WINDOWS OR IT WILL BE REMOVED ENTIRELY. :-)
//--------------------------------------------------------------------------------- System Includes 
#define WIN32_LEAN_AND_MEAN
#if defined(_MSC_VER) && !defined(__GNUC__)
#include <atlbase.h>
#endif
#include <windows.h>
// winnt.h defines BitTest as _bittest (takes LONG*); override immediately with our bitwise version
#ifdef BitTest
#undef BitTest
#endif
#define BitTest( x, i ) ( ( (x) & (i) ) != 0 )
// winuser.h defines AnimateWindow as a Win32 API function-like macro; undefine it
// so that the game's AnimateWindow class (AnimateWindowManager.h) compiles without conflict.
#ifdef AnimateWindow
#undef AnimateWindow
#endif

#include <assert.h>
#include <ctype.h>
#ifdef _WIN32
#include <direct.h>   // _mkdir, _getcwd — Windows-only; POSIX code uses <unistd.h>
#include <excpt.h>    // Windows SEH (__try / __except) — not available on POSIX
#endif
#include <float.h>
#include <fstream>     // fstream.h is deprecated; use standard <fstream>
#ifdef _WIN32
#include <imagehlp.h> // Windows ImageHlp API (SymGetSymFromAddr etc.) — stubs in P5-07
#include <io.h>       // Windows low-level I/O (_read, _write) — POSIX uses <unistd.h>
#endif
#include <limits.h>
#ifdef _WIN32
#include <lmcons.h>   // Windows LAN Manager constants (UNLEN etc.)
#endif
// #include <mapicode.h>  // MAPI header — not in modern Windows SDK; MAPI codes unused by engine
#include <math.h>
#include <memory.h>
#include <mmsystem.h>
#include <objbase.h>
#ifdef _WIN32
#include <ocidl.h>    // Windows COM object interfaces — non-Windows uses compat/objbase.h
#endif
#include <process.h>
#include <shellapi.h>
#include <shlobj.h>
#ifdef _WIN32
#include <shlguid.h>  // Windows Shell GUIDs — no POSIX equivalent
#include <snmp.h>     // Windows SNMP API — not available on POSIX
#endif
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <sys/timeb.h>  // _ftime/_timeb — Windows extension; POSIX uses clock_gettime
#endif
#include <sys/types.h>
#include <tchar.h>
#include <time.h>
#ifdef _WIN32
#include <vfw.h>      // Video for Windows (AVI playback) — Windows-only
#endif
#include <winerror.h>
#include <wininet.h>
#include <winreg.h>

#ifdef _WIN32
#ifndef DIRECTINPUT_VERSION
#	define DIRECTINPUT_VERSION	0x800
#endif
#include <dinput.h>   // DirectInput 8 — Windows-only; non-Windows uses SDL3 (P5-03)
#endif

//------------------------------------------------------------------------------------ STL Includes
// srj sez: no, include STLTypesdefs below, instead, thanks
//#include <algorithm>
//#include <bitset>
//#include <hash_map>
//#include <list>
//#include <map>
//#include <queue>
//#include <set>
//#include <stack>
//#include <string>
//#include <vector>

//------------------------------------------------------------------------------------ RTS Includes
// Icky. These have to be in this order.
#include "Lib/BaseType.h"
#include "Common/STLTypedefs.h"
#include "Common/Errors.h"
#include "Common/Debug.h"
#include "Common/AsciiString.h"
#include "Common/SubsystemInterface.h"

#include "Common/GameCommon.h"
#include "Common/GameMemory.h"
#include "Common/GameType.h"
#include "Common/GlobalData.h"

// You might not want Kindof in here because it seems like it changes frequently, but the problem
// is that Kindof is included EVERYWHERE, so it might as well be precompiled.
#include "Common/INI.h"
#include "Common/KindOf.h"
#include "Common/DisabledTypes.h"
#include "Common/NameKeyGenerator.h"
#include "GameClient/ClientRandomValue.h"
#include "GameLogic/LogicRandomValue.h"
#include "Common/ObjectStatusTypes.h"

#include "Common/Thing.h"
#include "Common/UnicodeString.h"

#endif /* __PRERTS_H__ */
