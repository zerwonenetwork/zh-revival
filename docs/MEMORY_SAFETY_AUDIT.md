# Memory Safety Audit — zh-revival (P5-07)

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

Audit conducted: 2026-03-23
Scope: Engine-wide search for unsafe string/memory functions
Tool: ripgrep over all GameEngine / GameEngineDevice / GameRenderer / GameAudio /
      GameNetwork / GameClient / GameLogic / Main source trees
Phase 2 scope (network code) already addressed in P2-07.

---

## Summary

| Severity  | Count | Status |
|-----------|-------|--------|
| CRITICAL  | 4     | Fixed in this task |
| HIGH      | 7     | Fixed in this task |
| MEDIUM    | ~20   | Documented — future work |

---

## Raw counts

| Function   | Engine-wide occurrences | Network-only |
|------------|------------------------|--------------|
| `strcpy`   | ~80                    | ~35          |
| `strcat`   | ~40                    | ~5           |
| `sprintf`  | 239                    | 5            |
| `gets`     | 0                      | 0            |
| `sscanf %s`| 0                      | 0            |
| raw `malloc`| 4 (1 is the override)  | 0            |

---

## CRITICAL findings (fixed)

---

### MEM-C01 — WOLLoginMenu.cpp login nick/email/password overflow (x2 call sites)

**File:** `GeneralsMD/Code/GameEngine/Source/GameClient/GUI/GUICallbacks/Menus/WOLLoginMenu.cpp`
**Lines:** 1249–1251, 1338–1340
**Severity:** CRITICAL

**Description:** User-supplied text from UI text-entry widgets is strcpy'd directly
into fixed-size BuddyRequest struct fields with no length check:

```cpp
strcpy(req.arg.login.nick,     login.str());    // dest: char[31]
strcpy(req.arg.login.email,    email.str());    // dest: char[51]
strcpy(req.arg.login.password, password.str()); // dest: char[31]
```

The text-entry gadgets impose no character limit. An input longer than the
destination field overflows the stack-allocated BuddyRequest struct.

**Attack:** Local user; type a nick/password longer than 30 characters. Stack
smash overwrites adjacent struct fields and return address.

**Fix applied:** strncpy + explicit NUL-termination using the GP_* length constants.

**Status: FIXED**

---

### MEM-C02 — BuddyThread.cpp login field overflow (thread queue path)

**File:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/GameSpy/Thread/BuddyThread.cpp`
**Lines:** 566–568
**Severity:** CRITICAL

**Description:** `m_nick`, `m_email`, `m_pass` are `std::string` members set from
`incomingRequest.arg.login.*` (which are bounded) but are then strcpy'd to
`req.arg.login.*` struct fields of the same bounded size. If the std::string ever
exceeds the field size (e.g. via a future code path that sets it from an unchecked
source), this would overflow. Applied the same strncpy fix as MEM-C01 for
consistency and defence-in-depth.

**Status: FIXED**

---

### MEM-C03 — BuddyThread.cpp response nick/email/countrycode overflow (callback path)

**File:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/GameSpy/Thread/BuddyThread.cpp`
**Lines:** 512, 627–629, 654–656, 670–671
**Severity:** CRITICAL

**Description:** Multiple callback handlers copy fields from GameSpy `arg` pointers
(which may come from network data) into fixed-size response struct fields using
strcpy with no length check:

```cpp
strcpy(resp->arg.message.nick,        arg->nick);       // dest: char[GP_NICK_LEN=31]
strcpy(resp->arg.request.nick,        arg->nick);       // dest: char[31]
strcpy(resp->arg.request.email,       arg->email);      // dest: char[51]
strcpy(resp->arg.request.countrycode, arg->countrycode);// dest: char[3]
strcpy(resp->arg.status.nick,         arg->nick);       // dest: char[31]
strcpy(resp->arg.status.location,     status.locationString); // dest: char[256]
strcpy(resp->arg.status.statusString, status.statusString);   // dest: char[256]
```

The `countrycode` field is only 3 bytes — if the source is longer, as few as 1
extra character overflows.

**Fix applied:** strncpy + NUL-termination with GP_* constants.

**Status: FIXED**

---

### MEM-C04 — StackDump.cpp function_name strcat overflow in crash handler

**File:** `GeneralsMD/Code/GameEngine/Source/Common/System/StackDump.cpp`
**Lines:** 298–299
**Severity:** CRITICAL

