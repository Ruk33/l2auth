#include <assert.h>
#include <time.h>

#include "storage/sqlite.c"
#include "gs_ai.c"
#include "gs_character.c"
#include "gs_client_packets.c"
#include "gs_server_packets.c"
#include "gs_request.c"
#include "gs_session.c"
#include "l2_string.c"
#include "packet.c"
#include "util.c"

#include "include/config.h"
#include "include/gs_lib.h"

static struct gs_state *g_gs = 0;

void gs_lib_load(struct gs_state *gs)
{
    if (!gs) {
        log_normal("no game state passed to gs_lib_load. ignoring.");
        return;
    }

    if (!gs->send_response) {
        log_normal("gs state without send_response callback. ignoring.");
        return;
    }

    g_gs = gs;

    gs_request_initialize_server(g_gs);

    if (g_gs->game_ticks == 0) {
        g_gs->game_ticks      = 3600000 / MILLIS_IN_TICK;
        g_gs->game_start_time = time(0) * 1000 - 3600000;
    }
}

void gs_lib_unload(void)
{
}

void gs_lib_new_conn(struct platform_socket *socket)
{
    if (!g_gs) {
        log_normal("game state not initialized. ignoring new con.");
        return;
    }

    if (!socket) {
        log_normal("new conn without socket? ignoring.");
        return;
    }

    gs_request_new_conn(g_gs, socket);
}

void gs_lib_new_req(struct platform_socket *socket, void *buf, size_t n)
{
    if (!g_gs) {
        log_normal("game state not initialized. ignoring new request.");
        return;
    }

    if (!socket) {
        log_normal("new request without socket? ignoring.");
        return;
    }

    gs_request(g_gs, socket, buf, n);
}

void gs_lib_disconnect(struct platform_socket *socket)
{
    if (!g_gs) {
        log_normal("game state not initialized. ignoring disconnect.");
        return;
    }

    if (!socket) {
        log_normal("disconnect request without socket? ignoring.");
        return;
    }

    log_normal("client disconnected.");
    gs_request_disconnect(g_gs, socket);
}

void gs_lib_tick(double delta)
{
    // u64_t old_ticks = 0;
    u64_t run_time = 0;

    if (!g_gs) {
        log_normal("game state not initialized. ignoring tick.");
        return;
    }

    /*
     * Todo: refactor
     * Thanks to the L2J project for providing this first iteration of the
     * code.
     */

    // old_ticks        = g_gs->game_ticks;
    run_time         = time(0) * 1000 - g_gs->game_start_time;
    g_gs->game_ticks = (u64_t) (run_time / MILLIS_IN_TICK);

    // if (old_ticks != g_gs->game_ticks) {
    gs_request_tick(g_gs, delta);
    // }
}
