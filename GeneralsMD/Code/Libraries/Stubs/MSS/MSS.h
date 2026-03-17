// MSS/MSS.h — stub forwarding header
//
// When STUB_IMPL is defined (i.e. ZH_USE_STUBS=ON), this header is found
// before the real MSS SDK on the include path and forwards to the named
// stub so all Miles types and no-op implementations are pulled in.
//
// Real SDK swap-in: remove -DSTUB_IMPL and supply the genuine
// Libraries/Include/MSS/MSS.h from a licensed Miles 6 installation.

#pragma once
#include "../MilesSoundStub.h"
