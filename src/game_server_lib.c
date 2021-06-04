#include "include/util.h"
#include "include/socket.h"
#include "include/conn.h"
#include "include/storage.h"
#include "include/gs_session.h"
#include "include/game_server_request.h"
#include "include/game_server_lib.h"

void game_server_lib_load(
        conn_send_response_cb send_response_cb,
        gs_session_save_cb save_sessions_cb)
{
        conn_set_cb(send_response_cb);
        storage_open();
        gs_session_set_save_cb(save_sessions_cb);
}

void game_server_lib_unload(void)
{
        storage_close();
        gs_sessions_save();
}

void game_server_lib_load_sessions(void *sessions, size_t n)
{
        if (!sessions || !n) {
                return;
        }
        gs_session_load(sessions, n);
}

void game_server_lib_new_conn(socket_t *socket)
{
        game_server_request_new_conn(socket);
}

void game_server_lib_new_req(socket_t *socket, byte_t *buf, size_t n)
{
        game_server_request(socket, buf, n);
}

void game_server_lib_disconnect(socket_t *socket)
{
        game_server_request_disconnect(socket);
}
