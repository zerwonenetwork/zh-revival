/*
 * zh-replay-info — Zero Hour replay metadata extractor (JSON output)
 *
 * This project is not affiliated with or endorsed by Electronic Arts.
 * Command & Conquer is a trademark of Electronic Arts.
 * You must own the original game to use this software.
 *
 * Usage: zh-replay-info <file.rep>
 *
 * Outputs strict JSON to stdout, no other text.
 * Exit codes:
 *   0  success (valid or invalid replay — JSON output in both cases)
 *   2  tool error (no argument, cannot allocate)
 *
 * Schema:
 * {
 *   "schema_version": 1,
 *   "replay_version": <int or null>,
 *   "map_hash": null,                     (not stored in replay header)
 *   "map_name": null,                     (map name write was commented out)
 *   "date_iso": "<ISO8601 or null>",
 *   "duration_ticks": <int or null>,
 *   "players": [{"name":"...","faction":"...","slot":<int>,"is_ai":<bool>}],
 *   "winner": null,                        (not determinable from header alone)
 *   "build_version": "<string or null>"
 * }
 */

#include "../libzhreplay/zhreplay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Escape a string for JSON output */
static void json_string(FILE *f, const char *s)
{
    fputc('"', f);
    if (s == NULL) {
        fputc('"', f);
        return;
    }
    for (; *s; ++s) {
        unsigned char c = (unsigned char)*s;
        if (c == '"')       fputs("\\\"", f);
        else if (c == '\\') fputs("\\\\", f);
        else if (c == '\n') fputs("\\n",  f);
        else if (c == '\r') fputs("\\r",  f);
        else if (c == '\t') fputs("\\t",  f);
        else if (c < 0x20)  fprintf(f, "\\u%04x", c);
        else                fputc(c, f);
    }
    fputc('"', f);
}

static void print_error_json(void)
{
    printf("{\"error\":\"invalid replay file\",\"schema_version\":1}\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: zh-replay-info <file.rep>\n");
        return 2;
    }

    const char *path = argv[1];
    ZHReplay *r = zh_replay_open(path);
    if (r == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        return 2;
    }

    if (!zh_replay_valid(r)) {
        zh_replay_close(r);
        print_error_json();
        return 0;
    }

    ZHReplayHeader h;
    zh_replay_header(r, &h);

    /* Count commands for duration cross-check */
    unsigned long command_count = 0;
    ZHCommand cmd;
    while (zh_replay_next_command(r, &cmd) == ZH_OK)
        ++command_count;

    zh_replay_close(r);

    /* Build date_iso string */
    char date_iso[32] = "";
    int has_date = (h.date_year > 0);
    if (has_date) {
        snprintf(date_iso, sizeof(date_iso),
            "%04u-%02u-%02uT%02u:%02u:%02uZ",
            h.date_year, h.date_month, h.date_day,
            h.time_hour, h.time_minute, h.time_second);
    }

    /* Output JSON */
    printf("{\n");
    printf("  \"schema_version\": 1,\n");

    /* replay_version: use versionNumber if non-zero */
    if (h.version_number != 0)
        printf("  \"replay_version\": %u,\n", h.version_number);
    else
        printf("  \"replay_version\": null,\n");

    /* map_hash: not stored in header */
    printf("  \"map_hash\": null,\n");

    /* map_name: write was commented out in engine */
    printf("  \"map_name\": null,\n");

    /* date_iso */
    if (has_date) {
        printf("  \"date_iso\": ");
        json_string(stdout, date_iso);
        printf(",\n");
    } else {
        printf("  \"date_iso\": null,\n");
    }

    /* duration_ticks */
    if (h.frame_duration > 0)
        printf("  \"duration_ticks\": %u,\n", h.frame_duration);
    else
        printf("  \"duration_ticks\": null,\n");

    /* players array */
    printf("  \"players\": [");
    int first = 1;
    for (int i = 0; i < ZH_MAX_PLAYERS; ++i) {
        /* Only emit slots that appear in the game_options string */
        char key[8];
        snprintf(key, sizeof(key), "S%d=", i);
        if (strstr(h.game_options, key) == NULL)
            continue;
        if (!first) printf(", ");
        first = 0;
        printf("{\"name\":");
        json_string(stdout, h.players[i].name[0] ? h.players[i].name : NULL);
        printf(",\"faction\":");
        json_string(stdout, h.players[i].faction[0] ? h.players[i].faction : NULL);
        printf(",\"slot\":%d", i);
        printf(",\"is_ai\":%s}", h.players[i].is_ai ? "true" : "false");
    }
    printf("],\n");

    /* winner: not determinable from header alone */
    printf("  \"winner\": null,\n");

    /* build_version */
    if (h.version_string[0]) {
        printf("  \"build_version\": ");
        json_string(stdout, h.version_string);
        printf("\n");
    } else {
        printf("  \"build_version\": null\n");
    }

    printf("}\n");
    return 0;
}
