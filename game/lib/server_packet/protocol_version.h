#ifndef LIB_SERVER_PACKET_PROTOCOL_VERSION_H
#define LIB_SERVER_PACKET_PROTOCOL_VERSION_H

#include "../headers.h"

struct ServerPacketProtocolVersion {
        byte content[9];
};

#define SERVER_PACKET_PROTOCOL_VERSION_SIZE sizeof(struct ServerPacketProtocolVersion)
#define SERVER_PACKET_PROTOCOL_VERSION_FULL_SIZE sizeof(short) + sizeof(char) + SERVER_PACKET_PROTOCOL_VERSION_SIZE

void server_packet_protocol_version(packet *dest);

#endif
