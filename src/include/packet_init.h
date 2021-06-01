#ifndef INCLUDE_PACKET_INIT_H
#define INCLUDE_PACKET_INIT_H

#include "util.h"
#include "packet.h"

typedef struct {
        byte_t session_id[4];
        byte_t protocol[4];
        byte_t modulus[128];
} packet_init_t;

void packet_init(packet_init_t *dest, byte_t *modulus);

void packet_init_pack(packet_t *dest, packet_init_t *src);

#endif
