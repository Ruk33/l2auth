#ifndef INCLUDE_CONN_H
#define INCLUDE_CONN_H

#include "util.h"
#include "packet.h"
#include "os_io.h"

#define conn_send_packet(socket, packet) \
        conn_send_response(socket, packet, (size_t) packet_size(packet))

typedef void (*conn_send_response_cb)(os_io_t *socket, void *buf, size_t n);

void conn_set_cb(conn_send_response_cb cb);

void conn_send_response(os_io_t *socket, void *buf, size_t n);

#endif
