#ifndef INCLUDE_GAME_SERVER_LIB_H
#define INCLUDE_GAME_SERVER_LIB_H

#include "util.h"
#include "socket.h"
#include "conn.h"
#include "gs_session.h"

void game_server_lib_load(
        conn_send_response_cb cb,
        gs_session_save_cb save_sessions_cb);

void game_server_lib_unload(void);

void game_server_lib_load_sessions(void *sessions, size_t n);

void game_server_lib_new_conn(socket_t *socket);

void game_server_lib_new_req(socket_t *socket, byte_t *buf, size_t n);

void game_server_lib_disconnect(socket_t *socket);

#endif
