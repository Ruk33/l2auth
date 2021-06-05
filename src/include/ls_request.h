#ifndef INCLUDE_LS_REQUEST_H
#define INCLUDE_LS_REQUEST_H

#include "util.h"
#include "os_socket.h"

void ls_request_new_conn(os_socket_t *socket);

void ls_request(os_socket_t *socket, byte_t *buf, size_t n);

void ls_request_disconnect(os_socket_t *socket);

#endif
