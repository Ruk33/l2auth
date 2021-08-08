#ifndef INCLUDE_GS_PACKET_REVIVE_H
#define INCLUDE_GS_PACKET_REVIVE_H

#include "util.h"
#include "packet.h"
#include "gs_types.h"

struct gs_packet_revive {
        u32_t obj_id;
};

void gs_packet_revive_pack(packet_t *dest, struct gs_packet_revive *src);

#endif
