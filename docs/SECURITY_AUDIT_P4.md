# Security Audit P4 — Anti-Cheat

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

**Task:** P4-01
**Date:** 2026-03-23
**Scope:** Cheat vectors, impossible-command injection, replay tamper-evidence
**Format:** Same as docs/SECURITY_AUDIT_P2.md

---

## Executive Summary

Zero Hour's multiplayer security relies entirely on **client-side validation** of
cheat commands, resource costs, and build limits. A modified client can bypass
every check documented here because the game logic dispatcher (`GameLogicDispatch.cpp`)
trusts the content of network messages without re-validating them on receipt.

This is an architectural problem, not a collection of isolated bugs. The finding
table below describes the specific vectors, but the root cause is a single pattern:
validation lives in the input-translation layer (`CommandXlat.cpp`) which is only
reached via the local UI, not via incoming network messages.

**This audit informs the anti-cheat design. No code is changed in P4-01.**

---

## Findings

---

### VULN-P4-001

**SEVERITY:** CRITICAL
**FILE:** `GeneralsMD/Code/GameEngine/Source/GameClient/MessageStream/PlaceEventTranslator.cpp`
**LINE:** ~269–285
**DESCRIPTION:** No resource cost validation before `MSG_DOZER_CONSTRUCT` is transmitted. `PlaceEventTranslator` calls `isLocationLegalToBuild()` (checks terrain, shroud, object overlap) but never checks whether the issuing player can afford the structure. The message is appended to the network stream unconditionally.
**EXPLOIT:** A modified client sends `MSG_DOZER_CONSTRUCT` for any structure the player cannot afford. Because the game logic dispatcher also performs no cost check before calling `TheBuildAssistant->buildObjectNow()`, all clients build the structure for the cheat player without deducting cost. Result: unlimited free buildings.
**FIX:** Add an affordability check in `PlaceEventTranslator` before queuing the message, AND add a server-side (dispatcher) re-validation of cost when processing `MSG_DOZER_CONSTRUCT`.
**BREAKS_PROTOCOL:** no

---

### VULN-P4-002

**SEVERITY:** CRITICAL
**FILE:** `GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogicDispatch.cpp`
**LINE:** ~1387–1435
**DESCRIPTION:** No deduplication or rate-limiting on `MSG_DOZER_CONSTRUCT`. The dispatcher calls `buildObjectNow()` for every message received with no frame-based minimum interval, no message-ID tracking, and no per-slot build queue depth check.
**EXPLOIT:** A modified client sends two or more identical `MSG_DOZER_CONSTRUCT` messages within the same processing window. Combined with VULN-P4-001 (no cost check), a player can construct an unlimited number of structures. Even without VULN-P4-001, rapid-fire identical commands could build multiple copies when only one was intended.
**FIX:** Track last-build-frame per dozer unit; reject a second build command for the same unit within N ticks. Alternatively enforce a server-side build queue that prevents duplicate insertions.
**BREAKS_PROTOCOL:** no

---

### VULN-P4-003

**SEVERITY:** CRITICAL
**FILE:** `GeneralsMD/Code/GameEngine/Source/Common/Recorder.cpp`
**LINE:** 553 (magic), 561–596 (header), 724+ (command stream)
**DESCRIPTION:** Replay files contain no hash or signature over the command stream. The header stores `exeCRC` and `iniCRC` (lines 595–596) for version-matching only. The command stream (frames, player indices, command types, arguments) is written as raw binary with no integrity protection. There is no footer checksum.
**EXPLOIT:** Download a community replay, edit it in a hex editor to insert cheat commands (e.g. fake `MSG_CHEAT_ADD_CASH` frames), remove opponent moves, or alter outcome data, then re-share as authentic evidence. The parser (`readReplayHeader`) only validates the `GENREP` magic and CRCs — it does not detect any command-stream tampering.
**FIX:** Compute a SHA-256 (or FNV-1a for speed) over the entire command stream and store it in the replay footer. Verify the hash when loading for playback. For full tamper-evidence, store a Merkle tree over 1000-tick chunks.
**BREAKS_PROTOCOL:** yes — replay format version bump required

---

### VULN-P4-004

