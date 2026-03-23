// malloc.h — compatibility shim for MSVC <malloc.h>
// On Linux/macOS, malloc/free/realloc are in <stdlib.h>.
// MSVC also exposed _alloca, _malloca here; map to alloca.
#pragma once
#include <stdlib.h>
#ifndef _alloca
#include <alloca.h>
#define _alloca alloca
#define _malloca alloca
#define _freea(p) ((void)(p))
#endif
