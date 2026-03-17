// bink.h — stub forwarding header
//
// When STUB_IMPL is defined (i.e. ZH_USE_STUBS=ON), this header is found
// before the real Bink SDK on the include path and forwards to the named
// stub so all Bink types and no-op implementations are pulled in.
//
// Real SDK swap-in: remove -DSTUB_IMPL and supply the genuine bink.h from
// a licensed RAD Game Tools Bink 1.x SDK installation.

#pragma once
#include "BinkVideoStub.h"
