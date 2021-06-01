#ifndef INCLUDE_SERVER_LIST_H
#define INCLUDE_SERVER_LIST_H

#include "config.h"
#include "util.h"
#include "packet.h"
#include "server.h"

typedef struct {
        u8_t     count;
        byte_t   reserved; // Unknown
        server_t servers[MAX_SERVERS];
} packet_server_list_t;

void packet_server_list_add(packet_server_list_t *dest, server_t *src);

void packet_server_list_pack(packet_t *dest, packet_server_list_t *src);

#endif
