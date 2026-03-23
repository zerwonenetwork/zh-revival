/*
 * zh-replay-validate — Zero Hour replay file validator
 *
 * This project is not affiliated with or endorsed by Electronic Arts.
 * Command & Conquer is a trademark of Electronic Arts.
 * You must own the original game to use this software.
 *
 * Usage: zh-replay-validate <file.rep>
 *
 * Exit codes:
 *   0  valid replay
 *   1  invalid replay (bad magic, truncated, parse error)
 *   2  tool error (no file argument, file not found)
 */

#include "../libzhreplay/zhreplay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: zh-replay-validate <file.rep>\n");
        return 2;
    }

    const char *path = argv[1];
    ZHReplay *r = zh_replay_open(path);
    if (r == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        return 2;
    }

    if (!zh_replay_valid(r)) {
        ZHReplayError err = zh_replay_error(r);
        fprintf(stdout, "INVALID: %s\n", zh_replay_error_string(err));
        zh_replay_close(r);
        return 1;
    }

    ZHReplayHeader h;
    zh_replay_header(r, &h);

    /* Walk the command stream to count commands and detect truncation */
    unsigned long command_count = 0;
    ZHReplayError cmd_err = ZH_OK;
    ZHCommand cmd;
    while ((cmd_err = zh_replay_next_command(r, &cmd)) == ZH_OK) {
        ++command_count;
    }

    zh_replay_close(r);

    if (cmd_err == ZH_ERR_IO || cmd_err == ZH_ERR_PARSE) {
        fprintf(stdout,
            "INVALID: command stream error after %lu commands: %s\n",
            command_count,
            zh_replay_error_string(cmd_err));
        return 1;
    }

    /* cmd_err == ZH_ERR_TRUNCATED means clean EOF — that is normal */
    char duration_str[32] = "?";
    if (h.frame_duration > 0) {
        /* ZH runs at ~30 FPS nominal tick rate */
        unsigned int secs = h.frame_duration / 30;
        snprintf(duration_str, sizeof(duration_str), "%us", secs);
    }

    char player_str[256] = "";
    if (h.num_players >= 2) {
        /* Print first two player names if available */
        snprintf(player_str, sizeof(player_str), "%s vs %s",
            h.players[0].name[0] ? h.players[0].name : "(unknown)",
            h.players[1].name[0] ? h.players[1].name : "(unknown)");
    } else {
        snprintf(player_str, sizeof(player_str), "(single-player)");
    }

    fprintf(stdout,
        "VALID: %s — %lu commands — %s\n",
        player_str,
        command_count,
        duration_str);
    return 0;
}
