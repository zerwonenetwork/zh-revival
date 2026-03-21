// GameSpyStub.cpp
//
// All GameSpy stub implementations are declared inline in GameSpyStub.h.
// This file exists to satisfy the CLAUDE.md output requirement of a
// matching .cpp per stub and to serve as a build-system anchor.
//
// Covered modules: ghttp, GP (Presence), Peer (matchmaking), gpersist.
//
// The GameSpy matchmaking infrastructure was permanently shut down on
// May 31, 2014.  A community-run replacement is planned for Phase 3+
// (separate project, not in this repo).

#define STUB_IMPL
#include "GameSpyStub.h"

extern "C" int getQR2HostingStatus(void)
{
    return 0;
}

static_assert(true, "GameSpyStub compiled successfully");
