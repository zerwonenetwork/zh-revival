# Contributing to zh-revival

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

---

## Non-Affiliation Statement

By contributing, you acknowledge that this project is a community effort
not affiliated with or endorsed by Electronic Arts Inc. in any form.
Zero Hour and Command & Conquer are trademarks of Electronic Arts.
This project operates under the GPLv3 licence (plus EA's additional terms — see
the root `LICENSE` file). All contributions must comply with both.

---

## Getting Started

### 1. Fork and clone

```bash
# Fork via GitHub, then:
git clone https://github.com/<your-username>/zh-revival.git
cd zh-revival
git remote add upstream https://github.com/zerwonenetwork/zh-revival.git
```

### 2. Build the project

See [`BUILDING.md`](../BUILDING.md) for the full build walkthrough.
Short version:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### 3. Create a branch

```
git checkout -b task/<TASK-ID>-short-description
```

Examples:
- `task/P3-01-borderless-fullscreen`
- `fix/desync-log-flush`
- `docs/contributing-guide`

---

## Commit Messages

Format: `type(scope): description`

| type | when to use |
|------|------------|
| `feat` | new feature |
| `fix` | bug fix |
| `docs` | documentation only |
| `refactor` | code restructuring, no behaviour change |
| `test` | test additions or fixes |
| `chore` | build, CI, or tooling changes |

Scope is the subsystem or task ID (e.g. `GameNetwork`, `P2-05`, `replay`).

```
fix(GameNetwork): bounds-check filename copy in readFileMessage

Prevents OOB write when a malicious peer sends a filename longer
than _MAX_PATH. Fixes VULN-001 from docs/SECURITY_AUDIT_P2.md.
```

For AI-authored commits, append:
```
Co-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>
```

---

## Pull Request Requirements

1. **CI must pass** — both Windows MSVC and Linux GCC jobs green.
2. **One reviewer required** — assign a maintainer.
3. **Describe what changed** — use the PR template:
   - What problem does this solve?
   - How was it tested?
   - Determinism impact (none / low / review needed).
4. **No game assets** — `.big`, `.ini` data files, textures, audio must not be
   included. The project requires an existing legal game installation.
5. **No gameplay changes** — unit stats, damage values, AI behaviour, and
   balance changes are out of scope. See CLAUDE.md for the full rule list.

---

## Code Style

The existing codebase uses these conventions (match what you see):

- **Indentation:** Tabs (1 tab = 4 spaces displayed).
- **Braces:** Opening brace on the same line for control flow:
  ```cpp
  if (condition) {
      doSomething();
  }
  ```
- **Naming:** `camelCase` for variables/methods, `PascalCase` for classes,
  `UPPER_SNAKE_CASE` for constants and macros.
- **Hungarian prefixes:** `m_` for member variables, `g_` for globals,
  `k_` for constants (some files — match the local file style).
- **Comments:** Document the *why*, not the *what*. Prefer `//` for inline.
- **`NULL` vs `nullptr`:** The codebase uses `NULL`. Use `NULL` unless the
  file already uses `nullptr`.
- **No `std::unordered_map` / `std::unordered_set` in simulation code** —
  breaks determinism. Use `std::map` or sorted containers instead.
- **No new singletons without discussion** — the global singleton list is
  already large.

---

## Determinism Rules

Any change that touches simulation code (everything under `GameLogic/`) must:

1. Note the determinism impact in the PR description.
2. Not use non-deterministic operations:
   - No wall-clock time (`GetTickCount()`, `time()`, `rand()`)
   - No pointer addresses
   - No `std::unordered_*` containers
   - No floating-point operations that depend on FPU precision flags
3. Pass the CRC verification step in the CI headless replay smoke test
   (P2-09 infrastructure; full test pending P5-03).

---

## Reporting Bugs

Use the GitHub issue tracker. Include:

- **Game version** (zh-revival build hash or tag)
- **Windows version** and GPU
- **Steps to reproduce** (exact steps, ideally with a replay file)
- **Expected vs actual behaviour**
- **Logs** — attach `Logs/` files if relevant (desync logs, disconnect logs)
- **Is this reproducible?** (always / sometimes / once)

A bug report template is in `.github/ISSUE_TEMPLATE/bug_report.md`.

---

## What We Will Not Accept

- Game asset files (`.big`, textures, audio)
- Proprietary SDK code (Miles, Bink, SafeDisc, GameSpy) — use the stubs
- Gameplay balance changes
- EA trademarks in branding, binary names, or public documentation
- Code that re-enables online multiplayer against EA's original servers
  (GameSpy is shut down; only self-hosted or LAN is in scope)

---

## Licence

All contributions are licensed under **GPLv3** (the same licence as the
EA source release) plus EA's additional terms. By opening a pull request,
you confirm that your contribution may be distributed under these terms.

Do not include code from proprietary sources, other GPL-incompatible
projects, or code you do not have the right to contribute.
