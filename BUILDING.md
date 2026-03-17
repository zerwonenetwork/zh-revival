# BUILDING — zh-revival

This repo is a modern CMake build of **Zero Hour (GeneralsMD)**.

## Requirements

- Windows 10/11
- Visual Studio 2022 (Desktop development with C++)
- CMake 3.25+
- Git

Optional:

- [vcpkg](https://github.com/microsoft/vcpkg) (recommended; used in CI)
- Legacy DirectX SDK libs (only needed if you want to link real D3D8/DInput/DSound libs)

## Quick build (Windows, MSVC)

From a “Developer PowerShell for VS 2022”:

```powershell
cmake -B build -S . -G "Visual Studio 17 2022" -A Win32 -DZH_USE_STUBS=ON
cmake --build build --config Release --parallel
```

Output:

- `GeneralsMD/Run/zh-revival.exe`

## Build with vcpkg (recommended)

This repo includes a `vcpkg.json` manifest (currently `zlib`).

```powershell
git clone https://github.com/microsoft/vcpkg.git .\vcpkg
.\vcpkg\bootstrap-vcpkg.bat -disableMetrics

cmake -B build -S . -G "Visual Studio 17 2022" -A Win32 `
  --toolchain ".\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x86-windows `
  -DZH_USE_STUBS=ON

cmake --build build --config Release --parallel
```

## Stub mode (default)

`ZH_USE_STUBS` is **ON by default**.

This enables building without proprietary SDKs:

- Miles Sound System (MSS)
- Bink
- SafeDisc
- GameSpy

If you have licensed SDKs and want to use them:

```powershell
cmake -B build -S . -DZH_USE_STUBS=OFF
```

## DirectX SDK libraries (optional)

If you want to link legacy DirectX libs (d3d8/d3dx8/dinput8/dxguid/dsound), pass:

```powershell
cmake -B build -S . -DDXSDK_LIB_DIR="C:\\Path\\To\\DXSDK\\Lib\\x86"
```

If `DXSDK_LIB_DIR` is not set, the build will warn and skip linking those libs.

## Troubleshooting

- **CMake can’t find a 32-bit toolchain**: ensure you pass `-A Win32` for Visual Studio generators.
- **Missing proprietary headers**: keep `-DZH_USE_STUBS=ON`.
- **“Serious Error” / DX init failures at runtime**: see `PROGRESS.md` notes for P1-08 (startup D3D8 fallback + logging).

