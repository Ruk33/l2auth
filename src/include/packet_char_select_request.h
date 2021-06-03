#ifndef INCLUDE_PACKET_CHAR_SELECT_REQUEST_H
#define INCLUDE_PACKET_CHAR_SELECT_REQUEST_H

#include "packet.h"

typedef struct {
        int index;
} packet_char_select_request_t;

void packet_char_select_request_unpack(
        packet_char_select_request_t *dest,
        packet_t *src);

#endif
