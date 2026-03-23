# Zero Hour Architecture

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

**Task:** P4-02
**Date:** 2026-03-23
**Audience:** C++ developers new to this codebase

---

## High-Level Overview

Zero Hour is built on a custom RTS engine internally called **RTS3** (see
`GameEngine/Include/Common/GameCommon.h` line 35). It is not based on a
public or shared "SAGE" library — the codebase is self-contained. The engine
uses a **lockstep multiplayer model**, a **tick-based deterministic simulation**
at 30 FPS, and a clean subsystem architecture where every major system
implements a common `SubsystemInterface` contract (init / reset / update).

All code lives under `GeneralsMD/Code/`. The entry point is
`GeneralsMD/Code/Main/WinMain.cpp`, which calls `GameMain()`, which creates
`TheGameEngine` via `CreateGameEngine()` and calls `TheGameEngine->init()` then
`TheGameEngine->execute()`.

---

## Major Subsystems

### GameEngine — Orchestrator

**Headers:** `GameEngine/Include/Common/GameEngine.h`
**Source:** `GameEngine/Source/Common/GameEngine.cpp`
**Singleton:** `extern GameEngine *TheGameEngine`

The top-level coordinator. Owns the main loop, FPS limiter, and the lifecycle
of every other subsystem. It creates and initializes GameLogic, GameClient,
AudioManager, MessageStream, FileSystem, ModuleFactory, and ThingFactory during
`init()`. The `execute()` method runs the game loop until `m_quitting` is set.

---

### GameLogic — Simulation

**Headers:** `GameEngine/Include/GameLogic/`
**Source:** `GameEngine/Source/GameLogic/System/GameLogic.cpp`
**Singleton:** `extern GameLogic *TheGameLogic`

The deterministic simulation core. Everything that affects game state — unit
movement, combat, resource collection, AI decisions, script execution — lives
here or is called from here. GameLogic ticks at `LOGICFRAMES_PER_SECOND = 30`
(defined in `GameCommon.h:72`).

Subcomponents:
- **Object system** (`GameLogic/Object/`) — all in-game objects (units, buildings,
  projectiles). Each object is identified by an `ObjectID` (integer).
- **AI** (`GameLogic/AI/`) — AIStates, AIGroup, TurretAI, guard logic.
- **ScriptEngine** (`GameLogic/ScriptEngine/`) — mission/map script execution.
- **Map** (`GameLogic/Map/`) — terrain logic, bridges, water.

GameLogic also computes a per-frame CRC of the entire simulation state after
each tick for multiplayer desync detection.

---

### GameClient — Rendering and UI

**Headers:** `GameEngine/Include/GameClient/`
**Source:** `GameEngine/Source/GameClient/`
**Singleton:** `extern GameClient *TheGameClient`

Everything the player sees and interacts with that does not affect simulation
state. GameClient manages `Drawable` objects (visual representations of
`Object` instances), the GUI window system, input translation, the tactical
camera, and particle effects.

Key sub-singletons owned by or closely associated with GameClient:
- `TheDisplay` — rendering device abstraction (D3D8 via `GameEngineDevice/`)
- `TheWindowManager` — GUI window tree
- `TheInGameUI` — HUD, selection, command bar
- `TheMouse`, keyboard — raw input → `GameMessage` translation

GameClient does NOT affect simulation state. The coupling is one-way:
GameLogic drives what exists, GameClient decides how to draw it.

---

### GameNetwork — Multiplayer Synchronization

**Headers:** `GameEngine/Include/GameNetwork/`
**Source:** `GameEngine/Source/GameNetwork/`
**Singleton:** `extern NetworkInterface *TheNetwork`

Manages all peer-to-peer multiplayer communication. Implements the lockstep
synchronization model: before GameLogic is allowed to advance one tick, the
network layer must confirm that input commands from all connected players for
that tick have arrived (`NetworkInterface::isFrameDataReady()`).

Key components:
- `FrameData` / `FrameDataManager` — per-tick command packets from each player.
- `Connection` — per-peer UDP connection with retry/ACK.
- `FileTransfer` — map distribution to joining clients (P2-03 added retry logic).
- `GameSpy/` — WOL/GameSpy online matchmaking (stub-able for LAN-only builds).
- `LANAPI` — LAN lobby and game discovery.