**SEVERITY:** CRITICAL
**FILE:** `GeneralsMD/Code/GameEngine/Source/GameClient/MessageStream/CommandXlat.cpp` (guard present), `GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogicDispatch.cpp` (guard absent)
**LINE:** CommandXlat ~3395–3449 (guard); GameLogicDispatch — not found
**DESCRIPTION:** All `MSG_CHEAT_*` messages have their multiplayer guard only in the client-side translation layer (`CommandXlat.cpp`, which is invoked by the local keyboard/UI path). When the same message type arrives via the network, it bypasses `CommandXlat` entirely and is processed by `GameLogicDispatch.cpp` without any guard.
**Messages affected:** `MSG_CHEAT_ADD_CASH`, `MSG_CHEAT_GIVE_ALL_SCIENCES`, `MSG_CHEAT_GIVE_SCIENCEPURCHASEPOINTS`, `MSG_CHEAT_INSTANT_BUILD`, `MSG_CHEAT_DESHROUD`, `MSG_CHEAT_SWITCH_TEAMS`, `MSG_CHEAT_KILL_SELECTION`, `MSG_CHEAT_TOGGLE_HAND_OF_GOD_MODE`, `MSG_CHEAT_SHOW_HEALTH`
**EXPLOIT:** A modified client sends `MSG_CHEAT_ADD_CASH` directly onto the network. All clients process it in `GameLogicDispatch`, adding cash to the cheat player. The `!isInMultiplayerGame()` check in `CommandXlat` is never reached.
**FIX:** Add multiplayer guards in `GameLogicDispatch.cpp` for every `MSG_CHEAT_*` message type. These messages should be silently dropped (`DESTROY_MESSAGE`) in any multiplayer session regardless of origin.
**BREAKS_PROTOCOL:** no

---

### VULN-P4-005

**SEVERITY:** HIGH
**FILE:** `GeneralsMD/Code/GameEngine/Source/GameClient/MessageStream/CommandXlat.cpp`
**LINE:** ~3300–3322
**DESCRIPTION:** `MSG_CHEAT_RUNSCRIPT1`–`MSG_CHEAT_RUNSCRIPT9` invoke `TheScriptEngine->runScript()` — an arbitrary script execution path. The only guard (`!isInMultiplayerGame()`) is in `CommandXlat`, which is never reached for network-originated messages. The `GameLogicDispatch` handler for these messages has no multiplayer guard.
**EXPLOIT:** A modified client sends `MSG_CHEAT_RUNSCRIPT1`–`9` in a multiplayer game. The script is executed on all clients with the target name `KEY_F1`–`KEY_F9`. These scripts can trigger arbitrary in-game events (spawn units, end game, etc.) depending on map script content.
**FIX:** Same as VULN-P4-004: add a multiplayer guard drop in `GameLogicDispatch` for all `MSG_CHEAT_RUNSCRIPT*` message types.
**BREAKS_PROTOCOL:** no

---

### VULN-P4-006

**SEVERITY:** HIGH
**FILE:** `GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogicDispatch.cpp`
**LINE:** ~804–815 (move), ~1207–1224 (attack)
**DESCRIPTION:** Move and attack commands operate on a pre-filtered selection group. The player ownership check (`removeAnyObjectsNotOwnedByPlayer`) is applied to the selection group at dispatch time (line ~358), which is correct. However, commands that reference objects by ID directly (not via selection group) — such as certain direct-attack variants — do not re-verify unit ownership before executing.
**EXPLOIT:** A modified client sends an attack or move message referencing an opponent's unit by its ObjectID. If the command path uses the ID directly rather than the group-filtered path, it may issue movement or attack orders to units controlled by another player.
**FIX:** Add an explicit `getControllingPlayer() == thisPlayer` check in every direct-ID command handler in GameLogicDispatch (analogous to the existing check at line ~1447 for `MSG_CANCEL_BUILD_FROM_QUEUE`).
**BREAKS_PROTOCOL:** no

---

### VULN-P4-007

**SEVERITY:** MEDIUM
**FILE:** `GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogicDispatch.cpp`
**LINE:** ~1387 (build dispatch)
**DESCRIPTION:** No rate limiting on build commands per dozer unit per frame. While VULN-P4-002 covers the duplicate-build case, there is also no throttle on the total number of build commands any single player can issue per second. An adversarial client sending thousands of commands per frame could cause severe performance degradation or memory exhaustion across all clients.
**EXPLOIT:** Send a flood of `MSG_DOZER_CONSTRUCT` or `MSG_DO_ATTACK_OBJECT` messages. All clients process every message synchronously in the command list, causing frame stalls.
**FIX:** Add a per-player per-tick command count cap. If a player exceeds N commands in one tick (e.g. N=100), discard the excess and log the event. This is also a DoS mitigation.
**BREAKS_PROTOCOL:** no

