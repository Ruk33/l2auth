#ifndef LIB_SERVER_PACKET_QUEST_LIST_H
#define LIB_SERVER_PACKET_QUEST_LIST_H

#include "../headers.h"

struct ServerPacketQuestList {
        byte empty[7];
};

#define SERVER_PACKET_QUEST_LIST_SIZE sizeof(struct ServerPacketQuestList)
#define SERVER_PACKET_QUEST_LIST_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_QUEST_LIST_SIZE

void server_packet_quest_list(packet *dest);

#endif
