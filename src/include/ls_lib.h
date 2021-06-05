#ifndef INCLUDE_LS_LIB_H
#define INCLUDE_LS_LIB_H

#include "util.h"
#include "os_socket.h"
#include "conn.h"

void ls_lib_load(conn_send_response_cb cb);

void ls_lib_new_conn(os_socket_t *socket);

void ls_lib_new_req(os_socket_t *socket, byte_t *buf, size_t n);

void ls_lib_disconnect(os_socket_t *socket);

#endif
