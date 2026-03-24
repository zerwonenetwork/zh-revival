cmake_minimum_required(VERSION 3.25)
project(zh-revival VERSION 0.1.0 LANGUAGES C CXX)
# RC (Windows resource compiler) is Windows-only — enable after project() so
# non-Windows toolchains (macOS/Linux) don't fail looking for windres/rc.
if(WIN32)
  enable_language(RC)
endif()

# ─────────────────────────────────────────────
#  Non-Windows compatibility shim
#  On Linux/macOS (not cross-compiling to Windows via mingw which has its
#  own windows.h), inject our compat header:
#   1. include_directories(BEFORE) — so #include <windows.h> etc. resolve here
#   2. add_compile_options(-include) — force-inject before EVERY .c/.cpp unit
#      so __cdecl, __forceinline etc. are defined even in files that never
#      explicitly include windows.h
# ─────────────────────────────────────────────
if(UNIX AND NOT CMAKE_CROSSCOMPILING)
  set(ZH_COMPAT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/GeneralsMD/Code/Include/compat")
  include_directories(BEFORE "${ZH_COMPAT_INCLUDE_DIR}")
  add_compile_options(
    "-include" "${ZH_COMPAT_INCLUDE_DIR}/windows.h"
  )
endif()

# ─────────────────────────────────────────────
#  C++ standard
# ─────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────────
#  Platform / compiler settings
# ─────────────────────────────────────────────
# Windows 10 minimum version constants — used by compat headers and Windows SDK headers
# to enable newer APIs.  Always set these so compat/windows.h type defs are consistent.
add_compile_definitions(
    _WIN32_WINNT=0x0A00
    WINVER=0x0A00
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
)
# WIN32 / _WINDOWS — Windows platform identifier macros.
# On actual Windows builds (MSVC / MinGW cross-compile) the compiler defines these
# automatically.  On native Linux/macOS builds they must NOT be defined: legacy code
# uses them to select Windows-only code paths (e.g. udp.h includes <io.h> when
# _WINDOWS is defined, but <io.h> does not exist on POSIX).
if(WIN32 OR CMAKE_CROSSCOMPILING)
    add_compile_definitions(WIN32 _WINDOWS)
else()
    # On native Linux/macOS: define _UNIX so that Westwood legacy code takes the
    # POSIX/Unix branch in #ifdef _UNIX guards throughout WWLib (data.cpp, etc.).
    add_compile_definitions(_UNIX)
endif()
# _MBCS is MSVC-only: enables tchar.h multi-byte character functions (_mbslen, _mbscmp).
# On MinGW cross-compilation _mbslen/etc. are not available as import stubs, so we leave
# _MBCS undefined and let tchar.h map _tcsclen→strlen, _tcscmp→strcmp (ANSI mode).
if(MSVC)
    add_compile_definitions(_MBCS)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # -fpermissive: downgrade function-ptr→void* implicit conversion errors to warnings.
    # This is required for legacy MSVC-extension code (FunctionLexicon, etc.) until
    # those sites are updated in P5-07 (full memory safety audit).
    add_compile_options(-fpermissive)
    # -Wno-narrowing: legacy trig/lookup tables use large unsigned hex constants in signed
    # int arrays (e.g. 0xFFFFFFFA = -6 in fixed-point). These are intentional bit patterns.
    # The actual type correctness will be addressed in P5-07.
    add_compile_options(-Wno-narrowing)
    # Silence pointer↔int cast warnings from 25-year-old HKEY/handle code.
    # The game stores HKEY in int fields; safe on 32-bit, benign stub on 64-bit.
    add_compile_options(-Wno-pointer-to-int-cast -Wno-int-to-pointer-cast)
    # _USE_MATH_DEFINES: enable M_PI, M_PI_2 etc. from <math.h> on MinGW.
    add_compile_definitions(_USE_MATH_DEFINES)
    # --allow-multiple-definition: several legacy source pairs define the same symbol
    # (mutex.cpp/critsection.cpp both define CriticalSectionClass; WinMain.cpp/trim.cpp
    # both define strtrim). MSVC silently picks the first definition; replicate that here.
    # These duplicates will be cleaned up in P5-07.
    add_link_options(-Wl,--allow-multiple-definition)
    # C++17 removed the 'register' storage-class specifier. 25-year-old SAGE code
    # uses it in a few files. Redefine to empty so GCC silently drops it.
    add_compile_definitions(register=)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    # Clang-specific: suppress warnings from 25-year-old pointer/handle code that
    # stores HKEY (void*) in int fields.  Benign on stub builds; fix tracked in P5-07.
    add_compile_options(
        -Wno-pointer-to-int-cast    # GCC-style pointer→int cast warning name
        -Wno-int-to-pointer-cast    # GCC-style int→pointer cast warning name
        -Wno-shorten-64-to-32       # Clang/AppleClang: 64-bit→32-bit truncation
        -Wno-non-pod-varargs        # StringClass passed through variadic (wwprofile.cpp)
        -Wno-deprecated-declarations
        -Wno-int-conversion         # implicit integer conversions
    )
    # C++17 removed 'register' storage-class specifier. Redefine to empty on Clang.
    add_compile_definitions(register=)
endif()

if(MSVC)
    # /W3   — warning level 3 (matches original .dsp)
    # /MP   — parallel compilation
    # /EHsc — C++ exceptions (replaces legacy /GX)
    # /Zi   — full PDB for all configs
    add_compile_options(/W3 /MP /EHsc /Zi)

    # Suppress warnings that are noise from 25-year-old C++ at /W3 on modern MSVC.
    # These will be revisited in P5-07 (full memory safety audit).
    add_compile_options(
        /wd4996   # deprecated POSIX names (strcpy etc) — fixed in P2-07
        /wd4244   # implicit int->float narrowing in legacy math
        /wd4267   # size_t->int narrowing (32-bit codegen assumption)
        /wd4305   # double->float truncation in constant expressions
        /wd4018   # signed/unsigned mismatch in comparisons
        /wd4311   # P5-04: pointer truncation in 64-bit — remaining sites audited in P5-07
        /wd4312   # P5-04: conversion from int to pointer of greater size — same
    )

    # P5-04: Win64 build.
    # To build 64-bit: cmake -B build64 -S . -A x64
    # The only source-level change required is StackDump.cpp (DWORD→DWORD_PTR) and
    # version.cpp (multiplayer channel bit).  All other int/pointer truncations in
    # Tools/ and Libraries/ are suppressed by /wd4311 /wd4312 above and scheduled
    # for full cleanup in P5-07 (full memory safety audit).
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(STATUS "[zh-revival] Win64 build (P5-04)")
    else()
        message(STATUS "[zh-revival] Win32 build")
    endif()

    # /MD Release, /MDd Debug — matches original .dsp
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# ─────────────────────────────────────────────
#  Build configurations
#  Release | Debug | Internal | Profile
# ─────────────────────────────────────────────
if(NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Release;Debug;Internal;Profile"
        CACHE STRING "Available build configurations" FORCE)
endif()

# ─────────────────────────────────────────────
#  Source root (GeneralsMD = Zero Hour)
# ─────────────────────────────────────────────
set(ZH ${CMAKE_SOURCE_DIR}/GeneralsMD/Code)

# ─────────────────────────────────────────────
#  DirectX 8 SDK path
#  Set -DDXSDK_LIB_DIR=<path/to/DXSDK/Lib/x86> on the cmake command line.
#  If absent, DirectX libraries are skipped (connect P1-03 stubs instead).
# ─────────────────────────────────────────────
if(NOT DEFINED DXSDK_LIB_DIR)
    message(WARNING
        "[zh-revival] DXSDK_LIB_DIR not set. "
        "DirectX 8 libs (d3d8, d3dx8, dinput8, dxguid, dsound) will not be linked. "
        "Pass -DDXSDK_LIB_DIR=<legacy DXSDK/Lib/x86> or wire in the P1-03 stubs.")
    set(DXSDK_LIB_DIR "")
endif()

# ─────────────────────────────────────────────
#  Stub mode (P1-03)
#  ON by default — enables building without proprietary SDKs.
#  Turn OFF only when real Miles, Bink, SafeDisc, and GameSpy SDKs are
#  present on the include path.
# ─────────────────────────────────────────────
option(ZH_USE_STUBS
    "Use no-op stub implementations for proprietary SDKs (Miles, Bink, SafeDisc, GameSpy)"
    ON)

# ─────────────────────────────────────────────
#  SDL3 window + input backend (P5-03a/b)
#  When ON: SDL3 creates the window; HWND is extracted and subclassed with the
#  existing WndProc.  D3D8/DXVK uses the same HWND — no rendering changes.
#  SDL3 event pump replaces the Win32-only message loop so the game can
#  eventually run on Linux/macOS after DXVK and platform layers are in place.
# ─────────────────────────────────────────────
option(ZH_SDL3_WINDOW
    "Use SDL3 for window creation and event pumping (requires SDL3 via vcpkg)"
    OFF)

# ─────────────────────────────────────────────
#  DXVK D3D8-to-Vulkan translation layer (P5-03c)
#  No code changes required — dx8wrapper.cpp uses LoadLibrary("D3D8.DLL").
#  When ON and DXVK_DLL_DIR is set, CMake copies d3d8.dll + d3d9.dll from
#  the specified directory to the build output alongside the game binary.
#  At runtime Windows loads DXVK's d3d8.dll instead of the system one.
# ─────────────────────────────────────────────
option(ZH_DXVK
    "Copy DXVK D3D8→Vulkan translation DLLs to build output (requires DXVK_DLL_DIR)"
    OFF)

# ─────────────────────────────────────────────
#  OpenAL audio backend (P5-01)
#  Requires ZH_USE_STUBS=ON.  When ON, the no-op Miles stubs are replaced
#  with real OpenAL-backed implementations via OpenALMilesShim.cpp.
# ─────────────────────────────────────────────
option(ZH_OPENAL_AUDIO
    "Replace no-op Miles Sound stubs with real OpenAL audio (requires openal-soft via vcpkg)"
    OFF)

# ─────────────────────────────────────────────
#  FFmpeg video backend (P5-02)
#  Requires ZH_USE_STUBS=ON.  When ON, the no-op Bink stubs are replaced
#  with real FFmpeg-backed implementations via FFmpegBinkShim.cpp.
#  FFmpeg's Bink decoder is LGPL 2.1+ — compatible with this project's GPLv3.
# ─────────────────────────────────────────────
option(ZH_FFMPEG_VIDEO
    "Replace no-op Bink video stubs with real FFmpeg decoder (requires ffmpeg via vcpkg)"
    OFF)

if(ZH_USE_STUBS)
    message(STATUS "[zh-revival] Stub mode ON  — proprietary SDK stubs active (P1-03)")
    set(ZH_STUB_DIR ${ZH}/Libraries/Stubs)

    set(ZH_STUB_SOURCES
        ${ZH_STUB_DIR}/MilesSoundStub.cpp
        ${ZH_STUB_DIR}/BinkVideoStub.cpp
        ${ZH_STUB_DIR}/SafeDiscStub.cpp
        ${ZH_STUB_DIR}/GameSpyStub.cpp
    )

    if(ZH_OPENAL_AUDIO)
        message(STATUS "[zh-revival] OpenAL audio backend ON (P5-01)")
        find_package(OpenAL REQUIRED)
        list(REMOVE_ITEM ZH_STUB_SOURCES ${ZH_STUB_DIR}/MilesSoundStub.cpp)
        list(APPEND      ZH_STUB_SOURCES ${ZH_STUB_DIR}/OpenALMilesShim.cpp)
    endif()

    if(ZH_FFMPEG_VIDEO)
        message(STATUS "[zh-revival] FFmpeg video backend ON (P5-02)")
        # FFmpeg is supplied via vcpkg (ffmpeg[avcodec,avformat,swscale])
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
            libavcodec libavformat libswscale libavutil)
        list(REMOVE_ITEM ZH_STUB_SOURCES ${ZH_STUB_DIR}/BinkVideoStub.cpp)
        list(APPEND      ZH_STUB_SOURCES ${ZH_STUB_DIR}/FFmpegBinkShim.cpp)
    endif()

    # Build a small stub library so the .cpp files get compiled.
    # This catches any type or ABI errors in the stub headers at build time.
    add_library(zh_stubs STATIC ${ZH_STUB_SOURCES})
    # The stub dir must appear BEFORE the real SDK dirs so the forwarding
    # headers (MSS/MSS.h, bink.h, GameSpy/…, Common/SafeDisc/CdaPfn.h)
    # are found instead of the real (absent) SDK headers.
    target_include_directories(zh_stubs PUBLIC ${ZH_STUB_DIR})
    target_compile_definitions(zh_stubs PUBLIC STUB_IMPL)

    if(ZH_OPENAL_AUDIO)
        target_compile_definitions(zh_stubs PUBLIC ZH_OPENAL_AUDIO)
        target_link_libraries(zh_stubs PUBLIC OpenAL::OpenAL)
    endif()

    if(ZH_FFMPEG_VIDEO)
        target_compile_definitions(zh_stubs PUBLIC ZH_FFMPEG_VIDEO)
        target_link_libraries(zh_stubs PUBLIC PkgConfig::FFMPEG)
    endif()

    # Propagate STUB_IMPL (and optional backend flags) to all TUs in this build.
    add_compile_definitions(STUB_IMPL)
    if(ZH_OPENAL_AUDIO)
        add_compile_definitions(ZH_OPENAL_AUDIO)
    endif()
    if(ZH_FFMPEG_VIDEO)
        add_compile_definitions(ZH_FFMPEG_VIDEO)
    endif()