---

### GameAudio — Sound

**Headers:** `GameEngine/Include/Common/GameAudio.h`
**Source:** `GameEngine/Source/Common/Audio/`
**Singleton:** `extern AudioManager *TheAudio`

Sound selection, playback, and pitch/volume randomization. Uses the Miles Sound
System (proprietary; stubbed via `Libraries/Stubs/MilesSoundStub.h` for
stub builds). Audio RNG uses its own seed (`GameAudioRandomValue`) and is
intentionally isolated from the simulation RNG — audio divergence between
clients does not cause desync.

---

### GameEngineDevice — Platform Layer

**Source:** `GameEngineDevice/Source/W3DDevice/` and `Win32Device/`

The DirectX 8 + Win32 implementation of the abstract interfaces declared in
`GameEngine/Include/`. This is where concrete rendering (`W3DDisplay`),
windowing (`Win32GameEngine`), and mouse input (`Win32Mouse`) live. P5-03 will
replace this layer with DXVK + SDL3.

---

## Main Game Loop

```
GameEngine::execute()                       GameEngine.cpp:~795
│
└─ loop until quitting:
    GameEngine::update()                    GameEngine.cpp:~752
    │
    ├─ TheRadar->update()
    ├─ TheAudio->update()                   audio tick
    ├─ TheGameClient->update()              input, GUI, drawables, camera
    ├─ TheMessageStream->propagateMessages() route pending messages
    ├─ TheNetwork->update()                 receive/send packets
    ├─ TheCDManager->update()
    │
    └─ if isFrameDataReady() OR not multiplayer:
        TheGameLogic->update()              simulation tick
        │
        ├─ TheScriptEngine->update()
        ├─ TheTerrainLogic->update()
        ├─ processCommandList()             execute all player commands
        │   └─ GameLogicDispatch.cpp        routes by GameMessage::Type
        └─ calculateCRC()                   desync detection hash
```

**FPS limit:** `m_maxFPS = 45` by default (configurable via INI, command line,
or `setFramesPerSecondLimit()`). The simulation tick rate is fixed at 30 Hz
regardless of render FPS.

---

## INI Data Flow

Zero Hour uses a **table-driven INI parser** (`GameEngine/Include/Common/INI.h`).

```
INI files on disk (Data/*.ini)
        │
        ▼
INI::load(filename, INI_LOAD_*)
        │  parses tokens using FieldParse[] tables
        ▼
ThingTemplate objects (units, buildings, weapons, etc.)
        │  stored in ThingFactory by name
        ▼
Object::init()  ─── at runtime, each Object queries its ThingTemplate
```

`GlobalData` (`GameEngine/Include/Common/GlobalData.h`) is the singleton
container for all global INI settings (resolution, network params, gameplay
tuning, font names, etc.). It is loaded first during engine init; subsystems
read from `TheGlobalData` (const) or `TheWritableGlobalData` (mutable).

The INI parser uses three load modes:
- `INI_LOAD_OVERWRITE` — replace existing definition with new one
- `INI_LOAD_CREATE_OVERRIDES` — create a derived definition
- `INI_LOAD_MULTIFILE` — accumulate from multiple files (base game + mods)

---

## Multiplayer Sync Model

Zero Hour uses **deterministic lockstep** — the gold standard for RTS multiplayer.

### How it works

1. Each client translates local player input into `GameMessage` objects and
   queues them in a local `CommandList` for the current tick.

2. `TheNetwork` collects these commands, timestamps them with the current
   frame number, and broadcasts them to all other clients as `FrameData`
   packets.

3. Before GameEngine calls `TheGameLogic->update()`, it checks
   `TheNetwork->isFrameDataReady()`. If any peer's commands for the next tick
   have not yet arrived, the logic tick is skipped (the display frame still
   renders, keeping the UI smooth). This is the **lockstep wait**.

4. Once all peers have delivered their commands, `GameLogic::processCommandList()`
   executes every command from every player in a fixed deterministic order.
   All clients execute the exact same commands in the exact same order — no
   client-side prediction, no reconciliation.

5. At the end of each tick, `GameLogic::calculateCRC()` computes a hash of
   key simulation state (unit positions, health, resources). In multiplayer,
   this CRC is broadcast and compared. A mismatch triggers desync logging
   (`Logs/desync_*.log`, added in P2-02).

