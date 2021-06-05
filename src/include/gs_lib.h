#ifndef INCLUDE_GS_LIB_H
#define INCLUDE_GS_LIB_H

#include "util.h"
#include "os_socket.h"
#include "conn.h"
#include "gs_session.h"

void gs_lib_load(conn_send_response_cb cb, gs_session_save_cb save_sessions_cb);

void gs_lib_unload(void);

void gs_lib_load_sessions(void *sessions, size_t n);

void gs_lib_new_conn(os_socket_t *socket);

void gs_lib_new_req(os_socket_t *socket, byte_t *buf, size_t n);

void gs_lib_disconnect(os_socket_t *socket);

#endif
