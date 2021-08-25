#ifndef INCLUDE_CONN_H
#define INCLUDE_CONN_H

#include "util.h"
#include "packet.h"
#include "os_io.h"

#define conn_send_packet(socket, packet) \
        conn_send_response(socket, packet, (size_t) packet_size(packet))

typedef void (*conn_send_response_cb)(struct os_io *socket, void *buf, size_t n);
typedef void (*conn_disconnect_cb)(struct os_io *socket);
typedef u32_t (*conn_text_ip_to_u32_cb)(char *ip);

void conn_set_send_response(conn_send_response_cb cb);

void conn_set_disconnect(conn_disconnect_cb cb);

void conn_set_text_ip_to_u32(conn_text_ip_to_u32_cb cb);

void conn_send_response(struct os_io *socket, void *buf, size_t n);

void conn_disconnect(struct os_io *socket);

u32_t conn_text_ip_to_u32(char *ip);

#endif
