#ifndef INCLUDE_LS_PACKET_SERVER_LIST_H
#define INCLUDE_LS_PACKET_SERVER_LIST_H

#include "config.h"
#include "util.h"
#include "packet.h"
#include "server.h"

typedef struct {
        u8_t count;
        byte_t reserved; // Unknown
        server_t servers[MAX_SERVERS];
} ls_packet_server_list_t;

void ls_packet_server_list_add(ls_packet_server_list_t *dest, server_t *src);

void ls_packet_server_list_pack(packet_t *dest, ls_packet_server_list_t *src);

#endif
