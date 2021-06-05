#ifndef INCLUDE_GS_PACKET_QUEST_LIST_H
#define INCLUDE_GS_PACKET_QUEST_LIST_H

#include "util.h"
#include "packet.h"

typedef struct {
        byte_t empty[7];
} gs_packet_quest_list_t;

void gs_packet_quest_list_pack(packet_t *dest, gs_packet_quest_list_t *src);

#endif
