#ifndef PACKET_CREATE_CHAR_H
#define PACKET_CREATE_CHAR_H

#include "packet.h"

typedef struct {
        int response;
} packet_create_char_t;

void packet_create_char_pack(packet_t *dest, packet_create_char_t *src);

#endif
