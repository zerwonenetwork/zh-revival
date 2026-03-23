# Zero Hour Replay File Format (.rep)

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

**Task:** P3-02
**Date:** 2026-03-22
**Source:** `GeneralsMD/Code/GameEngine/Source/Common/Recorder.cpp`

---

## Overview

Zero Hour replay files use a flat binary format with no overall length prefix or
file checksum. The file extension is `.rep`. The default output path is
`Replays/LastReplay.rep` (overwritten each game). Named saves copy the file with
a player-chosen name.

All multi-byte integers are **little-endian** (x86 native). Strings are either
null-terminated ASCII or null-terminated UTF-16LE ("UnicodeString") depending on
context. There is no compression.

---

## File Layout

```
[MAGIC]          6 bytes       ASCII "GENREP" (no null terminator)
[STATS HEADER]   variable      Pre-allocated fields, backfilled at game end
[METADATA]       variable      Replay name, timestamp, version, CRCs
[GAME OPTIONS]   variable      Slot-list ASCII string + local player index
[DIFFICULTY]     4 bytes       Int32
[GAME MODE]      4 bytes       Int32
[RANK POINTS]    4 bytes       Int32
[MAX FPS]        4 bytes       Int32
[COMMAND STREAM] variable      One command record per recorded game message
[EOF]                          No footer or trailing checksum
```

---

## Section Details

### 1. Magic

| Offset | Size | Type   | Value    | Notes                      |
|--------|------|--------|----------|----------------------------|
| 0      | 6    | char[] | `GENREP` | Not null-terminated        |

### 2. Stats Header

Written at start of recording with zero values; backfilled by `logGameStart()`,
`logGameEnd()`, and `logPlayerDisconnect()` once those events fire.

| Field           | Size            | Type        | Notes                                         |
|-----------------|-----------------|-------------|-----------------------------------------------|
| startTime       | sizeof(time_t)  | time_t      | Unix timestamp; 4 bytes on Win32, 8 on Win64  |
| endTime         | sizeof(time_t)  | time_t      | Same size as startTime                        |
| frameDuration   | 4               | UInt32      | Total game length in simulation ticks         |
| desyncGame      | sizeof(Bool)    | Bool (Int)  | 1 if CRC mismatch was detected                |
| quitEarly       | sizeof(Bool)    | Bool (Int)  | 1 if game ended before natural conclusion     |
| playerDiscons   | 8 × sizeof(Bool)| Bool[8]     | One flag per slot (MAX_SLOTS=8); 1 if disconnected |

> **Note on `Bool` size:** `Bool` is `typedef int Bool` in this codebase, so
> each Bool field is **4 bytes**, not 1. This means the stats header is:
> `2 × sizeof(time_t) + 4 + 4 + 4 + 8 × 4` = 56 bytes on Win32 (32-bit time_t)
> or 60 bytes on Win64 (64-bit time_t).

### 3. Metadata

| Field             | Size       | Type        | Notes                                      |
|-------------------|------------|-------------|--------------------------------------------|
| replayName        | variable   | UTF-16LE + null (wchar_t 0) | Localized UI name ("Last Replay") |
| timeVal           | 16         | SYSTEMTIME  | Win32 SYSTEMTIME struct (8 × UInt16)       |
| versionString     | variable   | UTF-16LE + null | Human-readable version, e.g. "1.04 ZH"   |
| versionTimeString | variable   | UTF-16LE + null | Build timestamp string                    |
| versionNumber     | 4          | UInt32      | Packed version number                      |
| exeCRC            | 4          | UInt32      | CRC of the game executable                 |
| iniCRC            | 4          | UInt32      | CRC of the INI data                        |

UTF-16LE strings are written with `fwprintf(file, L"%ws", str)` followed by
`fputwc(0, file)` — one `wchar_t` (2 bytes) null terminator.

### 4. Game Options

| Field           | Size       | Type         | Notes                                     |
|-----------------|------------|--------------|-------------------------------------------|
| gameOptions     | variable   | ASCII + null | `GameInfoToAsciiString()` format (see below) |
| localPlayerIndex| variable   | ASCII + null | Decimal integer as string, e.g. `"0\0"`  |

#### GameInfo ASCII String Format

The slot-list string is produced by `GameInfoToAsciiString()` and parsed back by
`ParseAsciiStringToGameInfo()`. It encodes map name, seed, CRC interval, and one
entry per slot (player name, IP, faction, color, team, etc.) in a semicolon- or
colon-delimited text format. The exact field ordering is defined in
`GameNetwork/GameInfo.cpp`. This field is variable length and null-terminated.

### 5. Game Settings

| Field         | Size | Type  | Notes                                           |
|---------------|------|-------|-------------------------------------------------|
| difficulty    | 4    | Int32 | `GameDifficulty` enum value                     |
| gameMode      | 4    | Int32 | `originalGameMode` — SP/MP/Skirmish etc.        |
| rankPoints    | 4    | Int32 | Starting rank points added at game start        |
| maxFPS        | 4    | Int32 | FPS cap selected for this game                  |

---

## Command Stream

After the header, the remainder of the file is a sequence of command records.
Each record corresponds to one `GameMessage` recorded by `writeToFile()`.

