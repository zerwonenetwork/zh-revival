# mingw32.cmake — CMake cross-compile toolchain for i686-w64-mingw32
#
# Used by ci-build.yml Linux job to cross-compile the Windows x86 binary
# from an Ubuntu runner using the mingw-w64 toolchain.
#
# This file is ONLY for the CI cross-build. Windows builds use the MSVC
# generator directly and do not use this toolchain file.

set(CMAKE_SYSTEM_NAME    Windows)
set(CMAKE_SYSTEM_VERSION 10)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(TOOLCHAIN_PREFIX i686-w64-mingw32)

find_program(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}-gcc)
find_program(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
find_program(CMAKE_RC_COMPILER  ${TOOLCHAIN_PREFIX}-windres)

if(NOT CMAKE_C_COMPILER)
    message(FATAL_ERROR "mingw32 C compiler not found (${TOOLCHAIN_PREFIX}-gcc). "
                        "Install: sudo apt-get install gcc-mingw-w64-i686")
endif()

# Where to look for target libraries — only the sysroot
set(CMAKE_FIND_ROOT_PATH  /usr/${TOOLCHAIN_PREFIX})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# MinGW-specific: suppress MSVC-only flags that CMakeLists.txt sets via if(MSVC)
# (the if(MSVC) blocks are skipped automatically because MSVC is FALSE here)
