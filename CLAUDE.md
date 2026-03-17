# Zero Hour Revival — Master Execution Document

---

## REPO SETUP (run once before first session)

```bash
# 1. Fork on GitHub: https://github.com/electronicarts/CnC_Generals_Zero_Hour
#    Fork destination: zerwonenetwork/zh-revival

# 2. Clone your fork
git clone https://github.com/zerwonenetwork/zh-revival.git
cd zh-revival

# 3. Add EA source as upstream (to pull future changes if any)
git remote add upstream https://github.com/electronicarts/CnC_Generals_Zero_Hour.git

# 4. Drop these two files at repo root:
#    CLAUDE.md     ← this file
#    PROGRESS.md   ← the progress tracker

# 5. Start Claude Code
claude
```

---


# Claude Code reads this file at the start of every session.
# This is the single source of truth for the entire project.

---

## WHO YOU ARE

You are the primary AI engineering agent for the **ZerwOne — zh-revival**.
Your job is to execute this plan completely, phase by phase, task by task.
You are working on a real codebase. Every change you make will ship to real players.

---

## WHAT THIS PROJECT IS

A community-driven revival of Command & Conquer: Generals — Zero Hour.
EA released the full source code on February 27, 2025 under GPLv3 + additional terms.
We are building on the EA source, forked to: https://github.com/zerwonenetwork/zh-revival

The goal: make Zero Hour stable, playable online, moddable, and eventually cross-platform.
We do this without selling anything, without using EA trademarks, and without touching gameplay.

---

## HOW YOU WORK

### Session start (do this every single session)
1. Read this file completely
2. Read PROGRESS.md to see what is already done
3. Identify the next incomplete task
4. State what you are about to do and wait for confirmation
5. Do the task
6. Update PROGRESS.md when done

### Rules that never change
- One task per session. Finish it. Then stop.
- Always create a new git branch before touching any file: `git checkout -b task/[TASK-ID]`
- Always show what you plan to change BEFORE you change it
- Never commit directly to main
- Never change unit stats, damage values, AI behavior, or game balance
- Never change simulation tick logic without explicit human approval
- Never use std::unordered_map or std::unordered_set in the simulation path (breaks determinism)
- Never bundle or embed game asset files (.big, .ini, textures, audio)
- Never use EA trademarks. Use "zh-revival" as the project name.
- Every change that touches network or simulation code must include a determinism note
- If you are unsure about a change: stop, explain the uncertainty, ask before proceeding

### Output format for every task
When you finish a task, produce this summary:
  TASK: [task ID and name]
  BRANCH: [branch name]
  FILES CHANGED: [list]
  WHAT CHANGED: [one paragraph]
  DETERMINISM IMPACT: [none / low / review needed]
  HOW TO TEST: [exact steps]
  NEXT TASK: [next task ID]

---

## PROGRESS TRACKING

You maintain a file called PROGRESS.md at the repo root.
Format:

  ## Phase 1 — Stability
  - [x] P1-01 CMake migration — completed 2026-MM-DD
  - [ ] P1-02 vcpkg manifest — not started
  ...

Update it at the end of every session. This is how you and the human know where you are.

---

## THE FULL TASK LIST

### ═══════════════════════════════════════
### PHASE 1 — MAKE THE GAME DEPENDABLE
### Goal: Zero Hour runs without crashing on modern Windows
### Do not move to Phase 2 until all Phase 1 tasks are complete and CI is green
### ═══════════════════════════════════════

#### P1-01 — CMake build system migration
WHAT: Convert the legacy rts.dsw / .dsp VC6 project files into a modern CMakeLists.txt
WHY: Nothing else in this plan works without a reproducible modern build
HOW:
  1. Read rts.dsw and every .dsp file it references
  2. Map every build target (game exe, tools, libs) to a CMake target
  3. Set C++ standard to C++20 per-target
  4. Set Windows minimum version to 0x0A00 (Windows 10)
  5. Set MSVC warning level /W3
  6. Use target_include_directories, not global include_directories
  7. Write the complete CMakeLists.txt — show it before writing
MUST NOT: Change any source file. Build system only.
OUTPUT: CMakeLists.txt at repo root
VERIFY: `cmake -B build -S . && cmake --build build --config Release` produces a binary

#### P1-02 — vcpkg dependency manifest
WHAT: Create vcpkg.json listing all open-source replaceable dependencies
WHY: Reproducible dependency management for all contributors
HOW:
  1. Inventory all third-party dependencies in /Code/Libraries/
  2. For each one: check if it exists in the vcpkg registry
  3. Add vcpkg-available deps to vcpkg.json with pinned versions
  4. Document non-vcpkg deps (Miles, Bink, SafeDisc, GameSpy) separately in DEPENDENCIES.md
OUTPUT: vcpkg.json, vcpkg-lock.json, DEPENDENCIES.md
VERIFY: `vcpkg install` completes without errors

