#ifndef LIB_SERVER_PACKET_D0_H
#define LIB_SERVER_PACKET_D0_H

#include "../headers.h"

struct ServerPacketD0 {
        short unknown;
        int manor_size;
};

#define SERVER_PACKET_D0_SIZE sizeof(struct ServerPacketD0)
#define SERVER_PACKET_D0_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_D0_SIZE

void server_packet_d0(packet *dest);

#endif