### Command Record Format

```
[frame]       4 bytes   UInt32       — simulation tick when this command executes
[type]        4 bytes   Int32        — GameMessage::Type enum value
[playerIndex] 4 bytes   Int32        — slot index of the issuing player (0–7)
[numTypes]    1 byte    UInt8        — number of argument-type descriptor pairs
[typeDesc...]           pairs        — numTypes × (argTypeEnum + argCount), 1 byte each
[args...]               variable     — argument data, one entry per argument
```

### Argument Type Descriptor Pairs

Each pair is two `UInt8` values:
- `argType` — `GameMessageArgumentDataType` enum (see table below)
- `argCount` — number of consecutive arguments of this type

### Argument Data Types

| Enum value | Name                       | Size per arg | Notes                            |
|------------|----------------------------|--------------|----------------------------------|
| 0          | ARGUMENTDATATYPE_INTEGER   | 4 bytes      | `Int32`                          |
| 1          | ARGUMENTDATATYPE_REAL      | 4 bytes      | `float` (IEEE 754 single)        |
| 2          | ARGUMENTDATATYPE_BOOLEAN   | 4 bytes      | `Bool` = `Int32`; 0=false, 1=true|
| 3          | ARGUMENTDATATYPE_OBJECTID  | 4 bytes      | Object ID (UInt32)               |
| 4          | ARGUMENTDATATYPE_DRAWABLEID| 4 bytes      | Drawable ID (UInt32)             |
| 5          | ARGUMENTDATATYPE_TEAMID    | 4 bytes      | Team ID (UInt32)                 |
| 6          | ARGUMENTDATATYPE_LOCATION  | 12 bytes     | `Coord3D`: 3 × float (x, y, z)  |
| 7          | ARGUMENTDATATYPE_PIXEL     | 8 bytes      | `ICoord2D`: 2 × Int32 (x, y)    |
| 8          | ARGUMENTDATATYPE_PIXELREGION| 16 bytes    | `IRegion2D`: 4 × Int32 (lo.x, lo.y, hi.x, hi.y) |
| 9          | ARGUMENTDATATYPE_TIMESTAMP | 4 bytes      | UInt32 tick value                |
| 10         | ARGUMENTDATATYPE_WIDECHAR  | 2 bytes      | Single UTF-16 character (`wchar_t`) |
| 11         | ARGUMENTDATATYPE_UNKNOWN   | (skip)       | Not written                      |

The total argument count for a record is the sum of all `argCount` values across
all type descriptors.

---

## Replay Termination

There is no end-of-stream marker. Playback ends when the game detects that the
recorded frame count has been reached, or when `fread` returns 0 (EOF). Parsers
should treat unexpected EOF as a truncated (possibly still valid) replay.

---

## CRC / Seed Fields in Replay Context

- `exeCRC` and `iniCRC` in the header must match the loading client's values for
  playback to proceed (version CRC enforcement, P2-08).
- The game logic RNG seed is embedded in the `gameOptions` GameInfo string.
  `GetGameLogicRandomSeed()` reads it back during playback via
  `InitGameLogicRandom(getSeed())`.

---

## Known Issues / Gaps

| Issue | Notes |
|-------|-------|
| `sizeof(Bool)` = 4 | `Bool` is `int`, not `uint8_t`. Parsers must read 4-byte booleans in the stats header and per-argument booleans. |
| `sizeof(time_t)` varies | 4 on Win32, 8 on Win64. Replay files are not portable between 32-bit and 64-bit builds. |
| No file-level checksum | A truncated file cannot be distinguished from a complete one without replaying all commands. |
| GameInfo string format | Undocumented; see `GameInfo.cpp:GameInfoToAsciiString()` for the canonical encoder. |
| Map name not written | The map name write was commented out (see `Recorder.cpp:692–693`). Map identity is recovered from the GameInfo string instead. |
| Player name block commented out | The per-player name/faction/color block was commented out and replaced with the GameInfo slot list (lines 654–673). |

---

## Quick Reference: Header Byte Map (Win32 / 32-bit time_t)

```
Offset  Size  Field
0       6     Magic "GENREP"
6       4     startTime (time_t, 32-bit)
10      4     endTime (time_t, 32-bit)
14      4     frameDuration (UInt32)
18      4     desyncGame (Bool=Int32)
22      4     quitEarly (Bool=Int32)
26      32    playerDiscons[8] (Bool=Int32 each)
58      var   replayName (UTF-16LE + 2-byte null)
?       16    timeVal (SYSTEMTIME)
?       var   versionString (UTF-16LE + null)
?       var   versionTimeString (UTF-16LE + null)
?       4     versionNumber (UInt32)
?       4     exeCRC (UInt32)
?       4     iniCRC (UInt32)
?       var   gameOptions (ASCII + 1-byte null)
?       var   localPlayerIndex (ASCII decimal + 1-byte null)
?       4     difficulty (Int32)
?       4     gameMode (Int32)
?       4     rankPoints (Int32)
?       4     maxFPS (Int32)
?       ...   command stream
```

Fields marked `var` have variable length due to null-terminated strings.
All offsets after the stats header must be computed by reading sequentially.
