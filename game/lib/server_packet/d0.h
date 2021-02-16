#ifndef LIB_SERVER_PACKET_D0_H
#define LIB_SERVER_PACKET_D0_H

#include <headers.h>

struct ServerPacketD0 {
        short unknown;
        int manor_size;
};

typedef struct ServerPacketD0 server_packet_d0_t;

void server_packet_d0(packet *dest);

#endif
