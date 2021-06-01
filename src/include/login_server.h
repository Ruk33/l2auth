#ifndef INCLUDE_LOGIN_SERVER_H
#define INCLUDE_LOGIN_SERVER_H

#include <stdio.h>
#include "util.h"
#include "socket.h"
#include "packet.h"

#define login_server_log(...)        \
        {                            \
                printf(__VA_ARGS__); \
                printf("\n");        \
        }

#define login_server_send_packet(socket, packet) \
        login_server_send(socket, packet, (size_t) packet_size(packet))

typedef void (*send_response_cb)(socket_t *socket, byte_t *buf, size_t n);

void login_server_set_send_response(send_response_cb cb);

// void login_server_log(char *msg);

void login_server_send(socket_t *socket, byte_t *buf, size_t n);

#endif