#### P1-03 — Dependency stubs (Miles, Bink, SafeDisc, GameSpy)
WHAT: Create stub headers + empty implementations for all closed-source deps
WHY: Enables the project to compile without proprietary SDKs
HOW:
  Create /Code/Libraries/Stubs/ with one file per stubbed library:
  - MilesSoundStub.h — stub the Miles Sound System API surface
  - BinkVideoStub.h  — stub the Bink video playback API
  - SafeDiscStub.h   — stub SafeDisc CD validation calls (return success)
  - GameSpyStub.h    — stub GameSpy network API (all functions return offline/error)
  Each stub: #define STUB_IMPL, interface-compatible function signatures,
  minimal no-op bodies that return safe default values
  Add #ifdef STUB_IMPL guards so real SDKs can be swapped back in
MUST NOT: Change any game logic. Stubs return safe defaults only.
OUTPUT: /Code/Libraries/Stubs/*.h and *.cpp
VERIFY: Project compiles with stubs enabled. Audio/video features degrade gracefully.

#### P1-04 — GitHub Actions CI build pipeline
WHAT: Automated build check on every pull request
WHY: No broken builds land on main
HOW:
  Create .github/workflows/ci-build.yml:
  - Triggers: pull_request to main, push to main
  - Matrix: Windows (MSVC), Linux (GCC via Docker)
  - Steps: checkout, setup vcpkg, cmake configure, cmake build Release
  - Upload build artifact on success
  - Fail fast on first error
  Create .github/workflows/ci-release.yml:
  - Triggers: tag push matching v*.*.*
  - Steps: build Release, sign binary (uses ${{ secrets.SIGN_CERT }}), 
    create GitHub Release, upload signed artifact
MUST NOT: Put secrets inline. Use ${{ secrets.* }} references only.
OUTPUT: .github/workflows/ci-build.yml, .github/workflows/ci-release.yml
VERIFY: A test PR triggers the workflow and both platforms build green

#### P1-05 — HKLM to HKCU registry migration
WHAT: Move all registry reads/writes from HKEY_LOCAL_MACHINE to HKEY_CURRENT_USER
WHY: Modern Windows often denies non-admin writes to HKLM. Causes "corrupted registry" errors.
HOW:
  1. Search entire codebase for: RegOpenKeyEx, RegCreateKeyEx, RegSetValueEx,
     RegQueryValueEx, RegDeleteKey with HKEY_LOCAL_MACHINE or HKLM
  2. List every occurrence (file, line, purpose)
  3. Replace each with HKEY_CURRENT_USER equivalent
  4. Preserve the same key path under HKCU
MUST NOT: Touch registry calls related to CD-key validation (leave those alone)
MUST NOT: Touch anything in /Code/Libraries/ (external deps)
OUTPUT: Modified config/registry files
VERIFY: Game launches and saves settings without admin rights on Windows 11

#### P1-06 — Fix R4: Particle accumulation crash (Win+L)
WHAT: Fix crash/freeze caused by particle system accumulation when screen is locked
WHY: When Win+L is pressed, D3D device becomes non-cooperative. Particle manager stops
     updating but keeps accumulating. On unlock: thousands of particles fire at once,
     memory spikes, game crashes or freezes.
HOW:
  1. Find the particle system manager — search for ParticleSystem, ParticleManager
  2. Find the main particle update loop
  3. Find where it checks D3D device cooperative state
  4. Identify the accumulation path when device is non-cooperative
  5. Add: on device resume, flush/discard accumulated particle requests safely
     OR: pause accumulation while device is non-cooperative
  6. Verify the fix does not affect particle behavior during normal gameplay
DETERMINISM: Particles are visual only — fix should have zero determinism impact.
             Confirm this in your output.
OUTPUT: Modified particle manager file(s)
VERIFY: Lock screen (Win+L), wait 10 seconds, unlock — game continues without crash

#### P1-07 — Fix R2: Alt-Tab / fullscreen device loss crash
WHAT: Fix crash when Alt-Tabbing out of fullscreen mode
WHY: D3D8 device loss on focus change is not handled — surfaces aren't released/reset properly
HOW:
  1. Find all IDirect3DDevice8 usage in rendering code
  2. Find every place where HRESULT is returned but not checked
  3. Find every place where D3DERR_DEVICELOST is not handled
  4. Find the window message handler — look for WM_ACTIVATEAPP, WM_SIZE
  5. Implement proper device loss handling:
     - On D3DERR_DEVICELOST: release all non-managed resources
     - Poll TestCooperativeLevel until D3DERR_DEVICENOTRESET
     - Call Reset() with current presentation params
     - Recreate released resources
  6. Alternatively: use the d3d8to9 wrapper pattern if full reset is too risky
DETERMINISM: Rendering only — no determinism impact.
OUTPUT: Modified D3D device / rendering init files
VERIFY: Alt-Tab in and out of fullscreen 10 times without crash

#### P1-08 — Fix R1: Startup "Serious Error" / DirectX init failure
WHAT: Fix crash on launch with "Serious Error" on modern systems with modern GPU drivers
WHY: Legacy D3D8 device creation fails on certain GPU/driver combos (Intel iGPU, new NVIDIA/AMD)
HOW:
  1. Find the D3D8 initialization code — device creation, adapter enumeration
  2. Find where it selects the GPU adapter
  3. Add fallback: if preferred adapter fails, try D3DADAPTER_DEFAULT
  4. Add better error logging: log HRESULT code, adapter name, driver version
  5. Add user-facing error message with actionable advice (not just "Serious Error")
  6. Consider: if D3D8 init fails entirely, suggest the user install d3d8.dll wrapper
OUTPUT: Modified D3D init file(s)
VERIFY: Game launches on a system where it previously showed Serious Error

#### P1-09 — Fix R5: Large-match pathfinder crash (~1000 entities)
WHAT: Fix crash when unit+building count exceeds ~1000 in a match
WHY: Fixed-size buffer overflow in pathfinder open/closed list
HOW:
  1. Find the pathfinding system — search for Pathfind, OpenList, ClosedList, AStar
  2. Find fixed-size arrays or static buffers used for entity movement queues
  3. For each fixed-size buffer: determine the maximum value it receives at runtime
  4. Replace with std::vector or similar dynamic container
  5. CRITICAL: replacement containers must have deterministic iteration order
     (std::vector is fine, std::unordered_* is NOT)
  6. Also check: shadow render list for the "too many shadows" crash mode —
     find the shadow list buffer and apply same fix
DETERMINISM: Pathfinding IS in the simulation path. Flag any concern in output.
OUTPUT: Modified pathfinder file(s)
VERIFY: Load a map, spawn 1200+ units, confirm no crash

#### P1-10 — Fix R3: Audio loss after minimize
WHAT: Fix audio going silent when game is minimized and restored
WHY: Miles Sound System (or stub) loses device context on minimize, doesn't restore
HOW:
  1. Find the audio system initialization and update code
  2. Find where the game handles WM_ACTIVATE or minimize/restore window messages
  3. Add audio context restore call on window restore/focus regain
  4. If using Miles stub: add a no-op restore hook that can be wired to real Miles later
  5. If using stub and audio is already non-functional: document the TODO clearly
OUTPUT: Modified audio system file(s)
VERIFY: Minimize game, wait 5 seconds, restore — audio continues playing

#### P1-11 — Remove superfluous CD/DRM checks
WHAT: Remove legacy CD presence checks that block launch on modern digital installs
WHY: The game is sold digitally on Steam and EA App. CD checks always fail there.
HOW:
  1. Search for CD check code: GetDriveTypeA, FindFirstFile targeting CD paths,
     SafeDisc validation calls, volume serial number checks
  2. List every CD/DRM check found
  3. For checks that are ONLY for CD validation: replace with return success / skip
  4. For SafeDisc API calls: stub already created in P1-03, wire to it
MUST NOT: Remove Steam/EA App license verification
MUST NOT: Touch any online authentication or multiplayer auth code
OUTPUT: Modified startup / init files
VERIFY: Game launches from Steam install without CD present

#### P1-12 — Create BUILDING.md
WHAT: Write step-by-step build instructions for new contributors
WHY: Without this, contributors can't get started. This is the first thing they read.
HOW: Write /BUILDING.md covering:
  1. Prerequisites: Visual Studio 2022, CMake 3.25+, vcpkg, Git
  2. Clone and submodule setup
  3. vcpkg install step
  4. CMake configure command (exact command to copy-paste)
  5. Build command (exact command to copy-paste)
  6. How to run the game from the build output (requires owning the game)
  7. Troubleshooting: most common build errors and fixes
  8. Linux build instructions (Docker path)
  Keep it under 300 lines. Use code blocks for every command.
OUTPUT: BUILDING.md
VERIFY: A developer who has never seen this repo can build it following only this doc

#### P1-FINAL — Create PROGRESS.md and tag v0.1
WHAT: Initialize the progress tracker and tag the first release
HOW:
  1. Create PROGRESS.md with all Phase 1 tasks marked complete
  2. Write a one-line summary of what each task did
  3. Run full release build
  4. Verify binary runs on a clean Windows 11 install with the game
  5. Tag: `git tag v0.1-stability`
OUTPUT: PROGRESS.md, git tag v0.1-stability
VERIFY: CI passes, binary launches, top crashes from Phase 1 are gone


### ═══════════════════════════════════════
### PHASE 2 — FIX ONLINE PAIN
### Goal: Multiplayer sessions complete without mismatch, DC-bug, or map transfer failures
### Do not move to Phase 3 until all Phase 2 tasks complete and replay CI test passes
### ═══════════════════════════════════════

#### P2-01 — RNG discipline audit
WHAT: Find and categorize every use of random number generation in the codebase
WHY: Non-deterministic RNG in simulation = mismatches between clients
HOW:
  Search for: rand(), srand(), std::rand, mt19937, random_device,
              any custom GameEngine RNG functions
  For each use:
  - Is it in the simulation path? (affects game state) → SIMULATION
  - Is it in rendering/audio/UI? (visual only) → AUDIO-VISUAL
  - Unknown? → UNKNOWN
  Produce a table: location | function | category | uses shared seed? | notes
  For SIMULATION category: verify seed is set at game start and identical on all clients
  For AUDIO-VISUAL: document that these can use their own seed
  Flag any SIMULATION RNG that does NOT use the shared seed as CRITICAL
OUTPUT: docs/RNG_AUDIT.md — full table of findings
VERIFY: Human reviews the table before any fixes are made

#### P2-02 — Per-tick state hashing for mismatch detection
WHAT: Add state hash comparison every N ticks to detect when clients diverge
WHY: Currently mismatches are only discovered when they cause visible desyncs.
     State hashing detects divergence immediately and tells us which subsystem diverged.
HOW:
  1. Implement a hash function: FNV-1a or xxHash (fast, deterministic, no pointer hashing)
  2. Hash covers: unit positions (integer coords), unit health, resource counts,
     active order queues — minimum set to detect divergence
  3. Hash runs at end of every 16th tick (make interval configurable via INI: HashTickInterval)
  4. In multiplayer: compare local hash with hash received from host
  5. On mismatch: log to Logs/desync_YYYYMMDD_HHMMSS.log with:
     - Tick number
     - Local hash
     - Remote hash
     - Which player(s) diverged
     - Dump of local unit count, resource counts at that tick
  6. Phase 2: LOG ONLY — do not disconnect player on mismatch yet
  7. Hash function MUST produce identical output on Win32 and Win64
     (no sizeof(pointer), no memory address hashing)
DETERMINISM: This IS the determinism system. It must not affect simulation outcome.
             Hash must be computed AFTER the tick completes, read-only.
OUTPUT: Modified simulation/network files, new Logs/ output format
VERIFY: Run two clients on same map, confirm hashes match every 16 ticks.
        Artificially introduce a state divergence, confirm it is detected and logged.

#### P2-03 — Map transfer retry and content validation
WHAT: Make map transfers reliable and safe
WHY: Map transfers fail silently. Failed transfer = can't join game.
     Unvalidated map transfers = malicious map can crash all clients.
HOW:
  1. Find the map transfer code (host sending map file to joining player)
  2. Add retry logic: on transfer failure, retry up to 3 times with 2s delay
  3. Add content validation on receiver side:
     - File size must be under 50MB (reject silently if exceeded)
     - File header must match known .map format magic bytes
     - SHA256 of received file must match hash sent by host in the lobby message
  4. Add clear UI error: "Map transfer failed after 3 attempts. Ask host to retry."
  5. CRITICAL: a map that fails validation must NEVER be loaded. Reject and log.
  6. Log all transfer attempts and validation results to Logs/map_transfer.log
SECURITY: This is a security-sensitive change. The validation must happen BEFORE
          the map file is touched by the map loader. No exceptions.
OUTPUT: Modified map transfer / network files
VERIFY: Transfer a known-good map: succeeds and validates.
        Transfer a corrupted map file: rejected with error, game does not crash.

#### P2-04 — Full network message security audit
WHAT: Audit all network-facing code for unsafe input handling
WHY: A malicious player can send crafted network messages to crash or exploit other clients
HOW:
  Find all functions that parse incoming network messages.
  For each: trace data flow from network receive to use.
  Look for:
  CRITICAL:
    - strcpy/strcat with user-supplied length/content (buffer overflow)
    - memcpy where length comes from untrusted data
    - Array index from untrusted data without range check
    - printf-family functions with user-controlled format string
  HIGH:
    - Integer overflow in size calculation from network data
    - Use-after-free in message handling
    - Null pointer dereference on malformed packet
    - Path traversal in any file reference from network data
  Produce vulnerability report using this format per finding:
    VULN-ID: [sequential number]
    SEVERITY: CRITICAL / HIGH / MEDIUM
    FILE: [path]
    LINE: [number]
    DESCRIPTION: [what the bug is]
    EXPLOIT: [how a malicious player triggers it]
    FIX: [minimal code change]
    BREAKS_PROTOCOL: yes/no
MUST: Produce the full report FIRST. Do not fix anything yet.
      Human must approve the report before any fixes proceed.
OUTPUT: docs/SECURITY_AUDIT_P2.md — full vulnerability report
VERIFY: Human reviews report and approves fix priority before P2-05 begins

#### P2-05 — Fix CRITICAL vulnerabilities from P2-04
WHAT: Fix all CRITICAL-severity findings from the security audit
HOW:
  For each CRITICAL finding in docs/SECURITY_AUDIT_P2.md:
  1. Apply the minimal fix proposed in the audit
  2. Verify: the fix handles malformed input gracefully (no crash, no exploit)
  3. Verify: valid input still works correctly after the fix
  4. For each fix: note if it changes the network protocol (BREAKS_PROTOCOL)
  5. Protocol-breaking fixes need a version bump in the network handshake
MUST NOT: Fix more than approved. Stick to the audit findings only.
OUTPUT: Modified network parsing files. Updated docs/SECURITY_AUDIT_P2.md (mark fixed)
VERIFY: Each fix: send a crafted malformed packet, confirm no crash.

#### P2-06 — Fix HIGH vulnerabilities from P2-04
Same as P2-05 but for HIGH-severity findings. Same process.

#### P2-07 — Replace unsafe string functions in network code
WHAT: Replace strcpy, strcat, sprintf, gets in all network-facing code paths
HOW:
  1. Search network code for: strcpy, strcat, sprintf, gets, sscanf with %s
  2. For each: classify as network-facing (from untrusted source) or internal
  3. Replace network-facing uses:
     - strcpy(dst, src) → strncpy_s(dst, sizeof(dst), src, _TRUNCATE)
     - strcat(dst, src) → strncat_s(dst, sizeof(dst), src, _TRUNCATE)
     - sprintf(buf, fmt, ...) → snprintf(buf, sizeof(buf), fmt, ...)
  4. Every replacement must preserve identical behavior for valid inputs
SCOPE: Network-facing code only. Do not touch simulation internals here.
OUTPUT: Modified network layer files
VERIFY: All original network functionality works. Malformed input no longer crashes.

#### P2-08 — Version CRC enforcement
WHAT: Prevent players with different binary versions from joining the same game
WHY: Mixed versions cause mismatches and undefined behavior
HOW:
  1. Find the multiplayer join/handshake code
  2. Add: compute SHA256 of game binary at startup, store as build fingerprint
  3. In lobby join request: include build fingerprint
  4. Host: reject join if fingerprint doesn't match, with clear error message:
     "Your game version doesn't match the host. Update your installation."
  5. Make the fingerprint visible in the lobby player list
  6. The official patch CI must embed the build hash at compile time
OUTPUT: Modified network handshake / lobby files
VERIFY: Two different build versions: join rejected with clear message.
        Same build version: join succeeds normally.

#### P2-09 — Headless replay CI determinism test
WHAT: Add a CI test that plays back a known replay and verifies output is identical
WHY: Catches determinism regressions automatically before they ship
HOW:
  1. Create a known-good 5-minute replay file for a simple 1v1 match
  2. Add a --headless-replay flag to the game binary that:
     - Loads the replay without rendering
     - Fast-forwards through all ticks
     - Outputs final state hash to stdout
     - Exits with code 0 if hash matches expected, 1 if not
  3. Add to ci-build.yml: after build, run headless replay, fail CI if hash mismatch
OUTPUT: Test replay file, modified game entry point, updated ci-build.yml
VERIFY: CI runs replay test on every PR. Intentionally corrupt a sim value,
        confirm CI fails.

#### P2-10 — DC-bug recovery improvement
WHAT: Improve behavior when the "DC bug" (disconnect-menu loop) occurs
WHY: Currently the disconnect-menu loop is unrecoverable — player must quit.
HOW:
  1. Find the disconnect handling code in multiplayer session management
  2. Find where the disconnect-menu loop is triggered
  3. Add: timeout-based auto-recovery attempt (wait 10s, attempt reconnect once)
  4. Add: if recovery fails, show a clean "You were disconnected" screen
     with the option to return to lobby or main menu cleanly
  5. Log disconnect events to Logs/disconnect.log with timestamp and reason code
  6. Do NOT attempt to "fix" the root network cause here — that is P3+ scope
OUTPUT: Modified multiplayer session / UI files
VERIFY: Simulate a DC event — game shows clean recovery UI instead of looping menu

#### P2-FINAL — Tag v0.2 and update PROGRESS.md
WHAT: Tag Phase 2 completion
HOW: Update PROGRESS.md. Run full release build. Tag v0.2-online.
     Verify multiplayer session completes without mismatch on a test match.


### ═══════════════════════════════════════
### PHASE 3 — QUALITY OF LIFE
### Goal: The game feels modern. Resolution, replay, settings — no INI editing.
### ═══════════════════════════════════════

#### P3-01 — Borderless fullscreen and widescreen support
WHAT: Add robust borderless fullscreen mode + custom resolution without INI editing
HOW:
  1. Find windowing initialization code
  2. Add command-line flag: -borderless / -windowed / -fullscreen
  3. Add resolution override: -width X -height Y
  4. Add borderless fullscreen mode: WS_POPUP window at desktop resolution
  5. Fix cursor edge-scroll: re-enable edge scrolling in borderless mode
     (currently disabled because cursor leaves window)
  6. Store window mode preference in user config (HKCU, from P1-05)
OUTPUT: Modified windowing / init files
VERIFY: Game runs in borderless 1920x1080, 2560x1440, 3840x2160. Edge scroll works.

#### P3-02 — Replay format specification
WHAT: Document the exact binary format of .rep replay files
HOW:
  1. Find all code that WRITES .rep files (replay recording)
  2. Trace the write path completely — every field written in order
  3. Document:
     - File header: magic bytes, version field, map hash, player list format
     - Per-tick command records: record type, player index, command data
     - Footer/checksum format
  4. Note which fields changed between ZH 1.04 and community patches
OUTPUT: docs/REPLAY_FORMAT.md — complete binary format specification
VERIFY: Parse a known .rep file manually using the spec. Every byte accounted for.

#### P3-03 — libzhreplay: replay parser library
WHAT: C++ library for reading and parsing .rep replay files
HOW:
  1. Using docs/REPLAY_FORMAT.md as spec, implement tools/libzhreplay/
  2. Public API:
     - ZHReplay* zh_replay_open(const char* path)  // open and parse
     - int zh_replay_valid(ZHReplay* r)             // 1 if valid
     - ZHReplayHeader zh_replay_header(ZHReplay* r) // get header
     - ZHCommand zh_replay_next_command(ZHReplay* r)// iterate commands
     - void zh_replay_close(ZHReplay* r)            // cleanup
  3. Must handle malformed/truncated replays without crashing
  4. Must be version-aware (handle ZH 1.04 and community patch versions)
  5. No external dependencies beyond C++ standard library
OUTPUT: tools/libzhreplay/*.h and *.cpp
VERIFY: Parse 5 different .rep files. No crashes on corrupted inputs.

#### P3-04 — zh-replay-validate: CLI validator
WHAT: Command-line tool to validate .rep file integrity
HOW:
  Build on libzhreplay. tools/zh-replay-validate/main.cpp
  Usage: zh-replay-validate <file.rep>
  Output on valid:
    VALID: [player1] vs [player2] on [map] — [N] commands — [duration]s
  Output on invalid:
    INVALID: [error description at byte offset X]
  Exit codes: 0 = valid, 1 = invalid, 2 = tool error
  Must never crash on any input, including /dev/zero and random bytes
OUTPUT: tools/zh-replay-validate/
VERIFY: Valid replay → exit 0. Truncated file → exit 1. Random bytes → exit 1.

#### P3-05 — zh-replay-info: JSON metadata extractor
WHAT: CLI tool that outputs replay metadata as machine-readable JSON
HOW:
  Build on libzhreplay. tools/zh-replay-info/main.cpp
  Usage: zh-replay-info <file.rep>
  Output (strict JSON, no extra text):
  {
    "schema_version": 1,
    "replay_version": <int or null>,
    "map_hash": "<sha256 or null>",
    "map_name": "<string or null>",
    "date_iso": "<ISO8601 or null>",
    "duration_ticks": <int or null>,
    "players": [{"name":"...","faction":"...","slot":<int>,"is_ai":<bool>}],
    "winner": "<name or null>",
    "build_version": "<string or null>"
  }
  Unknown fields → null (never omit, never error)
  Invalid file → {"error": "invalid replay file", "schema_version": 1}
OUTPUT: tools/zh-replay-info/
VERIFY: Output parses as valid JSON. All fields present. null for unknowns.

#### P3-06 — Replay HUD improvements
WHAT: Better observer/replay experience with timestamp and speed controls
HOW:
  1. Find the replay/observer HUD code
  2. Add visible tick counter + elapsed time display in observer mode
  3. Add replay speed control: 0.5x / 1x / 2x / 4x (keybinds: [ and ])
  4. Fix crash that occurs in player selection in replay mode (without GenTool)
  5. Add "Jump to tick" input for replay analysis
OUTPUT: Modified replay/observer UI files
VERIFY: Load a replay. Tick counter visible. Speed controls work. No crash on player select.

#### P3-FINAL — Tag v0.3
Update PROGRESS.md. Tag v0.3-qol.


### ═══════════════════════════════════════
### PHASE 4 — BUILD THE ECOSYSTEM
### Goal: Zero Hour is easy to install, mod, watch, and maintain
### ═══════════════════════════════════════

#### P4-01 — Anti-cheat security audit
WHAT: Audit for cheat vectors and input validation gaps
HOW:
  Focus on:
  - Can a player send impossible action commands? (build two buildings simultaneously, 
    spend resources they don't have, move units to invalid positions)
  - Can player-controlled data influence other clients' simulation?
  - Are replay files tamper-evident? (hash of commands vs. game outcome)
  Produce: docs/SECURITY_AUDIT_P4.md — same format as P2-04
  This audit informs the anti-cheat design, not a code sprint
OUTPUT: docs/SECURITY_AUDIT_P4.md

#### P4-02 — Architecture documentation
WHAT: Generate complete architecture documentation from the source
HOW: Read the source tree and produce docs/ARCHITECTURE.md covering:
  1. Major subsystems: GameLogic, GameNetwork, GameRenderer, GameAudio,
     GameClient, GameEngine — what each does, what files belong to it
  2. Main game loop: tick rate, update order, render order
  3. How INI data flows through the engine (load → parse → apply to objects)
  4. Multiplayer sync model: how clients stay in sync, what gets replicated
  5. SAGE engine relationship: what SAGE is, how ZH extends it
  Write for a C++ developer new to this codebase.
  1000–2000 words. Headers. No walls of text.
OUTPUT: docs/ARCHITECTURE.md
VERIFY: A developer who has never seen the codebase can understand the structure.

#### P4-03 — INI modding reference
WHAT: Document all INI object types and their fields for modders
HOW: Read the INI parser and all .ini files. Produce docs/modding/INI_REFERENCE.md
  For each major INI type (Unit, Building, Weapon, Armor, FX, CommandSet, etc.):
  - Object type name and what it represents
  - All accepted fields with type, valid range, and meaning
  - A minimal working example
  Unknown field behavior → mark [BEHAVIOR UNCLEAR — needs playtesting]
  Write for a modder who knows the game but not C++.
OUTPUT: docs/modding/INI_REFERENCE.md
VERIFY: A modder can use this doc to create a valid custom unit from scratch.

#### P4-04 — Contributor guide
WHAT: Document how to contribute to the project
HOW: Write docs/CONTRIBUTING.md covering:
  1. How to fork and clone the repo
  2. Branch naming convention: task/[TASK-ID]-short-description
  3. Commit message format: type(scope): description [AGT-XX] if AI-authored
  4. PR requirements: CI must pass, one reviewer required, describe what changed
  5. Code style guide (derived from what exists in the codebase)
  6. How to report bugs (issue template)
  7. Non-affiliation statement: contributors acknowledge this is not affiliated with EA
  8. License: GPLv3 + EA additional terms summary
OUTPUT: docs/CONTRIBUTING.md, .github/ISSUE_TEMPLATE/bug_report.md

#### P4-05 — Mod manager foundation
WHAT: Safe install/uninstall system for .big mod files
HOW:
  1. Create a ModManager class in the game that:
     - Scans a /Mods/ folder in the game directory
     - Loads .big files in priority order (highest number = highest priority)
     - Tracks which mod provides which files (for conflict detection)
     - Reports conflicts to a log: which mod overwrites which base game file
  2. Add a command-line flag --no-mods for vanilla play
  3. The mod folder is user-configurable via HKCU registry (from P1-05)
  4. No UI yet — that comes in a future launcher task
MUST NOT: Change how base game assets are loaded in unmodded play
OUTPUT: New ModManager files, modified game init
VERIFY: Place a test .big file in /Mods/ — it loads. Remove it — vanilla loads.

#### P4-FINAL — Tag v0.4
Update PROGRESS.md. Tag v0.4-ecosystem.


### ═══════════════════════════════════════
### PHASE 5 — NEXT-GEN ENGINE
### Goal: Win64, Linux, macOS. No proprietary runtime deps. Future-proof.
### ═══════════════════════════════════════

#### P5-01 — Replace Miles Sound System with OpenAL
WHAT: Replace proprietary Miles Sound System with OpenAL (cross-platform, open-source)
WHY: Miles is proprietary and Windows-only. OpenAL enables Linux/macOS.
HOW:
  1. Map the full Miles API surface used in this game (from MilesSoundStub.h in P1-03)
  2. Implement each function using OpenAL equivalents
  3. Maintain the same interface so the rest of the game doesn't need to change
  4. Test: all in-game sounds play correctly (unit sounds, ambient, music, UI)
  5. Add to vcpkg.json: openal-soft
MUST NOT: Change how the game calls audio functions. Replace the backend only.
OUTPUT: New audio backend in /Code/GameAudio/OpenAL/
VERIFY: All audio plays correctly. No regression in sound behavior.

#### P5-02 — Replace Bink video with open decoder
WHAT: Replace Bink video playback with an open-source alternative
HOW:
  1. Find all Bink playback calls (from BinkVideoStub.h in P1-03)
  2. Implement equivalent using FFmpeg (for .bik decode) or stub with still image
  3. If FFmpeg: use a minimal build (just Bink decoder if license allows,
     or re-encode videos to WebM/VP8 for distribution)
  4. Fallback: if no open Bink decoder is available, skip cutscenes with a note in log
     and display the mission briefing text instead
OUTPUT: New video backend, fallback behavior documented
VERIFY: Intro/briefing videos play. Or: graceful skip with no crash.

#### P5-03 — DXVK + SDL3 graphics and windowing layer
WHAT: Replace DirectX 8 + Win32 windowing with DXVK (D3D8→Vulkan) + SDL3
WHY: This is the path to Linux and macOS. Following GeneralsX proof-of-concept.
HOW:
  This is a large task. Break it into subtasks and do them in order:
  P5-03a: Replace Win32 window creation with SDL3 window
  P5-03b: Replace DirectInput keyboard/mouse with SDL3 input
  P5-03c: Integrate DXVK: D3D8 calls now go to Vulkan via DXVK translation layer
  P5-03d: Test on Windows first. All rendering must work identically.
  P5-03e: Test on Linux. Fix any platform-specific issues.
  Reference: https://github.com/fbraz3/GeneralsX (SDL3+DXVK implementation)
MUST NOT: Change the rendering logic — only the platform layer underneath it
OUTPUT: New platform layer, updated CMake for SDL3+DXVK deps
VERIFY: Game renders identically to original on Windows. Then boots on Linux.

#### P5-04 — Win64 build
WHAT: Add 64-bit Windows build target alongside the existing 32-bit target
WHY: Removes 4GB address space limit. Fixes large-match memory pressure.
HOW:
  1. Fix all 32-bit-only assumptions: sizeof(pointer) comparisons, int/pointer casts,
     fixed-size serialization that assumes 32-bit types
  2. Add Win64 CMake target
  3. Build Win64 Release and verify it runs
  4. 64-bit clients get their own multiplayer compatibility channel (P2-08 version CRC)
     so 32-bit and 64-bit clients don't accidentally join the same game
OUTPUT: Win64 build target, updated CMake
VERIFY: Win64 binary launches and plays through a skirmish game without crash.

#### P5-05 — Linux native build
WHAT: Linux native binary via Docker-based build
HOW:
  1. DXVK + SDL3 layer from P5-03 handles the platform differences
  2. Fix any remaining Linux-specific issues:
     - File path case sensitivity: Linux is case-sensitive, ensure all asset paths match
     - Any Windows-only API calls missed in P5-03
  3. Update .github/workflows/ci-build.yml: Linux build now runs the actual game binary
     in a headless test (not just compile)
  4. Write docs/modding/LINUX_BUILD.md
OUTPUT: Linux-compatible binary, CI test, Linux build docs
VERIFY: Game launches and completes a skirmish on Ubuntu 22.04 LTS.

#### P5-06 — macOS ARM64 build
WHAT: Native macOS Apple Silicon build
HOW:
  1. Add MoltenVK: translates Vulkan to Metal on macOS
  2. SDL3 already handles macOS windowing
  3. Fix any macOS-specific issues (bundle structure, entitlements for network)
  4. Write docs/modding/MACOS_BUILD.md
  Reference: GeneralsX has working macOS ARM64 builds to learn from
OUTPUT: macOS ARM64 binary, macOS build docs
VERIFY: Game launches on Apple M1 or M2 Mac and completes a skirmish.

#### P5-07 — Full memory safety audit
WHAT: Replace all remaining unsafe string/memory functions engine-wide
WHY: Phase 2 covered network code. Phase 5 covers everything else.
HOW:
  Search engine-wide for: strcpy, strcat, sprintf, gets, scanf %s (unbounded),
  manual malloc without corresponding free, raw pointer arithmetic on unknown-size buffers
  For each: classify severity, propose replacement
  Fix all CRITICAL, then HIGH, then document remaining MEDIUM for future work
OUTPUT: docs/MEMORY_SAFETY_AUDIT.md, modified source files
VERIFY: Run with AddressSanitizer (ASAN) on Linux build. Zero ASAN errors on startup + skirmish.

#### P5-FINAL — Tag v1.0
Update PROGRESS.md with all phases complete.
Tag v1.0-revival.
Write CHANGELOG.md summarizing all changes from v0.1 to v1.0.
This is the stable release.


---

## FILE STRUCTURE YOU MAINTAIN

GeneralsGameCode/
├── CLAUDE.md               ← this file (do not modify)
├── PROGRESS.md             ← you update this after every task
├── BUILDING.md             ← P1-12 creates this
├── CHANGELOG.md            ← P5-FINAL creates this
├── CMakeLists.txt          ← P1-01 creates this
├── vcpkg.json              ← P1-02 creates this
├── DEPENDENCIES.md         ← P1-02 creates this
├── .github/
│   └── workflows/
│       ├── ci-build.yml    ← P1-04 creates this
│       └── ci-release.yml  ← P1-04 creates this
├── .claude/
│   └── (agent context files — do not modify)
├── Code/
│   └── Libraries/
│       └── Stubs/          ← P1-03 creates this
│           ├── MilesSoundStub.h
│           ├── BinkVideoStub.h
│           ├── SafeDiscStub.h
│           └── GameSpyStub.h
├── docs/
│   ├── ARCHITECTURE.md     ← P4-02 creates this
│   ├── CONTRIBUTING.md     ← P4-04 creates this
│   ├── RNG_AUDIT.md        ← P2-01 creates this
│   ├── REPLAY_FORMAT.md    ← P3-02 creates this
│   ├── SECURITY_AUDIT_P2.md← P2-04 creates this
│   ├── SECURITY_AUDIT_P4.md← P4-01 creates this
│   ├── MEMORY_SAFETY_AUDIT.md ← P5-07 creates this
│   └── modding/
│       ├── INI_REFERENCE.md← P4-03 creates this
│       ├── LINUX_BUILD.md  ← P5-05 creates this
│       └── MACOS_BUILD.md  ← P5-06 creates this
└── tools/
    ├── libzhreplay/        ← P3-03 creates this
    ├── zh-replay-validate/ ← P3-04 creates this
    └── zh-replay-info/     ← P3-05 creates this

---

## LEGAL CONSTRAINTS (never violate these)

- This project is GPLv3. All distributed source and binaries must comply.
- EA additional terms: no EA trademarks in branding, naming, or generated text.
- Use "zh-revival" as the project name. Never "Command & Conquer [anything]" in
  project branding, binary names, or documentation titles.
- Every public-facing document must include:
  "This project is not affiliated with or endorsed by Electronic Arts.
   Command & Conquer is a trademark of Electronic Arts.
   You must own the original game to use this software."
- No game assets (audio, textures, .ini files, .big files) may be distributed.
  The game requires an existing legal installation of Zero Hour.
- Community mods must be non-commercial and distributed free of charge.
- Donations may be accepted for server costs only (no in-game donor perks).

---

## YOUR FIRST ACTION IN EVERY SESSION

1. Read this file
2. Read PROGRESS.md (or create it if it doesn't exist yet)
3. Say: "Current task: [TASK-ID] — [task name]. Here is what I plan to do: [summary]"
4. Wait for confirmation
5. Execute

