#ifndef INCLUDE_LS_PACKET_PLAY_OK_H
#define INCLUDE_LS_PACKET_PLAY_OK_H

#include "packet.h"

typedef struct {
        int playOK1;
        int playOK2;
} ls_packet_play_ok_t;

void ls_packet_play_ok_pack(packet_t *dest, ls_packet_play_ok_t *src);

#endif
