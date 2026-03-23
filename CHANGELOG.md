# CHANGELOG — zh-revival

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

---

## v1.0-revival — 2026-03-23

Complete revival of Command & Conquer: Generals — Zero Hour.
All five phases complete: stability, online, quality-of-life, ecosystem, and cross-platform.

---

### Phase 1 — Stability (v0.1-stability, 2026-03-17)

**P1-01 CMake build system migration**
Replaced legacy VC6 `.dsw`/`.dsp` project files with a modern `CMakeLists.txt`.
All build targets (game exe, tools, libraries) are now CMake targets with C++20,
`/W3`, and `target_include_directories`. Reproducible builds on all supported compilers.

**P1-02 vcpkg dependency manifest**
Created `vcpkg.json` for all open-source dependencies. Non-vcpkg proprietary SDKs
(Miles, Bink, SafeDisc, GameSpy) documented in `DEPENDENCIES.md`.

**P1-03 Dependency stubs**
Created stub headers and implementations for all closed-source dependencies:
- `MilesSoundStub.h` — Miles Sound System (audio)
- `BinkVideoStub.h` — Bink video playback
- `SafeDiscStub.h` — SafeDisc CD-ROM validation (returns success)
- `GameSpyStub.h` — GameSpy network API (returns offline/error)

Game compiles and runs without proprietary SDKs. Audio/video degrade gracefully.

**P1-04 GitHub Actions CI pipeline**
`ci-build.yml`: build on Windows (MSVC x86), Linux cross-compile (mingw-w64 → Win32),
Linux native (GCC x86_64), and macOS ARM64 on every PR and push to main.
`ci-release.yml`: automated release build on version tags.

**P1-05 HKLM to HKCU registry migration**
All registry reads/writes migrated from `HKEY_LOCAL_MACHINE` to `HKEY_CURRENT_USER`.
Game no longer requires administrator rights for settings.

**P1-06 Win+L particle crash fix**
Particle system now pauses accumulation when the D3D device is non-cooperative
(screen lock). On device resume, accumulated requests are safely discarded.
No regression to particle behavior during normal gameplay.

**P1-07 Alt-Tab device loss crash fix**
`DX8Wrapper::Reset_Device()` now checks `TestCooperativeLevel` before releasing
resources. Device loss state is properly handled; surfaces are released and recreated
on device reset.

**P1-08 Startup "Serious Error" fix**
D3D8 device creation now logs HRESULT codes and adapter details on failure.
Fallback chain: software vertex processing → D16 depth buffer → default adapter.
Actionable error message replaces the previous generic "Serious Error" dialog.

**P1-09 Large-match pathfinder crash fix**
Pathfinder request queue length increased from 512 to 4096 entries. Eliminates
buffer overflow crash when unit+building count exceeds ~1000.

**P1-10 Audio loss after minimize fix**
Audio device context is now restored on window focus regain. No more silent audio
after minimize+restore.

**P1-11 CD/DRM check removal**
Removed legacy SafeDisc CD-presence checks that blocked launch on modern digital
installs (Steam, EA App). Steam/EA App license verification is untouched.

**P1-12 BUILDING.md**
Step-by-step build guide for new contributors covering prerequisites, CMake
configure, build, and run steps. Includes troubleshooting for common errors.

---

### Phase 2 — Online (v0.2-online, 2026-03-22)

**P2-01 RNG discipline audit**
Full audit of all random number generation. Three-tier system (GameLogic/GameClient/
GameAudio) correctly separates simulation from audio-visual RNG. Seed propagation
via lobby handshake confirmed. Findings documented in `docs/RNG_AUDIT.md`.

**P2-02 Per-tick desync logging**
CRC mismatch handler now writes `Logs/desync_YYYYMMDD_HHMMSS.log` with tick number,
local CRC, and per-player CRCs. Log-only; no disconnect triggered.

**P2-03 Map transfer retry and validation**
Map transfers retry up to 3 times with 2-second delay on failure. Received maps
are validated: size ≤ 50 MB, `.map` magic bytes checked, SHA256 must match host's
lobby hash. Invalid maps are rejected before the loader touches them.

**P2-04 Network security audit**
Full audit of all network message parsing. Findings: 5 CRITICAL, 7 HIGH.
Full report in `docs/SECURITY_AUDIT_P2.md`.

**P2-05 Fix CRITICAL network vulnerabilities**
Fixed VULN-001 through VULN-005 from the audit: filename OOB strcpy, unchecked
`dataLength`, chat buffer off-by-one, truncated packet OOB reads.

