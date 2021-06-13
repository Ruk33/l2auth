#ifndef INCLUDE_GS_PACKET_VALIDATE_POS_H
#define INCLUDE_GS_PACKET_VALIDATE_POS_H

#include "util.h"
#include "packet.h"
#include "gs_character.h"

typedef struct {
        u32_t id;
        i32_t x;
        i32_t y;
        i32_t z;
        i32_t heading;
} gs_packet_validate_pos_t;

void gs_packet_validate_pos(gs_packet_validate_pos_t *dest, gs_character_t *src);

void gs_packet_validate_pos_pack(packet_t *dest, gs_packet_validate_pos_t *src);

#endif
