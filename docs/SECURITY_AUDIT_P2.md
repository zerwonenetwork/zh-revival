# Security Audit — Phase 2 (Network Code)

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

**Task:** P2-04
**Date:** 2026-03-22
**Audited path:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/`
**Status:** Findings documented — fixes applied in P2-05 (CRITICAL) and P2-06 (HIGH)

---

## Summary

| Severity | Count | Fixed |
|----------|-------|-------|
| CRITICAL | 5 | P2-05 |
| HIGH | 7 | P2-06 |
| MEDIUM | 0 | — |

Primary root causes:
1. No bounds checking before reading lengths from network packets
2. Fixed-size stack/heap buffers filled from untrusted network data without length limits
3. Integer overflow in accumulated counters from network data
4. Missing null-pointer checks on external API results

---

## CRITICAL Vulnerabilities

### VULN-001
- **SEVERITY:** CRITICAL
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetPacket.cpp`
- **LINE:** ~5705–5710
- **DESCRIPTION:** Unbounded filename copy in `readFileMessage()`. A `char filename[_MAX_PATH]` stack buffer is filled byte-by-byte from network packet data with no bounds check. If no null terminator is present in the packet, the copy walks off the end of the stack buffer.
- **EXPLOIT:** Attacker sends a FILE message where the filename field has no null terminator and length > `_MAX_PATH`. Overwrites stack return address → arbitrary code execution.
- **FIX:** Add `(c - filename) < (_MAX_PATH - 1)` guard in the copy loop.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-05

### VULN-002
- **SEVERITY:** CRITICAL
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetPacket.cpp`
- **LINE:** ~5732–5737
- **DESCRIPTION:** Identical unbounded filename copy in `readFileAnnounceMessage()`. Same pattern as VULN-001.
- **EXPLOIT:** Same as VULN-001 via FILEANNOUNCE message.
- **FIX:** Same bounds guard in copy loop.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-05

### VULN-003
- **SEVERITY:** CRITICAL
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetPacket.cpp`
- **LINE:** ~5714–5720
- **DESCRIPTION:** Unchecked `dataLength` field in `readFileMessage()`. A 4-byte length is read from the packet and passed directly to `NEW UnsignedByte[dataLength]` and then `memcpy`. A 0xFFFFFFFF value causes heap overflow.
- **EXPLOIT:** Attacker sends FILE message with `dataLength = 0xFFFFFFFF`. Allocation fails or allocates less than requested; subsequent memcpy corrupts the heap.
- **FIX:** Reject `dataLength > 50MB` before allocation.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-05

### VULN-004
- **SEVERITY:** CRITICAL
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetPacket.cpp`
- **LINE:** ~5567, ~5591
- **DESCRIPTION:** Fixed-size `UnsignedShort text[256]` buffer overflow in `readChatMessage()` and `readDisconnectChatMessage()`. A 1-byte `length` field controls `memcpy` and then `text[length] = 0`. If `length == 256`, the null write is one element past the array end.
- **EXPLOIT:** Attacker sends CHAT message with `length = 256`. Off-by-one write corrupts adjacent stack data.
- **FIX:** Reject `length >= 256` before the memcpy.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-05

### VULN-005
- **SEVERITY:** CRITICAL
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetPacket.cpp`
- **LINE:** ~55–148
- **DESCRIPTION:** Missing per-read bounds check in `ConstructNetCommandMsgFromRawData()`. Each branch reads a fixed-size type from `data + offset` without first verifying `offset + sizeof(type) <= dataLength`. A packet truncated just before a field causes an out-of-bounds read.
- **EXPLOIT:** Attacker sends a packet ending with marker byte `'C'` and no following bytes. Read of 2-byte `commandID` reads 1 byte past the buffer.
- **FIX:** Add `if (offset + sizeof(X) > dataLength) break;` before every `memcpy` in the function.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-05

---

## HIGH Vulnerabilities

