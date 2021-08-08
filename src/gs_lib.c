#include <assert.h>
#include <math.h>
#include <time.h>

#include "storage/sqlite.c"
#include "conn.c"
#include "gs_ai.c"
#include "gs_character_template.c"
#include "gs_character.c"
#include "gs_crypt.c"
#include "gs_packet_action_request.c"
#include "gs_packet_attack.c"
#include "gs_packet_auth_login.c"
#include "gs_packet_auth_request.c"
#include "gs_packet_auto_attack.c"
#include "gs_packet_char_info.c"
#include "gs_packet_char_select_request.c"
#include "gs_packet_char_select.c"
#include "gs_packet_create_char_request.c"
#include "gs_packet_create_char.c"
#include "gs_packet_d0.c"
#include "gs_packet_die.c"
#include "gs_packet_leave_world.c"
#include "gs_packet_move_request.c"
#include "gs_packet_move.c"
#include "gs_packet_new_char.c"
#include "gs_packet_npc_info.c"
#include "gs_packet_protocol_version.c"
#include "gs_packet_quest_list.c"
#include "gs_packet_revive_request.c"
#include "gs_packet_revive.c"
#include "gs_packet_status.c"
#include "gs_packet_restart.c"
#include "gs_packet_target_selected.c"
#include "gs_packet_user_info.c"
#include "gs_packet_validate_pos_request.c"
#include "gs_packet_validate_pos.c"
#include "gs_random_id.c"
#include "gs_request.c"
#include "gs_session.c"
#include "l2_string.c"
#include "list.c"
#include "packet.c"
#include "recycle_id.c"
#include "util.c"

#include "include/config.h"
#include "include/gs_lib.h"
#include "include/log.h"

static gs_lib_t *lib = 0;

void gs_lib_load(gs_lib_t *gs_lib)
{
        if (!gs_lib) {
                log("no gs lib passed to gs_lib_load. ignoring.");
                return;
        }

        if (!gs_lib->send_response) {
                log("gs lib without send_response cb. ignoring.");
                return;
        }

        lib = gs_lib;

        if (lib->state.game_ticks == 0) {
                lib->state.game_ticks      = 3600000 / MILLIS_IN_TICK;
                lib->state.game_start_time = time(0) * 1000 - 3600000;
        }

        conn_set_cb(gs_lib->send_response);
}

void gs_lib_unload(void)
{
}

void gs_lib_new_conn(struct os_io *socket)
{
        if (!lib) {
                log("lib is not initialized. ignoring new con.");
                return;
        }

        if (!socket) {
                log("new conn without socket? ignoring.");
                return;
        }

        gs_request_new_conn(&lib->state, socket);
}

void gs_lib_new_req(struct os_io *socket, void *buf, size_t n)
{
        if (!lib) {
                log("lib is not initialized. ignoring new request.");
                return;
        }

        if (!socket) {
                log("new request without socket? ignoring.");
                return;
        }

        gs_request(&lib->state, socket, buf, n);
}

void gs_lib_disconnect(struct os_io *socket)
{
        if (!lib) {
                log("lib is not initialized. ignoring disconnect.");
                return;
        }

        if (!socket) {
                log("disconnect request without socket? ignoring.");
                return;
        }

        log("client disconnected.");
        gs_request_disconnect(&lib->state, socket);
}

void gs_lib_tick(double delta)
{
        u64_t old_ticks = 0;
        u64_t run_time  = 0;

        if (!lib) {
                log("lib is not initialized. ignoring tick.");
                return;
        }

        /*
         * Todo: refactor
         * Thanks to the L2J project for providing this first iteration of the
         * code.
         */

        old_ticks             = lib->state.game_ticks;
        run_time              = time(0) * 1000 - lib->state.game_start_time;
        lib->state.game_ticks = (u64_t)(run_time / MILLIS_IN_TICK);

        if (old_ticks != lib->state.game_ticks) {
                gs_request_tick(&lib->state, delta);
        }
}
