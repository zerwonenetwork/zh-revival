// GameSpyStub.h — stub for GameSpy SDK (ghttp, Peer, GP, gpersist)
//
// GameSpy services were permanently shut down on May 31, 2014.
// This stub replaces all GameSpy SDK headers so the project compiles
// without the proprietary/defunct GameSpy SDK source.
//
// Stub behaviour:
//   - All connection functions return "offline" / error codes
//   - HTTP requests return failure immediately
//   - Multiplayer matchmaking through old GameSpy infrastructure is
//     non-functional by design; replacement planned in Phase 3+
//
// Guard: wrap all content in #ifdef STUB_IMPL so callers that supply
// real GameSpy headers can compile without picking up these definitions.
//
// Covered SDK modules:
//   ghttp   — HTTP client (ghttpGet, ghttpHead, ghttpThink, ...)
//   GP      — GameSpy Presence / profile service (gpConnect, ...)
//   Peer    — Peer-to-peer lobby / matchmaking (peerConnect, ...)
//   gpersist— Statistics / persistent storage
//
// This project is not affiliated with or endorsed by Electronic Arts.
// Command & Conquer is a trademark of Electronic Arts.
// You must own the original game to use this software.

#pragma once
#ifndef ZH_GAMESPY_STUB_H
#define ZH_GAMESPY_STUB_H

#ifdef STUB_IMPL

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <stddef.h>  // NULL

// ===========================================================================
//  Shared GameSpy primitives
// ===========================================================================

typedef unsigned int  gsi_u32;
typedef int           gsi_i32;
typedef unsigned char gsi_u8;

// GPResult / GHTTPResult share a "success = 0" convention
#define GS_SUCCESS  0
#define GS_FAILED   1

// ===========================================================================
//  GP string-length constants (used by BuddyThread.h, etc.)
// ===========================================================================
#ifndef GP_NICK_LEN
#define GP_NICK_LEN              31
#endif
#ifndef GP_EMAIL_LEN
#define GP_EMAIL_LEN             51
#endif
#ifndef GP_PASSWORD_LEN
#define GP_PASSWORD_LEN          31
#endif
#ifndef GP_STATUS_STRING_LEN
#define GP_STATUS_STRING_LEN     256
#endif
#ifndef GP_LOCATION_STRING_LEN
#define GP_LOCATION_STRING_LEN   256
#endif
#ifndef GP_COUNTRYCODE_LEN
#define GP_COUNTRYCODE_LEN       3
#endif
#ifndef GP_REASON_LEN
#define GP_REASON_LEN            256
#endif

// ===========================================================================
//  ghttp — HTTP client module
// ===========================================================================

typedef int           GHTTPRequest;   // negative = invalid request
typedef int           GHTTPBool;      // 0 = false, 1 = true
typedef int           GHTTPResult;    // GHTTPSuccess, GHTTPParseURLFailed, ...

// GHTTPResult codes used in MainMenuUtils.cpp callback parameters
#define GHTTPSuccess        0
#define GHTTPParseURLFailed 1
#define GHTTPHostLookupFailed 2
#define GHTTPSocketFailed   3
#define GHTTPConnectFailed  4
#define GHTTPBadResponse    5
#define GHTTPRequestRejected 6
#define GHTTPUnauthorized   7
#define GHTTPForbidden      8
#define GHTTPFileNotFound   9
#define GHTTPServerError    10
#define GHTTPFileWriteFailed 11
#define GHTTPFileReadFailed 12
#define GHTTPTimedOut       13
#define GHTTPNetworkError   14
#define GHTTPFileIncomplete 15
#define GHTTPFileToBig      16
#define GHTTPEncryptionError 17

#define GHTTPTrue   1
#define GHTTPFalse  0

// Callback type invoked when an HTTP request completes
typedef GHTTPBool (* GHTTPRequestCallback)(
    GHTTPRequest  request,
    GHTTPResult   result,
    char*         buffer,
    int           bufferLen,
    void*         param);