**Description:** `function_name[512]` is filled via `strcpy(name, psymbol->Name)`
where `psymbol->MaxNameLength = 512`. A 512-byte symbol name completely fills the
buffer. Immediately after, `strcat(name, "();")` appends 3 characters — overflowing
by 3 bytes.

Although this is in the crash handler path (not normal execution), a symbolic name
could be crafted to trigger this in any debug/crash scenario.

**Fix applied:** Changed `function_name[512]` to `function_name[516]` (512 + 3 + NUL).

**Status: FIXED**

---

## HIGH findings (fixed)

---

### MEM-H01 — StackDump.cpp sprintf into MAX_PATH buffer with 512-byte inputs

**File:** `GeneralsMD/Code/GameEngine/Source/Common/System/StackDump.cpp`
**Line:** 473
**Severity:** HIGH

**Description:**
```cpp
static char line[MAX_PATH];       // 260 bytes
static char function_name[512];
static char filename[MAX_PATH];
sprintf(line, "  %s(%d) : %s 0x%08p", filename, linenumber, function_name, address);
```
`function_name` alone can be 512 bytes; `line` is only 260 bytes. Format string
output can exceed buffer.

**Fix applied:** `snprintf(line, sizeof(line), ...)` + truncate without crash.

**Status: FIXED**

---

### MEM-H02 — StackDump.cpp strcpy filename from debug info

**File:** `GeneralsMD/Code/GameEngine/Source/Common/System/StackDump.cpp`
**Line:** 316
**Severity:** HIGH

**Description:**
```cpp
static char filename[MAX_PATH];  // 260 bytes
strcpy(filename, line.FileName); // FileName from IMAGEHLP_LINE — unbounded
```
Debug symbol file paths can exceed MAX_PATH on deeply nested build trees.

**Fix applied:** `strncpy` + NUL-terminate at `MAX_PATH - 1`.

**Status: FIXED**

---

### MEM-H03 — FirewallHelper.cpp getManglerName: no size constraint

