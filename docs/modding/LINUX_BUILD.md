# Linux Build Guide — zh-revival (P5-05)

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

## Prerequisites

| Tool | Minimum version | Install |
|------|----------------|---------|
| GCC / Clang | GCC 11 or Clang 14 | `apt install gcc g++` |
| CMake | 3.25 | `apt install cmake` |
| Ninja | any | `apt install ninja-build` |
| vcpkg | latest | see below |
| SDL3 dev headers | 3.x | via vcpkg or `apt install libsdl3-dev` |
| pkg-config | any | `apt install pkg-config` |

**Optional (for FFmpeg video, P5-02):**
```
apt install libavcodec-dev libavformat-dev libswscale-dev libavutil-dev
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

# 2. Bootstrap vcpkg
git clone --depth=1 https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh -disableMetrics

# 3. Install dependencies
./vcpkg/vcpkg install --triplet x64-linux

# 4. Configure (SDL3 window backend enabled)
cmake -B build -S . \
  -G Ninja \
  --toolchain ./vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DVCPKG_TARGET_TRIPLET=x64-linux \
  -DZH_USE_STUBS=ON \
  -DZH_SDL3_WINDOW=ON \
  -DCMAKE_BUILD_TYPE=Release

# 5. Build
cmake --build build --parallel

# 6. Run (requires game assets in working directory)
./GeneralsMD/Run/zh-revival
```

---

## CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `ZH_USE_STUBS` | `ON` | Use no-op stubs for Miles/Bink/SafeDisc/GameSpy |
| `ZH_SDL3_WINDOW` | `OFF` | **Must be ON for Linux** — SDL3 window and event pump |
| `ZH_OPENAL_AUDIO` | `OFF` | Real OpenAL audio via openal-soft (P5-01) |
| `ZH_FFMPEG_VIDEO` | `OFF` | Real Bink video decode via FFmpeg (P5-02) |

---

## Graphics

On Linux the game uses DXVK to translate Direct3D 8 calls to Vulkan:

1. Install Vulkan drivers for your GPU:
   ```bash
   # NVIDIA
   apt install nvidia-vulkan-icd
   # AMD / Intel
   apt install mesa-vulkan-drivers
   ```

2. Obtain DXVK d3d8 and d3d9 DLLs (x32 build) from the DXVK project.
   Place them in the same directory as the game binary.

3. Set `DXVK_HUD=version` to confirm DXVK is active on launch.

> **Note:** DXVK is not yet automatically copied by the build — this will be
> improved in a later release.  See [DXVK releases](https://github.com/doitsujin/dxvk).

---

## File Path Case Sensitivity

Linux filesystems are case-sensitive.  All asset paths in the engine use
Windows-style backslashes and mixed case (e.g. `Data\Textures\Terrain.tga`).

The engine normalises backslashes to forward slashes at runtime.  If you see
"file not found" errors for assets you know are present, check the case of
directory and file names in your game installation.

Known common mismatches to watch for:
- `Data/` vs `data/` — the engine always uses `Data/`
- `.INI` vs `.ini` extensions — game INI files use `.ini` lowercase
- Movie files: `Data/Movies/` must match the case in your installation

---

## Audio

With `ZH_OPENAL_AUDIO=ON`, the game uses openal-soft for audio output.
On Ubuntu 22.04 this usually works out of the box with PulseAudio or PipeWire.

If you get "no audio device" errors:
```bash
# Verify openal-soft can find an output device
apt install alsa-utils
aplay -l
```

---

## Known Limitations (P5-05)

The following are known issues with the Linux build that will be addressed
in future phases:

- **DXVK must be installed manually** — no automatic download or copy yet
- **Multiplayer** — GameSpy stubs are no-ops; online play requires the P3+
  replacement network layer
- **Cutscene video** — requires `ZH_FFMPEG_VIDEO=ON` and FFmpeg installed
- **Remaining Windows API usage** — some subsystems still reference
  `<windows.h>` types directly; full audit is P5-07

---

## Troubleshooting

**`libSDL3.so: cannot open shared object file`**
→ Install SDL3 dev package or ensure the vcpkg-built SDL3 library is on
  `LD_LIBRARY_PATH`.

**`vulkan: no suitable physical device`**
→ Install Vulkan drivers (see Graphics section above) and verify with
  `vulkaninfo`.

**`zh-revival: error while loading shared libraries: libavcodec.so`**
→ Only relevant with `ZH_FFMPEG_VIDEO=ON`.  Install FFmpeg dev libraries.

**Game crashes immediately on startup**
→ Verify game assets are present in the working directory (`Data/` folder
  and `.big` archive files from a legal game installation).