**P2-06 Fix HIGH network vulnerabilities**
Fixed VULN-006 through VULN-012: `totalArgCount` overflow, unchecked `setData`
length, null `h_addr_list`, wrapper chunk bounds, `sprintf→snprintf`, null alloc guard.

**P2-07 Unsafe string functions in network code**
Replaced `strcpy`/`strcat`/`sprintf` in all network-facing code paths with
bounded equivalents. Dead-code `strcpy` in `udp.cpp` removed.

**P2-08 Version CRC enforcement**
LAN join handshake now rejects players whose `exeCRC` or `iniCRC` doesn't match
the host. Clear error message: "Your game version doesn't match the host."

**P2-09 Headless replay CI test**
`--headless-replay <file.rep>` flag added to the game binary. Loads and fast-forwards
a replay without rendering; exits 0 on success. CI step added to `ci-build.yml`.

**P2-10 DC-bug recovery**
Disconnect handler now waits 10 seconds and attempts reconnect once.
On failure: clean "You were disconnected" screen with lobby/main-menu options.
Disconnect events logged to `Logs/disconnect.log`.

---

### Phase 3 — Quality of Life (v0.3-qol, 2026-03-22)

**P3-01 Borderless fullscreen and widescreen**
`-borderless` / `-windowed` / `-fullscreen` / `-width X -height Y` command-line flags.
Borderless mode uses `WS_POPUP` at desktop resolution. Edge scroll re-enabled in
borderless mode. Window mode preference persisted in HKCU registry.

**P3-02 Replay format specification**
Complete binary format specification for `.rep` files derived from `Recorder.cpp`.
Covers file header, per-tick command records, footer. Caveats for `sizeof(time_t)`
and `Bool=Int32` documented. Written to `docs/REPLAY_FORMAT.md`.

**P3-03 libzhreplay**
C library for parsing `.rep` replay files. Public API: `zh_replay_open`,
`zh_replay_valid`, `zh_replay_header`, `zh_replay_next_command`, `zh_replay_close`.
Handles malformed/truncated input without crashing. No external dependencies.

**P3-04 zh-replay-validate**
CLI tool: `zh-replay-validate <file.rep>`. Outputs `VALID` or `INVALID` with details.
Exit codes: 0 = valid, 1 = invalid, 2 = tool error. Never crashes on any input.

**P3-05 zh-replay-info**
CLI tool: `zh-replay-info <file.rep>`. Outputs strict JSON with replay metadata
(version, map hash, date, duration, players, winner, build version). Unknown fields
are `null`; invalid file returns `{"error": "invalid replay file"}`.

**P3-06 Replay HUD improvements**
Tick counter and elapsed time overlay in observer/replay mode. Replay speed control:
0.5× / 1× / 2× / 4× via `[` and `]` keys. Crash on player selection in replay mode
fixed (null guard + slot index fix in `ControlBarObserver`).

---

### Phase 4 — Ecosystem (v0.4-ecosystem, 2026-03-23)

**P4-01 Anti-cheat security audit**
Audit of all cheat vectors: impossible commands, resource spoofing, replay tampering.
4 CRITICAL, 2 HIGH, 2 MEDIUM findings. Root cause: `MSG_CHEAT_*` guards exist only
in the client layer, not in `GameLogicDispatch`. Full report in
`docs/SECURITY_AUDIT_P4.md`.

**P4-02 Architecture documentation**
`docs/ARCHITECTURE.md`: subsystem overview (GameLogic, GameNetwork, GameRenderer,
GameAudio, GameClient), main loop tick rate and update order, INI data flow,
lockstep multiplayer sync model, key singletons table.

**P4-03 INI modding reference**
`docs/modding/INI_REFERENCE.md`: all major INI types (Object, Weapon, Armor,
CommandButton, CommandSet, SpecialPower, Upgrade) with field tables, valid ranges,
and minimal working examples. Written for modders who know the game but not C++.

**P4-04 Contributor guide**
`docs/CONTRIBUTING.md`: fork setup, branch naming, commit message format, PR
requirements, code style guide, determinism rules, and non-affiliation statement.
`github/ISSUE_TEMPLATE/bug_report.md` bug report template.

**P4-05 Mod manager foundation**
`ModManager` class: scans `<UserData>/Mods/` for `.big` files, loads in numeric
priority order, logs file conflicts to `Logs/mod_conflicts.log`. `--no-mods` flag
for vanilla play. Mod folder path configurable via HKCU registry.

