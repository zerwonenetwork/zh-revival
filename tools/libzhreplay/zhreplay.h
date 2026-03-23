/*
 * libzhreplay — Zero Hour replay file parser
 *
 * This project is not affiliated with or endorsed by Electronic Arts.
 * Command & Conquer is a trademark of Electronic Arts.
 * You must own the original game to use this software.
 *
 * Public C API. See docs/REPLAY_FORMAT.md for the format specification.
 */

#ifndef ZHREPLAY_H
#define ZHREPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* -------------------------------------------------------------------------
 * Opaque handle
 * ---------------------------------------------------------------------- */
typedef struct ZHReplay ZHReplay;

/* -------------------------------------------------------------------------
 * Error codes returned by zh_replay_open and query functions
 * ---------------------------------------------------------------------- */
typedef enum ZHReplayError {
    ZH_OK              =  0,
    ZH_ERR_NULL        = -1,   /* null argument */
    ZH_ERR_IO          = -2,   /* file could not be opened / read */
    ZH_ERR_MAGIC       = -3,   /* magic bytes not "GENREP" */
    ZH_ERR_TRUNCATED   = -4,   /* file ended before header was complete */
    ZH_ERR_PARSE       = -5,   /* malformed field (e.g. bad string) */
    ZH_ERR_VERSION     = -6,   /* unrecognised format version */
} ZHReplayError;

/* -------------------------------------------------------------------------
 * Player info extracted from the GameInfo slot string
 * ---------------------------------------------------------------------- */
#define ZH_MAX_PLAYERS 8
#define ZH_NAME_LEN    128

typedef struct ZHPlayer {
    char     name[ZH_NAME_LEN];   /* UTF-8 encoded display name */
    char     faction[ZH_NAME_LEN];/* faction/side identifier */
    int      slot;                /* 0-based slot index */
    int      is_ai;               /* 1 if this is a computer player */
    int      is_local;            /* 1 if this was the recording player */
    uint32_t ip;                  /* IP as uint32 (0 for AI / unknown) */
} ZHPlayer;

/* -------------------------------------------------------------------------
 * Replay header — all fields that can be extracted without running the sim
 * ---------------------------------------------------------------------- */
typedef struct ZHReplayHeader {
    /* file identity */
    int      valid;              /* 1 if header parsed successfully */
    int      error_code;        /* ZHReplayError if !valid */

    /* timestamps */
    int64_t  start_time_unix;   /* seconds since epoch; 0 if unknown */
    int64_t  end_time_unix;     /* seconds since epoch; 0 if unknown */
    uint32_t frame_duration;    /* total ticks recorded */

    /* outcome flags */
    int      desync_detected;   /* 1 if CRC mismatch was logged */
    int      quit_early;        /* 1 if game ended before natural finish */
    int      player_disconnected[ZH_MAX_PLAYERS]; /* per-slot disconnect flag */

    /* version */
    char     version_string[256];      /* e.g. "1.04 ZH" */
    char     version_time_string[256]; /* build timestamp string */
    uint32_t version_number;
    uint32_t exe_crc;
    uint32_t ini_crc;

    /* game settings */
    int      difficulty;         /* GameDifficulty enum value */
    int      game_mode;          /* originalGameMode enum value */
    int      rank_points;
    int      max_fps;

    /* players */
    int      num_players;
    ZHPlayer players[ZH_MAX_PLAYERS];
    int      local_player_index; /* -1 if unknown/single-player */

    /* raw GameInfo string (null-terminated ASCII) */
    char     game_options[4096];

    /* wall-clock date/time from SYSTEMTIME struct */
    uint16_t date_year;
    uint16_t date_month;   /* 1–12 */
    uint16_t date_day;
    uint16_t time_hour;
    uint16_t time_minute;
    uint16_t time_second;
} ZHReplayHeader;

/* -------------------------------------------------------------------------
 * Command argument types (mirrors ARGUMENTDATATYPE_* enum in engine)
 * ---------------------------------------------------------------------- */
