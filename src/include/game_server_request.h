#ifndef INCLUDE_GAME_SERVER_REQUEST_H
#define INCLUDE_GAME_SERVER_REQUEST_H

#include "util.h"
#include "socket.h"

void game_server_request_new_conn(socket_t *socket);

void game_server_request(socket_t *socket, byte_t *buf, size_t n);

void game_server_request_disconnect(socket_t *socket);

#endif
