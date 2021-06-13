#ifndef INCLUDE_GS_PACKET_CREATE_CHAR_H
#define INCLUDE_GS_PACKET_CREATE_CHAR_H

#include "packet.h"

typedef struct {
        int response;
} gs_packet_create_char_t;

void gs_packet_create_char_pack(packet_t *dest, gs_packet_create_char_t *src);

#endif
