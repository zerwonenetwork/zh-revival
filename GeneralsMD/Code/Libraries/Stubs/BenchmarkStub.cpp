/*
**  zh-revival benchmark stub
**
**  The original benchmark helper shipped as an optional library. The open
**  source drop does not include an implementation, but the game only uses it
**  to populate performance hints. Returning safe defaults keeps startup logic
**  intact and selects conservative settings.
*/

#include "benchmark.h"

extern "C" int RunBenchmark(
    int argc,
    char *argv[],
    float *floatResult,
    float *intResult,
    float *memResult)
{
    (void)argc;
    (void)argv;

    if (floatResult != nullptr) {
        *floatResult = 0.0f;
    }
    if (intResult != nullptr) {
        *intResult = 0.0f;
    }
    if (memResult != nullptr) {
        *memResult = 0.0f;
    }

    return 0;
}
