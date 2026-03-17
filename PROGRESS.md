# ZerwOne / zh-revival — Progress Tracker

> Repo: https://github.com/ZerwOne/zh-revival
> Forked from: https://github.com/electronicarts/CnC_Generals_Zero_Hour
> Updated by Claude Code at the end of every session.
> Last updated: 2026-03-17

---

## Phase 1 — Stability (Weeks 1–10)
- [x] P1-01 — CMake build system migration — completed 2026-03-17
- [x] P1-02 — vcpkg dependency manifest — completed 2026-03-17
- [x] P1-03 — Dependency stubs (Miles, Bink, SafeDisc, GameSpy) — completed 2026-03-17
- [x] P1-04 — GitHub Actions CI pipeline — completed 2026-03-17
- [x] P1-05 — HKLM to HKCU registry migration — completed 2026-03-17
- [x] P1-06 — Fix R4: particle accumulation crash (Win+L) — completed 2026-03-17
- [ ] P1-07 — Fix R2: Alt-Tab / fullscreen device loss crash
- [ ] P1-08 — Fix R1: Startup Serious Error / DirectX init failure
- [ ] P1-09 — Fix R5: Large-match pathfinder crash
- [ ] P1-10 — Fix R3: Audio loss after minimize
- [ ] P1-11 — Remove superfluous CD/DRM checks
- [ ] P1-12 — Create BUILDING.md
- [ ] P1-FINAL — Tag v0.1-stability

## Phase 2 — Online (Weeks 11–24)
- [ ] P2-01 — RNG discipline audit
- [ ] P2-02 — Per-tick state hashing for mismatch detection
- [ ] P2-03 — Map transfer retry and content validation
- [ ] P2-04 — Full network message security audit
- [ ] P2-05 — Fix CRITICAL vulnerabilities from P2-04
- [ ] P2-06 — Fix HIGH vulnerabilities from P2-04
- [ ] P2-07 — Replace unsafe string functions in network code
- [ ] P2-08 — Version CRC enforcement
- [ ] P2-09 — Headless replay CI determinism test
- [ ] P2-10 — DC-bug recovery improvement
- [ ] P2-FINAL — Tag v0.2-online

## Phase 3 — Quality of Life (Weeks 25–38)
- [ ] P3-01 — Borderless fullscreen and widescreen support
- [ ] P3-02 — Replay format specification
- [ ] P3-03 — libzhreplay: replay parser library
- [ ] P3-04 — zh-replay-validate: CLI validator
- [ ] P3-05 — zh-replay-info: JSON metadata extractor
- [ ] P3-06 — Replay HUD improvements
- [ ] P3-FINAL — Tag v0.3-qol

## Phase 4 — Ecosystem (Weeks 39–52)
- [ ] P4-01 — Anti-cheat security audit
- [ ] P4-02 — Architecture documentation
- [ ] P4-03 — INI modding reference
- [ ] P4-04 — Contributor guide
- [ ] P4-05 — Mod manager foundation
- [ ] P4-FINAL — Tag v0.4-ecosystem

## Phase 5 — Next-Gen Engine (Weeks 53–92)
- [ ] P5-01 — Replace Miles Sound System with OpenAL
- [ ] P5-02 — Replace Bink video with open decoder
- [ ] P5-03 — DXVK + SDL3 graphics and windowing layer
- [ ] P5-04 — Win64 build
- [ ] P5-05 — Linux native build
- [ ] P5-06 — macOS ARM64 build
- [ ] P5-07 — Full memory safety audit
- [ ] P5-FINAL — Tag v1.0-revival

---

## Session log

| Date | Task | Branch | Status | Notes |
|------|------|--------|--------|-------|
| 2026-03-17 | P1-01 CMake migration | task/P1-01-cmake | done | CMakeLists.txt at repo root, targets GeneralsMD (Zero Hour) |
| 2026-03-17 | P1-02 vcpkg manifest | task/P1-02-vcpkg | done | vcpkg.json (zlib only vcpkg dep), DEPENDENCIES.md (full inventory) |
| 2026-03-17 | P1-03 dependency stubs | task/P1-03-stubs | done | MilesSoundStub.h, BinkVideoStub.h, SafeDiscStub.h, GameSpyStub.h + forwarding headers + CMake ZH_USE_STUBS option |
| 2026-03-17 | P1-04 CI pipeline | task/P1-04-ci | done | ci-build.yml (Windows MSVC + Linux GCC cross), ci-release.yml (sign + GitHub Release), mingw32.cmake toolchain |
| 2026-03-17 | P1-05 HKLM→HKCU registry | task/P1-05-hkcu | done | registry.cpp reads HKCU-first; added missing SetStringInRegistry/SetUnsignedIntInRegistry writing to HKCU; fixed autorun.cpp + wolInit.cpp |
| 2026-03-17 | P1-06 particle Win+L crash | task/P1-06-particle-crash | done | Added Display::isDeviceLost() virtual, W3DDisplay::isDeviceLost() via DX8Wrapper; ParticleSystemManager::update() skips when device lost |