**File:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/FirewallHelper.cpp`
**Line:** 622
**Severity:** HIGH

**Description:**
```cpp
// Caller (NAT.cpp:812): Char manglerName[256];
void FirewallHelperClass::getManglerName(Int manglerIndex, Char *nameBuf)
{
    AsciiString host;
    TheGameSpyConfig->getManglerLocation(manglerIndex, host, port);
    strcpy(nameBuf, host.str());  // host AsciiString — no length check against 256
}
```
`host` is read from the GameSpy configuration, which may ultimately come from a
server response. An AsciiString can hold more than 256 characters.

**Fix applied:** Added `Int nameBufLen` parameter; changed to `strncpy` + NUL-terminate.
Caller updated to pass `sizeof(manglerName)`.

**Status: FIXED**

---

### MEM-H04 — FirewallHelper.cpp detectionBeginUpdate: temp_name overflow

**File:** `GeneralsMD/Code/GameEngine/Source/GameNetwork/FirewallHelper.cpp`
**Line:** 689
**Severity:** HIGH

**Description:**
```cpp
char temp_name[256];
strcpy(temp_name, mangler_name_ptr);  // from TheGameSpyConfig — no length bound
```
Same source as MEM-H03.

**Fix applied:** `strncpy` + NUL-terminate.

**Status: FIXED**

---

### MEM-H05 — GameLogic.cpp mapName to _MAX_PATH filename buffer

**File:** `GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogic.cpp`
**Lines:** 2408, 2417
**Severity:** HIGH

**Description:**
```cpp
char filename[_MAX_PATH];         // 260 bytes
strcpy(filename, mapName.str());  // mapName from network (map transfer path)
strcpy(filename, TheGameState->getSaveGameInfo()->pristineMapName.str());
```
`mapName` originates from the network map transfer path (validated in P2-03 to ≤50MB
file size, but not validated for path length). A crafted map filename > 259 characters
overflows the stack buffer.

**Fix applied:** `snprintf(filename, _MAX_PATH, "%s", mapName.str())`.

**Status: FIXED**

---

### MEM-H06 — GameLogic.cpp GameSpy status strcpy

**File:** `GeneralsMD/Code/GameEngine/Source/GameLogic/System/GameLogic.cpp`
**Line:** 2334
**Severity:** HIGH

**Description:**
```cpp
strcpy(req.arg.status.statusString, "Playing");  // dest: char[GP_STATUS_STRING_LEN=256]
```
The literal "Playing" is safe. However, this pattern (strcpy to a status field)
is used inconsistently — other call sites in GameSpy code copy runtime strings.
Fixed for consistency with the strncpy pattern; literal copy is safe but sets
the correct pattern for future modifications.

**Fix applied:** `strncpy` + NUL-terminate.

**Status: FIXED**

---

### MEM-H07 — GameSpeech.cpp _MAX_PATH buffer from INI dialog filenames

**File:** `GeneralsMD/Code/GameEngine/Source/Common/Audio/GameSpeech.cpp`
**Lines:** 1048–1068
**Severity:** HIGH

**Description:**
```cpp
char name[_MAX_PATH];
strcpy(name, speechInfo->m_dialogFiles[soundToPlay].str());
```
`m_dialogFiles` are AsciiStrings loaded from INI files. No validation ensures
these fit within `_MAX_PATH`. A modded INI with long audio file paths causes a
stack overflow.

**Fix applied:** `snprintf(name, _MAX_PATH, "%s", speechInfo->...)`.

**Status: FIXED**

---

## MEDIUM findings (documented — future work)

These are lower-risk uses of unsafe functions in paths not exposed to external
untrusted input, or where the buffer is demonstrably sized to match the source.

| ID | File | Lines | Function | Notes |
|----|------|-------|----------|-------|
| MEM-M01 | INI.cpp | 379, 392, 734–792 | strcpy/strcat | source and dest both `INI_MAX_CHARS_PER_LINE` (1028). Safe by design but should use snprintf. |
| MEM-M02 | AsciiString.cpp | 130,132,149,156,158 | strcpy/strcat | pre-sized: `ensureUniqueBufferOfSize(numCharsNeeded)` called first. Safe. |
| MEM-M03 | Debug.cpp | 201,357–362,674–677 | strcpy/strcat | crash handler and debug paths. buffers are large (_MAX_PATH×2) |
| MEM-M04 | GameMemory.cpp | 2976–2977,3200–3204 | strcpy/strcat | internal profiling output. buf[1024], all literal strings |
| MEM-M05 | PerfTimer.cpp | 373–377 | strcpy/strcat | .csv profiling paths. strcat(".csv") to known-length fname |
| MEM-M06 | String.cpp | 173,174,194,195,296 | strcpy/strcat | internal UnicodeString. buffer sized to m_data length |
| MEM-M07 | SaveGame.cpp | 857, 1621 | strcpy | buf is AsciiString buffer sized before use |
| MEM-M08 | GameWindowManagerScript.cpp | multiple | strcpy | local file paths from INI data |
| MEM-M09 | GameSpy threads (PeerThread/BuddyThread) | login struct literals | strcpy | "Offline"/"Online"/literal strings to 256-byte fields. safe. |
| MEM-M10 | GameLogic.cpp:4457,4466 | strcpy(&buf[1], ...) | commandSetName from internal engine state, not network |
| MEM-M11 | W3DFileSystem.cpp | 192–322 | strcat(m_filePath, filename) | m_filePath is 260 bytes. filename from local INI config |
| MEM-M12 | MemoryInit.cpp:775 | strcat(buf, "\\Data\\INI\\...") | buf sized, literal appended |
| MEM-M13 | 239 sprintf calls | engine-wide | to be addressed incrementally — low priority (no untrusted sources) |

---

## Methodology

1. `ripgrep` for `\bstrcpy\b`, `\bstrcat\b`, `\bsprintf\b`, `\bgets\b`, `\bsscanf\b.*%s\b`, `\bmalloc\b`
2. For each hit: traced data source (internal / INI / local config / network)
3. Classified by: destination buffer size, source max size, external controllability
4. CRITICAL = untrusted or semi-trusted input, no bounds check, concrete overflow path
5. HIGH = internal but demonstrably overflowable or in security-sensitive path
6. MEDIUM = bounded by matching constants or internal data only

---

## ASAN validation (planned)

Run with AddressSanitizer on Linux (P5-07 spec):
```bash
cmake -B build -S . -DZH_USE_STUBS=ON -DZH_SDL3_WINDOW=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer" \
  -DCMAKE_C_FLAGS="-fsanitize=address" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"
cmake --build build
./GeneralsMD/Run/zh-revival  # exercise startup + skirmish
```

Expected result after this fix pass: zero ASAN errors on all CRITICAL and HIGH paths.
MEDIUM items may produce ASAN warnings on specific code paths until future cleanup.