else()
    message(STATUS "[zh-revival] Stub mode OFF — expecting real proprietary SDKs on include path")
endif()

# ─────────────────────────────────────────────
#  SDL3 window + input (P5-03a/b)
# ─────────────────────────────────────────────
if(ZH_SDL3_WINDOW)
    message(STATUS "[zh-revival] SDL3 window backend ON (P5-03a/b)")
    find_package(SDL3 REQUIRED CONFIG)
    add_compile_definitions(ZH_SDL3_WINDOW)
endif()

# ─────────────────────────────────────────────
#  DXVK runtime DLL copy (P5-03c)
#  Set -DDXVK_DLL_DIR=<path/to/dxvk/x32> on the cmake command line.
#  Expected files: d3d8.dll (DXVK) and optionally d3d9.dll.
# ─────────────────────────────────────────────
if(ZH_DXVK)
    if(NOT DEFINED DXVK_DLL_DIR OR NOT EXISTS "${DXVK_DLL_DIR}")
        message(WARNING
            "[zh-revival] ZH_DXVK=ON but DXVK_DLL_DIR is not set or does not exist. "
            "Pass -DDXVK_DLL_DIR=<path/to/dxvk/x32> containing d3d8.dll.")
    else()
        message(STATUS "[zh-revival] DXVK DLL copy ON — source: ${DXVK_DLL_DIR}")
    endif()
