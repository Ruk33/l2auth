#ifndef LIB_SERVER_PACKET_RESTART_H
#define LIB_SERVER_PACKET_RESTART_H

#include <headers.h>

struct ServerPacketRestart {
        int response;
};

typedef struct ServerPacketRestart server_packet_restart_t;

void server_packet_restart(packet *dest);

#endif
