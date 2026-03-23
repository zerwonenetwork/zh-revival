// new.h — compatibility shim for the old MSVC <new.h> header
//
// MSVC used to ship <new.h> as an alias for <new> (C++ placement new, etc.)
// GCC/Clang only have <new> — this shim bridges the gap.
//
// This project is not affiliated with or endorsed by Electronic Arts.
#pragma once
#include <new>
