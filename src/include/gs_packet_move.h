#ifndef INCLUDE_GS_PACKET_MOVE_H
#define INCLUDE_GS_PACKET_MOVE_H

#include "util.h"
#include "gs_types.h"
#include "packet.h"

typedef struct {
        u32_t id;
        i32_t prev_x;
        i32_t prev_y;
        i32_t prev_z;
        i32_t new_x;
        i32_t new_y;
        i32_t new_z;
} gs_packet_move_t;

void gs_packet_move(gs_packet_move_t *dest, struct gs_character *src);

void gs_packet_move_pack(packet_t *dest, gs_packet_move_t *src);

#endif
