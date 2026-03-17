cmake_minimum_required(VERSION 3.25)
project(zh-revival VERSION 0.1.0 LANGUAGES CXX RC)

# ─────────────────────────────────────────────
#  C++ standard
# ─────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────────
#  Platform / compiler settings
# ─────────────────────────────────────────────
# Windows 10 minimum
add_compile_definitions(
    _WIN32_WINNT=0x0A00
    WINVER=0x0A00
    WIN32
    _WINDOWS
    _MBCS
)

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
    )

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
#  Common include paths shared by all targets
# ─────────────────────────────────────────────
set(ZH_COMMON_INCLUDES
    ${ZH}/Libraries/Include
    ${ZH}/Libraries/Include/Granny
    ${ZH}/Libraries/Include/MSS
    ${ZH}/GameEngine/Include
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
zh_glob_sources(WWSHADE_SRCS ${ZH}/Libraries/Source/WWVegas/wwshade)
add_library(wwshade STATIC ${WWSHADE_SRCS})
target_include_directories(wwshade PUBLIC ${ZH_COMMON_INCLUDES})
zh_target_defs(wwshade)

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
add_library(benchmark_lib STATIC ${BENCHMARK_SRCS})
target_include_directories(benchmark_lib PUBLIC
    ${ZH}/Libraries/Source/Benchmark
    ${ZH_COMMON_INCLUDES}
)

# ─── Compression ─────────────────────────────
zh_glob_sources(COMPRESSION_SRCS ${ZH}/Libraries/Source/Compression)
add_library(compression STATIC ${COMPRESSION_SRCS})
target_include_directories(compression PUBLIC
    ${ZH}/Libraries/Source/Compression
    ${ZH_COMMON_INCLUDES}
)

# ─── EABrowserDispatch ───────────────────────
zh_glob_sources(EABROWSER_SRCS ${ZH}/Libraries/Source/EABrowserDispatch)
add_library(eabrowserdispatch STATIC ${EABROWSER_SRCS})
target_include_directories(eabrowserdispatch PUBLIC
    ${ZH}/Libraries/Source/EABrowserDispatch
    ${ZH_COMMON_INCLUDES}
)

# ─── GameSpy libraries ────────────────────────
#  Source layout: Libraries/Source/GameSpy/GameSpy/<module>/
set(GAMESPY_ROOT ${ZH}/Libraries/Source/GameSpy/GameSpy)

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

# ─── GameEngine  (core game logic) ───────────
zh_glob_sources(GAMEENGINE_SRCS ${ZH}/GameEngine/Source)
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

zh_target_defs(zh-revival)

# Link order mirrors the original LINK32 line in RTS.dsp
target_link_libraries(zh-revival PRIVATE
    # WWVegas / engine libs
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
    # DirectX 8 — only linked when DXSDK_LIB_DIR is set
    $<$<BOOL:${DXSDK_LIB_DIR}>:d3d8>
    $<$<BOOL:${DXSDK_LIB_DIR}>:d3dx8>
    $<$<BOOL:${DXSDK_LIB_DIR}>:dxguid>
    $<$<BOOL:${DXSDK_LIB_DIR}>:dinput8>
    $<$<BOOL:${DXSDK_LIB_DIR}>:dsound>
    # Windows system libraries (always available in Windows SDK)
    kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32
    ole32 oleaut32 uuid odbc32 odbccp32
    winmm vfw32 wsock32 imm32 wininet
    # Proprietary audio/video .lib files
    # These are either present in Libraries/Lib (from an existing install)
    # or replaced by P1-03 stubs. Both cases use the same filename.
    $<$<EXISTS:${ZH}/Libraries/Lib/binkw32.lib>:${ZH}/Libraries/Lib/binkw32.lib>
    $<$<EXISTS:${ZH}/Libraries/Lib/mss32.lib>:${ZH}/Libraries/Lib/mss32.lib>
)

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
