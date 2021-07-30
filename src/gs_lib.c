#include <assert.h>
#include <math.h>

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
#include "gs_packet_leave_world.c"
#include "gs_packet_move_request.c"
#include "gs_packet_move.c"
#include "gs_packet_new_char.c"
#include "gs_packet_npc_info.c"
#include "gs_packet_protocol_version.c"
#include "gs_packet_quest_list.c"
#include "gs_packet_target_selected.c"
#include "gs_packet_user_info.c"
#include "gs_packet_validate_pos_request.c"
#include "gs_packet_validate_pos.c"
#include "gs_random_id.c"
#include "gs_request.c"
#include "gs_session.c"
#include "l2_string.c"
#include "packet.c"
#include "recycle_id.c"
#include "util.c"

#include "include/gs_lib.h"

void gs_lib_load(gs_lib_t *gs_lib)
{
        assert(gs_lib);
        conn_set_cb(gs_lib->send_response);
        gs_session_set(gs_lib->sessions, &gs_lib->session_count);
        gs_character_set(gs_lib->characters, &gs_lib->character_count);
}

void gs_lib_unload(void)
{
}

void gs_lib_new_conn(os_io_t *socket)
{
        assert(socket);
        gs_request_new_conn(socket);
}

void gs_lib_new_req(os_io_t *socket, void *buf, size_t n)
{
        if (!socket) {
                return;
        }

        gs_request(socket, buf, n);
}

void gs_lib_disconnect(os_io_t *socket)
{
        log("client disconnected.");

        if (!socket) {
                log("disconnected client has no socket?");
                return;
        }

        gs_request_disconnect(socket);
}

void gs_lib_tick(double delta)
{
        gs_request_tick(delta);
}