// Callback for HEAD requests (no body)
typedef GHTTPBool (* GHTTPHeadCallback)(
    GHTTPRequest  request,
    GHTTPResult   result,
    void*         param);

// Blocking flag type
typedef GHTTPBool GHTTPBlocking;

// --- ghttp stub functions ---------------------------------------------------

// Initiate a GET request.  Returns -1 (invalid request) in stub mode.
inline GHTTPRequest ghttpGet(
    const char*          url,
    GHTTPBool            blocking,
    GHTTPRequestCallback callback,
    void*                param)
{
    (void)url; (void)blocking;
    // Invoke callback immediately with a network-error result so the caller
    // receives a clean failure rather than waiting forever.
    if (callback)
        callback(-1, GHTTPNetworkError, nullptr, 0, param);
    return -1;
}

// Initiate a HEAD request (metadata only, no body).
inline GHTTPRequest ghttpHead(
    const char*       url,
    GHTTPBool         blocking,
    GHTTPHeadCallback callback,
    void*             param)
{
    (void)url; (void)blocking;
    if (callback)
        callback(-1, GHTTPNetworkError, param);
    return -1;
}

// Process pending async HTTP requests.  Stub: nothing to process.
inline void ghttpThink(void) {}

// Set a proxy server string.  Stub: ignored.
inline void ghttpSetProxy(const char* server) { (void)server; }

// Retrieve response headers for a completed request.
inline const char* ghttpGetHeaders(GHTTPRequest request)
{
    (void)request;
    return "";
}

// Cancel an outstanding request.
inline void ghttpCancelRequest(GHTTPRequest request) { (void)request; }

// Clean up all pending requests (called at shutdown).
inline void ghttpCleanup(void) {}

// ===========================================================================
//  GP — GameSpy Presence / profile service
// ===========================================================================

typedef int  GPProfile;   // numeric profile ID
typedef int  GPEnum;      // status enum (online, away, playing, ...)
typedef int  GPResult;    // GP_NO_ERROR = 0, ...
typedef void* GPConnection; // opaque connection handle

// GPResult codes
#define GP_NO_ERROR              0
#define GP_MEMORY_ERROR          1
#define GP_PARAMETER_ERROR       2
#define GP_NETWORK_ERROR         3
#define GP_SERVER_ERROR          4
#define GP_NOT_LOGGED_IN         5
#define GP_CONNECTION_CLOSED     6

// GP buddy status values
#define GP_OFFLINE   0
#define GP_ONLINE    1
#define GP_PLAYING   2
#define GP_STAGING   3
#define GP_CHATTING  4
#define GP_AWAY      5

// GP error codes (used in BuddyThread.h / BuddyThread.cpp)
typedef int GPErrorCode;
#define GP_GENERAL              0
#define GP_PARSE                1
#define GP_NOT_LOGGED_IN_ERR    2
#define GP_BAD_SESSKEY          3
#define GP_DATABASE             4
#define GP_NETWORK_ERR          5
#define GP_FORCED_DISCONNECT    6
#define GP_CONNECTION_CLOSED_ERR 7
#define GP_UDP_LAYER            8

// GP buddy request / message / status argument structs
typedef struct GPRecvBuddyRequestArg_s
{
    GPProfile   profile;
    unsigned    date;
    char        nick[GP_NICK_LEN];
    char        email[GP_EMAIL_LEN];
    char        countrycode[GP_COUNTRYCODE_LEN];
    wchar_t     reason[GP_REASON_LEN];
} GPRecvBuddyRequestArg;

typedef struct GPRecvBuddyMessageArg_s
{
    GPProfile   profile;
    unsigned    date;
    char        nick[GP_NICK_LEN];
    wchar_t     message[256];
} GPRecvBuddyMessageArg;

typedef struct GPRecvBuddyStatusArg_s
{
    GPProfile   profile;
    char        nick[GP_NICK_LEN];
    GPEnum      status;
    char        statusString[GP_STATUS_STRING_LEN];
    char        locationString[GP_LOCATION_STRING_LEN];
    char        countrycode[GP_COUNTRYCODE_LEN];
} GPRecvBuddyStatusArg;

