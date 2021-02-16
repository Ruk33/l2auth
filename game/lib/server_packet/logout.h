#ifndef LIB_SERVER_PACKET_LOGOUT_H
#define LIB_SERVER_PACKET_LOGOUT_H

#include <headers.h>

struct ServerPacketLogout {
        byte_t empty[4];
};

typedef struct ServerPacketLogout server_packet_logout_t;

void server_packet_logout(packet *dest);

#endif
