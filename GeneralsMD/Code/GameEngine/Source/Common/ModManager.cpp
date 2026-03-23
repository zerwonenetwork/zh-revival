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
//----------------------------------------------------------------------------

#include "PreRTS.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <sys/stat.h>

#include <vector>
#include <algorithm>
#include <cstdio>

#include "Common/ModManager.h"
#include "Common/ArchiveFileSystem.h"
#include "Common/ArchiveFile.h"
#include "Common/GlobalData.h"
#include "Common/FileSystem.h"
#include "Common/AsciiString.h"

//----------------------------------------------------------------------------
// Registry path (reuses the same HKCU base as P3-01 window mode prefs)
//----------------------------------------------------------------------------

#define ZH_REGKEY_OPTIONS   "Software\\zh-revival\\Options"
#define ZH_REGVAL_MODSDIR   "ModsDir"

//----------------------------------------------------------------------------
// Static member initialisation
//----------------------------------------------------------------------------

Bool ModManager::s_noMods = FALSE;

//----------------------------------------------------------------------------
// Helpers
//----------------------------------------------------------------------------

/// Numeric sort: compare leading digit sequences of two filenames so that
/// "10_Foo.big" sorts after "9_Bar.big".  Falls back to strcmp for equal
/// prefixes or no leading digits.
static bool modBigLessThan( const AsciiString& a, const AsciiString& b )
{
    const char *pa = a.str();
    const char *pb = b.str();

    // Extract optional leading integer prefix (digits before any non-digit)
    long ia = 0, ib = 0;
    Bool hasA = FALSE, hasB = FALSE;
    if (*pa >= '0' && *pa <= '9') { ia = atol(pa); hasA = TRUE; }
    if (*pb >= '0' && *pb <= '9') { ib = atol(pb); hasB = TRUE; }

    if (hasA && hasB && ia != ib) return ia < ib;   // numeric compare
    return strcmp(pa, pb) < 0;                       // lexicographic fallback
}

/// Write one line to the mod conflict log.  The file is opened in append mode
/// so multiple calls accumulate in a single log per session.
static void logConflict( FILE *fp, const char *virtualPath,
                          const char *existingBig, const char *newBig )
{
    if (fp)
    {
        fprintf(fp, "OVERRIDE  %-60s  [%s] -> [%s]\n",
                virtualPath, existingBig, newBig);
    }
}

//----------------------------------------------------------------------------
// ModManager::getModsFolder
//----------------------------------------------------------------------------
AsciiString ModManager::getModsFolder( void )
{
    // 1. Try HKCU registry value.
    char regVal[MAX_PATH] = {};
    HKEY hk;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, ZH_REGKEY_OPTIONS, 0, KEY_READ, &hk) == ERROR_SUCCESS)
    {
        DWORD type = REG_SZ;
        DWORD size = sizeof(regVal);
        RegQueryValueExA(hk, ZH_REGVAL_MODSDIR, NULL, &type,
                         reinterpret_cast<LPBYTE>(regVal), &size);
        RegCloseKey(hk);
    }

    if (regVal[0] != '\0')
    {
        AsciiString result(regVal);
        if (!result.endsWith("\\") && !result.endsWith("/"))
            result.concat('\\');
        return result;
    }

    // 2. Default: <UserData>/Mods/
    AsciiString defaultPath;
    if (TheGlobalData)
        defaultPath = TheGlobalData->getPath_UserData();
    defaultPath.concat("Mods\\");
    return defaultPath;
}

//----------------------------------------------------------------------------
// ModManager::saveModsFolderToRegistry
//----------------------------------------------------------------------------
void ModManager::saveModsFolderToRegistry( const AsciiString& path )
{
    HKEY hk;
    DWORD disp;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, ZH_REGKEY_OPTIONS, 0, NULL,
                        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL,
                        &hk, &disp) == ERROR_SUCCESS)
    {
        RegSetValueExA(hk, ZH_REGVAL_MODSDIR, 0, REG_SZ,
                       reinterpret_cast<const BYTE *>(path.str()),
                       static_cast<DWORD>(strlen(path.str()) + 1));
        RegCloseKey(hk);
    }
}

