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

#include "always.h"
#include "win.h"
#include "wwdebug.h"

#ifdef _WIN32
// On Windows these are true globals defined once here; win.h declares them extern.
// On non-Windows (Linux/macOS) win.h provides static per-TU stubs instead.
HINSTANCE	ProgramInstance;
HWND		MainWindow;
bool GameInFocus = false;
#endif // _WIN32

/***********************************************************************************************
 * Print_Win32Error -- Print the Win32 error message.                                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   6/21/01    DEL : Created.                                                                 *
 *=============================================================================================*/
#if defined(_WIN32) && defined(_DEBUG)
void __cdecl Print_Win32Error(unsigned long win32Error)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, win32Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);

	WWDEBUG_SAY(("Win32 Error: %s\n", (const char*)lpMsgBuf));
	LocalFree(lpMsgBuf);
}
#endif // _WIN32 && _DEBUG

