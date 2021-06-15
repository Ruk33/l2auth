#ifndef INCLUDE_GS_PACKET_LEAVE_WORLD_H
#define INCLUDE_GS_PACKET_LEAVE_WORLD_H

#include "util.h"
#include "packet.h"

typedef struct {
        byte_t type;
} gs_packet_leave_world_t;

void gs_packet_leave_world_pack(packet_t *dest, gs_packet_leave_world_t *src);

#endif
