#ifndef LIB_SERVER_PACKET_SHOW_MAP_H
#define LIB_SERVER_PACKET_SHOW_MAP_H

#include "../headers.h"

struct ServerPacketShowMap {
        int map_id;
};

#define SERVER_PACKET_SHOW_MAP_SIZE sizeof(struct ServerPacketShowMap)
#define SERVER_PACKET_SHOW_MAP_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_SHOW_MAP_SIZE

void server_packet_show_map(packet *dest, int map_id);

#endif
