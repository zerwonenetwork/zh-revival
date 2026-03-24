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

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#endif
// On Linux/macOS: all Windows types come from compat/windows.h (force-included).
// SOCKET on non-Windows:
#ifndef _WIN32
#  ifndef ZH_COMPAT_SOCKET_DEFINED
#  define ZH_COMPAT_SOCKET_DEFINED
typedef unsigned int SOCKET;
#  define INVALID_SOCKET ((SOCKET)(~0u))
#  define SOCKET_ERROR   (-1)
#  endif
#endif
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

inline GHTTPRequest ghttpHead(
    const char*          url,
    GHTTPBool            blocking,
    GHTTPRequestCallback callback,
    void*                param)
{
    (void)url; (void)blocking;
    if (callback)
        callback(-1, GHTTPNetworkError, nullptr, 0, param);
    return -1;
}

// Process pending async HTTP requests.  Stub: nothing to process.
inline void ghttpThink(void) {}
inline void ghttpStartup(void) {}

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

typedef int   GPProfile;    // numeric profile ID
typedef int   GPEnum;       // status enum (online, away, playing, ...)
typedef int   GPResult;     // GP_NO_ERROR = 0, ...
typedef void* GPConnection; // opaque connection handle

// GPResult codes
#define GP_NO_ERROR              0
#define GP_MEMORY_ERROR          1
#define GP_PARAMETER_ERROR       2
#define GP_NETWORK_ERROR         3
#define GP_SERVER_ERROR          4
#define GP_NOT_LOGGED_IN         5
#define GP_CONNECTION_CLOSED     6
#define GP_CONNECTED             7

#define GP_BLOCKING              1
#define GP_CHECK_CACHE           1
#define GP_DONT_CHECK_CACHE      0
#define GP_FATAL                 1
#define GP_MASK_NONE             0
#define GP_FIREWALL              1
#define GP_NO_FIREWALL           0

#define GP_ERROR                 1
#define GP_RECV_BUDDY_MESSAGE    2
#define GP_RECV_BUDDY_REQUEST    3
#define GP_RECV_BUDDY_STATUS     4
#define GP_RECV_GAME_INVITE      5

// GP buddy status values
#define GP_OFFLINE   0
#define GP_ONLINE    1
#define GP_PLAYING   2
#define GP_STAGING   3
#define GP_CHATTING  4
#define GP_AWAY      5

// GP error codes (used in BuddyThread.h / BuddyThread.cpp)
// These mirror the real GameSpy GP SDK error code enumeration.
typedef int GPErrorCode;
#define GP_GENERAL                      0
#define GP_PARSE                        1
#ifndef GP_NOT_LOGGED_IN
#define GP_NOT_LOGGED_IN                2
#endif
#define GP_BAD_SESSKEY                  3
#define GP_DATABASE                     4
#define GP_NETWORK                      7
#define GP_FORCED_DISCONNECT            8
#undef GP_CONNECTION_CLOSED
#define GP_CONNECTION_CLOSED            9
#define GP_UDP_LAYER                    8
#define GP_LOGIN                       256
#define GP_LOGIN_TIMEOUT               257
#define GP_LOGIN_BAD_NICK              258
#define GP_LOGIN_BAD_EMAIL             259
#define GP_LOGIN_BAD_PASSWORD          260
#define GP_LOGIN_BAD_PROFILE           261
#define GP_LOGIN_PROFILE_DELETED       262
#define GP_LOGIN_CONNECTION_FAILED     263
#define GP_LOGIN_SERVER_AUTH_FAILED    264
#define GP_NEWUSER                     512
#define GP_NEWUSER_BAD_NICK            513
#define GP_NEWUSER_BAD_PASSWORD        514
#define GP_UPDATEUI                    768
#define GP_UPDATEUI_BAD_EMAIL          769
#define GP_NEWPROFILE                 1024
#define GP_NEWPROFILE_BAD_NICK        1025
#define GP_NEWPROFILE_BAD_OLD_NICK    1026
#define GP_UPDATEPRO                  1280
#define GP_UPDATEPRO_BAD_NICK         1281
#define GP_ADDBUDDY                   1536
#define GP_ADDBUDDY_BAD_FROM          1537
#define GP_ADDBUDDY_BAD_NEW           1538
#define GP_ADDBUDDY_ALREADY_BUDDY     1539
#define GP_AUTHADD                    1792
#define GP_AUTHADD_BAD_FROM           1793
#define GP_AUTHADD_BAD_SIG            1794
#define GP_STATUS                     2048
#define GP_BM                         2304
#define GP_BM_NOT_BUDDY               2305
#define GP_GETPROFILE                 2560
#define GP_GETPROFILE_BAD_PROFILE     2561
#define GP_DELBUDDY                   2816
#define GP_DELBUDDY_NOT_BUDDY         2817
#define GP_DELPROFILE                 3072
#define GP_DELPROFILE_LAST_PROFILE    3073
#define GP_SEARCH                     3328
#define GP_SEARCH_CONNECTION_FAILED   3329

