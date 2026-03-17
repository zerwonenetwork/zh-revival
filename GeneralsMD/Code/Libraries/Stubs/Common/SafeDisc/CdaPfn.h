// Common/SafeDisc/CdaPfn.h — stub forwarding header
//
// When STUB_IMPL is defined (i.e. ZH_USE_STUBS=ON), this header is found
// before the real SafeDisc SDK on the include path and forwards to the
// named stub so all SafeDisc types and no-op implementations are pulled in.
//
// Original include site: GeneralsMD/Code/Main/WinMain.cpp line 52
//   #include "Common/SafeDisc/CdaPfn.h"
//
// Real SDK swap-in: remove -DSTUB_IMPL and supply the genuine CdaPfn.h
// from a licensed SafeDisc SDK (or remove the call-sites entirely in P1-11).

#pragma once
#include "../../../Stubs/SafeDiscStub.h"