typedef struct GPErrorArg_s
{
    GPResult    result;
    GPErrorCode errorCode;
    int         fatal;
    char        errorString[256];
} GPErrorArg;

// GP connection state
typedef struct GPConnectResponseArg_s
{
    GPResult    result;
    GPProfile   profile;
} GPConnectResponseArg;

typedef void (* GPConnectResponseCallback)(
    GPConnection        connection,
    GPConnectResponseArg* arg,
    void*               param);

// NOTE: GPErrorCallback and GPRecvBuddyMessageCallback are used as *function*
// names by the game code (GameSpyGP.h declares them as regular C functions).
// We must NOT typedef those names here — doing so causes C2365 redefinition.
// The callback type names get a _Func suffix to avoid the clash.
typedef void (* GPErrorCallbackFunc)(
    GPConnection connection,
    GPErrorArg*  arg,
    void*        param);

// Callback fired when a buddy message or status arrives
typedef void (* GPRecvBuddyMessageCallbackFunc)(
    GPConnection           connection,
    GPRecvBuddyMessageArg* arg,
    void*                  param);

// GameSpy chat color enum (used by GameSpyChat.h)
typedef enum
{
    GSCOLOR_DEFAULT     = 0,
    GSCOLOR_SYSTEM      = 1,
    GSCOLOR_PLAYER      = 2,
    GSCOLOR_EMOTE       = 3,
    GSCOLOR_ERROR       = 4,
    GSCOLOR_NUM_COLORS
} GameSpyColors;

// --- GP stub functions ------------------------------------------------------

// Initialise a GP connection object.  Returns GP_NO_ERROR but leaves the
// connection in a permanently-offline state.
inline GPResult gpInitialize(GPConnection* connection,
                              int           productId,
                              int           namespaceId,
                              int           sdkRevision)
{
    (void)productId; (void)namespaceId; (void)sdkRevision;
    if (connection) *connection = nullptr;
    return GP_NO_ERROR;
}

// Connect to GameSpy presence service.  Always returns network error.
inline GPResult gpConnect(GPConnection              connection,
                           const char*               email,
                           const char*               nick,
                           const char*               password,
                           int                       firewalled,
                           GPConnectResponseCallback callback,
                           void*                     param)
{
    (void)connection; (void)email; (void)nick;
    (void)password; (void)firewalled;
    if (callback)
    {
        GPConnectResponseArg arg = {};
        arg.result  = GP_NETWORK_ERROR;
        arg.profile = 0;
        callback(connection, &arg, param);
    }
    return GP_NETWORK_ERROR;
}

// Create a new GameSpy user account.  Always returns network error.
inline GPResult gpConnectNewUser(GPConnection              connection,
                                  const char*               nick,
                                  const char*               uniquenick,
                                  const char*               email,
                                  const char*               password,
                                  const char*               cdkey,
                                  int                       firewalled,
                                  GPConnectResponseCallback callback,
                                  void*                     param)
{
    (void)connection; (void)nick; (void)uniquenick; (void)email;
    (void)password; (void)cdkey; (void)firewalled;
    if (callback)
    {
        GPConnectResponseArg arg = {};
        arg.result  = GP_NETWORK_ERROR;
        arg.profile = 0;
        callback(connection, &arg, param);
    }
    return GP_NETWORK_ERROR;
}

// Disconnect and clean up.
inline void gpDisconnect(GPConnection connection) { (void)connection; }

// Destroy the GP connection object.
inline void gpDestroy(GPConnection* connection) { if(connection) *connection = nullptr; }

// Process pending GP callbacks.  Stub: nothing to process.
inline GPResult gpProcess(GPConnection connection)
{
    (void)connection;
    return GP_NO_ERROR;
}

// ===========================================================================
//  Peer — GameSpy peer-to-peer lobby and matchmaking
// ===========================================================================