// GP buddy request / message / status argument structs
typedef struct GPRecvBuddyRequestArg_s
{
    GPProfile   profile;
    unsigned    date;
    char        nick[GP_NICK_LEN];
    char        email[GP_EMAIL_LEN];
    char        countrycode[GP_COUNTRYCODE_LEN];
    char        reason[GP_REASON_LEN];
} GPRecvBuddyRequestArg;

typedef struct GPRecvBuddyMessageArg_s
{
    GPProfile   profile;
    unsigned    date;
    char        nick[GP_NICK_LEN];
    char        message[256];
} GPRecvBuddyMessageArg;

typedef struct GPRecvBuddyStatusArg_s
{
    GPProfile   profile;
    int         index;
    char        nick[GP_NICK_LEN];
    GPEnum      status;
    char        statusString[GP_STATUS_STRING_LEN];
    char        locationString[GP_LOCATION_STRING_LEN];
    char        countrycode[GP_COUNTRYCODE_LEN];
} GPRecvBuddyStatusArg;

typedef struct GPGetInfoResponseArg_s
{
    GPResult    result;
    GPProfile   profile;
    char        nick[GP_NICK_LEN];
    char        email[GP_EMAIL_LEN];
    char        countrycode[GP_COUNTRYCODE_LEN];
} GPGetInfoResponseArg;

typedef struct GPBuddyStatus_s
{
    GPProfile   profile;
    GPEnum      status;
    char        statusString[GP_STATUS_STRING_LEN];
    char        locationString[GP_LOCATION_STRING_LEN];
} GPBuddyStatus;

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
    GPConnection*       connection,
    GPConnectResponseArg* arg,
    void*               param);

// NOTE: GPErrorCallback and GPRecvBuddyMessageCallback are used as *function*
// names by the game code (GameSpyGP.h declares them as regular C functions).
// We must NOT typedef those names here — doing so causes C2365 redefinition.
// The callback type names get a _Func suffix to avoid the clash.
typedef void (* GPErrorCallbackFunc)(
    GPConnection* connection,
    GPErrorArg*  arg,
    void*        param);

// Callback fired when a buddy message or status arrives
typedef void (* GPRecvBuddyMessageCallbackFunc)(
    GPConnection*          connection,
    GPRecvBuddyMessageArg* arg,
    void*                  param);

typedef void (* GPCallback)(
    GPConnection* connection,
    void*         arg,
    void*         param);

// NOTE: GameSpyColors enum is defined in GameNetwork/GameSpy/PeerDefs.h which is
// always included alongside this stub.  Do NOT define it here to avoid redefinition.

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

inline GPResult gpInitialize(GPConnection* connection, int productId)
{
    return gpInitialize(connection, productId, 0, 0);
}

