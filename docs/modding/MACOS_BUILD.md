# macOS ARM64 Build Guide — zh-revival (P5-06)

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

## Prerequisites

| Tool | Minimum version | Install |
|------|----------------|---------|
| Xcode Command Line Tools | 14+ | `xcode-select --install` |
| Homebrew | any | see https://brew.sh |
| CMake | 3.25 | `brew install cmake` |
| Ninja | any | `brew install ninja` |
| SDL3 | 3.x | `brew install sdl3` |
| MoltenVK | any | `brew install molten-vk` |
| pkg-config | any | `brew install pkg-config` |
| vcpkg | latest | see below |

**Optional (for FFmpeg video, P5-02):**
```bash
brew install ffmpeg
```

**Runtime requirement:** You must own a legal installation of Command & Conquer
Generals — Zero Hour.  Copy the game's `Data/` directory and `.big` files to the
same directory as the built binary before launching.

---

## Quick Start

```bash
# 1. Clone
git clone https://github.com/zerwonenetwork/zh-revival.git
cd zh-revival

# 2. Install Homebrew dependencies
brew install cmake ninja sdl3 molten-vk pkg-config

# 3. Bootstrap vcpkg
git clone --depth=1 https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh -disableMetrics

# 4. Install dependencies
./vcpkg/vcpkg install --triplet arm64-osx

# 5. Configure (SDL3 window backend + MoltenVK)
cmake -B build -S . \
  -G Ninja \
  --toolchain ./vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DVCPKG_TARGET_TRIPLET=arm64-osx \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DZH_USE_STUBS=ON \
  -DZH_SDL3_WINDOW=ON \
  -DCMAKE_BUILD_TYPE=Release

# 6. Build
cmake --build build --parallel

# 7. Run (requires game assets in working directory)
./GeneralsMD/Run/zh-revival
```

---

## CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `ZH_USE_STUBS` | `ON` | Use no-op stubs for Miles/Bink/SafeDisc/GameSpy |
| `ZH_SDL3_WINDOW` | `OFF` | **Must be ON for macOS** — SDL3 window and event pump |
| `ZH_OPENAL_AUDIO` | `OFF` | Real OpenAL audio via openal-soft (P5-01) |
| `ZH_FFMPEG_VIDEO` | `OFF` | Real Bink video decode via FFmpeg (P5-02) |

---

## Graphics (MoltenVK / Vulkan → Metal)

The game renders via Direct3D 8. On macOS, DXVK translates D3D8 calls to Vulkan,
and MoltenVK translates Vulkan to Metal.

### Translation stack

```
Game (D3D8) → DXVK (d3d8.dll) → Vulkan API → MoltenVK → Metal → GPU
```

### Setup

1. Install MoltenVK:
   ```bash
   brew install molten-vk
   ```

2. Obtain DXVK macOS-compatible d3d8 and d3d9 libraries (x64 build) from the
   DXVK project or DXVK-macOS forks.
   Place them in the same directory as the game binary.

3. Set `MVK_CONFIG_USE_METAL_ARGUMENT_BUFFERS=1` if you hit descriptor limit errors.

4. Verify MoltenVK is active:
   ```bash
   VK_ICD_FILENAMES=$(brew --prefix molten-vk)/share/vulkan/icd.d/MoltenVK_icd.json \
     ./GeneralsMD/Run/zh-revival
   ```

> **Note:** DXVK is not yet automatically copied by the build — this will be
> improved in a later release.

---

## Audio

With `ZH_OPENAL_AUDIO=ON`, the game uses openal-soft for audio output.
openal-soft on macOS uses CoreAudio as the backend and works without additional setup.

Install via vcpkg:
```bash
./vcpkg/vcpkg install openal-soft:arm64-osx
```

Then reconfigure with `-DZH_OPENAL_AUDIO=ON`.

---

## File Path Case Sensitivity

macOS filesystems default to case-insensitive but case-preserving (HFS+/APFS).
This means asset path case issues that would break on Linux usually work on macOS.
However, if you use a case-sensitive APFS volume, see the Linux build guide for
known case mismatches to watch for.

---

## App Bundle and Entitlements

The current build produces a bare binary, not a `.app` bundle. Network entitlements
are not yet configured.  To run multiplayer (once the P3+ network layer is in place),
you will need to sign the binary with network entitlements or disable Gatekeeper for
the binary:

```bash
# Temporary — for local testing only
xattr -cr ./GeneralsMD/Run/zh-revival
```

A proper `.app` bundle with entitlements is planned for a future release.

---

## Known Limitations (P5-06)

- **DXVK must be installed manually** — no automatic download or copy yet
- **App bundle** — bare binary only; no `.app` bundle or code signing
- **Multiplayer** — GameSpy stubs are no-ops; online play requires the P3+
  replacement network layer
- **Cutscene video** — requires `ZH_FFMPEG_VIDEO=ON` and FFmpeg installed
- **Rosetta 2** — x86_64 binary via Rosetta 2 is not tested; use the native
  ARM64 build for best results

---

## Troubleshooting

**`dyld: Library not loaded: @rpath/libSDL3.dylib`**
→ SDL3 is not on the library path. Run:
```bash
export DYLD_LIBRARY_PATH=$(brew --prefix sdl3)/lib:$DYLD_LIBRARY_PATH
```
Or install SDL3 via vcpkg so it is bundled with the build.

**`vulkan: VK_ERROR_INCOMPATIBLE_DRIVER`**
→ MoltenVK ICD is not on the search path. Set:
```bash
export VK_ICD_FILENAMES=$(brew --prefix molten-vk)/share/vulkan/icd.d/MoltenVK_icd.json
```

**`zh-revival: code signature invalid`**
→ Gatekeeper blocked the unsigned binary. Run `xattr -cr ./GeneralsMD/Run/zh-revival`.

**Game crashes immediately on startup**
→ Verify game assets are present in the working directory (`Data/` folder
  and `.big` archive files from a legal game installation).
