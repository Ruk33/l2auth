#ifndef INCLUDE_GS_PACKET_TARGET_SELECTED_H
#define INCLUDE_GS_PACKET_TARGET_SELECTED_H

#include "util.h"
#include "packet.h"

typedef struct {
        i32_t target_id;
        u32_t color;
} gs_packet_target_selected_t;

void gs_packet_target_selected_pack(
        packet_t *dest,
        gs_packet_target_selected_t *src);

#endif