// Connect to GameSpy presence service.  Always returns network error.
inline GPResult gpConnect(GPConnection*             connection,
                          const char*               nick,
                          const char*               email,
                          const char*               password,
                          int                       firewalled,
                          int                       blocking,
                          GPConnectResponseCallback callback,
                          void*                     param)
{
    (void)connection; (void)nick; (void)email;
    (void)password; (void)firewalled; (void)blocking;
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
inline GPResult gpConnectNewUser(GPConnection*             connection,
                                 const char*               nick,
                                 const char*               email,
                                 const char*               password,
                                 int                       firewalled,
                                 int                       blocking,
                                 GPConnectResponseCallback callback,
                                 void*                     param)
{
    (void)connection; (void)nick; (void)email;
    (void)password; (void)firewalled; (void)blocking;
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
inline void gpDisconnect(GPConnection* connection) { (void)connection; }

// Destroy the GP connection object.
inline void gpDestroy(GPConnection* connection) { if(connection) *connection = nullptr; }

// Process pending GP callbacks.  Stub: nothing to process.
inline GPResult gpProcess(GPConnection* connection)
{
    (void)connection;
    return GP_NO_ERROR;
}

inline GPResult gpSetCallback(GPConnection* connection, GPEnum callbackType, GPCallback callback, void* param)
{
    (void)connection; (void)callbackType; (void)callback; (void)param;
    return GP_NO_ERROR;
}

inline GPResult gpDeleteProfile(GPConnection* connection)
{
    (void)connection;
    return GP_NO_ERROR;
}

inline GPResult gpSendBuddyMessage(GPConnection* connection, GPProfile profile, const char* message)
{
    (void)connection; (void)profile; (void)message;
    return GP_NETWORK_ERROR;
}

inline GPResult gpDeleteBuddy(GPConnection* connection, GPProfile profile)
{
    (void)connection; (void)profile;
    return GP_NO_ERROR;
}

inline GPResult gpAuthBuddyRequest(GPConnection* connection, GPProfile profile)
{
    (void)connection; (void)profile;
    return GP_NO_ERROR;
}

inline GPResult gpDenyBuddyRequest(GPConnection* connection, GPProfile profile)
{
    (void)connection; (void)profile;
    return GP_NO_ERROR;
}

inline GPResult gpSendBuddyRequest(GPConnection* connection, GPProfile profile, const char* reason)
{
    (void)connection; (void)profile; (void)reason;
    return GP_NETWORK_ERROR;
}

inline GPResult gpSetStatus(GPConnection* connection, GPEnum status, const char* statusString, const char* locationString)
{
    (void)connection; (void)status; (void)statusString; (void)locationString;
    return GP_NO_ERROR;
}

inline GPResult gpSetInfoMask(GPConnection* connection, GPEnum mask)
{
    (void)connection; (void)mask;
    return GP_NO_ERROR;
}

inline GPResult gpGetInfo(GPConnection* connection, GPProfile profile, int checkCache, int blocking, GPCallback callback, void* param)
{
    (void)checkCache; (void)blocking;
    if (callback) {
        GPGetInfoResponseArg arg = {};
        arg.result = GP_NETWORK_ERROR;
        arg.profile = profile;
        callback(connection, &arg, param);
    }
    return GP_NETWORK_ERROR;
}

inline GPResult gpConnectNewUser(GPConnection* connection,
                                 const char*   nick,
                                 const char*   email,
                                 const char*   password,
                                 int           firewalled,
                                 int           blocking,
                                 GPCallback    callback,
                                 void*         param)
{
    (void)connection; (void)nick; (void)email;
    (void)password; (void)firewalled; (void)blocking;
    if (callback)
    {
        GPConnectResponseArg arg = {};
        arg.result  = GP_NETWORK_ERROR;
        arg.profile = 0;
        callback(connection, &arg, param);
    }
    return GP_NETWORK_ERROR;
}

inline GPResult gpConnect(GPConnection* connection,
                          const char*   nick,
                          const char*   email,
                          const char*   password,
                          int           firewalled,
                          int           blocking,
                          GPCallback    callback,
                          void*         param)
{
    (void)connection; (void)nick; (void)email;
    (void)password; (void)firewalled; (void)blocking;
    if (callback)
    {
        GPConnectResponseArg arg = {};
        arg.result  = GP_NETWORK_ERROR;
        arg.profile = 0;
        callback(connection, &arg, param);
    }
    return GP_NETWORK_ERROR;
}

inline int gpGetBuddyIndex(GPConnection* connection, GPProfile profile)
{
    (void)connection; (void)profile;
    return -1;
}

inline GPResult gpGetBuddyStatus(GPConnection* connection, int index, GPBuddyStatus* status)
{
    (void)connection; (void)index;
    if (status) {
        status->profile = 0;
        status->status = GP_OFFLINE;
        status->statusString[0] = '\0';
        status->locationString[0] = '\0';
    }
    return GP_NO_ERROR;
}

inline GPResult gpIsConnected(GPConnection* connection)
{
    (void)connection;
    return GP_NOT_LOGGED_IN;
}

inline GPResult gpIsConnected(GPConnection* connection, GPEnum* isConnected)
{
    (void)connection;
    if (isConnected)
        *isConnected = GP_OFFLINE;
    return GP_NOT_LOGGED_IN;
}

// ===========================================================================
//  Peer — GameSpy peer-to-peer lobby and matchmaking
// ===========================================================================

typedef void* PEER;    // opaque Peer handle (NULL = not connected)
typedef int   PEERBool;// 0 = false, non-zero = true

#define PEER_TRUE  1
#define PEER_FALSE 0
#define PEERTrue   PEER_TRUE
#define PEERFalse  PEER_FALSE
#define PEER_FLAG_OP 0x00000001

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
    PEERJoinFailed      = 7,
    PEERNoConnection    = 8   // No chat connection
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

// Listing update type — used in room/game listing callbacks
typedef enum
{
    PEER_CLEAR    = 0,  // Clear the entire list
    PEER_ADD      = 1,  // Add an entry
    PEER_UPDATE   = 2,  // Update an existing entry
    PEER_REMOVE   = 3,  // Remove an entry
    PEER_COMPLETE = 4   // Listing is complete
} PEERUpdateType;

// SBServer — server browser server handle.
typedef struct SBServer_s
{
    void* keyvals;
} *SBServer;

typedef enum
{
    key_server     = 0,
    key_player     = 1,
    key_team       = 2
} qr2_key_type;

typedef enum
{
    e_qrnoerror              = 0,
    e_qrwsockerror           = 1,
    e_qrbinderror            = 2,
    e_qrdnserror             = 3,
    e_qrconnerror            = 4,
    e_qrnochallengeerror     = 5,
    e_qrsenderror            = 6,
    e_qrnothinitializederror = 7
} qr2_error_t;

typedef void* qr2_buffer_t;
typedef void* qr2_keybuffer_t;

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
typedef void (* PeerJoinRoomCallback)      (PEER peer, PEERBool success, PEERJoinResult result,
                                             RoomType roomType, void* param);
typedef void (* PeerGetProfileIDCallback)  (PEER peer, PEERBool success, const char* nick,
                                             int profileID, void* param);
typedef void (* PeerListGroupRoomsCallback)(PEER peer, PEERBool success, int groupID, SBServer server,
                                             const char* name, int numWaiting, int maxWaiting,
                                             int numGames, int numPlaying, void* param);
typedef void (* PeerAuthenticateCDKeyCallback)(PEER peer, int result, const char* message, void* param);
typedef void (* PeerListingGamesCallbackEx)(PEER peer, PEERBool success, const char* name, SBServer server,
                                             PEERBool staging, int msg, int percentListed, void* param);
typedef void (* PeerPlayerChangedNickCallback)(PEER peer, RoomType room, const char* oldNick,
                                               const char* newNick, void* param);
typedef void (* PeerPlayerFlagsChangedCallback)(PEER peer, RoomType room, const char* nick,
                                                int oldFlags, int newFlags, void* param);
typedef void (* PeerPlayerInfoCallback)(PEER peer, RoomType room, const char* nick,
                                        unsigned int IP, int profileID, void* param);
typedef void (* PeerRoomUTMCallback)(PEER peer, RoomType room, const char* nick,
                                     const char* command, const char* parameters,
                                     PEERBool authenticated, void* param);
typedef void (* PeerPlayerUTMCallback)(PEER peer, const char* sender,
                                       const char* command, const char* parameters,
                                       PEERBool authenticated, void* param);
typedef void (* PeerGlobalKeyChangedCallback)(PEER peer, const char* nick,
                                              const char* key, const char* value, void* param);
typedef void (* PeerRoomKeyChangedCallback)(PEER peer, RoomType room, const char* nick,
                                            const char* key, const char* value, void* param);
typedef void (* PeerGameStartedCallback)(PEER peer, unsigned int IP, const char* message, void* param);
typedef void (* PeerQRServerKeyCallback)(PEER peer, int keyid, qr2_buffer_t outbuf, void* param);
typedef void (* PeerQRPlayerKeyCallback)(PEER peer, int keyid, int index, qr2_buffer_t outbuf, void* param);
typedef void (* PeerQRTeamKeyCallback)(PEER peer, int keyid, int index, qr2_buffer_t outbuf, void* param);
typedef void (* PeerQRKeyListCallback)(PEER peer, qr2_key_type keytype, qr2_keybuffer_t keybuffer, void* param);
typedef int  (* PeerQRCountCallback)(PEER peer, qr2_key_type keytype, void* param);
typedef void (* PeerQRAddErrorCallback)(PEER peer, qr2_error_t error, char* errmsg, void* param);
typedef void (* PeerQRNatNegotiateCallback)(PEER peer, int cookie, void* param);
typedef void (* PeerKickedCallback)(PEER peer, RoomType room, const char* nick, const char* reason, void* param);
typedef void (* PeerNewPlayerListCallback)(PEER peer, RoomType room, void* param);

// Peer callback table — callers fill this in and pass it to peerConnect
typedef struct PEERCallbacks_s
{
    PeerDisconnectedCallback disconnected;
    PeerRoomMsgCallback      roomMessage;
    PeerPlayerMsgCallback    playerMessage;
    PeerPlayerJoinedCallback playerJoined;
    PeerPlayerLeftCallback   playerLeft;
    PeerPlayerChangedNickCallback  playerChangedNick;
    PeerPlayerFlagsChangedCallback playerFlagsChanged;
    PeerPlayerInfoCallback         playerInfo;
    PeerRoomUTMCallback            roomUTM;
    PeerPlayerUTMCallback          playerUTM;
    PeerGlobalKeyChangedCallback   globalKeyChanged;
    PeerRoomKeyChangedCallback     roomKeyChanged;
    void*                    ready;
    PeerGameStartedCallback  gameStarted;
    void*                    autoMatch;
    void*                    autoMatchStatus;
    PeerQRServerKeyCallback  qrServerKey;
    PeerQRPlayerKeyCallback  qrPlayerKey;
    PeerQRTeamKeyCallback    qrTeamKey;
    PeerQRKeyListCallback    qrKeyList;
    PeerQRCountCallback      qrCount;
    PeerQRAddErrorCallback   qrAddError;
    PeerQRNatNegotiateCallback qrNatNegotiateCallback;
    PeerKickedCallback       kicked;
    PeerNewPlayerListCallback newPlayerList;
    void*                    param;
} PEERCallbacks;

// --- Peer stub functions ----------------------------------------------------

#ifndef NUM_RESERVED_KEYS
#define NUM_RESERVED_KEYS 50
#endif
#ifndef HOSTNAME_KEY
#define HOSTNAME_KEY 1
#endif
#ifndef GAMENAME_KEY
#define GAMENAME_KEY 2
#endif
#ifndef GAMEVER_KEY
#define GAMEVER_KEY 3
#endif
#ifndef MAPNAME_KEY
#define MAPNAME_KEY 4
#endif
#ifndef PEER_STOP_REPORTING
#define PEER_STOP_REPORTING 1
#endif
#ifndef PEER_IN_USE
#define PEER_IN_USE 1
#endif

inline PEER peerInitialize(PEERCallbacks* callbacks)
{
    (void)callbacks;
    return reinterpret_cast<PEER>(1);
}

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

inline PEERBool peerConnect(PEER              peer,
                            const char*       nick,
                            int               profileID,
                            PeerNickErrorCallback nickErrorCallback,
                            PeerConnectCallback   connectCallback,
                            void*             param,
                            PEERBool          blocking)
{
    (void)peer; (void)nick; (void)profileID; (void)nickErrorCallback; (void)blocking;
    if (connectCallback)
        connectCallback(peer, PEER_FALSE, param);
    return PEER_FALSE;
}

// Disconnect and destroy a Peer connection.
inline void peerDisconnect(PEER peer) { (void)peer; }
inline void peerShutdown(PEER peer) { (void)peer; }

// Process pending callbacks.  Stub: nothing to process.
inline void peerThink(PEER peer) { (void)peer; }
inline PEERBool peerSetTitle(PEER peer, const char* gameName, const char* secretKey,
                              const char* gameName2, const char* secretKey2,
                              const char* version, int maxUpdates, PEERBool blocking,
                              const PEERBool* pingRooms, const PEERBool* crossPingRooms)
{
    (void)peer; (void)gameName; (void)secretKey; (void)gameName2; (void)secretKey2;
    (void)version; (void)maxUpdates; (void)blocking; (void)pingRooms; (void)crossPingRooms;
    return PEER_TRUE;
}
inline PEERBool peerSetTitle(PEER peer, const char* gameName, const char* secretKey,
                              const char* gameName2, const char* secretKey2,
                              unsigned int version, int maxUpdates, PEERBool blocking,
                              const PEERBool* pingRooms, const PEERBool* crossPingRooms)
{
    (void)peer; (void)gameName; (void)secretKey; (void)gameName2; (void)secretKey2;
    (void)version; (void)maxUpdates; (void)blocking; (void)pingRooms; (void)crossPingRooms;
    return PEER_TRUE;
}
inline void peerSetRoomWatchKeys(PEER peer, RoomType room, int numKeys, const char** keys, PEERBool addKeys)
{
    (void)peer; (void)room; (void)numKeys; (void)keys; (void)addKeys;
}
inline void peerSetRoomKeys(PEER peer, RoomType room, const char* nick, int numKeys, char** keys, char** values)
{
    (void)peer; (void)room; (void)nick; (void)numKeys; (void)keys; (void)values;
}
inline void peerSetRoomKeys(PEER peer, RoomType room, const char* nick, int numKeys, const char** keys, const char** values)
{
    (void)peer; (void)room; (void)nick; (void)numKeys; (void)keys; (void)values;
}
inline void peerStopGame(PEER peer) { (void)peer; }
inline void peerLeaveRoom(PEER peer, RoomType room, void* callback)
{
    (void)peer; (void)room; (void)callback;
}
inline void peerLeaveRoom(PEER peer, RoomType room, const char* reason)
{
    (void)peer; (void)room; (void)reason;
}
inline void peerJoinGroupRoom(PEER peer, int roomID, PeerJoinRoomCallback callback, void* param, PEERBool blocking)
{
    (void)roomID; (void)blocking;
    if (callback)
        callback(peer, PEER_FALSE, PEERJoinFailed, GroupRoom, param);
}
inline void peerJoinStagingRoom(PEER peer, SBServer server, const char* password, PeerJoinRoomCallback callback, void* param, PEERBool blocking)
{
    (void)server; (void)password; (void)blocking;
    if (callback)
        callback(peer, PEER_FALSE, PEERJoinFailed, StagingRoom, param);
}
inline void peerCreateStagingRoomWithSocket(PEER peer, const char* roomName, int maxPlayers,
                                            const char* password, int socketHandle, int port,
                                            PeerJoinRoomCallback callback, void* param, PEERBool blocking)
{
    (void)roomName; (void)maxPlayers; (void)password; (void)socketHandle; (void)port; (void)blocking;
    if (callback)
        callback(peer, PEER_FALSE, PEERJoinFailed, StagingRoom, param);
}
inline void peerStartListingGames(PEER peer, const unsigned char* keys, int numKeys,
                                  const char* filter, PeerListingGamesCallback callback, void* param)
{
    (void)keys; (void)numKeys; (void)filter;
    if (callback)
        callback(peer, PEER_FALSE, PEER_FALSE, nullptr, PEER_FALSE, param);
}
inline void peerStartListingGames(PEER peer, const unsigned char* keys, int numKeys,
                                  const char* filter, PeerListingGamesCallbackEx callback, void* param)
{
    (void)keys; (void)numKeys; (void)filter;
    if (callback)
        callback(peer, PEER_FALSE, nullptr, nullptr, PEER_FALSE, PEER_COMPLETE, 0, param);
}
inline void peerStopListingGames(PEER peer) { (void)peer; }
inline PEERBool peerStartGame(PEER peer, void* reportingData, int reportMode)
{
    (void)peer; (void)reportingData; (void)reportMode;
    return PEER_FALSE;
}
inline void peerUTMPlayer(PEER peer, const char* nick, const char* command, const char* message, PEERBool reliable)
{
    (void)peer; (void)nick; (void)command; (void)message; (void)reliable;
}
inline void peerUTMRoom(PEER peer, RoomType room, const char* command, const char* message, PEERBool reliable)
{
    (void)peer; (void)room; (void)command; (void)message; (void)reliable;
}
inline void peerUpdateGame(PEER peer, SBServer server, PEERBool broadcast)
{
    (void)peer; (void)server; (void)broadcast;
}
inline void peerStateChanged(PEER peer) { (void)peer; }
inline PEERBool peerIsConnected(PEER peer)
{
    (void)peer;
    return PEER_FALSE;
}
inline void peerGetPlayerProfileID(PEER peer, const char* nick, PeerGetProfileIDCallback callback, void* param, PEERBool blocking)
{
    (void)nick; (void)blocking;
    if (callback)
        callback(peer, PEER_FALSE, nick, 0, param);
}
inline void peerGetPlayerFlags(PEER peer, const char* nick, RoomType room, int* flags)
{
    (void)peer; (void)nick; (void)room;
    if (flags)
        *flags = 0;
}
inline void peerMessagePlayer(PEER peer, const char* nick, const char* message, MessageType type)
{
    (void)peer; (void)nick; (void)message; (void)type;
}
inline void peerMessageRoom(PEER peer, RoomType room, const char* message, MessageType type)
{
    (void)peer; (void)room; (void)message; (void)type;
}

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

inline void peerGetRoomKeys(PEER peer, RoomType room, const char* nick,
                              int numKeys, const char** keys,
                              PeerGetKeysCallback callback, void* param, PEERBool blocking)
{
    (void)peer; (void)room; (void)nick; (void)numKeys; (void)keys; (void)blocking;
    if (callback)
        callback(peer, PEER_FALSE, room, nick, 0, nullptr, nullptr, param);
}

inline void peerAuthenticateCDKey(PEER peer, const char* cdkey, PeerAuthenticateCDKeyCallback callback, void* param, PEERBool blocking)
{
    (void)peer; (void)cdkey; (void)blocking;
    if (callback)
        callback(peer, 0, "offline", param);
}

inline void peerParseQuery(PEER peer, char* query, int len, void* from)
{
    (void)peer; (void)query; (void)len; (void)from;
}

inline unsigned int peerGetLocalIP(PEER peer)
{
    (void)peer;
    return 0;
}

inline void peerListGroupRooms(PEER peer, void* filter, PeerListGroupRoomsCallback callback, void* param, PEERBool blocking)
{
    (void)peer; (void)filter; (void)blocking;
    if (callback)
        callback(peer, PEER_FALSE, 0, nullptr, nullptr, 0, 0, 0, 0, param);
}

inline void peerRetryWithNick(PEER peer, const char* nick)
{
    (void)peer; (void)nick;
}

inline void peerGetPlayerInfoNoWait(PEER peer, const char* nick, unsigned int* IP, int* profileID)
{
    (void)peer; (void)nick;
    if (IP)
        *IP = 0;
    if (profileID)
        *profileID = 0;
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
inline int SBServerHasBasicKeys(SBServer server) { (void)server; return 0; }
inline int SBServerHasFullKeys(SBServer server) { (void)server; return 0; }

// ===========================================================================
//  gpersist / gstats — statistics and persistent storage
// ===========================================================================

typedef void* GPersistConn;   // opaque persistence connection

typedef int PersistType;
typedef int persisttype_t;
#define PERSIST_PRIVATE  0
#define PERSIST_PUBLIC   1
#define GE_NOERROR       0
#define pd_public_ro     0
#define pd_public_rw     1

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

inline int InitStatsConnection(int localProfileId)
{
    (void)localProfileId;
    return 0;
}

inline void chatSetLocalIP(unsigned int localIP)
{
    (void)localIP;
}

inline char gcd_gamename[32] = {0};
inline char gcd_secret_key[32] = {0};

#ifndef SNAP_FINAL
#define SNAP_FINAL 1
#endif

inline void NewGame(int gameID)
{
    (void)gameID;
}

inline int SendGameSnapShot(void* gameHandle, const char* results, int snapshotType)
{
    (void)gameHandle; (void)results; (void)snapshotType;
    return 0;
}

inline void FreeGame(void* gameHandle)
{
    (void)gameHandle;
}

inline int IsStatsConnected()
{
    return 0;
}

inline void PersistThink()
{
}

inline void CloseStatsConnection()
{
}

inline const char* GetChallenge(const char* unused)
{
    (void)unused;
    return "";
}

inline void GenerateAuth(const char* challenge, char* input, char* output)
{
    (void)challenge; (void)input;
    if (output) {
        output[0] = '\0';
    }
}

typedef void (* PersistAuthCallback)(int localid, int profileid, int authenticated, char* errmsg, void* instance);
typedef void (* PersistSetDataCallback)(int localid, int profileid, persisttype_t type, int index, int success, void* instance);
typedef void (* PersistGetValuesCallback)(int localid, int profileid, persisttype_t type, int index, int success, char* data, int len, void* instance);

inline void PreAuthenticatePlayerPM(int localid, int profileid, const char* validate, PersistAuthCallback callback, void* instance)
{
    (void)localid; (void)profileid; (void)validate;
    if (callback) {
        char errmsg[] = "";
        callback(localid, profileid, 0, errmsg, instance);
    }
}

inline void PreAuthenticatePlayerCD(int localid, const char* keyType, const char* cdkeyHash, const char* validationToken, PersistAuthCallback callback, void* instance)
{
    (void)localid; (void)keyType; (void)cdkeyHash; (void)validationToken;
    if (callback) {
        char errmsg[] = "";
        callback(localid, 0, 0, errmsg, instance);
    }
}

inline void GetPersistDataValues(int localid, int profileid, persisttype_t type, int index, const char* keyFilter, PersistGetValuesCallback callback, void* instance)
{
    (void)keyFilter;
    if (callback) {
        char emptyData[] = "";
        callback(localid, profileid, type, index, 0, emptyData, 0, instance);
    }
}

inline void SetPersistDataValues(int localid, int profileid, persisttype_t type, int index, char* data, PersistSetDataCallback callback, void* instance)
{
    (void)data;
    if (callback) {
        callback(localid, profileid, type, index, 0, instance);
    }
}

// ===========================================================================
//  QR2 (Query & Reporting 2) stubs
//  Used by PeerThread.cpp for server-key/player-key reporting callbacks.
// ===========================================================================


// Registered key list (stub — always empty)
static const char* qr2_registered_key_list[] = { "" };

// QR2 buffer helpers — no-ops in the stub
inline void qr2_buffer_add    (qr2_buffer_t    buf, const char* value) { (void)buf; (void)value; }
inline void qr2_buffer_add_int(qr2_buffer_t    buf, int value)          { (void)buf; (void)value; }
inline void qr2_keybuffer_add (qr2_keybuffer_t buf, int key_id)         { (void)buf; (void)key_id; }

// QR2 key registration — no-op in stub
inline void qr2_register_key(int key_id, const char* key_name) { (void)key_id; (void)key_name; }

// Marker so callers can #ifdef around GameSpy-specific code paths
#define GAMESPY_STUB  1   // defined when using stubs; absent with real SDK

#endif // STUB_IMPL
#endif // ZH_GAMESPY_STUB_H

