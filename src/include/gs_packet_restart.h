#ifndef INCLUDE_GS_PACKET_RESTART_H
#define INCLUDE_GS_PACKET_RESTART_H

#include "packet.h"

typedef struct {
        int response;
} gs_packet_restart_t;

void gs_packet_restart_pack(packet_t *dest, gs_packet_restart_t *src);

#endif