typedef enum ZHArgType {
    ZH_ARG_INTEGER     = 0,
    ZH_ARG_REAL        = 1,
    ZH_ARG_BOOLEAN     = 2,
    ZH_ARG_OBJECTID    = 3,
    ZH_ARG_DRAWABLEID  = 4,
    ZH_ARG_TEAMID      = 5,
    ZH_ARG_LOCATION    = 6,  /* Coord3D: 3 floats */
    ZH_ARG_PIXEL       = 7,  /* ICoord2D: 2 ints */
    ZH_ARG_PIXELREGION = 8,  /* IRegion2D: 4 ints */
    ZH_ARG_TIMESTAMP   = 9,
    ZH_ARG_WIDECHAR    = 10,
    ZH_ARG_UNKNOWN     = 11,
} ZHArgType;

typedef struct ZHCoord3D    { float x, y, z; }                    ZHCoord3D;
typedef struct ZHICoord2D   { int32_t x, y; }                     ZHICoord2D;
typedef struct ZHIRegion2D  { int32_t lo_x, lo_y, hi_x, hi_y; }  ZHIRegion2D;

typedef union ZHArgValue {
    int32_t    integer;
    float      real;
    int32_t    boolean;    /* 0 or 1 */
    uint32_t   object_id;
    uint32_t   drawable_id;
    uint32_t   team_id;
    ZHCoord3D  location;
    ZHICoord2D pixel;
    ZHIRegion2D pixel_region;
    uint32_t   timestamp;
    uint16_t   wide_char;
} ZHArgValue;

typedef struct ZHArg {
    ZHArgType  type;
    ZHArgValue value;
} ZHArg;

/* Maximum arguments per command — engine allows up to 255 type-descriptor
 * pairs × 255 args each, but practical commands never exceed ~16.
 * We cap at 64 to bound stack usage; commands with more args are skipped. */
#define ZH_MAX_ARGS 64

/* -------------------------------------------------------------------------
 * A single recorded command
 * ---------------------------------------------------------------------- */
typedef struct ZHCommand {
    uint32_t frame;        /* simulation tick */
    int32_t  type;         /* GameMessage::Type */
    int32_t  player_index; /* 0-based slot */
    int      num_args;
    ZHArg    args[ZH_MAX_ARGS];
} ZHCommand;

/* -------------------------------------------------------------------------
 * API
 * ---------------------------------------------------------------------- */

/*
 * Open and parse the header of a replay file.
 * Returns NULL on allocation failure (extremely rare).
 * Check zh_replay_valid() after calling this.
 */
ZHReplay *zh_replay_open(const char *path);

/*
 * Returns 1 if the header parsed successfully, 0 on error.
 * Always safe to call even if zh_replay_open returned a non-NULL pointer.
 */
int zh_replay_valid(const ZHReplay *r);

/*
 * Returns the last error code (ZHReplayError).
 */
ZHReplayError zh_replay_error(const ZHReplay *r);

/*
 * Fills *header_out with the parsed header.
 * Returns ZH_OK on success, ZH_ERR_NULL if r is NULL.
 */
ZHReplayError zh_replay_header(const ZHReplay *r, ZHReplayHeader *header_out);

/*
 * Reads the next command from the command stream into *cmd_out.
 * Returns ZH_OK on success.
 * Returns ZH_ERR_TRUNCATED on clean EOF.
 * Returns ZH_ERR_IO on read error.
 * Returns ZH_ERR_PARSE on malformed command (command is skipped).
 * Call repeatedly until it returns non-ZH_OK to iterate all commands.
 */
ZHReplayError zh_replay_next_command(ZHReplay *r, ZHCommand *cmd_out);

/*
 * Rewind the command stream to the start (re-opens file internally).
 */
ZHReplayError zh_replay_rewind(ZHReplay *r);

/*
 * Close the file and free all memory.
 */
void zh_replay_close(ZHReplay *r);

/*
 * Return a human-readable string for an error code.
 */
const char *zh_replay_error_string(ZHReplayError err);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ZHREPLAY_H */
