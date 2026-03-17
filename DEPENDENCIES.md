# zh-revival — Dependency Reference

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

This document inventories every third-party dependency in
`GeneralsMD/Code/Libraries/` and describes how to obtain it, its license
status, and its long-term replacement plan.

---

## Quick summary

| Dependency | Type | In vcpkg? | In repo? | Status |
|---|---|---|---|---|
| zlib | Open source | YES | Gitignored | Add via `vcpkg install` |
| Miles Sound System (MSS) | Proprietary | No | Gitignored | Stub in P1-03 → Replace in P5-01 |
| Bink Video | Proprietary | No | Gitignored | Stub in P1-03 → Replace in P5-02 |
| SafeDisc | Proprietary DRM | No | Not present | Stub in P1-03 |
| GameSpy SDK | Defunct middleware | No | Gitignored | Stub in P1-03 |
| DirectX 8 SDK | Legacy Microsoft SDK | No | Not present | Manual install → Replace in P5-03 |
| Granny (animation) | Proprietary | No | Gitignored | No replacement planned yet |
| LZH CompLib | Unknown license | No | Headers gitignored | Needs evaluation |
| STLport 4.5.3 | Open source (obsolete) | No | Gitignored | Not needed — use MSVC STL |
| WPAudio | EA first-party | No | In repo | In-tree, no action needed |
| EAC (compression) | EA first-party | No | In repo | In-tree, no action needed |
| WWVegas (all) | EA first-party | No | In repo | In-tree, no action needed |

---

## vcpkg-managed dependencies

### zlib 1.3.1
- **Purpose:** Deflate compression used in .big archive and save-file I/O
  (plugs into `Libraries/Source/Compression/ZLib/`, which is gitignored)
- **vcpkg name:** `zlib`
- **Minimum version:** `1.3.1`
- **License:** zlib license (permissive)
- **Install:** `vcpkg install zlib:x86-windows`
- **CMake integration:** `find_package(ZLIB REQUIRED)` after vcpkg toolchain

---

## Non-vcpkg dependencies (must be obtained separately)

### Miles Sound System (MSS / mss32)
- **Vendor:** RAD Game Tools (now part of Epic Games)
- **Version used:** Miles 6.x (Miles 6 SDK)
- **Files gitignored:**
  - `Libraries/Include/MSS/` — C API headers
  - `Libraries/Source/WWVegas/Miles6/` — wrapper source and headers
  - `Libraries/Lib/mss32.lib` — import library
  - `mss32.dll` — runtime (ships with the original game install)
- **License:** Proprietary — cannot be redistributed
- **How to obtain:** Must own a licensed copy of the Miles SDK or use the
  version bundled with a legal game installation. The game's `mss32.dll`
  is typically found in the Zero Hour game folder.
- **Stub:** `Code/Libraries/Stubs/MilesSoundStub.h` — created in task P1-03.
  The stub allows the project to compile without the real SDK.
  Audio features degrade gracefully to silence.
- **Replacement plan:** Task P5-01 — replace with **openal-soft** (vcpkg).

### Bink Video (binkw32)
- **Vendor:** RAD Game Tools (now part of Epic Games)
- **Version used:** Bink 1.x
- **Files gitignored:**
  - `Libraries/Lib/binkw32.lib` — import library
  - `binkw32.dll` — runtime (ships with the original game install)
- **License:** Proprietary — cannot be redistributed
- **How to obtain:** Ships with a legal Zero Hour installation.
- **Stub:** `Code/Libraries/Stubs/BinkVideoStub.h` — created in task P1-03.
  The stub allows the project to compile without the real SDK.
  Video playback features degrade to a blank screen / skip.
- **Replacement plan:** Task P5-02 — replace with **FFmpeg** or a
  re-encoded open-format video pipeline.

### SafeDisc
- **Vendor:** Macrovision (defunct product line)
- **Version used:** SafeDisc (CD copy protection, exact version unknown)
- **Status:** SafeDisc was disabled by Microsoft in Windows 10 (KB3086255).
  The check always fails on modern systems.
- **Files:** Not present in the released source. References appear as API
  calls in the game startup code.
- **License:** Proprietary
- **Stub:** `Code/Libraries/Stubs/SafeDiscStub.h` — created in task P1-03.
  The stub returns "CD present / validation passed" unconditionally.
