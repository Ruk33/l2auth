#ifndef INCLUDE_GS_PACKET_D0_H
#define INCLUDE_GS_PACKET_D0_H

#include "packet.h"

typedef struct {
        short unknown;
        int manor_size;
} gs_packet_d0_t;

void gs_packet_d0_pack(packet_t *dest, gs_packet_d0_t *src);

#endif