typedef void* PEER;    // opaque Peer handle (NULL = not connected)
typedef int   PEERBool;// 0 = false, non-zero = true

#define PEER_TRUE  1
#define PEER_FALSE 0

// PEERJoinResult — returned in joinRoomCallback
typedef enum
{
    PEERJoinSuccess     = 0,
    PEERFullRoom        = 1,
    PEERInviteOnlyRoom  = 2,
    PEERBannedFromRoom  = 3,
    PEERBadPassword     = 4,
    PEERAlreadyInRoom   = 5,
    PEERNoTitleRoom     = 6,
    PEERJoinFailed      = 7
} PEERJoinResult;

// Room types
typedef enum
{
    TitleRoom   = 0,
    GroupRoom   = 1,
    StagingRoom = 2,
    NumRooms    = 3
} RoomType;

// Message types
typedef enum
{
    NormalMessage  = 0,
    ActionMessage  = 1,
    NoticeMessage  = 2,
    TeamMessage    = 3
} MessageType;

// SBServer — server browser server handle (opaque pointer)
typedef void* SBServer;

// Common Peer callback pointer types
typedef void (* PeerConnectCallback)       (PEER peer, PEERBool success, void* param);
typedef void (* PeerDisconnectedCallback)  (PEER peer, const char* reason, void* param);
typedef void (* PeerRoomMsgCallback)       (PEER peer, RoomType room, const char* nick,
                                             const char* msg, MessageType type, void* param);
typedef void (* PeerPlayerMsgCallback)     (PEER peer, const char* nick,
                                             const char* msg, MessageType type, void* param);
typedef void (* PeerPlayerJoinedCallback)  (PEER peer, RoomType room, const char* nick, void* param);
typedef void (* PeerPlayerLeftCallback)    (PEER peer, RoomType room, const char* nick,
                                             const char* reason, void* param);
typedef void (* PeerEnumPlayersCallback)   (PEER peer, PEERBool success, RoomType room,
                                             int index, const char* nick, int flags, void* param);
typedef void (* PeerNickErrorCallback)     (PEER peer, int type, const char* nick, void* param);
typedef void (* PeerGetKeysCallback)       (PEER peer, PEERBool success, RoomType room,
                                             const char* nick, int num,
                                             char** keys, char** values, void* param);
typedef void (* PeerListingGamesCallback)  (PEER peer, PEERBool success, PEERBool staging,
                                             SBServer server, PEERBool staging2, void* param);

// Peer callback table — callers fill this in and pass it to peerConnect
typedef struct PEERCallbacks_s
{
    PeerDisconnectedCallback disconnected;
    PeerRoomMsgCallback      roomMessage;
    PeerPlayerMsgCallback    playerMessage;
    PeerPlayerJoinedCallback playerJoined;
    PeerPlayerLeftCallback   playerLeft;
    void*                    playerChangedNick;  // unused in stub
    void*                    playerInfo;
    void*                    ready;
    void*                    gameStarted;
    void*                    autoMatch;
    void*                    autoMatchStatus;
} PEERCallbacks;

// --- Peer stub functions ----------------------------------------------------

// Connect to the Peer service.  Returns NULL (no connection).
inline PEER peerConnect(PEERCallbacks*     callbacks,
                         const char*        nick,
                         int                groupID,
                         PeerConnectCallback callback,
                         void*              param,
                         PEERBool           blocking)
{
    (void)callbacks; (void)nick; (void)groupID; (void)blocking;
    if (callback)
        callback(nullptr, PEER_FALSE, param);
    return nullptr;
}

// Disconnect and destroy a Peer connection.
inline void peerDisconnect(PEER peer) { (void)peer; }

// Process pending callbacks.  Stub: nothing to process.
inline void peerThink(PEER peer) { (void)peer; }