endif()

# ─────────────────────────────────────────────
#  Common include paths shared by all targets
#  When ZH_USE_STUBS=ON the stub dir is prepended so forwarding headers
#  shadow the (absent) real SDK headers.
# ─────────────────────────────────────────────
if(ZH_USE_STUBS)
    set(ZH_COMMON_INCLUDES
        ${ZH}/Libraries/Stubs          # stubs FIRST — shadows real SDK dirs
        ${ZH}/Libraries/Include
        ${ZH}/Libraries/Include/Granny
        ${ZH}/GameEngine/Include
        ${ZH}/GameEngine/Include/Precompiled   # PreRTS.h needed by every GameEngine .cpp
        ${ZH}/GameEngineDevice/Include
        ${ZH}/Libraries/Source/WWVegas
        ${ZH}/Libraries/Source/WWVegas/WWLib
        ${ZH}/Libraries/Source/WWVegas/WWMath
        ${ZH}/Libraries/Source/WWVegas/WWDebug
        ${ZH}/Libraries/Source/WWVegas/WWSaveLoad
        ${ZH}/Libraries/Source/WWVegas/WW3D2
        ${ZH}/Libraries/Source/WWVegas/WWAudio
    )
else()
    set(ZH_COMMON_INCLUDES
        ${ZH}/Libraries/Include
        ${ZH}/Libraries/Include/Granny
        ${ZH}/Libraries/Include/MSS
        ${ZH}/GameEngine/Include
        ${ZH}/GameEngine/Include/Precompiled   # PreRTS.h needed by every GameEngine .cpp
        ${ZH}/GameEngineDevice/Include
        ${ZH}/Libraries/Source/WWVegas
        ${ZH}/Libraries/Source/WWVegas/WWLib
        ${ZH}/Libraries/Source/WWVegas/WWMath
        ${ZH}/Libraries/Source/WWVegas/WWDebug
        ${ZH}/Libraries/Source/WWVegas/WWSaveLoad
        ${ZH}/Libraries/Source/WWVegas/WW3D2
        ${ZH}/Libraries/Source/WWVegas/WWAudio
        ${ZH}/Libraries/Source/WWVegas/Miles6/include
    )
