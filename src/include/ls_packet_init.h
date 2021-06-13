#ifndef INCLUDE_LS_PACKET_INIT_H
#define INCLUDE_LS_PACKET_INIT_H

#include "util.h"
#include "packet.h"

typedef struct {
        byte_t session_id[4];
        byte_t protocol[4];
        byte_t modulus[128];
} ls_packet_init_t;

void ls_packet_init(ls_packet_init_t *dest, byte_t *modulus);

void ls_packet_init_pack(packet_t *dest, ls_packet_init_t *src);

#endif
