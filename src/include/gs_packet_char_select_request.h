#ifndef INCLUDE_GS_PACKET_CHAR_SELECT_REQUEST_H
#define INCLUDE_GS_PACKET_CHAR_SELECT_REQUEST_H

#include "packet.h"

typedef struct {
        int index;
} gs_packet_char_select_request_t;

void gs_packet_char_select_request_unpack(
        gs_packet_char_select_request_t *dest,
        packet_t *src);

#endif