- **Replacement plan:** Task P1-11 removes CD checks entirely.

### GameSpy SDK
- **Vendor:** GameSpy Industries (acquired by Glu Mobile 2004, service shut
  down May 31, 2014)
- **Version used:** GameSpy SDK (exact version unknown from gitignored source)
- **Modules used:**
  - `ghttp` — HTTP requests (patch checking, WOL integration)
  - `peer` — peer-to-peer matchmaking
  - `gp` — GameSpy Presence (login, buddy list)
  - `gstats` — statistics tracking
  - `pt` — patching
- **Files gitignored:**
  - `Libraries/Source/GameSpy/` — all GameSpy SDK source
- **License:** Proprietary (SDK license) — source cannot be redistributed
  under the terms it was originally licensed
- **Stub:** `Code/Libraries/Stubs/GameSpyStub.h` — created in task P1-03.
  All functions return "offline" or an error. Multiplayer matchmaking
  through the old GameSpy infrastructure does not work.
- **Replacement plan:** Phase 3+ — community-run matchmaking server
  (separate project, not part of this repo).

### DirectX 8 SDK
- **Vendor:** Microsoft
- **Version used:** DirectX 8.1 (d3d8, d3dx8, dinput8, dxguid, dsound)
- **Files not in repo:** Headers and .lib files are part of the
  legacy DirectX SDK (last release: June 2010).
- **License:** Microsoft proprietary (EULA allows redistribution of
  DirectX redistributables but not the SDK itself)
- **How to obtain:**
  1. Download the **Microsoft DirectX SDK (June 2010)** installer from
     Microsoft's website.
  2. Install to the default path.
  3. Pass to CMake:
     ```
     -DDXSDK_LIB_DIR="C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
     ```
- **Replacement plan:** Task P5-03 — replace with **DXVK** (Vulkan
  translation layer) + **SDL3** for windowing and input.

### Granny Animation System (granny2)
- **Vendor:** RAD Game Tools (now part of Epic Games)
- **Purpose:** Skeletal animation for unit and character models
- **Files gitignored:**
  - `Libraries/Include/Granny/` — Granny C API headers
  - `Libraries/Lib/granny2.lib` — import library (assumed name)
- **License:** Proprietary — cannot be redistributed
- **How to obtain:** Must own a licensed copy of the Granny SDK.
- **Replacement plan:** Not yet scheduled. Granny is used only for
  animation playback; a potential open-source replacement is **ozz-animation**
  but this requires model data conversion which is out of scope for now.

---

## Bundled source with unclear or gitignored components

### LZH CompLib (LZHCompress)
- **Location:** `Libraries/Source/Compression/LZHCompress/`
- **Purpose:** Lempel-Ziv-Huffman compression used in save files and
  map data (referenced from `NoxCompress.cpp`)
- **Status:** `CompLibHeader/` and `CompLibSource/` subdirectories are
  gitignored. Only `NoxCompress.cpp` and `NoxCompress.h` (thin wrapper)
  are present.
- **License:** Unknown — needs investigation before any redistribution.
  The "Nox" name suggests this code originated from Westwood's Nox (2000).
- **Action needed:** Determine original license. If compatible with GPLv3,
  restore the source. If proprietary, replace with an open LZ77/Huffman
  implementation (e.g., the zlib deflate stream already in the dep list).

---

## Dependencies NOT needed (can be removed)

### STLport 4.5.3
- **Location:** `Libraries/Source/STLport-4.5.3/` (fully gitignored)
- **Purpose:** An alternative STL implementation for compilers with poor
  STL support (notably MSVC 6 circa 1998–2002).
- **Status:** Not needed. Modern MSVC (2019+) has a complete, high-quality
  STL. STLport 4.5.3 is incompatible with C++11 and later.
- **Action:** Directory should remain gitignored. Do not reference it in
  the CMake build.

---

## How to set up all dependencies for a full build

See [BUILDING.md](BUILDING.md) for step-by-step instructions.

Short version:
1. `vcpkg install zlib:x86-windows` — the only vcpkg-managed dep
2. Install the DirectX SDK June 2010 and pass `-DDXSDK_LIB_DIR=...`
3. Obtain `mss32.dll` and `binkw32.dll` from a legal Zero Hour install
4. Build with stubs enabled (P1-03) to compile without Miles/Bink/GameSpy SDKs

---

*Last updated: 2026-03-17*
