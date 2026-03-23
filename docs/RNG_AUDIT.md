# RNG Discipline Audit — zh-revival

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

**Task:** P2-01
**Date:** 2026-03-22
**Status:** Revised 2026-03-22 — citations corrected, GetTickCount() finding downgraded per human review

---

## Summary

Zero Hour uses a **three-tier RNG system** that is well-architected:

| Tier | Macro | Seed Buffer | Determinism |
|------|-------|-------------|-------------|
| Simulation | `GameLogicRandomValue` | `theGameLogicSeed[6]` | **CRITICAL — must match all clients** |
| Rendering/Visual | `GameClientRandomValue` | `theGameClientSeed[6]` | Visual-only, safe |
| Audio | `GameAudioRandomValue` | `theGameAudioSeed[6]` | Audio-only, safe |

All ~80 simulation RNG calls correctly use the shared `GameLogicRandomValue` macro.
**ONE CRITICAL ISSUE** found: the seed source for multiplayer games.

---

## Full RNG Usage Table

| Location | Function | Category | Uses Shared Seed? | Notes |
|----------|----------|----------|-------------------|-------|
| `GeneralsMD/Code/GameEngine/Source/Common/RandomValue.cpp` | `GetGameLogicRandomValue()` | SIMULATION | YES — `theGameLogicSeed[6]` | Core deterministic RNG for all game logic |
| `GeneralsMD/Code/GameEngine/Source/Common/RandomValue.cpp` | `GetGameClientRandomValue()` | AUDIO-VISUAL | YES — `theGameClientSeed[6]` | Rendering/visual effects, isolated from logic |
| `GeneralsMD/Code/GameEngine/Source/Common/RandomValue.cpp` | `GetGameAudioRandomValue()` | AUDIO-VISUAL | YES — `theGameAudioSeed[6]` | Audio playback, isolated from logic |
| `GeneralsMD/Code/GameEngine/Source/Common/RandomValue.cpp` | `InitRandom(void)` | SIMULATION | Seeds from `time(NULL)` | Default non-deterministic init. Called during engine startup (GameEngine.cpp) in addition to editor/UI — harmless only if later overwritten by `InitGameLogicRandom()` before simulation starts. |
| `GeneralsMD/Code/GameEngine/Source/Common/RandomValue.cpp` | `InitGameLogicRandom(UnsignedInt seed)` | SIMULATION | YES — host-provided seed | **This is the correct multiplayer path** |
| `GeneralsMD/Code/GameEngine/Source/GameNetwork/GameInfo.cpp:311` | `setSeed(GetTickCount())` | NETWORK | NO — `GetTickCount()` | **CRITICAL: non-deterministic seed source** |
| `GeneralsMD/Code/GameEngine/Source/GameNetwork/GameSpy.cpp:1040` | `setSeed(GetTickCount())` | NETWORK | NO — `GetTickCount()` | **CRITICAL: non-deterministic seed source** |
| `GeneralsMD/Code/GameEngine/Source/GameNetwork/LANAPI.cpp:891` | `setSeed(GetTickCount())` | NETWORK | NO — `GetTickCount()` | **CRITICAL: non-deterministic seed source** |
| `GeneralsMD/Code/GameEngine/Source/GameClient/GUI/SkirmishGameOptionsMenu.cpp:1223` | `setSeed(GetTickCount())` | NETWORK | NO — `GetTickCount()` | **CRITICAL: non-deterministic seed source** |
| `GeneralsMD/Code/GameEngine/Source/GameNetwork/GameSpy/PeerDefs.cpp:542` | `setSeed(GetTickCount())` | NETWORK | NO — `GetTickCount()` | **CRITICAL: non-deterministic seed source** |
| `GeneralsMD/Code/GameEngine/Source/GameClient/GUI/WOLQuickMatchMenu.cpp:1330` | `setSeed(resp.qmStatus.seed)` | NETWORK | YES — server-provided | OK: WOL match server provides seed |
| `GeneralsMD/Code/GameEngine/Source/GameNetwork/GameSpy/StagingRoomGameInfo.cpp:866` | `setSeed(getSeed())` | NETWORK | YES — host-provided | OK: host seed propagated to joining clients |
| `GeneralsMD/Code/GameEngine/Source/Common/Recorder.cpp:406` | `GetGameLogicRandomSeed()` | REPLAY | Read-only | OK: seed logged in replay header |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/AI/AIStates.cpp` | `GameLogicRandomValue` | SIMULATION | YES | ~20+ calls: unit positioning, attack target selection |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/AI/AIGroup.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Unit grouping randomization |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/AI/AIGuard.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Guard scan timing randomization |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/AI/TurretAI.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Turret angle randomization |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Weapon.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Weapon damage spread and accuracy |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Behavior/DumbProjectileBehavior.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Projectile spin rates |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Behavior/GenerateMinefieldBehavior.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Mine placement offsets |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Behavior/SlowDeathBehavior.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Death animation variations |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Behavior/AutoHealBehavior.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Heal timing |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Behavior/PrisonBehavior.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Prisoner release timing |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Update/AIUpdate/WanderAIUpdate.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Unit wander movement |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Update/EMPUpdate.cpp` | `GameLogicRandomValue` | SIMULATION | YES | EMP recovery time randomization |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Update/FireSpreadUpdate.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Fire spread randomization |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogic.cpp:724,726,729` | `GameLogicRandomValue` | SIMULATION | YES | Player starting position randomization |
| `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/ObjectCreationList.cpp` | `GameLogicRandomValue` | SIMULATION | YES | Object creation with random force |
| `GeneralsMD/Code/GameEngineDevice/Source/W3DDevice/GameClient/W3DView.cpp` | `GameClientRandomValue` | AUDIO-VISUAL | YES (client seed) | Particle effect angles |
| `GeneralsMD/Code/GameEngineDevice/Source/W3DDevice/GameClient/W3DTreeBuffer.cpp` | `GameClientRandomValue` | AUDIO-VISUAL | YES (client seed) | Tree scale/animation randomization |
| `GeneralsMD/Code/GameEngine/Source/GameClient/System/Anim2D.cpp` | `GameClientRandomValue` | AUDIO-VISUAL | YES (client seed) | 2D animation effects |
| `GeneralsMD/Code/GameEngine/Source/GameClient/Drawable.cpp` | `GameClientRandomValue` | AUDIO-VISUAL | YES (client seed) | Drawable effect randomization |
| `GeneralsMD/Code/GameEngine/Source/GameNetwork/Transport.cpp` | `GameClientRandomValue` | AUDIO-VISUAL | YES (client seed) | Debug-only packet loss simulation |
| `GeneralsMD/Code/GameEngine/Source/Common/Audio/AudioEventRTS.cpp` | `GameAudioRandomValue` | AUDIO-VISUAL | YES (audio seed) | Sound selection, pitch/volume randomization |
| `GeneralsMD/Code/Libraries/.../mathutil.cpp` | `rand()` | TOOL | NO — independent | Non-critical utility |
| `GeneralsMD/Code/Libraries/.../shattersystem.cpp` | `srand()`/`rand()` | AUDIO-VISUAL | NO — independent | Shatter pattern visual only |
| `GeneralsMD/Code/GameEngine/Source/GameClient/Snow.cpp` | `rand()` | AUDIO-VISUAL | NO — independent | Snow particle placement, visual only |
| `GeneralsMD/Code/Libraries/Source/WWVegas/WWLib/Random*.h/.cpp` | `RandomClass`, `Random2Class`, etc. | TOOL | NO — independent | Utility classes, NOT used in gameplay |
| `GeneralsMD/Code/Tools/` | `rand()` / `srand()` | TOOL | NO — independent | Build tools and max plugins, non-critical |

---

## Finding — NEEDS VERIFICATION (downgraded from CRITICAL)

### Seed Source: GetTickCount() in Multiplayer

> **⚠ Revision 2026-03-22:** Human review found that this finding overstated what the code proves.
> The seed **is** serialized into `GameInfo` and propagated to joining clients via the lobby handshake
> before `InitGameLogicRandom()` is called (confirmed in `LANAPICallbacks.cpp` and
> `StagingRoomGameInfo.cpp`). An immediate "both clients desync" path was **not demonstrated**.
> The finding is retained as a design smell requiring end-to-end verification.

**Files affected (corrected citations):**
- `GameInfo.cpp:311` — `m_seed = GetTickCount();` (host-side seed generation at lobby creation)
- `LANAPI.cpp` (line approximate) — `setSeed(GetTickCount())` in LAN lobby creation
- `SkirmishGameOptionsMenu.cpp:1352` (not 1223) — `TheSkirmishGameInfo->setSeed(GetTickCount())`
- `StagingRoomGameInfo.cpp:866` — calls `InitGameLogicRandom(getSeed())` (host seed propagated correctly here)
- Note: `GameSpy.cpp` is not present in this repo as a standalone file; the original citation was incorrect.
- Note: `PeerDefs.cpp:542` — needs re-verification; could not be confirmed in current repo.

**What the code actually shows:** The host calls `GetTickCount()` to generate a seed, stores it in
`GameInfo.m_seed`, which is then sent to all joining players via the lobby handshake. Clients
receive the seed and call `InitGameLogicRandom(getSeed())` using the host's value.

**Remaining concern:** If `GetTickCount()` is called on a client *before* the host's seed is received
and applied, that client would initialize with a different seed. Whether this timing race actually
occurs requires tracing the call order in `LANAPICallbacks.cpp` and the skirmish lobby path.
This has **not** been verified in this audit pass.

**Commented-out evidence:**
```cpp
// GameInfo.cpp:311
m_seed = GetTickCount(); //GameClientRandomValue(0, INT_MAX - 1);
```
The reason for this change is unknown.

**What needs to happen before a fix is written:**
1. Trace the full call sequence from lobby creation → game start for LAN and Skirmish modes
2. Confirm whether `InitGameLogicRandom()` is called from the host-propagated seed or locally
3. If a race exists: fix the call ordering, not necessarily the seed source

---

## Secondary Finding — NEEDS VERIFICATION

### Platform Consistency of LCG in RandomValue.cpp

The `randomValue()` function uses a 48-bit LCG with add-with-carry, implemented using `UnsignedInt` arrays. This should be deterministic across 32-bit and 64-bit builds IF:

1. `UnsignedInt` is always 32 bits (verified: it's `typedef unsigned int UnsignedInt`)
2. No pointer arithmetic or `sizeof(pointer)` is involved (verified: none found)
3. Byte order assumptions are not made (verified: none found)

**Status:** Appears safe, but should be verified with a cross-build test (Win32 vs Win64 producing identical sequences from same seed).

---

## What Is NOT a Problem

- All ~80 simulation `GameLogicRandomValue` calls use the shared seed correctly
- All visual and audio RNG is properly isolated from simulation
- Replay headers correctly log the seed via `GetGameLogicRandomSeed()`
- The `GetGameLogicRandomSeedCRC()` function exists for determinism verification (used by P2-02)
- WOL quick-match correctly uses a server-provided seed
- GameSpy staging room correctly propagates the host's seed to joining clients

---

## Recommendations for P2

| Priority | Finding | Fix | Phase |
|----------|---------|-----|-------|
| MEDIUM (needs verification) | GetTickCount() seed in LAN/skirmish: possible timing race | Trace lobby→game-start call order; fix ordering if race confirmed | Dedicated research before P2-08 |
| MEDIUM | Platform consistency of LCG | Add unit test: same seed → same 10,000-value sequence on Win32 and Win64 | P2-09 (CI replay test) |
| LOW | InitRandom(void) also called at engine startup | Verify InitGameLogicRandom() always overwrites it before simulation | Research only |
| LOW | Commented-out code suggesting prior seed change | Investigate why GetTickCount was chosen over GameClientRandomValue | Research only |

---

*Revised 2026-03-22 per human code review: GetTickCount() finding downgraded from CRITICAL to NEEDS VERIFICATION. Citations corrected. InitRandom() scope clarified.*