### What must be deterministic

- All RNG in the simulation path must use the shared `GameLogicRandomValue`
  macro (seeded from the host's `GetTickCount()` value, propagated to all
  clients via the lobby handshake).
- Object iteration order must be consistent (no `unordered_map` in the
  simulation path).
- Floating-point operations must produce identical results on all machines
  (x87 FPU, same compiler settings).

### RNG tiers (from docs/RNG_AUDIT.md)

| Tier | Macro | Determinism |
|------|-------|-------------|
| Simulation | `GameLogicRandomValue` | Must match all clients |
| Rendering | `GameClientRandomValue` | Visual-only, independent |
| Audio | `GameAudioRandomValue` | Audio-only, independent |

---

## Key Singletons Reference

| Singleton | Type | Purpose |
|-----------|------|---------|
| `TheGameEngine` | `GameEngine*` | Main orchestrator, owns lifecycle |
| `TheGameLogic` | `GameLogic*` | Deterministic simulation |
| `TheGameClient` | `GameClient*` | Rendering, UI, input |
| `TheNetwork` | `NetworkInterface*` | Multiplayer sync |
| `TheAudio` / `TheAudioManager` | `AudioManager*` | Sound |
| `TheDisplay` | `Display*` | Rendering device (D3D8) |
| `ThePlayerList` | `PlayerList*` | All player objects |
| `TheRecorder` | `RecorderClass*` | Replay recording / playback |
| `TheGlobalData` | `GlobalData*` (const) | Read-only config |
| `TheWritableGlobalData` | `GlobalData*` | Mutable config |
| `TheScriptEngine` | `ScriptEngine*` | Mission scripts |
| `TheMessageStream` | `MessageStream*` | Async message routing |
| `TheWindowManager` | `GameWindowManager*` | GUI tree |
| `TheInGameUI` | `InGameUI*` | HUD and selection |
| `TheTacticalView` | `TacticalView*` | Camera / viewport |
| `TheRadar` | `Radar*` | Minimap |

---

## SubsystemInterface Pattern

Every major system inherits from `SubsystemInterface`, which requires:

```cpp
virtual void init();     // Called once at startup
virtual void reset();    // Called between games (but not between frames)
virtual void update();   // Called once per frame (or tick for GameLogic)
```

This uniformity means `GameEngine::init()` can call `init()` on every
subsystem in the correct order, and `GameEngine::reset()` resets all of them
for a new game without restarting the process.

---

## File Tree Summary

```
GeneralsMD/Code/
├── Main/
│   └── WinMain.cpp             Entry point; window creation; arg parsing
├── GameEngine/
│   ├── Include/
│   │   ├── Common/             Core interfaces: GameEngine, INI, GlobalData...
│   │   ├── GameLogic/          GameLogic, AI, Object interfaces
│   │   ├── GameClient/         GameClient, Display, GUI interfaces
│   │   └── GameNetwork/        Network, FrameData, Connection interfaces
│   └── Source/
│       ├── Common/             GameEngine impl; INI; GlobalData; Recorder; Audio
│       ├── GameLogic/
│       │   ├── AI/             AIStates, AIGroup, TurretAI, Guard
│       │   ├── Map/            TerrainLogic, bridges
│       │   ├── Object/         All object behaviors and modules
│       │   ├── ScriptEngine/   Script actions and conditions
│       │   └── System/         GameLogic main loop, GameLogicDispatch
│       ├── GameClient/
│       │   ├── GUI/            Window manager, control bar, menus
│       │   ├── MessageStream/  CommandXlat, MetaEvent, input translation
│       │   └── (Drawable, InGameUI, etc.)
│       └── GameNetwork/
│           ├── GameSpy/        WOL / GameSpy online services
│           └── (Connection, FrameData, FileTransfer, etc.)
├── GameEngineDevice/
│   └── Source/
│       ├── W3DDevice/          DirectX 8 rendering (W3DDisplay, W3DView)
│       └── Win32Device/        Win32 windowing, Win32Mouse, Win32GameEngine
└── Libraries/
    ├── Source/WWVegas/         Westwood utility libraries (math, containers)
    └── Stubs/                  Open-source stub headers for proprietary deps
```