### VULN-006
- **SEVERITY:** HIGH
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetPacket.cpp`
- **LINE:** ~5222–5233
- **DESCRIPTION:** Integer overflow in `readGameMessage()`. `totalArgCount` accumulates `argCount` values (1 byte each) across loop iterations with no overflow check. A large sum causes a subsequent loop to iterate billions of times.
- **EXPLOIT:** Attacker sends GAMECOMMAND with `numArgTypes=2`, each `argCount=200`. `totalArgCount=400` is benign but with crafted values the loop overruns the data buffer.
- **FIX:** Cap `totalArgCount <= 10000` and validate `i` stays within `dataLength` each iteration.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-06

### VULN-007
- **SEVERITY:** HIGH
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetCommandMsg.cpp`
- **LINE:** ~856, ~943
- **DESCRIPTION:** Unchecked `dataLength` in `NetGameCommandMsg` constructors. `memcpy(m_data, data, dataLength)` with no check that `dataLength <= sizeof(m_data)`.
- **EXPLOIT:** Attacker sends GAMECOMMAND with `dataLength` exceeding the pre-allocated buffer → heap overflow.
- **FIX:** Add `if (dataLength > MAX_DATA_SIZE) return;` guard.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-06

### VULN-008
- **SEVERITY:** HIGH
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetMessageStream.cpp`
- **LINE:** ~207
- **DESCRIPTION:** Unchecked `sizeofMessageArg` in packet assembly loop. Value derived from message type table but used in `memcpy` without verifying remaining buffer space.
- **EXPLOIT:** Crafted message causes memcpy to write past `commandPacket->m_commands` buffer.
- **FIX:** Check `bytesUsed + sizeofMessageArg <= sizeof(commandPacket->m_commands)` before memcpy.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-06

### VULN-009
- **SEVERITY:** HIGH
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/GameSpy/StagingRoomGameInfo.cpp`
- **LINE:** ~192
- **DESCRIPTION:** Unchecked `h_addr_list` pointer from `gethostbyname()`. If `host_info` is non-null but `h_addr_list[0]` is null, the memcpy dereferences a null pointer.
- **EXPLOIT:** Crafted DNS response or network error returns a malformed `hostent` struct → null pointer dereference → crash.
- **FIX:** Check `host_info && host_info->h_addr_list && host_info->h_addr_list[0]` before memcpy.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-06

### VULN-010
- **SEVERITY:** HIGH
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetCommandWrapperList.cpp`
- **LINE:** ~111
- **DESCRIPTION:** Unchecked `msg->getDataLength()` in `addMessage()`. `memcpy(m_data + offset, msg->getData(), msg->getDataLength())` with no check that the offset + length fits in `m_data`.
- **EXPLOIT:** Attacker sends wrapper message with a large dataLength → heap overflow.
- **FIX:** Validate `offset + dataLength <= m_dataSize` before memcpy.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-06

### VULN-011
- **SEVERITY:** HIGH
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/GameSpy/Thread/PersistentStorageThread.cpp`
- **LINE:** ~1338
- **DESCRIPTION:** Unbounded `sprintf` building a key-value stats string with user-influenced indices.
- **EXPLOIT:** Crafted stats data causing the formatted output to overflow `kvbuf`.
- **FIX:** Replace `sprintf` with `snprintf(kvbuf, sizeof(kvbuf), ...)`.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-06

### VULN-012
- **SEVERITY:** HIGH
- **FILE:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/NetCommandMsg.cpp`
- **LINE:** ~856
- **DESCRIPTION:** Missing null check after `NEW` allocation. If allocation fails, subsequent use of the pointer is undefined behavior.
- **EXPLOIT:** Memory exhaustion causes null dereference.
- **FIX:** Add `if (!buf) return;` after every `NEW` in packet parsing paths.
- **BREAKS_PROTOCOL:** No
- **STATUS:** Fixed in P2-06
