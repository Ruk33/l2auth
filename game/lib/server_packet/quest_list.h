#ifndef LIB_SERVER_PACKET_QUEST_LIST_H
#define LIB_SERVER_PACKET_QUEST_LIST_H

#include <headers.h>

struct ServerPacketQuestList {
        byte_t empty[7];
};

typedef struct ServerPacketQuestList server_packet_quest_list_t;

void server_packet_quest_list(packet *dest);

#endif
