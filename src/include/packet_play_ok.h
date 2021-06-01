#ifndef INCLUDE_PACKET_PLAY_OK_H
#define INCLUDE_PACKET_PLAY_OK_H

#include "packet.h"

typedef struct {
        int playOK1;
        int playOK2;
} packet_play_ok_t;

void packet_play_ok_pack(packet_t *dest, packet_play_ok_t *src);

#endif
