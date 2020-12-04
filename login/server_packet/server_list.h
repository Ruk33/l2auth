#ifndef SERVER_PACKET_SERVER_LIST_H
#define SERVER_PACKET_SERVER_LIST_H

#include <data_structures/list.h>
#include "../storage/game_server.h"

struct ServerPacketServerList {
        unsigned char count;
        unsigned char reserved_space;
        struct StorageGameServer servers[255];
};

#define SERVER_PACKET_SERVER_LIST_SIZE sizeof(struct ServerPacketServerList)
#define SERVER_PACKET_SERVER_LIST_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_SERVER_LIST_SIZE
#define SERVER_PACKET_SERVER_LIST_FULL_SIZE_ENCRYPTED sizeof(short) + ((SERVER_PACKET_SERVER_LIST_SIZE + sizeof(char) + 7) & (~7))

void server_packet_server_list(unsigned char *dest, struct List *servers);

#endif
