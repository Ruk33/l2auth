#ifndef INCLUDE_GS_PACKET_ACTION_REQUEST_H
#define INCLUDE_GS_PACKET_ACTION_REQUEST_H

#include "util.h"
#include "packet.h"

typedef struct {
        u32_t target_id;
        i32_t origin_x;
        i32_t origin_y;
        i32_t origin_z;
        u8_t action; // 0 = click, 1 = shift click
} gs_packet_action_request_t;

void gs_packet_action_request_unpack(
        gs_packet_action_request_t *dest,
        packet_t *src);

#endif