---

### VULN-P4-008

**SEVERITY:** MEDIUM
**FILE:** `GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogicDispatch.cpp`
**LINE:** ~358 (player lookup)
**DESCRIPTION:** The player index in a received network message is taken at face value: `ThePlayerList->getNthPlayer(msg->getPlayerIndex())`. No cross-check confirms that `msg->getPlayerIndex()` matches the network slot the message actually arrived from. The P2-05 fix (VULN-003 in SECURITY_AUDIT_P2.md) validated `dataLength` but did not add source-slot binding.
**EXPLOIT:** A modified client sends a command message with `playerIndex` set to a different player's slot. The dispatcher executes the command as if it came from that player — issuing orders in their name, deducting their resources, or triggering their unit abilities.
**FIX:** In the network receive path, bind each received command message to the network slot it arrived from. In `GameLogicDispatch`, verify `msg->getPlayerIndex() == expectedSlotForSender` before processing. Requires the network layer to annotate messages with their source slot.
**BREAKS_PROTOCOL:** yes — requires protocol change to attach source-slot metadata

---

## Architectural Root Cause

All CRITICAL and HIGH findings share the same root cause:

> **Validation is split between two layers that don't share a trust boundary.**
> `CommandXlat.cpp` (UI layer) validates local user input correctly.
> `GameLogicDispatch.cpp` (simulation layer) processes both local and remote commands but validates almost nothing about remote command content.

The correct fix is to make `GameLogicDispatch` the single validation point for all
commands, regardless of origin. `CommandXlat` validation can remain as a UX
convenience (fast reject before queueing) but must not be the only line of defense.

---

## Replay Tamper-Evidence

Zero Hour replays currently provide only **version attestation** (exeCRC, iniCRC),
not command integrity. A fully tamper-evident replay would require:

1. A per-command HMAC or a Merkle tree over the command stream
2. The hash/root stored in a signed footer
3. Verification on replay load and on upload to leaderboard/tournament systems

Until VULN-P4-003 is fixed, replays should not be used as authoritative evidence
in competitive play.

---

## Summary Table

| ID | Severity | Description | File | Line |
|----|----------|-------------|------|------|
| VULN-P4-001 | CRITICAL | No resource cost check before MSG_DOZER_CONSTRUCT | PlaceEventTranslator.cpp | ~269 |
| VULN-P4-002 | CRITICAL | No build command deduplication or rate limit | GameLogicDispatch.cpp | ~1387 |
| VULN-P4-003 | CRITICAL | Replay file has no command-stream integrity hash | Recorder.cpp | 553+ |
| VULN-P4-004 | CRITICAL | MSG_CHEAT_* guards only in client layer, not dispatcher | CommandXlat.cpp / GameLogicDispatch.cpp | ~3395 |
| VULN-P4-005 | HIGH | MSG_CHEAT_RUNSCRIPT guard only in client layer | CommandXlat.cpp | ~3310 |
| VULN-P4-006 | HIGH | Move/attack direct-ID path skips ownership re-check | GameLogicDispatch.cpp | ~804, ~1207 |
| VULN-P4-007 | MEDIUM | No per-player per-tick command count cap (DoS vector) | GameLogicDispatch.cpp | ~1387 |
| VULN-P4-008 | MEDIUM | playerIndex not validated against network source slot | GameLogicDispatch.cpp | ~358 |

**Total:** 4 CRITICAL, 2 HIGH, 2 MEDIUM

---

## Recommendations for P4+ Work

| Priority | Finding | Recommended Fix |
|----------|---------|-----------------|
| CRITICAL | VULN-P4-004 / P4-005 | Drop all MSG_CHEAT_* in GameLogicDispatch during multiplayer |
| CRITICAL | VULN-P4-001 | Re-validate afford check in GameLogicDispatch for build commands |
| CRITICAL | VULN-P4-002 | Per-dozer last-build-frame check in dispatcher |
| CRITICAL | VULN-P4-003 | SHA-256 command-stream footer in replay format (protocol change) |
| HIGH | VULN-P4-006 | Add ownership re-check in all direct-ID command handlers |
| HIGH | VULN-P4-008 | Source-slot binding in network receive → dispatch path (protocol change) |
| MEDIUM | VULN-P4-007 | Per-player per-tick command count cap (e.g. 100) |

*Audit date: 2026-03-23. Human review required before any fix is implemented.*
