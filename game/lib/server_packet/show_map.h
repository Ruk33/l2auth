#ifndef LIB_SERVER_PACKET_SHOW_MAP_H
#define LIB_SERVER_PACKET_SHOW_MAP_H

#include <headers.h>

struct ServerPacketShowMap {
        int map_id;
};

typedef struct ServerPacketShowMap server_packet_show_map_t;

void server_packet_show_map(packet *dest, int map_id);

#endif
