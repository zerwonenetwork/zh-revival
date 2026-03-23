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

//----------------------------------------------------------------------------
// P4-05 — Mod Manager Foundation
// Scans a /Mods/ folder, loads .big files in priority order, and reports
// file conflicts to Logs/mod_conflicts.log.
//----------------------------------------------------------------------------

#pragma once

#ifndef __MODMANAGER_H_
#define __MODMANAGER_H_

#include "Common/AsciiString.h"

//===============================
// ModManager
//===============================
/**
 * Scans the user-configured Mods folder (default: <UserData>/Mods/),
 * loads each .big file in ascending numeric-prefix order so that higher
 * numbers override lower ones, and logs any file conflicts.
 *
 * Bypassed entirely when --no-mods is passed on the command line.
 *
 * The Mods folder path is stored in HKCU under:
 *   Software\zh-revival\Options  →  ModsDir (REG_SZ)
 * Defaults to <UserData>\Mods\ if the key is absent.
 */
//===============================
class ModManager
{
public:
    /// Load all mods from the configured mods folder.
    /// Called from parseCommandLine() after the base game archives are loaded.
    static void loadMods( void );

    /// Returns the resolved mods folder path (always ends with a backslash).
    static AsciiString getModsFolder( void );

    static void setNoMods( Bool value ) { s_noMods = value; }
    static Bool isNoMods( void )        { return s_noMods; }

private:
    static Bool s_noMods;  ///< Set true by --no-mods command-line flag

    /// Persist the mods folder path to HKCU.
    static void saveModsFolderToRegistry( const AsciiString& path );
};

#endif // __MODMANAGER_H_
