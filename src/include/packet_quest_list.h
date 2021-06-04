#ifndef INCLUDE_PACKET_QUEST_LIST_H
#define INCLUDE_PACKET_QUEST_LIST_H

#include "util.h"
#include "packet.h"

typedef struct {
        byte_t empty[7];
} packet_quest_list_t;

void packet_quest_list_pack(packet_t *dest, packet_quest_list_t *src);

#endif