endif()

# ─────────────────────────────────────────────
#  Per-configuration compile definitions
#  These mirror the original .dsp preprocessor blocks exactly.
# ─────────────────────────────────────────────
set(ZH_DEFS_RELEASE  IG_DEBUG_STACKTRACE NDEBUG _RELEASE)
set(ZH_DEFS_DEBUG    _DEBUG BROWSER_DEBUG WWDEBUG)
set(ZH_DEFS_INTERNAL NDEBUG _INTERNAL)
set(ZH_DEFS_PROFILE  IG_DEBUG_STACKTRACE NDEBUG _RELEASE _PROFILE)

# ─────────────────────────────────────────────
#  Helper: glob all .cpp/.c sources in a directory tree
# ─────────────────────────────────────────────
macro(zh_glob_sources VAR DIR)
    file(GLOB_RECURSE ${VAR}
        "${DIR}/*.cpp"
        "${DIR}/*.c"
        "${DIR}/*.CPP"   # some legacy files use uppercase extension (e.g. FTP.CPP, TARGA.CPP)
        "${DIR}/*.C"
    )
endmacro()

# ─────────────────────────────────────────────
#  Helper: apply standard per-config definitions to a target
# ─────────────────────────────────────────────
function(zh_target_defs TARGET)
    target_compile_definitions(${TARGET} PRIVATE
        $<$<CONFIG:Release>:${ZH_DEFS_RELEASE}>
        $<$<CONFIG:Debug>:${ZH_DEFS_DEBUG}>
        $<$<CONFIG:Internal>:${ZH_DEFS_INTERNAL}>
        $<$<CONFIG:Profile>:${ZH_DEFS_PROFILE}>
    )
