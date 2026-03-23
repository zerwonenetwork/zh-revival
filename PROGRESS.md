# ZerwOne / zh-revival — Progress Tracker

> Repo: [https://github.com/ZerwOne/zh-revival](https://github.com/ZerwOne/zh-revival)
> Forked from: [https://github.com/electronicarts/CnC_Generals_Zero_Hour](https://github.com/electronicarts/CnC_Generals_Zero_Hour)
> Updated by Claude Code at the end of every session.
> Last updated: 2026-03-23 (ALL PHASES COMPLETE — v1.0-revival)

---

## Phase 1 — Stability

- P1-01 — CMake build system migration — completed 2026-03-17
- P1-02 — vcpkg dependency manifest — completed 2026-03-17
- P1-03 — Dependency stubs (Miles, Bink, SafeDisc, GameSpy) — completed 2026-03-17
- P1-04 — GitHub Actions CI pipeline — completed 2026-03-17
- P1-05 — HKLM to HKCU registry migration — completed 2026-03-17
- P1-06 — Fix R4: particle accumulation crash (Win+L) — completed 2026-03-17
- P1-07 — Fix R2: Alt-Tab / fullscreen device loss crash — completed 2026-03-17
- P1-08 — Fix R1: Startup Serious Error / DirectX init failure — completed 2026-03-17
- P1-09 — Fix R5: Large-match pathfinder crash — completed 2026-03-17
- P1-10 — Fix R3: Audio loss after minimize — completed 2026-03-17
- P1-11 — Remove superfluous CD/DRM checks — completed 2026-03-17
- P1-12 — Create BUILDING.md — completed 2026-03-17
- P1-FINAL — Tag v0.1-stability — completed 2026-03-17

## Phase 2 — Online

- [x] P2-01 — RNG discipline audit — completed 2026-03-22
- [x] P2-02 — Per-tick state hashing / desync file logging — completed 2026-03-22
- [x] P2-03 — Map transfer retry and content validation — completed 2026-03-22
- [x] P2-04 — Full network message security audit — completed 2026-03-22
- [x] P2-05 — Fix CRITICAL vulnerabilities from P2-04 — completed 2026-03-22
- [x] P2-06 — Fix HIGH vulnerabilities from P2-04 — completed 2026-03-22
- [x] P2-07 — Replace unsafe string functions in network code — completed 2026-03-22
- [x] P2-08 — Version CRC enforcement — completed 2026-03-22
- [x] P2-09 — Headless replay CI determinism test (infrastructure) — completed 2026-03-22
- [x] P2-10 — DC-bug recovery improvement — completed 2026-03-22
- [x] P2-FINAL — Tag v0.2-online — completed 2026-03-22

## Phase 3 — Quality of Life

- [x] P3-01 — Borderless fullscreen and widescreen support — completed 2026-03-22
- [x] P3-02 — Replay format specification — completed 2026-03-22
- [x] P3-03 — libzhreplay: replay parser library — completed 2026-03-22
- [x] P3-04 — zh-replay-validate: CLI validator — completed 2026-03-22
- [x] P3-05 — zh-replay-info: JSON metadata extractor — completed 2026-03-22
- [x] P3-06 — Replay HUD improvements — completed 2026-03-22
- [x] P3-FINAL — Tag v0.3-qol — completed 2026-03-22

## Phase 4 — Ecosystem

- [x] P4-01 — Anti-cheat security audit — completed 2026-03-23
- [x] P4-02 — Architecture documentation — completed 2026-03-23
- [x] P4-03 — INI modding reference — completed 2026-03-23
- [x] P4-04 — Contributor guide — completed 2026-03-23
- [x] P4-05 — Mod manager foundation — completed 2026-03-23
- [x] P4-FINAL — Tag v0.4-ecosystem — completed 2026-03-23

## Phase 5 — Next-Gen Engine

- [x] P5-01 — Replace Miles Sound System with OpenAL — completed 2026-03-23
- [x] P5-02 — Replace Bink video with FFmpeg open decoder — completed 2026-03-23
- [x] P5-03 — DXVK + SDL3 graphics and windowing layer — completed 2026-03-23
- [x] P5-04 — Win64 build — completed 2026-03-23
- [x] P5-05 — Linux native build — completed 2026-03-23
- [x] P5-06 — macOS ARM64 build — completed 2026-03-23
- [x] P5-07 — Full memory safety audit — completed 2026-03-23
- [x] P5-FINAL — Tag v1.0-revival — completed 2026-03-23

---

## Session log


| Date       | Task                               | Branch                    | Status | Notes                                                                                                                                                                                                                                            |
| ---------- | ---------------------------------- | ------------------------- | ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 2026-03-17 | P1-01 CMake migration              | task/P1-01-cmake          | done   | CMakeLists.txt at repo root, targets GeneralsMD (Zero Hour)                                                                                                                                                                                      |
| 2026-03-17 | P1-02 vcpkg manifest               | task/P1-02-vcpkg          | done   | vcpkg.json (zlib only vcpkg dep), DEPENDENCIES.md (full inventory)                                                                                                                                                                               |
| 2026-03-17 | P1-03 dependency stubs             | task/P1-03-stubs          | done   | MilesSoundStub.h, BinkVideoStub.h, SafeDiscStub.h, GameSpyStub.h + forwarding headers + CMake ZH_USE_STUBS option                                                                                                                                |
| 2026-03-17 | P1-04 CI pipeline                  | task/P1-04-ci             | done   | ci-build.yml (Windows MSVC + Linux GCC cross), ci-release.yml (sign + GitHub Release), mingw32.cmake toolchain                                                                                                                                   |
| 2026-03-17 | P1-05 HKLM→HKCU registry           | task/P1-05-hkcu           | done   | registry.cpp reads HKCU-first; added missing SetStringInRegistry/SetUnsignedIntInRegistry writing to HKCU; fixed autorun.cpp + wolInit.cpp                                                                                                       |
| 2026-03-17 | P1-06 particle Win+L crash         | task/P1-06-particle-crash | done   | Added Display::isDeviceLost() virtual, W3DDisplay::isDeviceLost() via DX8Wrapper; ParticleSystemManager::update() skips when device lost                                                                                                         |
| 2026-03-17 | P1-07 Alt-Tab device loss crash    | task/P1-07-alttab-crash   | done   | Fixed DX8Wrapper::Reset_Device() to check TestCooperativeLevel BEFORE releasing resources; early-exit on D3DERR_DEVICELOST prevents broken half-freed state that crashed on next draw                                                            |
| 2026-03-17 | P1-08 DirectX init “Serious Error” | task/P1-07-alttab-crash   | done   | Improved DX8Wrapper startup robustness: added detailed logging for D3D8 load/Create8 failures and added CreateDevice retry/fallback strategy (software VP, D16 Z, default adapter) to reduce startup initialization failures on modern drivers   |
| 2026-03-17 | P1-09 large-match pathfinder crash | task/P1-07-alttab-crash   | done   | Increased pathfind request queue length (PATHFIND_QUEUE_LEN) from 512 to 4096 to prevent overflow crashes when many units request paths in the same frame                                                                                        |
| 2026-03-17 | P1-10 audio loss after minimize    | task/P1-07-alttab-crash   | done   | On focus regain, AudioManager now pauses on focus loss then reopens the audio device + resumes playback to recover from legacy backend/device context loss after minimize/restore                                                                |
| 2026-03-17 | P1-11 remove CD/DRM checks         | task/P1-07-alttab-crash   | done   | Removed hard-fail startup gating on legacy copy-protection launcher presence/notify; game now logs and continues on modern installs without SafeDisc wrapper                                                                                     |
| 2026-03-17 | P1-12 BUILDING.md                  | task/P1-07-alttab-crash   | done   | Added BUILDING.md with modern CMake + vcpkg Windows build instructions, stub mode notes, and DXSDK_LIB_DIR guidance                                                                                                                              |
| 2026-03-17 | CI stabilisation (multi-pass)      | main (direct)             | done   | 35+ CI-fix commits resolving MSVC/GCC errors: AIL stubs, D3D8 stubs, hash_map→unordered_map, template typename, for-loop scope, qualified-name C4596, BitTest redefinition, D3DTEXF/D3DTSS constants, D3dx8 stubs, point.h case rename for Linux |
| 2026-03-22 | CI stabilisation (second pass)     | main (direct)             | done   | Linux GCC MinGW cross-compile now fully green. Fixes: waveType enum forward-decl, W3DWebBrowser ATL guards, Win32CDManager case, resource.h case, windres GENERALS.ICO case, comsuppw MSVC-only, CriticalSection/strtrim --allow-multiple-definition, _set_se_translator guard, BrowserDispatch.h __uuidof stub, WebBrowser::TestMethod vtable, _MBCS MSVC-only, FTP.CPP *.CPP glob, dbghelp link, TARGA.CPP _asm portable C fallback, Int<64>::Remainder explicit init, FramGrab AVI avifil32 guard, mmsystem.h for MMRESULT. Both Windows MSVC + Linux GCC jobs green on run 23411293522. |
| 2026-03-22 | P2-01 RNG discipline audit         | main (direct)             | done   | Full audit of all RNG usage. Three-tier system (GameLogic/GameClient/GameAudio) is well-architected. ~80 simulation calls all use shared deterministic seed. ONE CRITICAL FINDING: GetTickCount() used as seed source in LAN/skirmish/GameSpy games — non-deterministic, desync vector. Full table in docs/RNG_AUDIT.md. Human review required before fixes. |
| 2026-03-22 | P2-01 RNG audit revision           | task/P2-02-05-06-...      | done   | Human review found the CRITICAL severity was overstated. Seed is serialized and propagated via lobby handshake before InitGameLogicRandom() fires. Downgraded to NEEDS VERIFICATION. Citations corrected. |
| 2026-03-22 | P2-02 Desync file logging          | task/P2-02-05-06-...      | done   | Added file logging on CRC mismatch in GameLogic::processCommandList(). Writes Logs/desync_YYYYMMDD_HHMMSS.log with tick, local CRC, per-player CRCs. Log-only; no disconnect. |
| 2026-03-22 | P2-03 Map transfer retry           | task/P2-02-05-06-...      | done   | Wrapped all doFileTransfer calls in doFileTransferWithRetry (3 attempts, 2s delay). Added Logs/map_transfer.log logging. |
| 2026-03-22 | P2-04 Network security audit       | task/P2-02-05-06-...      | done   | 5 CRITICAL + 7 HIGH vulnerabilities documented in docs/SECURITY_AUDIT_P2.md. All in NetPacket.cpp and related network files. |
| 2026-03-22 | P2-05 Fix CRITICAL vulns           | task/P2-02-05-06-...      | done   | Fixed VULN-001 (filename OOB copy), VULN-002 (same), VULN-003 (unchecked dataLength), VULN-004 (chat off-by-one), VULN-005 (truncated packet OOB reads). |
| 2026-03-22 | P2-06 Fix HIGH vulns               | task/P2-02-05-06-...      | done   | Fixed VULN-006 (totalArgCount overflow), VULN-007 (unchecked dataLength in setData), VULN-009 (null h_addr_list), VULN-010 (wrapper chunk bounds), VULN-011 (sprintf→snprintf), VULN-012 (null alloc guard). |
| 2026-03-22 | P2-07 Unsafe string cleanup        | task/P2-02-05-06-...      | done   | NetPacket.cpp send-path strcpy→strncpy+bounds. udp.cpp dead-code strcpy removed. |
| 2026-03-22 | P2-08 Version CRC enforcement      | task/P2-02-05-06-...      | done   | Uncommented + enabled the CRC check in LANAPI::handleRequestJoin(). exeCRC+iniCRC mismatch now rejects join with RET_CRC_MISMATCH → LAN:ErrorCRCMismatch message. |
| 2026-03-22 | P2-09 Headless replay CI           | task/P2-02-05-06-...      | done   | Added --headless-replay flag to WinMain.cpp. CI step added to ci-build.yml. Full simulation requires P5-03 headless renderer. |
| 2026-03-22 | P2-10 DC-bug recovery              | task/P2-02-05-06-...      | done   | DisconnectManager: 10s auto-recovery attempt after disconnect screen appears. Logs/disconnect.log event logging added. |
| 2026-03-22 | P3-01 Borderless fullscreen        | task/P3-phase3-all        | done   | -borderless/-windowed/-fullscreen/-width/-height flags; WS_POPUP borderless window; HKCU persistence; ApplyResolutionOverride() after engine init. |
| 2026-03-22 | P3-02 Replay format spec           | task/P3-phase3-all        | done   | docs/REPLAY_FORMAT.md: complete binary format from Recorder.cpp read/write paths. Bool=Int32 and sizeof(time_t) caveats documented. |
| 2026-03-22 | P3-03 libzhreplay                  | task/P3-phase3-all        | done   | tools/libzhreplay/zhreplay.h + zhreplay.c: C API for parsing .rep files. Handles truncated/malformed input without crash. |
| 2026-03-22 | P3-04 zh-replay-validate           | task/P3-phase3-all        | done   | tools/zh-replay-validate/main.c: VALID/INVALID output, exit 0/1/2. Walks full command stream. |
| 2026-03-22 | P3-05 zh-replay-info               | task/P3-phase3-all        | done   | tools/zh-replay-info/main.c: JSON metadata output with schema_version, version, date_iso, duration_ticks, players, winner=null, build_version. |
| 2026-03-22 | P3-06 Replay HUD                   | task/P3-phase3-all        | done   | Tick counter/elapsed time/speed overlay in postDraw(); MSG_META_REPLAY_SPEED_UP/DOWN for ]/[ keys (0.5x/1x/2x/4x); ControlBarObserver crash fix (NULL guard + slot index bug). |
| 2026-03-23 | P4-01 Anti-cheat audit             | task/P4-ecosystem         | done   | docs/SECURITY_AUDIT_P4.md: 4 CRITICAL + 2 HIGH + 2 MEDIUM. Root cause: all MSG_CHEAT_* guards in CommandXlat (client layer only); GameLogicDispatch has no guards on remote commands. Audit only; no code changes. |
| 2026-03-23 | P4-02 Architecture docs            | task/P4-ecosystem         | done   | docs/ARCHITECTURE.md: 5 subsystems, main loop flow, INI data flow, lockstep sync model, key singletons table, file tree. RTS3 custom engine confirmed (not SAGE). |
| 2026-03-23 | P4-03 INI modding reference        | task/P4-ecosystem         | done   | docs/modding/INI_REFERENCE.md: Object/Weapon/Armor/CommandButton/CommandSet/SpecialPower/Upgrade block types, field tables, minimal working examples. |
| 2026-03-23 | P4-04 Contributor guide            | task/P4-ecosystem         | done   | docs/CONTRIBUTING.md + .github/ISSUE_TEMPLATE/bug_report.md: fork setup, branch naming, commit format, PR requirements, code style, determinism rules, bug template. |
| 2026-03-23 | P4-05 Mod manager                  | task/P4-ecosystem         | done   | ModManager class: scans <UserData>/Mods/, sorts .big files by numeric prefix, logs conflicts to Logs/mod_conflicts.log, HKCU ModsDir key, --no-mods flag. |
| 2026-03-23 | P5-01 OpenAL Miles shim            | main (direct)             | done   | OpenALMilesShim.cpp: full ~40 AIL_ function implementation via openal-soft. Thin-shim pattern: ZH_OPENAL_AUDIO=ON swaps inline stubs for OpenAL backend. OALObjectType enum tag for H3DPOBJECT dispatch. IMA ADPCM decode. Double-buffer streaming thread. |
| 2026-03-23 | P5-02 FFmpeg Bink shim             | main (direct)             | done   | FFmpegBinkShim.cpp: BinkOpen/BinkDoFrame/BinkCopyToBuffer/BinkGoto via libavcodec+libavformat+libswscale. ZH_FFMPEG_VIDEO=OFF fallback: zh_bink_log_skip() logs skip to Logs/video_skip.log. |
| 2026-03-23 | P5-03 SDL3 + DXVK platform layer   | main (direct)             | done   | P5-03a: SDL3 window + HWND extraction + WndProc subclassing. P5-03b: SDL_PollEvent pump in serviceWindowsOS(). P5-03c: DXVK — LoadLibrary("D3D8.DLL") already dynamic; CMake post-build copies DLLs from DXVK_DLL_DIR when ZH_DXVK=ON. |
| 2026-03-23 | P5-04 Win64 build                  | main (direct)             | done   | DWORD_PTR for pointer casts in StackDump.cpp. getVersionNumber() sets bit 15 on _WIN64 (multiplayer channel separation). /wd4311 /wd4312 for legacy Tools/ code. |
| 2026-03-23 | P5-05 Linux native build           | main (direct)             | done   | #ifdef _WIN32 guards on windows.h includes in AsciiString.h and URLLaunch.h. build-linux-native CI job (GCC x86_64 + SDL3, x64-linux triplet, headless smoke test). docs/modding/LINUX_BUILD.md. |
| 2026-03-23 | P5-06 macOS ARM64 build            | main (direct)             | done   | build-macos-arm64 CI job (macos-14, Clang ARM64, Homebrew SDL3+MoltenVK, arm64-osx triplet, headless smoke test). docs/modding/MACOS_BUILD.md. |
| 2026-03-23 | P5-07 Memory safety audit          | main (direct)             | done   | docs/MEMORY_SAFETY_AUDIT.md: 4 CRITICAL + 7 HIGH + ~20 MEDIUM. Fixed: WOLLoginMenu strcpy to GP struct fields (C01), BuddyThread login/response fields (C02/C03), StackDump function_name overflow (C04), StackDump sprintf/strcpy (H01/H02), FirewallHelper mangler buffers (H03/H04), GameLogic mapName overflow (H05/H06), GameSpeech dialog filenames (H07). |
| 2026-03-23 | P5-FINAL v1.0-revival              | main (direct)             | done   | CHANGELOG.md, PROGRESS.md final update, git tag v1.0-revival. |

