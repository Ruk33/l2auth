#ifndef LIB_SERVER_PACKET_CREATE_CHAR_H
#define LIB_SERVER_PACKET_CREATE_CHAR_H

#include <headers.h>

struct ServerPacketCreateChar {
        int response;
};

typedef struct ServerPacketCreateChar server_packet_create_char_t;

void server_packet_create_char(packet *dest);

#endif