endfunction()

# ═════════════════════════════════════════════
#  LIBRARY TARGETS
# ═════════════════════════════════════════════

# ─── WWDebug ─────────────────────────────────
zh_glob_sources(WWDEBUG_SRCS ${ZH}/Libraries/Source/WWVegas/WWDebug)
add_library(wwdebug STATIC ${WWDEBUG_SRCS})
target_include_directories(wwdebug PUBLIC ${ZH_COMMON_INCLUDES})
zh_target_defs(wwdebug)

# ─── WWMath ──────────────────────────────────
zh_glob_sources(WWMATH_SRCS ${ZH}/Libraries/Source/WWVegas/WWMath)
add_library(wwmath STATIC ${WWMATH_SRCS})
target_include_directories(wwmath PUBLIC ${ZH_COMMON_INCLUDES})
zh_target_defs(wwmath)

# ─── WWLib ───────────────────────────────────
zh_glob_sources(WWLIB_SRCS ${ZH}/Libraries/Source/WWVegas/WWLib)
add_library(wwlib STATIC ${WWLIB_SRCS})
target_include_directories(wwlib PUBLIC ${ZH_COMMON_INCLUDES})
zh_target_defs(wwlib)
if(WIN32)
    target_link_libraries(wwlib PUBLIC winmm)
endif()

# ─── WWSaveLoad ──────────────────────────────
zh_glob_sources(WWSAVELOAD_SRCS ${ZH}/Libraries/Source/WWVegas/WWSaveLoad)
add_library(wwsaveload STATIC ${WWSAVELOAD_SRCS})
target_include_directories(wwsaveload PUBLIC ${ZH_COMMON_INCLUDES})
zh_target_defs(wwsaveload)

# ─── WWUtil ──────────────────────────────────
zh_glob_sources(WWUTIL_SRCS ${ZH}/Libraries/Source/WWVegas/Wwutil)
add_library(wwutil STATIC ${WWUTIL_SRCS})
target_include_directories(wwutil PUBLIC ${ZH_COMMON_INCLUDES})
zh_target_defs(wwutil)

# ─── wwshade ─────────────────────────────────
if(ZH_USE_STUBS)
    # Shader sources depend on generated shader bytecode headers which are not committed.
    # In stub builds we keep the target name but don't compile wwshade.
    add_library(wwshade INTERFACE)
    target_include_directories(wwshade INTERFACE ${ZH_COMMON_INCLUDES})
else()
    zh_glob_sources(WWSHADE_SRCS ${ZH}/Libraries/Source/WWVegas/wwshade)
    add_library(wwshade STATIC ${WWSHADE_SRCS})
    target_include_directories(wwshade PUBLIC ${ZH_COMMON_INCLUDES})
    zh_target_defs(wwshade)
endif()

# ─── WWDownload ──────────────────────────────
zh_glob_sources(WWDOWNLOAD_SRCS ${ZH}/Libraries/Source/WWVegas/WWDownload)
add_library(wwdownload STATIC ${WWDOWNLOAD_SRCS})
target_include_directories(wwdownload PUBLIC ${ZH_COMMON_INCLUDES})
zh_target_defs(wwdownload)

# ─── WWAudio  (Miles Sound System wrapper) ───
zh_glob_sources(WWAUDIO_SRCS ${ZH}/Libraries/Source/WWVegas/WWAudio)
add_library(wwaudio STATIC ${WWAUDIO_SRCS})
target_include_directories(wwaudio PUBLIC ${ZH_COMMON_INCLUDES})
zh_target_defs(wwaudio)
target_link_libraries(wwaudio PUBLIC wwlib wwmath wwdebug wwsaveload)
if(ZH_USE_STUBS)
    # Pull in stub types for Miles — zh_stubs is PUBLIC so dependents also see them
    target_link_libraries(wwaudio PUBLIC zh_stubs)
endif()

# ─── WW3D2  (DirectX 8 renderer) ─────────────
zh_glob_sources(WW3D2_SRCS ${ZH}/Libraries/Source/WWVegas/WW3D2)
add_library(ww3d2 STATIC ${WW3D2_SRCS})
target_include_directories(ww3d2 PUBLIC ${ZH_COMMON_INCLUDES})
if(DXSDK_LIB_DIR)
    target_include_directories(ww3d2 PUBLIC ${DXSDK_LIB_DIR}/../include)
    target_link_directories(ww3d2 PUBLIC ${DXSDK_LIB_DIR})
endif()
zh_target_defs(ww3d2)
target_link_libraries(ww3d2 PUBLIC wwlib wwmath wwdebug wwsaveload wwutil wwshade)