// Enumerate players in a room.
inline void peerEnumPlayers(PEER peer, RoomType room,
                              PeerEnumPlayersCallback callback, void* param)
{
    (void)peer; (void)room;
    // Signal end of enumeration with index=-1
    if (callback)
        callback(peer, PEER_TRUE, room, -1, nullptr, 0, param);
}

// Get key/value pairs for room or player properties.
inline void peerGetRoomKeys(PEER peer, RoomType room, const char* nick,
                              int numKeys, char** keys,
                              PeerGetKeysCallback callback, void* param)
{
    (void)peer; (void)room; (void)nick; (void)numKeys; (void)keys;
    if (callback)
        callback(peer, PEER_FALSE, room, nick, 0, nullptr, nullptr, param);
}

// --- SBServer property accessors (server browser) ---------------------------

inline const char* SBServerGetStringValue(SBServer server,
                                            const char* key,
                                            const char* defval)
{
    (void)server; (void)key;
    return defval ? defval : "";
}

inline int SBServerGetIntValue(SBServer server, const char* key, int defval)
{
    (void)server; (void)key;
    return defval;
}

inline const char* SBServerGetPlayerStringValue(SBServer server,
                                                  int      playerIndex,
                                                  const char* key,
                                                  const char* defval)
{
    (void)server; (void)playerIndex; (void)key;
    return defval ? defval : "";
}

inline int SBServerGetPlayerIntValue(SBServer server,
                                      int      playerIndex,
                                      const char* key,
                                      int      defval)
{
    (void)server; (void)playerIndex; (void)key;
    return defval;
}

// IP / port accessors (return 0.0.0.0 / port 0 in stub)
inline unsigned int SBServerGetPublicInetAddress (SBServer server) { (void)server; return 0; }
inline unsigned int SBServerGetPrivateInetAddress(SBServer server) { (void)server; return 0; }
inline unsigned short SBServerGetPublicQueryPort (SBServer server) { (void)server; return 0; }
inline unsigned short SBServerGetPrivateQueryPort(SBServer server) { (void)server; return 0; }

// ===========================================================================
//  gpersist / gstats — statistics and persistent storage
// ===========================================================================

typedef void* GPersistConn;   // opaque persistence connection

typedef int PersistType;
#define PERSIST_PRIVATE  0
#define PERSIST_PUBLIC   1

typedef void (* PersistGetDataCallback)(GPersistConn conn, int localIndex,
                                         PersistType type, int index,
                                         int success, const char* data,
                                         int len, void* param);
typedef void (* PersistSaveDataCallback)(GPersistConn conn, int localIndex,
                                          PersistType type, int index,
                                          int success, void* param);

inline GPersistConn gpPersistInit(int gameId, int secretKey,
                                   const char* hostName, const char* alternateHost,
                                   PeerConnectCallback callback, void* param)
{
    (void)gameId; (void)secretKey; (void)hostName; (void)alternateHost;
    if (callback) callback(nullptr, PEER_FALSE, param);
    return nullptr;
}

inline void gpPersistThink (GPersistConn conn) { (void)conn; }
inline void gpPersistShutdown(GPersistConn conn) { (void)conn; }

inline int gpPersistGetData(GPersistConn conn, int localIndex,
                              PersistType type, int index,
                              PersistGetDataCallback callback, void* param)
{
    (void)conn; (void)localIndex; (void)type; (void)index;
    if (callback) callback(conn, localIndex, type, index, 0, nullptr, 0, param);
    return 0;
}

inline int gpPersistSaveData(GPersistConn conn, int localIndex,
                               PersistType type, int index,
                               const char* data, int len,
                               PersistSaveDataCallback callback, void* param)
{
    (void)conn; (void)localIndex; (void)type; (void)index;
    (void)data; (void)len;
    if (callback) callback(conn, localIndex, type, index, 0, param);
    return 0;
}

// Marker so callers can #ifdef around GameSpy-specific code paths
#define GAMESPY_STUB  1   // defined when using stubs; absent with real SDK

#endif // STUB_IMPL
#endif // ZH_GAMESPY_STUB_H
