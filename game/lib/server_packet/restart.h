#ifndef LIB_SERVER_PACKET_RESTART_H
#define LIB_SERVER_PACKET_RESTART_H

#include "../headers.h"

struct ServerPacketRestart {
        int response;
};

#define SERVER_PACKET_RESTART_SIZE sizeof(struct ServerPacketRestart)
#define SERVER_PACKET_RESTART_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_RESTART_SIZE

void server_packet_restart(packet *dest);

#endif
