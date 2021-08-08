#ifndef INCLUDE_GS_PACKET_DIE_H
#define INCLUDE_GS_PACKET_DIE_H

#include "util.h"
#include "packet.h"
#include "gs_types.h"

struct gs_packet_die {
        u32_t obj_id;
        u32_t to_village;
        u32_t to_hideaway;
        u32_t to_castle;
        u32_t to_siege;
        u32_t sweepable;
        u32_t to_fixed;
};

void gs_packet_die_add_options(
        struct gs_packet_die *dest,
        struct gs_character *src);

void gs_packet_die_pack(packet_t *dest, struct gs_packet_die *src);

#endif