---

### Phase 5 — Next-Gen Engine (v1.0-revival, 2026-03-23)

**P5-01 OpenAL audio backend**
`OpenALMilesShim.cpp`: complete implementation of all ~40 `AIL_*` Miles Sound System
API functions using openal-soft. Thin-shim pattern — `ZH_OPENAL_AUDIO=ON` in CMake
switches from inline no-ops to the OpenAL backend without touching game code.
Features: WAV parsing, IMA ADPCM decode, pan/volume, 3D positioning (positional audio),
double-buffer streaming with background thread.

**P5-02 FFmpeg video backend**
`FFmpegBinkShim.cpp`: `BinkOpen`/`BinkDoFrame`/`BinkCopyToBuffer`/`BinkGoto` via
libavcodec + libavformat + libswscale. `ZH_FFMPEG_VIDEO=ON` enables FFmpeg decode.
When `OFF`, cutscenes are skipped with a log entry in `Logs/video_skip.log`.

**P5-03 SDL3 + DXVK platform layer**
- P5-03a: SDL3 window creation. HWND extracted via `SDL_PROP_WINDOW_WIN32_HWND_POINTER`;
  existing `WndProc` subclasses the SDL window.
- P5-03b: SDL3 event pump in `serviceWindowsOS()`. `SDL_EVENT_QUIT` posts `WM_QUIT`.
- P5-03c: DXVK — `dx8wrapper.cpp` already uses `LoadLibrary("D3D8.DLL")` dynamically;
  CMake copies `d3d8.dll`/`d3d9.dll` from `DXVK_DLL_DIR` when `ZH_DXVK=ON`.

**P5-04 Win64 build**
`DWORD_PTR` casts replace `DWORD` for pointer values in `StackDump.cpp`.
`getVersionNumber()` sets bit 15 on `_WIN64` builds — prevents 32-bit and 64-bit
clients from joining the same multiplayer session.
`/wd4311` `/wd4312` suppress legacy int/pointer truncation warnings in `Tools/`.

**P5-05 Linux native build**
`#ifdef _WIN32` guards on `<windows.h>` includes in `AsciiString.h` and
`URLLaunch.h`. CI: `build-linux-native` job on Ubuntu 22.04, GCC x86_64, SDL3,
x64-linux vcpkg triplet, headless smoke test. `docs/modding/LINUX_BUILD.md`.

**P5-06 macOS ARM64 build**
CI: `build-macos-arm64` job on macos-14 (Apple M1 runner), Clang, Homebrew
SDL3 + MoltenVK (Vulkan→Metal), arm64-osx vcpkg triplet, headless smoke test.
`docs/modding/MACOS_BUILD.md`.

**P5-07 Full memory safety audit**
Engine-wide scan for `strcpy`/`strcat`/`sprintf`/`gets`. 4 CRITICAL and 7 HIGH
findings fixed:
- Stack overflow in WOL login UI (user-controlled nick/email/password → `char[31]`)
- BuddyThread network callback `countrycode[3]` overflow
- `StackDump.cpp` `function_name[512]` + strcat overflow and sprintf to undersized buffer
- FirewallHelper mangler hostname buffers
- GameLogic map filename from network transfer
- GameSpeech INI dialog filename paths

~20 MEDIUM findings documented in `docs/MEMORY_SAFETY_AUDIT.md` for future cleanup.

---

## Summary of all output files

| Phase | Key files created |
|-------|------------------|
| P1 | `CMakeLists.txt`, `vcpkg.json`, `DEPENDENCIES.md`, `BUILDING.md`, `Code/Libraries/Stubs/*.h`, `.github/workflows/ci-*.yml` |
| P2 | `docs/RNG_AUDIT.md`, `docs/SECURITY_AUDIT_P2.md` |
| P3 | `docs/REPLAY_FORMAT.md`, `tools/libzhreplay/`, `tools/zh-replay-validate/`, `tools/zh-replay-info/` |
| P4 | `docs/ARCHITECTURE.md`, `docs/CONTRIBUTING.md`, `docs/modding/INI_REFERENCE.md`, `docs/SECURITY_AUDIT_P4.md` |
| P5 | `Code/Libraries/Stubs/OpenALMilesShim.cpp`, `Code/Libraries/Stubs/FFmpegBinkShim.cpp`, `docs/modding/LINUX_BUILD.md`, `docs/modding/MACOS_BUILD.md`, `docs/MEMORY_SAFETY_AUDIT.md`, `CHANGELOG.md` |
