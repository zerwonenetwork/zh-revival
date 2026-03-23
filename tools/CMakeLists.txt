# tools/CMakeLists.txt
# Zero Hour replay tools — libzhreplay, zh-replay-validate, zh-replay-info
#
# This project is not affiliated with or endorsed by Electronic Arts.
# Command & Conquer is a trademark of Electronic Arts.
# You must own the original game to use this software.

cmake_minimum_required(VERSION 3.16)

# libzhreplay (static library)
add_library(zhreplay STATIC
    libzhreplay/zhreplay.c
    libzhreplay/zhreplay.h
)
target_include_directories(zhreplay PUBLIC libzhreplay)

# zh-replay-validate
add_executable(zh-replay-validate
    zh-replay-validate/main.c
)
target_link_libraries(zh-replay-validate PRIVATE zhreplay)

# zh-replay-info
add_executable(zh-replay-info
    zh-replay-info/main.c
)
target_link_libraries(zh-replay-info PRIVATE zhreplay)
