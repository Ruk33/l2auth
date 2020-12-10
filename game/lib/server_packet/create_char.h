#ifndef LIB_SERVER_PACKET_CREATE_CHAR_H
#define LIB_SERVER_PACKET_CREATE_CHAR_H

#include "../headers.h"

struct ServerPacketCreateChar {
        int response;
};

#define SERVER_PACKET_CREATE_CHAR_SIZE sizeof(struct ServerPacketCreateChar)
#define SERVER_PACKET_CREATE_CHAR_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_CREATE_CHAR_SIZE

void server_packet_create_char(packet *dest);

#endif
