#ifndef LIB_SERVER_PACKET_LOGOUT_H
#define LIB_SERVER_PACKET_LOGOUT_H

#include "../headers.h"

struct ServerPacketLogout {
        byte empty[4];
};

#define SERVER_PACKET_LOGOUT_SIZE sizeof(struct ServerPacketLogout)
#define SERVER_PACKET_LOGOUT_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_LOGOUT_SIZE

void server_packet_logout(packet *dest);

#endif