# ─── Benchmark ───────────────────────────────
zh_glob_sources(BENCHMARK_SRCS ${ZH}/Libraries/Source/Benchmark)
if(BENCHMARK_SRCS)
    add_library(benchmark_lib STATIC ${BENCHMARK_SRCS})
    target_include_directories(benchmark_lib PUBLIC
        ${ZH}/Libraries/Source/Benchmark
        ${ZH_COMMON_INCLUDES}
    )
    zh_target_defs(benchmark_lib)
else()
    # Some source drops omit this optional library; keep the target name so
    # downstream link lines remain stable.
    add_library(benchmark_lib STATIC
        ${ZH}/Libraries/Stubs/BenchmarkStub.cpp
    )
    target_include_directories(benchmark_lib PUBLIC
        ${ZH}/Libraries/Source/Benchmark
        ${ZH_COMMON_INCLUDES}
    )
    zh_target_defs(benchmark_lib)
endif()

# Legacy profiling headers auto-link one of several standalone profile*.lib
# variants via `#pragma comment(lib, ...)`. The modern CMake build does not
# ship those archives, but Release and non-_PROFILE paths only need the
# library to exist so the linker can satisfy the pragma.
set(ZH_PROFILE_STUB_SRC "${CMAKE_BINARY_DIR}/profile_stub.cpp")
file(WRITE "${ZH_PROFILE_STUB_SRC}" "void zh_profile_stub(void) {}\n")
add_library(profile STATIC "${ZH_PROFILE_STUB_SRC}")
set_target_properties(profile PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${CMAKE_BINARY_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG    "${CMAKE_BINARY_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_INTERNAL "${CMAKE_BINARY_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_PROFILE  "${CMAKE_BINARY_DIR}"
    OUTPUT_NAME_RELEASE               "profile"
    OUTPUT_NAME_DEBUG                 "profiledebug"
    OUTPUT_NAME_INTERNAL              "profileinternal"
    OUTPUT_NAME_PROFILE               "profileprofile"
)

# ─── Compression ─────────────────────────────
zh_glob_sources(COMPRESSION_SRCS ${ZH}/Libraries/Source/Compression)
add_library(compression STATIC ${COMPRESSION_SRCS})
target_include_directories(compression PUBLIC
    ${ZH}/Libraries/Source/Compression
    ${ZH_COMMON_INCLUDES}
)
if(NOT ZH_USE_STUBS)
    # Only required when building with real external dependencies.
    # CI builds in stub mode, where we compile without zlib.
    find_package(ZLIB REQUIRED)
    target_link_libraries(compression PUBLIC ZLIB::ZLIB)
endif()

# ─── EABrowserDispatch ───────────────────────
zh_glob_sources(EABROWSER_SRCS ${ZH}/Libraries/Source/EABrowserDispatch)
if(EABROWSER_SRCS)
    add_library(eabrowserdispatch STATIC ${EABROWSER_SRCS})
    target_include_directories(eabrowserdispatch PUBLIC
        ${ZH}/Libraries/Source/EABrowserDispatch
        ${ZH_COMMON_INCLUDES}
    )
    zh_target_defs(eabrowserdispatch)
else()
    # Optional/proprietary library not present in open drop; keep a stub target.
    add_library(eabrowserdispatch INTERFACE)
    target_include_directories(eabrowserdispatch INTERFACE
        ${ZH}/Libraries/Source/EABrowserDispatch
        ${ZH_COMMON_INCLUDES}
    )
endif()

# ─── GameSpy libraries ────────────────────────
#  Source layout: Libraries/Source/GameSpy/GameSpy/<module>/
#  The GameSpy SDK source is proprietary and gitignored.
#  When ZH_USE_STUBS=ON the stubs cover the entire GameSpy API; real source
#  targets are replaced by INTERFACE libraries that just forward zh_stubs.
#  When ZH_USE_STUBS=OFF the real source must be present (private SDK).
set(GAMESPY_ROOT ${ZH}/Libraries/Source/GameSpy/GameSpy)

if(ZH_USE_STUBS)
    # Create INTERFACE libraries so that link lines that mention
    # gamespyhttp etc. still resolve without real source files.
    foreach(_gs IN ITEMS gamespyhttp gamespypeer gamespypresence gamespystats gamespypatching)
        add_library(${_gs} INTERFACE)
        target_link_libraries(${_gs} INTERFACE zh_stubs)
    endforeach()
else()
    zh_glob_sources(GAMESPY_HTTP_SRCS    ${GAMESPY_ROOT}/ghttp)
    zh_glob_sources(GAMESPY_PEER_SRCS    ${GAMESPY_ROOT}/peer)
    zh_glob_sources(GAMESPY_GP_SRCS      ${GAMESPY_ROOT}/gp)
    zh_glob_sources(GAMESPY_STATS_SRCS   ${GAMESPY_ROOT}/gstats)
    zh_glob_sources(GAMESPY_PT_SRCS      ${GAMESPY_ROOT}/pt)

    add_library(gamespyhttp     STATIC ${GAMESPY_HTTP_SRCS})
    add_library(gamespypeer     STATIC ${GAMESPY_PEER_SRCS})
    add_library(gamespypresence STATIC ${GAMESPY_GP_SRCS})
    add_library(gamespystats    STATIC ${GAMESPY_STATS_SRCS})
    add_library(gamespypatching STATIC ${GAMESPY_PT_SRCS})

    foreach(_gs IN ITEMS gamespyhttp gamespypeer gamespypresence gamespystats gamespypatching)
        target_include_directories(${_gs} PUBLIC
            ${GAMESPY_ROOT}
            ${ZH_COMMON_INCLUDES}
        )
        zh_target_defs(${_gs})
    endforeach()
endif()

# ─── GameEngine  (core game logic) ───────────
zh_glob_sources(GAMEENGINE_SRCS ${ZH}/GameEngine/Source)
# Exclude files that depend on unavailable SDKs (wpaudio/wsys, Windows Media Foundation)
list(REMOVE_ITEM GAMEENGINE_SRCS
    "${ZH}/GameEngine/Source/Common/Audio/GameSpeech.cpp"
    "${ZH}/GameEngine/Source/Common/Audio/simpleplayer.cpp"
    "${ZH}/GameEngine/Source/Common/Audio/urllaunch.cpp"
    "${ZH}/GameEngine/Source/GameNetwork/GameSpyGameInfo.cpp"
)
add_library(gameengine STATIC ${GAMEENGINE_SRCS})
target_include_directories(gameengine PUBLIC
    ${ZH}/GameEngine/Include
    ${GAMESPY_ROOT}
    ${ZH_COMMON_INCLUDES}
)
zh_target_defs(gameengine)
target_link_libraries(gameengine PUBLIC
    wwlib wwmath wwdebug wwsaveload wwutil
    eabrowserdispatch compression benchmark_lib
)
if(ZH_USE_STUBS)
    # GameSpy stubs — GameEngine/Source/GameNetwork/GameSpy/ includes ghttp,
    # Peer, GP headers throughout.
    target_link_libraries(gameengine PUBLIC zh_stubs)
endif()

# ─── GameEngineDevice  (D3D8 / audio device layer) ───
zh_glob_sources(GAMEENGINEDEVICE_SRCS ${ZH}/GameEngineDevice/Source)
add_library(gameenginedevice STATIC ${GAMEENGINEDEVICE_SRCS})
target_include_directories(gameenginedevice PUBLIC
    ${ZH}/GameEngineDevice/Include
    ${ZH}/GameEngine/Include
    ${ZH_COMMON_INCLUDES}
)
if(DXSDK_LIB_DIR)
    target_include_directories(gameenginedevice PUBLIC ${DXSDK_LIB_DIR}/../include)
    target_link_directories(gameenginedevice PUBLIC ${DXSDK_LIB_DIR})
endif()
zh_target_defs(gameenginedevice)
target_link_libraries(gameenginedevice PUBLIC
    gameengine ww3d2 wwaudio
    eabrowserdispatch compression benchmark_lib
)
if(ZH_USE_STUBS)
    # Bink and Miles stubs — gameenginedevice hosts BinkVideoPlayer.cpp
    # and MilesAudioManager.cpp which both include proprietary headers.
    target_link_libraries(gameenginedevice PUBLIC zh_stubs)
endif()

# ═════════════════════════════════════════════
#  MAIN EXECUTABLE: zh-revival  (Zero Hour)
# ═════════════════════════════════════════════
add_executable(zh-revival WIN32
    ${ZH}/Main/WinMain.cpp
    ${ZH}/Main/RTS.RC
)

target_include_directories(zh-revival PRIVATE
    ${ZH}/Main
    ${ZH_COMMON_INCLUDES}
)

target_link_directories(zh-revival PRIVATE
    ${CMAKE_BINARY_DIR}
)

zh_target_defs(zh-revival)
if(ZH_USE_STUBS)
    # WinMain.cpp includes Common/SafeDisc/CdaPfn.h — resolved via stubs.
    target_link_libraries(zh-revival PRIVATE zh_stubs)
endif()

# Link order mirrors the original LINK32 line in RTS.dsp
target_link_libraries(zh-revival PRIVATE
    # WWVegas / engine libs
    profile
    benchmark_lib
    ww3d2
    wwdebug
    wwlib
    wwutil
    wwmath
    gameengine
    gameenginedevice
    # Online / download
    gamespyhttp
    gamespypeer
    gamespypresence
    gamespystats
    gamespypatching
    wwdownload
    eabrowserdispatch
    # DirectX 8 — DInput/dxguid come from the Windows SDK, D3D8/D3DX8
    # still require the legacy DXSDK path when building against the real SDK.
    $<$<BOOL:${DXSDK_LIB_DIR}>:d3d8>
    $<$<BOOL:${DXSDK_LIB_DIR}>:d3dx8>
    $<$<BOOL:${DXSDK_LIB_DIR}>:dsound>
    # Windows system libraries (always available in Windows SDK)
    kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32
    ole32 oleaut32 uuid odbc32 odbccp32
    winmm vfw32 wsock32 imm32 wininet
    # avifil32: AVI capture (FramGrab.cpp). MSVC only — guarded with #ifdef _MSC_VER.
    $<$<NOT:$<CXX_COMPILER_ID:GNU>>:avifil32>
    dbghelp   # StackDump.cpp uses Sym*/StackWalk from DbgHelp.dll
    dinput8 dxguid
    # comsuppw is an MSVC-specific COM support library not available in MinGW.
    $<$<NOT:$<CXX_COMPILER_ID:GNU>>:comsuppw>
    # Proprietary audio/video .lib files
    # Present only when real SDKs are available (ZH_USE_STUBS=OFF + real libs).
    # When ZH_USE_STUBS=ON the stub headers provide inline no-ops so no .lib
    # is needed; the real .dll is not loaded at runtime.
    $<$<AND:$<NOT:$<BOOL:${ZH_USE_STUBS}>>,$<EXISTS:${ZH}/Libraries/Lib/binkw32.lib>>:${ZH}/Libraries/Lib/binkw32.lib>
    $<$<AND:$<NOT:$<BOOL:${ZH_USE_STUBS}>>,$<EXISTS:${ZH}/Libraries/Lib/mss32.lib>>:${ZH}/Libraries/Lib/mss32.lib>
)

# SDL3 (P5-03a/b)
if(ZH_SDL3_WINDOW)
    target_link_libraries(zh-revival PRIVATE SDL3::SDL3)
    target_link_libraries(gameenginedevice PUBLIC SDL3::SDL3)
endif()

# DXVK DLL copy (P5-03c) — post-build copy to the binary output directory
if(ZH_DXVK AND DEFINED DXVK_DLL_DIR AND EXISTS "${DXVK_DLL_DIR}")
    foreach(_dxvk_dll d3d8.dll d3d9.dll)
        if(EXISTS "${DXVK_DLL_DIR}/${_dxvk_dll}")
            add_custom_command(TARGET zh-revival POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${DXVK_DLL_DIR}/${_dxvk_dll}"
                    "$<TARGET_FILE_DIR:zh-revival>/${_dxvk_dll}"
                COMMENT "[zh-revival] Copying DXVK ${_dxvk_dll} to output"
            )
        endif()
    endforeach()
endif()

# Output directory mirrors original .dsp: ../Run/ relative to Code/
set_target_properties(zh-revival PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${CMAKE_SOURCE_DIR}/GeneralsMD/Run"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG    "${CMAKE_SOURCE_DIR}/GeneralsMD/Run"
    RUNTIME_OUTPUT_DIRECTORY_INTERNAL "${CMAKE_SOURCE_DIR}/GeneralsMD/Run"
    RUNTIME_OUTPUT_DIRECTORY_PROFILE  "${CMAKE_SOURCE_DIR}/GeneralsMD/Run"
    OUTPUT_NAME_RELEASE               "zh-revival"
    OUTPUT_NAME_DEBUG                 "zh-revival-debug"
    OUTPUT_NAME_INTERNAL              "zh-revival-internal"
    OUTPUT_NAME_PROFILE               "zh-revival-profile"
    PDB_OUTPUT_DIRECTORY              "${CMAKE_SOURCE_DIR}/GeneralsMD/Run"
)

# ─────────────────────────────────────────────
#  Replay tools (P3-03 / P3-04 / P3-05)
# ─────────────────────────────────────────────
add_subdirectory(tools)

# ─────────────────────────────────────────────
#  Configure summary
# ─────────────────────────────────────────────
message(STATUS "")
message(STATUS "zh-revival CMake configured")
message(STATUS "  Source root  : ${ZH}")
message(STATUS "  DXSDK libs   : ${DXSDK_LIB_DIR}")
message(STATUS "  C++ standard : C++20")
message(STATUS "  Configs      : Release | Debug | Internal | Profile")
message(STATUS "")
message(STATUS "  This project is not affiliated with or endorsed by Electronic Arts.")
message(STATUS "  Command & Conquer is a trademark of Electronic Arts.")
message(STATUS "  You must own the original game to use this software.")
message(STATUS "")
