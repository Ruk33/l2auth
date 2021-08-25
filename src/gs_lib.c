#include <assert.h>
#include <math.h>
#include <time.h>

#include "storage/sqlite.c"
#include "conn.c"
#include "gs_ai.c"
#include "gs_character_template.c"
#include "gs_character.c"
#include "gs_client_packets.c"
#include "gs_crypt.c"
#include "gs_server_packets.c"
#include "gs_request.c"
#include "gs_session.c"
#include "l2_string.c"
#include "list.c"
#include "packet.c"
#include "recycle_id.c"
#include "util.c"

#include "include/config.h"
#include "include/gs_lib.h"

static gs_lib_t *lib = 0;

void gs_lib_load(gs_lib_t *gs_lib)
{
        if (!gs_lib) {
                log_normal("no gs lib passed to gs_lib_load. ignoring.");
                return;
        }

        if (!gs_lib->send_response) {
                log_normal("gs lib without send_response cb. ignoring.");
                return;
        }

        lib = gs_lib;

        if (lib->state.game_ticks == 0) {
                lib->state.game_ticks      = 3600000 / MILLIS_IN_TICK;
                lib->state.game_start_time = time(0) * 1000 - 3600000;
        }

        conn_set_send_response(gs_lib->send_response);
}

void gs_lib_unload(void)
{
}

void gs_lib_new_conn(struct os_io *socket)
{
        if (!lib) {
                log_normal("lib is not initialized. ignoring new con.");
                return;
        }

        if (!socket) {
                log_normal("new conn without socket? ignoring.");
                return;
        }

        gs_request_new_conn(&lib->state, socket);
}

void gs_lib_new_req(struct os_io *socket, void *buf, size_t n)
{
        if (!lib) {
                log_normal("lib is not initialized. ignoring new request.");
                return;
        }

        if (!socket) {
                log_normal("new request without socket? ignoring.");
                return;
        }

        gs_request(&lib->state, socket, buf, n);
}

void gs_lib_disconnect(struct os_io *socket)
{
        if (!lib) {
                log_normal("lib is not initialized. ignoring disconnect.");
                return;
        }

        if (!socket) {
                log_normal("disconnect request without socket? ignoring.");
                return;
        }

        log_normal("client disconnected.");
        gs_request_disconnect(&lib->state, socket);
}

void gs_lib_tick(double delta)
{
        u64_t old_ticks = 0;
        u64_t run_time  = 0;

        if (!lib) {
                log_normal("lib is not initialized. ignoring tick.");
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