//----------------------------------------------------------------------------
// ModManager::loadMods
//----------------------------------------------------------------------------
void ModManager::loadMods( void )
{
    // --no-mods: vanilla play, skip everything.
    if (s_noMods)
    {
        DEBUG_LOG(("ModManager::loadMods - skipped (--no-mods flag set)\n"));
        return;
    }

    if (!TheArchiveFileSystem)
    {
        DEBUG_LOG(("ModManager::loadMods - TheArchiveFileSystem is NULL, aborting\n"));
        return;
    }

    AsciiString modsFolder = getModsFolder();
    DEBUG_LOG(("ModManager::loadMods - scanning '%s'\n", modsFolder.str()));

    // Create the Mods folder if it doesn't exist so users see it.
    struct _stat st;
    if (_stat(modsFolder.str(), &st) != 0)
    {
        CreateDirectoryA(modsFolder.str(), NULL);
        DEBUG_LOG(("ModManager::loadMods - created mods folder '%s'\n", modsFolder.str()));
    }

    // Enumerate all .big files in the mods folder (non-recursive).
    std::vector<AsciiString> bigFiles;
    {
        char searchPath[MAX_PATH];
        _snprintf_s(searchPath, sizeof(searchPath), _TRUNCATE,
                    "%s*.big", modsFolder.str());

        WIN32_FIND_DATAA fd;
        HANDLE hFind = FindFirstFileA(searchPath, &fd);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do {
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    bigFiles.push_back(AsciiString(fd.cFileName));
            } while (FindNextFileA(hFind, &fd));
            FindClose(hFind);
        }
    }

    if (bigFiles.empty())
    {
        DEBUG_LOG(("ModManager::loadMods - no .big files found in '%s'\n", modsFolder.str()));
        return;
    }

    // Sort by numeric prefix ascending (lowest number = lowest priority = loaded first).
    std::sort(bigFiles.begin(), bigFiles.end(), modBigLessThan);

    // Open the conflict log.
    char logPath[MAX_PATH];
    if (TheGlobalData)
    {
        _snprintf_s(logPath, sizeof(logPath), _TRUNCATE,
                    "%sLogs\\mod_conflicts.log", TheGlobalData->getPath_UserData().str());
    }
    else
    {
        _snprintf_s(logPath, sizeof(logPath), _TRUNCATE, "Logs\\mod_conflicts.log");
    }

    // Ensure Logs\ directory exists.
    {
        char logsDir[MAX_PATH];
        if (TheGlobalData)
            _snprintf_s(logsDir, sizeof(logsDir), _TRUNCATE,
                        "%sLogs", TheGlobalData->getPath_UserData().str());
        else
            _snprintf_s(logsDir, sizeof(logsDir), _TRUNCATE, "Logs");
        CreateDirectoryA(logsDir, NULL);
    }

    FILE *conflictLog = fopen(logPath, "a");
    if (conflictLog)
    {
        // Header for this session's load.
        SYSTEMTIME st2;
        GetLocalTime(&st2);
        fprintf(conflictLog,
                "=== ModManager session %04d-%02d-%02d %02d:%02d:%02d ===\n",
                st2.wYear, st2.wMonth, st2.wDay,
                st2.wHour, st2.wMinute, st2.wSecond);
        fprintf(conflictLog, "Mods folder: %s\n\n", modsFolder.str());
        fprintf(conflictLog, "%-5s %-60s  %-30s -> %-30s\n",
                "Type", "Virtual path", "Old archive", "New archive");
        fprintf(conflictLog, "%s\n",
                "--------------------------------------"
                "--------------------------------------");
    }

    Int loadedCount = 0;

    for (size_t i = 0; i < bigFiles.size(); ++i)
    {
        AsciiString fullPath;
        fullPath.format("%s%s", modsFolder.str(), bigFiles[i].str());

        DEBUG_LOG(("ModManager::loadMods - loading mod[%zu] '%s'\n", i, fullPath.str()));

        // Open the archive to inspect its contents before loading.
        ArchiveFile *archiveFile = TheArchiveFileSystem->openArchiveFile(fullPath.str());
        if (archiveFile == NULL)
        {
            DEBUG_LOG(("ModManager::loadMods - openArchiveFile failed for '%s'\n", fullPath.str()));
            if (conflictLog)
                fprintf(conflictLog, "FAILED    %-60s  (could not open archive)\n", bigFiles[i].str());
            continue;
        }

        // Get the list of all files this archive provides.
        FilenameList fileList;
        archiveFile->getFileListInDirectory(
            AsciiString(""), AsciiString(""), AsciiString("*"),
            fileList, TRUE);

        // Detect conflicts: a conflict is any file this mod provides that is
        // already mapped to a different archive (base game or earlier mod).
        Int conflictCount = 0;
        for (FilenameListIter it = fileList.begin(); it != fileList.end(); ++it)
        {
            AsciiString existingArchive = TheArchiveFileSystem->getArchiveFilenameForFile(*it);
            if (existingArchive.getLength() > 0 && existingArchive != fullPath)
            {
                logConflict(conflictLog, it->str(),
                            existingArchive.str(), fullPath.str());
                ++conflictCount;
            }
        }

        if (conflictLog && conflictCount == 0 && !fileList.empty())
        {
            fprintf(conflictLog, "CLEAN     %-60s  (%d files, no conflicts)\n",
                    bigFiles[i].str(), static_cast<int>(fileList.size()));
        }
        else if (conflictLog && conflictCount > 0)
        {
            fprintf(conflictLog, "          ... (%d conflict(s) from '%s')\n\n",
                    conflictCount, bigFiles[i].str());
        }

        // Register the archive in the lookup map so openFile() can resolve
        // files from this archive after the directory tree is updated.
        TheArchiveFileSystem->registerArchiveFile(fullPath, archiveFile);

        // Load into the directory tree.  overwrite=TRUE so higher-indexed
        // mods win over lower-indexed ones, and all mods win over base game.
        TheArchiveFileSystem->loadIntoDirectoryTree(archiveFile, fullPath, TRUE);

        DEBUG_LOG(("ModManager::loadMods - loaded '%s' (%d files, %d conflicts)\n",
                   bigFiles[i].str(), static_cast<int>(fileList.size()), conflictCount));

        ++loadedCount;
    }

    if (conflictLog)
    {
        fprintf(conflictLog, "\nTotal mods loaded: %d / %zu\n\n",
                loadedCount, bigFiles.size());
        fclose(conflictLog);
    }

    DEBUG_LOG(("ModManager::loadMods - done, %d mod(s) loaded\n", loadedCount));
}
