#ifndef INCLUDE_GS_PACKET_VALIDATE_POS_REQUEST_H
#define INCLUDE_GS_PACKET_VALIDATE_POS_REQUEST_H

#include "util.h"
#include "packet.h"

typedef struct {
        i32_t x;
        i32_t y;
        i32_t z;
        i32_t heading;
} gs_packet_validate_pos_request_t;

void gs_packet_validate_pos_request_unpack(
        gs_packet_validate_pos_request_t *dest,
        packet_t *src);

#endif
