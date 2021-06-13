#ifndef INCLUDE_GS_PACKET_MOVE_REQUEST_H
#define INCLUDE_GS_PACKET_MOVE_REQUEST_H

#include "util.h"
#include "packet.h"

typedef struct {
        i32_t x;
        i32_t y;
        i32_t z;
} gs_packet_move_request_t;

void gs_packet_move_request_unpack(gs_packet_move_request_t *dest, packet_t *s);

#endif
