#ifndef INCLUDE_LOGIN_SERVER_LIB_H
#define INCLUDE_LOGIN_SERVER_LIB_H

#include "util.h"
#include "socket.h"
#include "login_server.h"

void login_server_lib_load(send_response_cb cb);

void login_server_lib_new_conn(socket_t *socket);

void login_server_lib_new_req(socket_t *socket, byte_t *buf, size_t n);

void login_server_lib_disconnect(socket_t *socket);

#endif
