#ifndef LIB_SERVER_PACKET_PROTOCOL_VERSION_H
#define LIB_SERVER_PACKET_PROTOCOL_VERSION_H

#include <headers.h>

struct ServerPacketProtocolVersion {
        byte_t content[9];
};

typedef struct ServerPacketProtocolVersion server_packet_protocol_version_t;

void server_packet_protocol_version(packet *dest);

#endif
